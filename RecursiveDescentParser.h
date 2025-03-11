//
// Created by Dylan Pierre on 3/6/25.
//
#include <string>
#include "Token.h"
#include <Vector>

using namespace std;

#ifndef RECURSIVEDESCENTPARSER_H
#define RECURSIVEDESCENTPARSER_H



class RecursiveDescentParser {



private:
  struct CSTNode {
    Token token;
    CSTNode* leftChild;
    CSTNode* rightSibling;

    CSTNode(Token t) : token(t), leftChild(nullptr), rightSibling(nullptr) {}
    CSTNode(Token t, CSTNode* left, CSTNode* right)
        : token(t), leftChild(left), rightSibling(right) {}
  };

  CSTNode* Root;
  vector<Token> tokens;
  int currentTokenIndex;
  CSTNode* currentNode = nullptr;
  void printCSTHelper(CSTNode *node, bool isLeftChild) const;


public:
  RecursiveDescentParser(const vector<Token>& tokens);
  bool consumeToken(TokenType expectedType);
  void insertNode(Token currToken);
  void parseProgram();
  void functionDeclaration();

  void compoundStatement();

  void statement();

  void declarationStatement();

  void iterationStatement();

  void returnStatement();

  void printfStatement();

  void assignmentStatement();

  void numericalExpression();

  void booleanExpression();

  void selectionStatement();

  void identifierList();

  void parameterList();

  void procedureState();

  void procedureMainState();

  bool checkifNumericalExpression();

  bool checkifBooleanExpression();

  bool checkIfNumericalOperand();

  vector<Token> getTokens();
  void printCST() const;

};

#endif //RECURSIVEDESCENTPARSER_H