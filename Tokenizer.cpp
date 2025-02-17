//
// Created by dylan on 2/16/2025.
//

#include <Tokenizer.h>
#include <Token.h>
#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

enum State {S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13};

const unordered_map<string, TokenType> Tokenizer::keywordMap = {
    {"int", KEYWORD_INT},
    {"main", KEYWORD_MAIN},
    {"procedure", KEYWORD_PROCEDURE},
    {"void", KEYWORD_VOID},
    {"printf", KEYWORD_PRINTF}
};


Tokenizer::Tokenizer(const string code) {
    input = code;
    pos = 0;
}

Token Tokenizer::getNextToken() {
    Token token;
    size_t startPos;
    State currentState = S1;
    bool isKeyword = false;

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

            case S1:
                if (isalpha(currentChar)) {
                    startPos = pos;
                    currentState = S2;
                } else if (currentChar == '(') {
                    currentState = S3;
                } else if (currentChar == ')') {
                    currentState = S4;
                } else if (currentChar == '{') {
                    currentState = S5;
                } else if (currentChar == '}') {
                    currentState = S6;
                } else if (currentChar == '=') {
                    currentState = S7;
                } else if (currentChar == '-') {
                    currentState = S8;
                } else if (currentChar == ';') {
                    currentState = S10;
                } else if (currentChar == '"') {
                    currentState = S11;
                } else if (currentChar == ',') {
                    currentState = S13;
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

                //check if IDENTIFIER is actually a keyword. If it is, set the token's type.
                for (const auto& pair : keywordMap) {
                    if (pair.first == token.getValue()) {
                        token.setType(pair.second);
                        isKeyword = true;
                    }
                }
                if (!isKeyword) {
                    token.setType(IDENTIFIER);
                    currentState = S1;
                }

                return token;

            case S3:
                pos++;
                token.setValue("(");
                token.setType(L_PAREN);
                currentState = S1;
                return token;

            case S4:
                pos++;
                token.setValue(")");
                token.setType(R_PAREN);
                currentState = S1;
                return token;

            case S5:
                pos++;
                token.setValue("{");
                token.setType(L_BRACE);
                currentState = S1;
                return token;

            case S6:
                pos++;
                token.setValue("}");
                token.setType(R_BRACE);
                currentState = S1;
                return token;

            //Handles assigment operators and equals boolean comparisons.
            case S7:
                pos++;
                if (input[pos] == '=') {
                    token.setValue("==");
                    token.setType(BOOLEAN_EQUAL);
                    currentState = S1;
                    return token;
                } else if (input[pos] != '=') {
                    token.setValue("=");
                    token.setType(ASSIGNMENT_OPERATOR);
                    currentState = S1;
                    return token;
                }

            case S8:
                pos++;
                if (isdigit(input[pos])) {
                    startPos = pos - 1;
                    currentState = S9;
                    break;
                }
                token.setValue("-");
                token.setType(MINUS);
                currentState = S1;
                return token;

            case S9:
                if (isdigit(input[pos])) {
                    while (pos < input.size() && isalnum(input[pos])) {
                        pos++;
                    }
                    token.setValue(input.substr(startPos, pos - startPos));
                    token.setType(INTEGER);
                    currentState = S1;
                    return token;
                }

            case S10:
                pos++;
                token.setValue(";");
                token.setType(SEMICOLON);
                currentState = S1;
                return token;

            case S11:
                pos++;
                if (isalnum(input[pos])) {
                    startPos = pos;
                    currentState = S12;
                    break;
                }
                token.setValue("\"");
                token.setType(DOUBLE_QUOTE);
                currentState = S1;
                return token;

            case S12:
                while (pos < input.size() && (input[pos] != '"')) {
                    pos++;
                }
                token.setValue(input.substr(startPos, pos - startPos));
                token.setType(DOUBLE_QUOTED_STRING);
                currentState = S1;
                pos++;
                return token;

            case S13:
                pos++;
                token.setValue(",");
                token.setType(COMMA);
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
