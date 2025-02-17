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
