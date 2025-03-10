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

  enum parseState {PROGRAM, FUNCTION_DECLARATION, PROCEDURE_DECLARATION, MAIN_PROCEDURE,
    EXPRESSION, IDENTIFIER_STATE, PARAMETER_LIST, COMPOUND_STATEMENT, SELECTION_STATEMENT, BOOLEAN_EXPRESSION,
    DECLARATION_STATEMENT, ASSIGNMENT_STATEMENT, NUMERICAL_EXPRESSION, BLOCK_STATEMENT, STATEMENT, RETURN_STATEMENT,
    ITERATION_STATEMENT, PRINTF_STATEMENT,
    ACCEPT};



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
  parseState state;
  parseState prevState;
  void printCSTHelper(CSTNode *node, bool isLeftChild) const;


public:
  RecursiveDescentParser(const vector<Token>& tokens);
  bool consumeToken(TokenType expectedType);

  void insertNode(Token currToken);

  void updateState(parseState newState);

  bool checkSameLine(int, int);
  void parseProgram();
  vector<Token> getTokens();
  void printCST() const;

};

#endif //RECURSIVEDESCENTPARSER_H