//
// Created by dylan on 2/16/2025.
//

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.h"

using namespace std;

enum State {
    S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16, S17, S18, S19, S20, S21, S22, S23,
    S24, S25, S26
};
class Tokenizer {
private:
    string input;           // Input (code) to tokenize.
    size_t pos;             // Index of where we are currently at.
    size_t startPos;        // Starting position of a token.
    size_t lineCount;       // Keeps track of line numbers.
    bool isKeyword;         // Flag to check if current token is a keyword.
    bool insideString;      // Flag for inside string handling.
    State currentState;     // Holds the current state of the tokenizer.
    static const unordered_map<string, TokenType> keywordMap; // Dictionary of keywords.

public:
    Tokenizer(const string &code);
    Token getNextToken();              // Function to get the next token.
    vector<Token> tokenize();          // Function to tokenize the input.
};

#endif //TOKENIZER_H
