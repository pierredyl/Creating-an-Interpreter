//
// Created by dylan on 4/12/2025.
//

#ifndef ABSTRACTSYNAXTREE_H
#define ABSTRACTSYNAXTREE_H

#include <iostream>
#include <unordered_map>

#include "Token.h"
#include "RecursiveDescentParser.h"
using namespace std;

class AbstractSyntaxTree {
private:
    struct ASTNode {
        Token token;
        ASTNode* leftChild;
        ASTNode* rightSibling;
        int lineNumber = 0;
        string name;

        ASTNode(string name) : leftChild(nullptr), rightSibling(nullptr), name(name) {}
        ASTNode(Token t, ASTNode* left, ASTNode* right)
            : token(t), leftChild(left), rightSibling(right) {}
    };

    RecursiveDescentParser::CSTNode* CSTRoot;
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

    void insertNode(RecursiveDescentParser::CSTNode* node, string name);
    void printASTHelper(ASTNode *node, bool isLeftChild) const;
    void BuildASTHelper(RecursiveDescentParser::CSTNode* CSTRoot);

public:
    static const unordered_map<TokenType, string, int> typeMap;

    AbstractSyntaxTree(RecursiveDescentParser& CST);
    void printAST() const;
    void buildAST();


};

#endif //ABSTRACTSYNAXTREE_H
