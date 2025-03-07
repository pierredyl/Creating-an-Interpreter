//
// Created by Dylan Pierre on 3/6/25.
//
#include <string>
#include "Token.h"
#include <Vector>

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


public:

};

#endif //RECURSIVEDESCENTPARSER_H
