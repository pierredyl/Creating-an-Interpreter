//
// Created by dylan on 2/16/2025.
//


#include "Token.h"
#include <iostream>

using namespace std;

Token::Token() {
    type = ERROR;
    value = "";
}

void Token::setValue(const string& val) {
    value = val;
}

void Token::setType(const TokenType& t) {
    type = t;
}

TokenType Token::getType() const {
    return type;
}

string Token::getValue() const {
    return value;
}

string Token::typeToString(TokenType type) const{
    switch(type) {
        case IDENTIFIER:
            return "IDENTIFIER";
        case ERROR:
            return "ERROR";
    }
}
