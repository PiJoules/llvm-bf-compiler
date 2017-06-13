#ifndef _BF_H
#define _BF_H

#include <istream>
#include <memory>
#include <vector>
#include <unordered_map>

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"

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
        std::unique_ptr<std::istream> stream;

    public:
        LexToken token();
        void input(const std::string& code);
        bool eof();

        virtual ~Lexer(){}
};

std::unique_ptr<Module> compile_lexer(const Lexer& lex, LLVMContext& context);

class Parser {
    private:
        LLVMContext context;
        std::unique_ptr<Module> mod;
        Value* cells;  // Owned by temp IRBuilder
        Value* idx_ptr;
        int bracket_count = 0;

        // BF instructions
        void ptr_inc(IRBuilder<>&);
        void ptr_dec();
        void val_inc();
        void val_dec();
        void print_char();
        void read_char();
        void while_start();
        void while_end();

        void declare_c_functions();
        void create_main_block(const Lexer&);

    public:
        void parse(const Lexer& lex);
        void dump();

        virtual ~Parser(){}
};

#endif
