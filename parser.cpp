#include "bf.h"

#define NUM_CELLS 30000
#define CELL_SIZE 1

void Parser::ptr_inc(IRBuilder<> &builder){
    Value* idx = builder.CreateLoad(idx_ptr, "idx");
    Value* new_idx = builder.CreateAdd(idx, ConstantInt::get(context, APInt(32, 1)), "new_idx");
    builder.CreateStore(new_idx, idx_ptr);
}

void Parser::create_main_block(const Lexer& lex){
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

    /**
     * Test calls 
     */ 
    ptr_inc(builder);

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

void Parser::parse(const Lexer& lex){
    mod = std::move(make_unique<Module>("__main__", context));
    declare_c_functions();
    create_main_block(lex);
}

void Parser::dump(){
    mod->dump();
}
