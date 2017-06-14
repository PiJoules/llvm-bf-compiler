#ifndef _BF_H
#define _BF_H

#include <iostream>
#include <istream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <string> 
#include <stack>
#include <utility>

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
        bool eof() const;

        virtual ~Lexer(){}
};

class Parser {
    private:
        LLVMContext context;
        std::unique_ptr<Module> mod;
        Value* cells;  // Owned by temp IRBuilder
        Value* idx_ptr;
        std::stack<std::shared_ptr<std::pair<BasicBlock*, BasicBlock*>>> loop_pairs;

        // BF instructions
        void ptr_inc(IRBuilder<>&);
        void ptr_dec(IRBuilder<>&);
        void val_inc(IRBuilder<>&);
        void val_dec(IRBuilder<>&);
        void print_char(IRBuilder<>&);
        void read_char(IRBuilder<>&);
        void while_start(IRBuilder<>&);
        void while_end(IRBuilder<>&);

        void declare_c_functions();
        void create_main_block(Lexer&);

    public:
        void parse(Lexer& lex);
        void dump();

        virtual ~Parser(){}
};

#endif
