# Brainfuck LLVM Implementation
Practice using llvm API by implementing a compiler for bf.

LLVM v3.9

# Tests
Run `make`.

# Potential Improvements
- Use an integer to keep track of the position on the cell array rather than a pointer. Having a pointer always required having to load the index from the alloca'd space.
  - Doing this may allow using the same local variable to represent the index instead of having to constantly load.
  - Alternatively, the llvm ir code could be modularized into functions that each take the pointer to the cell array and manipulate that pointer (void ptr_inc(char* p){p++})
- Use statically alloca'd array on the stack instead of using calloc and free-ing the pointer.
- Did not implement JIT or optimization passes
- Did not implement way of reading bf from stdin or file. Just the hard coded test is implemented.
