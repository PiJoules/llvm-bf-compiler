#include <iostream>
#include "bf.h"

int main(){
    Lexer lex;

    LLVMContext context;

    auto mod = compile_lexer(lex, context);
    //delete mod;

    return 0;
}
