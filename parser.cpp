#include "bf.h"

//#include "llvm/Analysis/Verifier.h"
//#include "llvm/IR/DerivedTypes.h"
//#include "llvm/IR/IRBuilder.h"
//#include "llvm/IR/LLVMContext.h"

std::unique_ptr<Module> compile_lexer(const Lexer& lex, LLVMContext& context){
    std::unique_ptr<Module> mod = make_unique<Module>("__main__", context);

    return mod;
}
//Module* compile_lexer(const Lexer& lex){
//    LLVMContext *context = new LLVMContext();
//    Module *mod = new Module("__main__", *context);
//
//    return mod;
//}
