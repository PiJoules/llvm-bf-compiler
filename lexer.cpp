#include <string> 
#include <sstream>
#include <stdexcept>

#include "bf.h"


LexToken Lexer::token(){
    if (!stream){
        throw std::runtime_error("No stream initialized.");
    }

    char c;
    while ((c = stream->get()) != EOF){
        lexpos++;
        colno++;

        switch (c){
            case PTR_INC:
            case PTR_DEC:
            case VAL_INC:
            case VAL_DEC:
            case PRINT_CHAR:
            case READ_CHAR:
            case WHILE_START:
            case WHILE_END:
                return {
                    c,
                    lineno,
                    colno,
                    lexpos,
                };
            case NEWLINE:
                lineno++;
                colno = 0;
                break;
        }
    }

    return {
        EOF,
        lineno,
        colno,
        lexpos,
    };
}


Lexer::~Lexer(){
    delete stream;
}


void Lexer::input(const std::string& code){
    stream = new std::istringstream(code);
}


bool Lexer::eof(){
    return stream->peek() == EOF;
}
