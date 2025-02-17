//
// Created by dylan on 2/16/2025.
//

#include <Tokenizer.h>
#include <Token.h>
#include <iostream>
#include <vector>

using namespace std;

enum State {S1, S2};


Tokenizer::Tokenizer(const string code) {
    input = code;
    pos = 0;
}

Token Tokenizer::getNextToken() {
    Token token;
    size_t startPos;
    State currentState = S1;

    while (pos < input.size()) {
        //This while loop will skip whitespace until a valid input is found.
        while (pos < input.size() && isspace(input[pos])) {
            pos++;
        }

        //If the end of the input file is reached, return error token.
        if (pos >= input.size()) {
            token.setType(ERROR);
            return token;
        }

        char currentChar = input[pos];
        switch(currentState) {
            //If currentChar is alphanumeric, move to state 2
            case S1:
                if (isalpha(currentChar)) {
                    startPos = pos;
                    currentState = S2;
                }

            break;

            //As long as the input is alphanumeric, keep taking input.
            //Once it is no longer alphanumeric, create a token. Once
            //token returned, return to S1.
            case S2:
                while (pos < input.size() && isalnum(input[pos])) {
                    pos++;
                }

            token.setValue(input.substr(startPos, pos - startPos));
            token.setType(IDENTIFIER);
            currentState = S1;

            return token;
        }
    }

    // If we exit the loop without finding valid tokens, return an error.
    token.setType(ERROR);
    return token;
}

vector<Token> Tokenizer::tokenize() {
    vector<Token> tokens;
    Token token;

    //Keep storing tokens until the ERROR token is encountered.
    while ((token = getNextToken()).getType() != ERROR) {
        tokens.push_back(token);
    }

    return tokens;

}
