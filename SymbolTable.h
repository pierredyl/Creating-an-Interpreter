//
// Created by Dylan Pierre on 4/2/25.
//
#include <iostream>
#include "RecursiveDescentParser.h"
using namespace std;

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

class SymbolTable {

private:
    struct Symbol {
        string identifierName;
        string identifierType;
        string dataType;
        bool isArray;
        int arraySize;
        int scope;
        int symbolLineNumber;
        Symbol* next;

        // Constructor
        Symbol(string name, string type, string dtype, int scp, bool arr = false, int arrSize = 0, int lineNumber = -1)
            : identifierName(name), identifierType(type), dataType(dtype), scope(scp), isArray(arr), arraySize(arrSize), symbolLineNumber(lineNumber), next(nullptr) {}
    };

    Symbol* head;  // Head of the linked list
    RecursiveDescentParser::CSTNode* Root;
    int lineNumber = 0;
    void buildSymbolTableHelper(RecursiveDescentParser::CSTNode *node);
    int globalScope;
    bool inFuncOrProc = false;
    int depth = 0;

public:
    // Constructor
    SymbolTable(RecursiveDescentParser& SyntaxTree);

    // Insert a new symbol (variable, function, or parameter)
    void insertSymbol(std::string name, std::string type, std::string dtype, int scope, bool isArray = false, int arrSize = 0, int lineNumber = -1);

    void checkForDuplicate(const string &identifierName, int scope, int currentLineNumber);

    int getSymbolLineNumber();

    //insert function
    void insertFunction(std::string identifierName, string identifierType, string datatype, int scope, int lineNumber = -1);

    //insert procedure
    void insertProcedure(string identifiername, string identifierType, int scope, int lineNumber = -1);

    // Insert variable entry
    void insertVariable(std::string varName, std::string varType, int scope, bool isArray = false, int arrSize = 0, int lineNumber = -1);

    //Insert parameter
    void insertParameter(string identifierName, std::string varType, int scope, bool isArray = false, int arrSize = 0, int lineNumber = -1);

    RecursiveDescentParser::CSTNode *variableListHelper(RecursiveDescentParser::CSTNode *node, string datatype);

    RecursiveDescentParser::CSTNode* insertParameterHelper(RecursiveDescentParser::CSTNode *node, string identifierName);

    // Print symbol table
    void printTable();

    void buildSymbolTable();

};

#endif //SYMBOLTABLE_H
