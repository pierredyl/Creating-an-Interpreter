//
// Created by dylan on 3/6/2025.
//

#include "RecursiveDescentParser.h"

#include <iostream>
#include "RecursiveDescentParser.h"
using namespace std;


RecursiveDescentParser::RecursiveDescentParser(const vector<Token>& tokenList) {
    tokens = tokenList;
    state = PROGRAM;
    currentTokenIndex = 0;

    // Create a dummy program token
    Token programToken(PROGRAM_TOKEN, "program_root", 0);
    Root = new CSTNode(programToken);  // Initialize the root node

    if (!tokens.empty()) {
        parseProgram();  // Begin parsing the program
    } else {
        Root = nullptr;  // If no tokens, set root to nullptr
    }
}

bool RecursiveDescentParser::consumeToken(TokenType expectedType) {
    if (currentTokenIndex < tokens.size() && tokens[currentTokenIndex].getType() == expectedType) {
        currentTokenIndex++;
        return true;
    } else {
        std::cerr << "Error: Expected token type " << expectedType
                  << " but found " << tokens[currentTokenIndex].getType() << std::endl;
        return false;
    }
}

void RecursiveDescentParser::insertNode(Token currToken) {
    CSTNode* newNode = new CSTNode(currToken);
    if (newNode->token.getLineNumber() == currentNode->token.getLineNumber()) {
        currentNode->rightSibling = newNode;
        currentNode->leftChild = nullptr;
    } else {
        currentNode->leftChild = newNode;
        currentNode->rightSibling = nullptr;
    }
    currentNode = newNode;
}

void RecursiveDescentParser::updateState(parseState newState) {
    prevState = state;
    state = newState;
}


