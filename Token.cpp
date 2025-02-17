//
// Created by dylan on 2/16/2025.
//


#include "Token.h"
#include <iostream>
#include <unordered_map>

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
        case L_PAREN:
            return "L_PAREN";
        case R_PAREN:
            return "R_PAREN";
        case L_BRACE:
            return "L_BRACE";
        case R_BRACE:
            return "R_BRACE";
        case KEYWORD_PROCEDURE:
            return "KEYWORD_PROCEDURE";
        case KEYWORD_INT:
            return "KEYWORD_INT";
        case KEYWORD_MAIN:
            return "KEYWORD_MAIN";
        case KEYWORD_RETURN:
            return "KEYWORD_RETURN";
        case KEYWORD_FUNCTION:
            return "KEYWORD_FUNCTION";
        case KEYWORD_VOID:
            return "KEYWORD_VOID";
        case KEYWORD_PRINTF:
            return "KEYWORD_PRINTF";
        case BOOLEAN_EQUAL:
            return "BOOLEAN_EQUAL";
        case SEMICOLON:
            return "SEMICOLON";
        case ASSIGNMENT_OPERATOR:
            return "ASSIGNMENT_OPERATOR";
        case MINUS:
            return "MINUS";
        case INTEGER:
            return "INTEGER";
        case DOUBLE_QUOTE:
            return "DOUBLE_QUOTE";
        case MODULO:
            return "MODULO";
        case DOUBLE_QUOTED_STRING:
            return "DOUBLE_QUOTED_STRING";
        case COMMA:
            return "COMMA";
    }
}
