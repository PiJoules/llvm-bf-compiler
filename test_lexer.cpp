#include <cassert>
#include "bf.h"

void test_lexer_creation(){
    Lexer lex;
}

void test_empty_stream(){
    Lexer lex;
    lex.input("");

    LexToken tok = lex.token();
    assert(tok.value == EOF);
    assert(tok.lineno == 1);
    assert(tok.colno == 0);

    // Values shouldn't change
    tok = lex.token();
    assert(tok.value == EOF);
    assert(tok.lineno == 1);
    assert(tok.colno == 0);
}

void test_lexer_content(){
    Lexer lex;
    lex.input(">2\n6+");

    LexToken tok = lex.token();
    assert(tok.value == PTR_INC);
    assert(tok.lineno == 1);
    assert(tok.colno == 1);

    tok = lex.token();
    assert(tok.value == VAL_INC);
    assert(tok.lineno == 2);
    assert(tok.colno == 2);

    tok = lex.token();
    assert(tok.value == EOF);
    assert(tok.lineno == 2);
    assert(tok.colno == 2);
}


int main(){
    test_lexer_creation();
    test_empty_stream();
    test_lexer_content();
}
