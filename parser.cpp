#include "bf.h"

#define NUM_CELLS 30000
#define CELL_SIZE 1


void Parser::ptr_inc(IRBuilder<> &builder){
    Value* idx = builder.CreateLoad(idx_ptr, "idx");
    Value *new_idx = builder.CreateGEP(idx, ConstantInt::get(context, APInt(32, 1)), "new_idx");
    builder.CreateStore(new_idx, idx_ptr);
}

void Parser::ptr_dec(IRBuilder<> &builder){
    Value* idx = builder.CreateLoad(idx_ptr, "idx");
    Value *new_idx = builder.CreateGEP(idx, ConstantInt::get(context, APInt(32, -1)), "new_idx");
    builder.CreateStore(new_idx, idx_ptr);
}

void Parser::val_inc(IRBuilder<> &builder){
    Value* idx = builder.CreateLoad(idx_ptr, "idx");
    Value* val = builder.CreateLoad(idx, "val");
    Value* new_val = builder.CreateAdd(val, ConstantInt::get(context, APInt(8, 1)), "new_val");
    builder.CreateStore(new_val, idx);
}

void Parser::val_dec(IRBuilder<> &builder){
    Value* idx = builder.CreateLoad(idx_ptr, "idx");
    Value* val = builder.CreateLoad(idx, "val");
    Value* new_val = builder.CreateSub(val, ConstantInt::get(context, APInt(8, 1)), "new_val");
    builder.CreateStore(new_val, idx);
}

void Parser::print_char(IRBuilder<> &builder){
    Value* idx = builder.CreateLoad(idx_ptr, "idx");
    Value* val = builder.CreateLoad(idx, "val");
    val = builder.CreateZExt(val, Type::getInt32Ty(context));
    std::vector<Value*> args = {val};
    builder.CreateCall(mod->getFunction("putchar"), args);
}

void Parser::read_char(IRBuilder<> &builder){
    std::vector<Value*> args;
    Value* val = builder.CreateCall(mod->getFunction("getchar"), args, "val");
    val = builder.CreateTrunc(val, Type::getInt8Ty(context), "val");
    Value* idx = builder.CreateLoad(idx_ptr, "idx");
    builder.CreateStore(val, idx);
}

void Parser::while_start(IRBuilder<> &builder){
    // Create new loop blocks
    Function *main_func = builder.GetInsertBlock()->getParent();
    BasicBlock *loop_start = BasicBlock::Create(context, "loop_start", main_func);
    BasicBlock *loop_body = BasicBlock::Create(context, "loop_body", main_func);
    BasicBlock *loop_end = BasicBlock::Create(context, "loop_end", main_func);

    auto loop_pair = std::make_shared<std::pair<BasicBlock*, BasicBlock*>>(loop_start, loop_end);
    loop_pairs.push(loop_pair);

    // Create start of loop where the check is done
    builder.CreateBr(loop_start);
    builder.SetInsertPoint(loop_start);

    // Exit if current value is zero
    Value* idx = builder.CreateLoad(idx_ptr, "idx");
    Value* val = builder.CreateLoad(idx, "val");
    Value* not_zero = builder.CreateICmpNE(val, ConstantInt::get(context, APInt(8, 0)), "not_zero");
    builder.CreateCondBr(not_zero, loop_body, loop_end);

    // Otherwise continue on 
    builder.SetInsertPoint(loop_body);
}

void Parser::while_end(IRBuilder<> &builder){
    // Go back to previous loop start
    auto loop_pair = loop_pairs.top();
    loop_pairs.pop();
    auto loop_start = loop_pair.get()->first;
    auto loop_end = loop_pair.get()->second;
    builder.CreateBr(loop_start);

    // Pop the last loop end and insert it 
    builder.SetInsertPoint(loop_end);
}

