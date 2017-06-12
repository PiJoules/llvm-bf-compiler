#include "bf.h"

//#include "llvm/Analysis/Verifier.h"
//#include "llvm/IR/DerivedTypes.h"
//#include "llvm/IR/IRBuilder.h"
//#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

using namespace llvm;


std::unique_ptr<Module> compile(const Lexer& lex){
    //LLVMContext &context = getGlobalContext();
    LLVMContext context;
    std::unique_ptr<Module> mod = make_unique<Module>("__main__", context);

    return mod;
}
