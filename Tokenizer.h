//
// Created by dylan on 2/16/2025.
//

#ifndef TOKENIZER_H
#define TOKENIZER_H


#include <iostream>
#include <string>
#include <vector>
#include "Token.h"

using namespace std;

class Tokenizer {
    private:
        string input;           //input (code) to tokenize.
        size_t pos;             //index of where we are currently at.

    public:
        Tokenizer(const string code);
        Token getNextToken();
        vector<Token> tokenize();
};

#endif //TOKENIZER_H