void Parser::create_main_block(Lexer& lex){
    // Create the function 
    FunctionType *main_func_type = FunctionType::get(Type::getInt32Ty(context), false);
    Function* main_func = Function::Create(
            main_func_type, Function::ExternalLinkage, "main", mod.get());
    BasicBlock *main_block = BasicBlock::Create(context, "entry", main_func);

    IRBuilder<> builder(context);
    builder.SetInsertPoint(main_block);

    // Set up the cells  
    // char* cells = calloc(NUM_CELLS, CELL_SIZE);
    Function *calloc = mod->getFunction("calloc");
    std::vector<Value*> calloc_args = {
        ConstantInt::get(context, APInt(32, NUM_CELLS)),
        ConstantInt::get(context, APInt(32, CELL_SIZE)),
    };
    cells = builder.CreateCall(calloc, calloc_args, "cells");

    // index pointer 
    // char* idx_ptr = cells;
    idx_ptr = builder.CreateAlloca(Type::getInt8PtrTy(context), nullptr, "idx_ptr");
    builder.CreateStore(cells, idx_ptr);

    // Read tokens from the lexer 
    LexToken tok;
    while (!lex.eof()){
        tok = lex.token();

        switch (tok.value){
            case PTR_INC:
                ptr_inc(builder);
                break;
            case PTR_DEC:
                ptr_dec(builder);
                break;
            case VAL_INC:
                val_inc(builder);
                break;
            case VAL_DEC:
                val_dec(builder);
                break;
            case PRINT_CHAR:
                print_char(builder);
                break;
            case READ_CHAR:
                read_char(builder);
                break;
            case WHILE_START:
                while_start(builder);
                break;
            case WHILE_END:
                if (loop_pairs.empty()){
                    std::ostringstream err;
                    err << "Extra ']' found at (" << tok.lineno << ", " << tok.colno << ").";
                    throw std::runtime_error(err.str());
                }
                while_end(builder);
                break;
            default:
                std::ostringstream err;
                err << "Unknown character " << (int)tok.value << " on (" << tok.lineno << ", " << tok.colno << ").";
                throw std::runtime_error(err.str());
        }
    }

    if (!loop_pairs.empty()){
        throw std::runtime_error("There are more opening brackets than closing brackets.");
    }

    // free calls 
    std::vector<Value*> free_args = {cells};
    builder.CreateCall(mod->getFunction("free"), free_args);

    // Return 0
    builder.CreateRet(ConstantInt::get(context, APInt(32, 0)));
}

/**
 * Add all external functions used.
 * - calloc 
 * - free 
 * - getchar 
 * - putchar
 */
void Parser::declare_c_functions(){
    // extern calloc 
    std::vector<Type*> calloc_args = {
        Type::getInt32Ty(context),
        Type::getInt32Ty(context),
    };
    FunctionType *calloc_type = FunctionType::get(
            Type::getInt8PtrTy(context), calloc_args, false);
    Function::Create(calloc_type, Function::ExternalLinkage, "calloc", mod.get());

    // extern free 
    std::vector<Type*> free_args = {Type::getInt8PtrTy(context)};
    FunctionType *free_type = FunctionType::get(
            Type::getVoidTy(context), free_args, false);
    Function::Create(free_type, Function::ExternalLinkage, "free", mod.get());

    // getchar  
    FunctionType *getchar_type = FunctionType::get(Type::getInt32Ty(context), false);
    Function::Create(getchar_type, Function::ExternalLinkage, "getchar", mod.get());

    // putchar  
    std::vector<Type*> putchar_agrs = {Type::getInt32Ty(context)};
    FunctionType *putchar_type = FunctionType::get(
            Type::getInt32Ty(context), putchar_agrs, false);
    Function::Create(putchar_type, Function::ExternalLinkage, "putchar", mod.get());
}

void Parser::parse(Lexer& lex){
    mod = std::move(make_unique<Module>("__main__", context));
    declare_c_functions();
    create_main_block(lex);
}

void Parser::dump(){
    mod->dump();
}
