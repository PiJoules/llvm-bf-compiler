#include <iostream>
#include "bf.h"

int main(){
    Lexer lex;
    
    Parser p;
    p.parse(lex);
    p.dump();

    return 0;
}
