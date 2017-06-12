CC = g++
STD = c++11
LLVM_CONFIG = llvm-config-3.9
LLVMFLAGS = `$(LLVM_CONFIG) --cxxflags --ldflags --system-libs --libs core`
CPPFLAGS = -Wall -Werror -g -std=$(STD)


SRC = lexer.cpp \
	  parser.cpp 

OBJS = $(SRC:.cpp=.o)


.PHONY: test


%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< $(LLVMFLAGS) -o $@ -fexceptions


test_lexer: $(OBJS) test_lexer.cpp
	$(CC) $(CPPFLAGS) test_lexer.cpp $(OBJS) $(LLVMFLAGS) -o test_lexer 
	./test_lexer 

test: test_lexer
	test_lexer

clean:
	rm -rf *.out *.o test_lexer *.dwo
