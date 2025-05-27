# Creating an Interpreter

A lightweight interpreter for a simplified C-like language, written in C++. This project parses, analyzes, and executes C-style code using a custom-built Abstract Syntax Tree (AST) and symbol table.

## Features

- **Lexical Analysis**: Tokenizes input source code.
- **Parsing**: Builds an Abstract Syntax Tree (AST) using recursive descent parsing.
- **Symbol Table**: Manages variable declarations and scopes.
- **Interpretation**: Evaluates expressions and executes statements.
- **Control Flow**: Supports `if` and `while` statements.
- **Error Handling**: Provides basic error detection and reporting.

## Project Structure
├── ASTTestFiles/ # Test files for AST
├── parserTestFiles/ # Test files for parser
├── symbolTableTestFiles/ # Test files for symbol table
├── AbstractSyntaxTree.cpp/.h # AST implementation
├── Interpreter.cpp/.h # Interpreter logic
├── RecursiveDescentParser.cpp/.h# Parser implementation
├── SymbolTable.cpp/.h # Symbol table implementation
├── Token.cpp/.h # Token definitions
├── Tokenizer.cpp/.h # Lexer implementation
├── main.cpp # Entry point
├── CMakeLists.txt # Build configuration
└── README.md # Project documentation


