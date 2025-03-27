//
// Created by dylan on 2/16/2025.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

using namespace std;

enum TokenType {
    PROGRAM_TOKEN,
    IDENTIFIER,
    L_PAREN,
    KEYWORD_FOR,
    KEYWORD_WHILE,
    R_PAREN,
    LOGICAL_NOT,
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
    BOOLEAN_TRUE,
    BOOLEAN_FALSE,
    BOOLEAN_OR,
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
    SINGLE_QUOTE,
    DIVIDE,
    BOOLEAN_AND,
    L_BRACKET,
    R_BRACKET,
    KEYWORD_BOOL,
    ERROR
};

class Token {
    private:
        TokenType type;
        string value;
        int lineNumber;

    public:
        Token();
        Token(TokenType, string, int);
        //Setters
        void setValue(const string& value);
        void setType(const TokenType& type);
        void setLineNumber(int);

        //Getters
        TokenType getType() const;
        string getValue() const;
        size_t getLineNumber() const;

        string typeToString(TokenType type) const;
};

#endif //TOKEN_H
