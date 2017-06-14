#include <iostream>
#include "bf.h"

int main(){
    Lexer lex;

    // Hello world
    lex.input("++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.");
    
    Parser p;
    p.parse(lex);
    p.dump();

    return 0;
}
