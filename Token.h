//
// Created by dylan on 2/16/2025.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

using namespace std;

enum TokenType {
    IDENTIFIER,
    L_PAREN,
    R_PAREN,
    L_BRACE,
    R_BRACE,
    KEYWORD_INT,
    KEYWORD_RETURN,
    KEYWORD_PROCEDURE,
    KEYWORD_MAIN,
    KEYWORD_FUNCTION,
    KEYWORD_VOID,
    KEYWORD_PRINTF,
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_CHAR,
    KEYWORD_READ,
    KEYWORD_TRUE,
    KEYWORD_FALSE,
    DOUBLE_QUOTE,
    MODULO,
    DOUBLE_QUOTED_STRING,
    SEMICOLON,
    BOOLEAN_EQUAL,
    ASSIGNMENT_OPERATOR,
    MINUS,
    INTEGER,
    COMMA,
    ASTERISK,
    PLUS,
    GT_EQUAL,
    LT_EQUAL,
    GT,
    LT,
    SINGLE_QUOTED_STRING,
    BOOLEAN_AND,
    L_BRACKET,
    R_BRACKET,
    ERROR
};

class Token {
    private:
        TokenType type;
        string value;

    public:
        Token();
        //Setters
        void setValue(const string& value);
        void setType(const TokenType& type);

        //Getters
        TokenType getType() const;
        string getValue() const;

        string typeToString(TokenType type) const;
};

#endif //TOKEN_H
