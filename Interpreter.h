//
// Created by dylan on 4/19/2025.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H


#include "AbstractSyntaxTree.h"
#include "SymbolTable.h"

using namespace std;

class Interpreter {

public:
    Interpreter(AbstractSyntaxTree, SymbolTable);

    void interpret();

private:
    //AST Root Node
    AbstractSyntaxTree::ASTNode* ASTRoot;

    //Counter for Main
    AbstractSyntaxTree::ASTNode* currentMainProcedureNode;
    AbstractSyntaxTree::ASTNode* mainProcedureDeclarationNode;
    int mainScope;
    int mainBraceDepth;

    //Counters for function
    AbstractSyntaxTree::ASTNode* currentFunctionNode;
    AbstractSyntaxTree::ASTNode* functionDeclarationNode;
    int functionScope;
    int functionBraceDepth;
    int ifStatementDepth;

    //SymbolTable Root Node
    SymbolTable::Symbol* symbolRoot;


    AbstractSyntaxTree::ASTNode* findMainProcedure(AbstractSyntaxTree::ASTNode*);

    AbstractSyntaxTree::ASTNode *findFunctionNode(AbstractSyntaxTree::ASTNode *currentNode, string identifierName);

    void executeMain();

    AbstractSyntaxTree::ASTNode* moveNodeDownOneStatement(AbstractSyntaxTree::ASTNode *node);

    AbstractSyntaxTree::ASTNode *executeForLoop(AbstractSyntaxTree::ASTNode *node, int scope);

    AbstractSyntaxTree::ASTNode *FindEndOfForLoopBody(AbstractSyntaxTree::ASTNode *node);

    void executePrintf();

    int executeFunctionCall(string);
    bool isFunctionCall(AbstractSyntaxTree::ASTNode *node);
    void executeAssignment(AbstractSyntaxTree::ASTNode*, int);
    AbstractSyntaxTree::ASTNode* executeIfStatement(AbstractSyntaxTree::ASTNode *, int scope);

    AbstractSyntaxTree::ASTNode *executeBlock(AbstractSyntaxTree::ASTNode *node, int scope);

    bool isIfStatementTrue(AbstractSyntaxTree::ASTNode *node, int scope);
};


#endif //INTERPRETER_H
