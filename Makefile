CC = g++
STD = c++11
LLVM_CONFIG = llvm-config-3.9
LLVMFLAGS = `$(LLVM_CONFIG) --cxxflags --ldflags --system-libs --libs core`
CPPFLAGS = -Wall -Werror -g -std=$(STD)


SRC = lexer.cpp \
	  parser.cpp  

TESTS = lexer

OBJS = $(SRC:.cpp=.o)

test: test_lexer test_parser

.PHONY: test

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< $(LLVMFLAGS) -o $@ -fexceptions


test_lexer.out: test_lexer.cpp $(OBJS)
	$(CC) $(CPPFLAGS) $< $(OBJS) $(LLVMFLAGS) -o $@ 

test_lexer: test_lexer.out
	./test_lexer.out

test_parser.out: test_parser.cpp $(OBJS)
	$(CC) $(CPPFLAGS) $< $(OBJS) $(LLVMFLAGS) -o $@ 

test_parser: test_parser.out
	./test_parser.out

clean:
	rm -rf *.out *.o *.dwo 
