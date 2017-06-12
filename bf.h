#ifndef _BF_H
#define _BF_H

#include <istream>
#include <memory>
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

using namespace llvm;

#define PTR_INC '>'
#define PTR_DEC '<'
#define VAL_INC '+'
#define VAL_DEC '-'
#define PRINT_CHAR '.'
#define READ_CHAR ','
#define WHILE_START '['
#define WHILE_END ']'
#define NEWLINE '\n'


typedef struct LexToken LexToken;
struct LexToken {
    char value;
    int lineno, colno, lexpos;
};


class Lexer {
    private:
        int lexpos = 0;
        int lineno = 1, colno = 0;
        std::istream* stream = nullptr;

    public:
        LexToken token();
        void input(const std::string& code);
        bool eof();

        virtual ~Lexer();
};

std::unique_ptr<Module> compile_lexer(const Lexer& lex, LLVMContext& context);
//Module* compile_lexer(const Lexer& lex);

#endif
