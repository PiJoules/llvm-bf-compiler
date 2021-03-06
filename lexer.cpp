#include "bf.h"


LexToken Lexer::token(){
    if (eof()){
        return {
            EOF,
            lineno,
            colno,
            lexpos,
        };
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


void Lexer::input(const std::string& code){
    std::unique_ptr<std::istream> code_stream(new std::istringstream(code));
    stream = std::move(code_stream);
}


bool Lexer::eof() const {
    if (!stream){
        return true;
    }
    return stream->peek() == EOF;
}
