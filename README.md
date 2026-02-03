# MINIC

This is a minic compiler that emits LLVM IR. It was part of my course Compilers I.

The compiler has these features available:
- Integers
- Floats
- All operators
- Loop Statements
- If Statements
- Functions

For the development i used:
- Lexical Analysis: flex, emitting C lexer
- Syntax Analysis: bison, emitting C++ parser
- Semantic Analysis: C++
- IR Emitting: C++

Other Tools:
- Building project with make
- Building the Syntax Tree with graphviz
- Memory checks with valgrind

Software Patterns used:
- Syntax Tree nodes: composite pattern
- Symbol Table: Singleton pattern
- Semantic Analysis and IR Emitting: Visitor pattern

## How to run

For building the project just simply use:
```bash
make
```

For creating the Syntax Tree use:
```bash
make graph
```

To do a memory check use:
```bash
make val
```

To clean obj directory (object files) use:
```bash
make clean
```

To clean all files generated and obj directory use:
```bash
make distclean
```

## Notes

The compiler has the ability to be used as an interpreter but only calculating integers and the global declarations are done with a helper Visitor called Declarator.

In main.cc before main function there are some comments with things that could be improved.
