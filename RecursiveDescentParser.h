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
  };

  CSTNode * Root;
  vector<Token> tokens;
  Token currentToken;

  void printCSTHelper(CSTNode *node, bool isLeftChild) const;

public:
  RecursiveDescentParser(const vector<Token>& tokens);
  void printCST() const;
  void setTokens(const vector<Token> &tokens);
  vector<Token> getTokens();
};

#endif //RECURSIVEDESCENTPARSER_H
