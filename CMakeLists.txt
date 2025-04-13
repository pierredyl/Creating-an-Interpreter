cmake_minimum_required(VERSION 3.27)
project(Creating_an_Interpreter)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Creating_an_Interpreter
    main.cpp
    remove_comments.cpp
    remove_comments.h
        Token.h
        Tokenizer.h
        Tokenizer.cpp
        Token.cpp
        RecursiveDescentParser.h
        RecursiveDescentParser.cpp
        SymbolTable.h
        SymbolTable.cpp
        AbstractSyntaxTree.h
        AbstractSyntaxTree.cpp)
