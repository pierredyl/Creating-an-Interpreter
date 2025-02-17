//
// Created by dylan on 2/16/2025.
//

#include <Tokenizer.h>
#include <Token.h>
#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

enum State {
    S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, S13, S14, S15, S16, S17, S18, S19, S20, S21, S22, S23,
    S24, S25
};

const unordered_map<string, TokenType> Tokenizer::keywordMap = {
    {"int", KEYWORD_INT},
    {"main", KEYWORD_MAIN},
    {"procedure", KEYWORD_PROCEDURE},
    {"void", KEYWORD_VOID},
    {"printf", KEYWORD_PRINTF},
    {"if", KEYWORD_IF},
    {"else", KEYWORD_ELSE},
    {"function", KEYWORD_FUNCTION},
    {"return", KEYWORD_RETURN},
    {"TRUE", KEYWORD_TRUE},
    {"FALSE", KEYWORD_FALSE},
    {"read", KEYWORD_READ}
};


Tokenizer::Tokenizer(const string code) {
    input = code;
    pos = 0;
}

Token Tokenizer::getNextToken() {
    State currentState = S1;
    Token token;
    size_t startPos;
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
        cout << currentState << endl;
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
                } else if (currentChar == '%') {
                    currentState = S14;
                } else if (currentChar == '*') {
                    currentState = S15;
                } else if (currentChar == '+') {
                    currentState = S16;
                } else if (isdigit(currentChar)) {
                    startPos = pos;
                    currentState = S17;
                } else if (currentChar == '>' && input[pos+1] == '=') {
                    currentState = S18;
                } else if (currentChar == '<' && input[pos+1] == '=') {
                    currentState = S19;
                } else if (currentChar == '&') {
                    currentState = S20;
                } else if (currentChar == '\'') {
                    currentState = S21;
                } else if (currentChar == '<') {
                    currentState = S22;
                } else if (currentChar == '>') {
                    currentState = S23;
                } else if (currentChar == '[') {
                    currentState = S24;
                } else if (currentChar == ']') {
                    currentState = S25;
                }
            break;

            //As long as the input is alphanumeric, keep taking input.
            //Once it is no longer alphanumeric, create a token. Once
            //token returned, return to S1.
            case S2:
                while (pos < input.size() && (isalnum(input[pos]) || input[pos] == '_')) {
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
                if (input[pos + 1] != '"') {
                    startPos = pos;
                    currentState = S12;
                    break;
                }
                token.setValue("\"");
                token.setType(DOUBLE_QUOTE);
                currentState = S12;
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

            case S14:
                pos++;
                token.setValue("%");
                token.setType(MODULO);
                currentState = S1;
                return token;

            case S15:
                pos++;
                token.setValue("*");
                token.setType(ASTERISK);
                currentState = S1;
                return token;

            case S16:
                pos++;
                token.setValue("+");
                token.setType(PLUS);
                currentState = S1;
                return token;

            case S17:
                while (pos < input.size() && isdigit(input[pos])) {
                    pos++;
                }
            token.setValue(input.substr(startPos, pos - startPos));
            token.setType(INTEGER);
            currentState = S1;
            return token;

            case S18:
                pos += 2;
                token.setValue(">=");
                token.setType(GT_EQUAL);
                currentState = S1;
                return token;


            case S19:
                pos += 2;
                token.setValue("<=");
                token.setType(LT_EQUAL);
                currentState = S1;
                return token;

            case S20:
                pos++;
                if (input[pos] == '&') {
                    token.setValue("&&");
                    token.setType(BOOLEAN_AND);
                    currentState = S1;
                    pos++;
                    return token;
                }

            case S21:
                pos++;
                startPos = pos;
                while (pos < input.size() && input[pos] != '\'') {
                    pos++;
                }
                token.setValue(input.substr(startPos, pos - startPos));
                token.setType(SINGLE_QUOTED_STRING);
                currentState = S1;
                pos++;
                return token;

            case S22:
                pos++;
                token.setValue("<");
                token.setType(LT);
                currentState = S1;
                return token;

            case S23:
                pos++;
                token.setValue(">");
                token.setType(GT);
                currentState = S1;
                return token;

            case S24:
                pos++;
                token.setValue("[");
                token.setType(L_BRACKET);
                currentState = S1;
                return token;

            case S25:
                pos++;
                token.setValue("]");
                token.setType(R_BRACKET);
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
