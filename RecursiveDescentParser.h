//
// Created by Dylan Pierre on 3/6/25.
//
#include <string>
#include "Token.h"
#include <vector>

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
  bool inSelectionStatement = false;
  bool inIterationStatement = false;
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
  bool isStatement() const;
  void declarationStatement();
  bool checkNumericalOperand();
  bool isDatatypeSpecifier();

  bool isIdentifierList();

  bool isIdentifierArrayList();

  bool checkNumericalOperator();

  bool isRelationalExpression();

  void initializationExpression();

  void iterationStatement();

  void iterationAssignment();

  void returnStatement();
  void expression();
  void printfStatement();
  void assignmentStatement();
  void numericalExpression();

  bool isIdentifierArrayParameterList();

  void userDefinedFunction();

  void identifierAndIdentifierArrayParameterList();

  void booleanExpression();

  void numericalOperator();

  void numericalOperand();

  void selectionStatement();
  void blockStatement();
  void identifierAndArrayList();
  void identifierArrayList();
  void identifierList();
  void parameterList();
  void procedureState();
  void procedureMainState();
  bool checkifNumericalExpression();
  bool checkifBooleanExpression();
  vector<Token> getTokens();
  void printCST() const;

};

#endif //RECURSIVEDESCENTPARSER_H