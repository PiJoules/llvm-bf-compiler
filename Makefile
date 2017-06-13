CPP = g++
LLC = llc-3.9
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
	$(CPP) $(CPPFLAGS) -c $< $(LLVMFLAGS) -o $@ -fexceptions


test_lexer.out: test_lexer.cpp $(OBJS)
	$(CPP) $(CPPFLAGS) $< $(OBJS) $(LLVMFLAGS) -o $@ 

test_lexer: test_lexer.out
	./test_lexer.out

test_parser.out: test_parser.cpp $(OBJS)
	$(CPP) $(CPPFLAGS) $< $(OBJS) $(LLVMFLAGS) -o $@ 

test_parser: test_parser.out
	./test_parser.out 
	./test_parser.out 2> test_parser.ll
	$(LLC) -filetype=obj test_parser.ll
	$(CPP) test_parser.o -o a.out 
	./a.out

clean:
	rm -rf *.out *.o *.dwo *.s *.ll
