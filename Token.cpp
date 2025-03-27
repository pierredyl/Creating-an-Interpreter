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
    lineNumber = 0;
}

Token::Token(TokenType t, string s, int line) {
    type = t;
    value = s;
    lineNumber = line;
}


void Token::setValue(const string& val) {
    value = val;
}

void Token::setType(const TokenType& t) {
    type = t;
}

void Token::setLineNumber(int line) {
    lineNumber = line;
}

size_t Token::getLineNumber() const {
    return lineNumber;
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
        case KEYWORD_IF:
            return "KEYWORD_IF";
        case KEYWORD_ELSE:
            return "KEYWORD_ELSE";
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
        case ASTERISK:
            return "ASTERISK";
        case PLUS:
            return "PLUS";
        case GT_EQUAL:
            return "GT_EQUAL";
        case LT_EQUAL:
            return "LT_EQUAL";
        case KEYWORD_CHAR:
            return "KEYWORD_CHAR";
        case GT:
            return "GT";
        case LT:
            return "LT";
        case DIVIDE:
            return "/";
        case SINGLE_QUOTED_STRING:
            return "SINGLE_QUOTED_STRING";
        case BOOLEAN_AND:
            return "BOOLEAN_AND";
        case L_BRACKET:
            return "L_BRACKET";
        case R_BRACKET:
            return "R_BRACKET";
        case KEYWORD_READ:
            return "KEYWORD_READ";
        case KEYWORD_TRUE:
            return "KEYWORD_TRUE";
        case KEYWORD_FALSE:
            return "KEYWORD_FALSE";
        case KEYWORD_WHILE:
            return "KEYWORD_WHILE";
        case SINGLE_QUOTE:
            return "SINGLE_QUOTE";
        case LOGICAL_NOT:
            return "LOGICAL_NOT";
        case KEYWORD_FOR:
            return "KEYWORD_FOR";
        case KEYWORD_BOOL:
            return "KEYWORD_BOOL";

    }
}
