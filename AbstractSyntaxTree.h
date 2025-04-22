//
// Created by dylan on 4/12/2025.
//

#ifndef ABSTRACTSYNAXTREE_H
#define ABSTRACTSYNAXTREE_H

#include <iostream>
#include <unordered_map>

#include "Token.h"
#include "RecursiveDescentParser.h"
#include "SymbolTable.h"
using namespace std;

class Interpreter;


class AbstractSyntaxTree {
private:
    struct ASTNode {
        SymbolTable::Symbol* symbolPointer;
        string type;
        string typeName;
        ASTNode* leftChild;
        ASTNode* rightSibling;
        int lineNumber = 0;
        string name;

        ASTNode(string name) : leftChild(nullptr), rightSibling(nullptr), name(name) {}
        ASTNode(string name, string type, string typeName) : leftChild(nullptr), rightSibling(nullptr), name(name),
        type(type), typeName(typeName)
        {}
    };

    RecursiveDescentParser::CSTNode* CSTRoot;
    SymbolTable::Symbol* currentSymbolPointer;
    ASTNode* ASTRoot = nullptr;
    ASTNode* currentASTNode = nullptr;
    bool insideFuncDeclaration = false;
    bool inStatement = false;
    bool inIfStatement = false;
    bool inVariableList = false;
    bool inForStatement = false;
    bool inForStatement2 = false;
    bool inForStatement3 = false;
    int parenDepth = -1;

    void insertNode(RecursiveDescentParser::CSTNode *currentCSTNode, string name, string type, string typeName);
    void printASTHelper(ASTNode *node, bool isLeftChild) const;
    void BuildASTHelper(RecursiveDescentParser::CSTNode* CSTRoot);
    ASTNode* getRoot();

public:
    friend class SymbolTable;
    friend class Interpreter;

    AbstractSyntaxTree(RecursiveDescentParser& CST);

    AbstractSyntaxTree(RecursiveDescentParser &CST, SymbolTable &ST);

    void printAST() const;

    void linkASTandSymbolTable();

    void buildAST();

    void testLinking();

    void printSymbolTableForDeclaration(SymbolTable::Symbol* s);

};

#endif //ABSTRACTSYNAXTREE_H