void RecursiveDescentParser::parseProgram() {
    TokenType type;
    CSTNode* programNode = Root;
    currentNode = programNode;



    while (currentTokenIndex < tokens.size() && state != ACCEPT) {
        Token currentToken = tokens[currentTokenIndex];
        cout << "current state: " << state << ", ";
        cout << "current token: " << currentToken.getValue() << endl;

        switch (state) {
            case PROGRAM:
                type = currentToken.getType();
            if (type == KEYWORD_FUNCTION) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(FUNCTION_DECLARATION);
            } else if (type == KEYWORD_MAIN) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(MAIN_PROCEDURE);
            } else if (type == KEYWORD_PROCEDURE) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(PROCEDURE_DECLARATION);
            } else if (type == KEYWORD_CHAR || type == KEYWORD_BOOL || type == KEYWORD_INT) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(DECLARATION_STATEMENT);
            }
            break;

            case FUNCTION_DECLARATION:
                type = currentToken.getType();

            if ((type == KEYWORD_CHAR || type == KEYWORD_BOOL || type == KEYWORD_INT) &&
                tokens[currentTokenIndex + 1].getType() == R_PAREN) {
                updateState(PARAMETER_LIST);
                insertNode(currentToken);
                consumeToken(type);
                } else if (type == KEYWORD_CHAR || type == KEYWORD_BOOL || type == KEYWORD_INT) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == IDENTIFIER) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == L_PAREN && tokens[currentTokenIndex + 1].getType() == KEYWORD_VOID) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == R_PAREN) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == L_BRACE) {
                    insertNode(currentToken);
                    consumeToken(type);
                    updateState(COMPOUND_STATEMENT);
                } else if (type == R_BRACE) {
                    insertNode(currentToken);
                    consumeToken(type);
                    updateState(PROGRAM);
                } else if (type == L_PAREN) {
                    insertNode(currentToken);
                    consumeToken(type);
                }
            break;

            case PARAMETER_LIST:
                type = currentToken.getType();

            if (type == IDENTIFIER) {
                insertNode(currentToken);
                consumeToken(type);
            } else if (type == KEYWORD_CHAR || type == KEYWORD_BOOL || type == KEYWORD_INT) {
                insertNode(currentToken);
                consumeToken(type);
            }
            break;

            case COMPOUND_STATEMENT:
                type = currentToken.getType();
            if (type == KEYWORD_CHAR || type == KEYWORD_BOOL || type == KEYWORD_INT) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(DECLARATION_STATEMENT);
            } else if (type == IDENTIFIER) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(ASSIGNMENT_STATEMENT);
            } else if (type == KEYWORD_FOR || type == KEYWORD_WHILE) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(ITERATION_STATEMENT);
            } else if (type == KEYWORD_IF) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(SELECTION_STATEMENT);
            } else if (type == KEYWORD_RETURN) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(RETURN_STATEMENT);
            } else if (type == KEYWORD_PRINTF) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(PRINTF_STATEMENT);
            } else if (R_BRACE) {
                insertNode(currentToken);
                consumeToken(type);
            }
            break;

            case DECLARATION_STATEMENT:
                type = currentToken.getType();

            if (type == IDENTIFIER) {
                insertNode(currentToken);
                consumeToken(type);
            } else if (type == SEMICOLON) {
                insertNode(currentToken);
                consumeToken(type);
                state = prevState;
            }
            break;

            case ASSIGNMENT_STATEMENT:
                type = currentToken.getType();
            if (type == ASSIGNMENT_OPERATOR) {
                insertNode(currentToken);
                consumeToken(type);
            } else if (type == IDENTIFIER || type == INTEGER) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(NUMERICAL_EXPRESSION);
            } else if (type == SEMICOLON) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(COMPOUND_STATEMENT);
            }
            break;

            case NUMERICAL_EXPRESSION:
                type = currentToken.getType();
            if (type == L_PAREN) {
                insertNode(currentToken);
                consumeToken(type);
            } else if (type == IDENTIFIER || type == INTEGER) {
                insertNode(currentToken);
                consumeToken(type);
            } else if (type == PLUS || type == MINUS || type == ASTERISK || type == MODULO || type == DIVIDE) {
                insertNode(currentToken);
                consumeToken(type);
            } else if (type == R_PAREN &&
                tokens[currentTokenIndex + 1].getType() == IDENTIFIER ||
                tokens[currentTokenIndex + 1].getType() == INTEGER ||
                tokens[currentTokenIndex + 1].getType() == PLUS ||
                tokens[currentTokenIndex + 1].getType() == MINUS ||
                tokens[currentTokenIndex + 1].getType() == ASTERISK ||
                tokens[currentTokenIndex + 1].getType() == MODULO ||
                tokens[currentTokenIndex + 1].getType() == DIVIDE){
                insertNode(currentToken);
                consumeToken(type);
                } else if (type == R_PAREN) {
                    insertNode(currentToken);
                    consumeToken(type);
                    state = prevState;
                } else if (type == SEMICOLON) {
                    insertNode(currentToken);
                    consumeToken(type);
                    updateState(COMPOUND_STATEMENT);
                }
            break;

            case SELECTION_STATEMENT:
                type = currentToken.getType();

            if (type == L_PAREN) {
                insertNode(currentToken);
                consumeToken(type);
            } else if (type == BOOLEAN_TRUE || type == BOOLEAN_FALSE || type == IDENTIFIER) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(BOOLEAN_EXPRESSION);
            }
            break;

            case BOOLEAN_EXPRESSION:
                type = currentToken.getType();
            if ((type == IDENTIFIER) && tokens[currentTokenIndex + 1].getType() == BOOLEAN_AND ||
               tokens[currentTokenIndex + 1].getType() == BOOLEAN_OR) {
                insertNode(currentToken);
                consumeToken(type);
               } else if (type == IDENTIFIER && tokens[currentTokenIndex + 1].getType() == BOOLEAN_AND ||
                   tokens[currentTokenIndex + 1].getType() == BOOLEAN_OR) {
                   insertNode(currentToken);
                   consumeToken(type);
                   } else if ((type == LT_EQUAL || type == GT_EQUAL) &&
                       tokens[currentTokenIndex + 1].getType() == IDENTIFIER ||
                       tokens[currentTokenIndex + 1].getType() == INTEGER) {
                       insertNode(currentToken);
                       consumeToken(type);
                       updateState(NUMERICAL_EXPRESSION);
                       } else if (type == BOOLEAN_TRUE || type == BOOLEAN_FALSE || type == IDENTIFIER ||
                           type == BOOLEAN_EQUAL) {
                           insertNode(currentToken);
                           consumeToken(type);
                       } else if (type == L_BRACE) {
                           insertNode(currentToken);
                           consumeToken(type);
                           updateState(BLOCK_STATEMENT);
                       }
            break;

            case BLOCK_STATEMENT:
                updateState(COMPOUND_STATEMENT);
            break;

            case RETURN_STATEMENT:
                type = currentToken.getType();
            if (type == IDENTIFIER || type == INTEGER) {
                insertNode(currentToken);
                consumeToken(type);
            } else if (type == SEMICOLON) {
                insertNode(currentToken);
                consumeToken(type);
                updateState(COMPOUND_STATEMENT);
            }
            break;

            case PRINTF_STATEMENT:
                type = currentToken.getType();

                if (type == L_PAREN) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == DOUBLE_QUOTE) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == DOUBLE_QUOTED_STRING) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == R_PAREN) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == SEMICOLON) {
                    insertNode(currentToken);
                    consumeToken(type);
                    updateState(COMPOUND_STATEMENT);
                } else if (type == COMMA) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == IDENTIFIER) {
                    insertNode(currentToken);
                    consumeToken(type);
                }
            break;

            case PROCEDURE_DECLARATION:
                type = currentToken.getType();
                if (type == KEYWORD_CHAR || type == KEYWORD_BOOL || type == KEYWORD_INT) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == IDENTIFIER) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == L_PAREN && tokens[currentTokenIndex + 1].getType() == KEYWORD_VOID) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == R_PAREN) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == L_BRACE) {
                    insertNode(currentToken);
                    consumeToken(type);
                    updateState(COMPOUND_STATEMENT);
                } else if (type == R_BRACE) {
                    insertNode(currentToken);
                    consumeToken(type);
                    updateState(PROGRAM);
                } else if (type == L_PAREN) {
                    insertNode(currentToken);
                    consumeToken(type);
                }
            break;

            case ITERATION_STATEMENT:
                type = currentToken.getType();

        }
        }
    }


void RecursiveDescentParser::printCSTHelper(CSTNode* node, bool isLeftChild) const {
    // Base case: If the node is null, return
    if (node == nullptr) {
        return;
    }

    if (isLeftChild) {
        cout << "\n";
        cout << "↓\n";
    }

    cout << node->token.getValue();

    if (node->rightSibling != nullptr) {
        cout << " -> ";
    }

    // If there's a left child, print it recursively
    if (node->leftChild != nullptr) {
        printCSTHelper(node->leftChild, true);
    }

    // If there's a right sibling, print it recursively
    if (node->rightSibling != nullptr) {
        printCSTHelper(node->rightSibling, false);
    }
}





void RecursiveDescentParser::printCST() const {
    if (Root) {
        printCSTHelper(Root, false);
    } else {
        cout << "CST is empty." << endl;
    }
}