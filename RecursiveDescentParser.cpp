//
// Created by dylan on 3/6/2025.
//

#include "RecursiveDescentParser.h"

#include <iostream>
#include "RecursiveDescentParser.h"
using namespace std;


RecursiveDescentParser::RecursiveDescentParser(const vector<Token>& tokenList) {
    tokens = tokenList;
    currentTokenIndex = 1;
    Token firstToken(tokens[0].getType(), tokens[0].getValue(), tokens[0].getLineNumber());
    Root = new CSTNode(firstToken);
    currentNode = Root;

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

void RecursiveDescentParser::parseProgram() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_FUNCTION ||
        currentNode->token.getType() == KEYWORD_FUNCTION) {
        functionDeclaration();
    } else if (tokens[currentTokenIndex].getType() == KEYWORD_PROCEDURE &&
         tokens[currentTokenIndex + 1].getType() == KEYWORD_MAIN) {
        procedureMainState();
    } else if (tokens[currentTokenIndex].getType() == KEYWORD_PROCEDURE ||
        currentNode->token.getType() == KEYWORD_PROCEDURE) {
        procedureState();
    }
}

void RecursiveDescentParser::functionDeclaration() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_FUNCTION) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_FUNCTION);
        cout << "Consumed KEYWORD_FUNCTION" << endl;
    }

    if (tokens[currentTokenIndex].getType() == KEYWORD_INT ||
        tokens[currentTokenIndex].getType() == KEYWORD_CHAR ||
        tokens[currentTokenIndex].getType() == KEYWORD_BOOL) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed DATATYPE_SPECIFIER" << endl;
        }

    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consumed IDENTIFIER" << endl;
    }

    if (tokens[currentTokenIndex].getType() == L_PAREN &&
        (tokens[currentTokenIndex+1].getType() == KEYWORD_INT ||
         tokens[currentTokenIndex+1].getType() == KEYWORD_CHAR ||
         tokens[currentTokenIndex+1].getType() == KEYWORD_BOOL)) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(L_PAREN);
        cout << "Consumed L_PAREN" << endl;
        cout << "Moving to PARAMETER LIST" << endl;
        parameterList();
         }

    if (tokens[currentTokenIndex].getType() == R_PAREN) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(R_PAREN);
        cout << "Consumed R_PAREN" << endl;
    }

    if (tokens[currentTokenIndex].getType() == L_BRACE) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(L_BRACE);
        cout << "Consumed L_BRACE" << endl;
        cout << "Moving to COMPOUND_STATEMENT" << endl;
        compoundStatement();
    }

}
void RecursiveDescentParser::compoundStatement() {
    cout << "Moving to STATEMENT" << endl;
    statement();
}

void RecursiveDescentParser::statement() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_INT ||
         tokens[currentTokenIndex].getType() == KEYWORD_CHAR ||
         tokens[currentTokenIndex].getType() == KEYWORD_BOOL) {
        cout << "Moving to DECLARATION_STATEMENT" << endl;
        declarationStatement();
    }

    if (tokens[currentTokenIndex].getType() == IDENTIFIER &&
        tokens[currentTokenIndex + 1].getType() == ASSIGNMENT_OPERATOR) {
        cout << "Moving to ASSIGNMENT_STATEMENT" << endl;
        assignmentStatement();
    }

    if (tokens[currentTokenIndex].getType() == KEYWORD_FOR ||
        tokens[currentTokenIndex].getType() == KEYWORD_WHILE) {
        cout << "Moving to ITERATION_STATEMENT" << endl;
        iterationStatement();
    }

    if (tokens[currentTokenIndex].getType() == KEYWORD_IF) {
        cout << "Moving to SELECTION_STATEMENT" << endl;
        selectionStatement();
    }

    if (tokens[currentTokenIndex].getType() == KEYWORD_PRINTF) {
        cout << "Moving to PRINTF_STATEMENT" << endl;
        printfStatement();
    }

    if (tokens[currentTokenIndex].getType() == KEYWORD_RETURN) {
        cout << "Moving to KEYWORD_RETURN" << endl;
        returnStatement();
    }

    if (tokens[currentTokenIndex].getType() == L_BRACE) {
        cout << "Moving to BLOCK_STATEMENT" << endl;
        blockStatement();
    }


}

void RecursiveDescentParser::declarationStatement() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_INT ||
    tokens[currentTokenIndex].getType() == KEYWORD_CHAR ||
    tokens[currentTokenIndex].getType() == KEYWORD_BOOL) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed DATATYPE_SPECIFIER" << endl;
    }

    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consumed IDENTIFIER" << endl;
    } else if (tokens[currentTokenIndex].getType () == IDENTIFIER &&
                tokens[currentTokenIndex + 1].getType() == COMMA) {
        cout << "MOVING TO IDENTIFIER_LIST" << endl;
        identifierList();
    }

    if (tokens[currentTokenIndex].getType() == SEMICOLON) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(SEMICOLON);
        cout << "Finished declaration statement" << endl;
    }

}

void RecursiveDescentParser::iterationStatement() {

}

void RecursiveDescentParser::returnStatement() {

}

void RecursiveDescentParser::printfStatement() {

}

void RecursiveDescentParser::assignmentStatement() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consumed IDENTIFIER" << endl;
    }

    if (tokens[currentTokenIndex].getType() == ASSIGNMENT_OPERATOR) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(ASSIGNMENT_OPERATOR);
        cout << "Consumed ASSIGNMENT_OPERATOR" << endl;
    }

    if (checkifNumericalExpression()) {
        cout << "Moving to NUMERICAL_EXPRESSION" << endl;
        numericalExpression();

    } else if (checkifBooleanExpression()) {
        cout << "Moving to BOOLEAN_EXPRESSION" << endl;
        booleanExpression();
    }

    if (tokens[currentTokenIndex].getType() == SEMICOLON) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(SEMICOLON);
        cout << "Consumed SEMICOLON" << endl;
        cout << "Exiting assignmentStatement" << endl;
    }

}


void RecursiveDescentParser::numericalExpression() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER ||
        tokens[currentTokenIndex].getType() == INTEGER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed NUMERICAL_OPERAND" << endl;
    }

    if (tokens[currentTokenIndex].getType() == L_PAREN &&
        (tokens[currentTokenIndex+1].getType() == IDENTIFIER ||
         tokens[currentTokenIndex+1].getType() == INTEGER) &&
         tokens[currentTokenIndex+2].getType() == R_PAREN) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed L_PAREN" << endl;
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed NUMERICAL_OPERAD" << endl;
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed R_PAREN" << endl;
    }

    cout << "Finish numerical expression" << endl;

}


void RecursiveDescentParser::booleanExpression() {
    if (tokens[currentTokenIndex].getType() == BOOLEAN_TRUE ||
        tokens[currentTokenIndex].getType() == BOOLEAN_FALSE ||
        tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed BOOLEAN_TRUE, BOOLEAN_FALSE, OR IDENTIFIER" << endl;
    }

    if (tokens[currentTokenIndex].getType() == L_PAREN &&
        tokens[currentTokenIndex+1].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(L_PAREN);
        cout << "Consumed L_PAREN" << endl;
    }

    if (checkifNumericalExpression()) {
        cout << "Moving to NUMERICAL_EXPRESSION" << endl;
        numericalExpression();
        if (tokens[currentTokenIndex].getType() == LT_EQUAL) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(LT_EQUAL);
            cout << "Moving to NUMERICAL_EXPRESSION" << endl;
            numericalExpression();
        } else if (tokens[currentTokenIndex].getType() == GT_EQUAL) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(GT_EQUAL);
            cout << "Moving to NUMERICAL_EXPRESSION" << endl;
            numericalExpression();
        }
    }



}
void RecursiveDescentParser::selectionStatement() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_IF) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_IF);
        cout << "Consumed KEYWORD_IF" << endl;
    }

    if (checkifBooleanExpression()) {
        cout << "Moving to BOOLEAN_EXPRESSION" << endl;
        booleanExpression();
    }

    if (tokens[currentTokenIndex].getType() == R_PAREN) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(R_PAREN);
        cout << "Consumed R_PAREN" << endl;
        cout << "Moving to STATEMENT" << endl;
        statement();
    }
}

void RecursiveDescentParser::identifierList() {

}

void RecursiveDescentParser::parameterList() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_INT ||
        tokens[currentTokenIndex].getType() == KEYWORD_CHAR ||
        tokens[currentTokenIndex].getType() == KEYWORD_BOOL) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed DATATYPE_SPECIFIER" << endl;
    }

    if (tokens[currentTokenIndex].getType() == IDENTIFIER && (
        tokens[currentTokenIndex+1].getType() == KEYWORD_INT ||
        tokens[currentTokenIndex+1].getType() == KEYWORD_CHAR ||
        tokens[currentTokenIndex+1].getType() == KEYWORD_BOOL)) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consuming IDENTIFIER, repeating PARAMETER_LIST" << endl;
        parameterList();
    } else if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consuming IDENTIFIER, end of parameter list" << endl;
    }
}
void RecursiveDescentParser::procedureState() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_PROCEDURE) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_PROCEDURE);
        cout << "Consumed KEYWORD_PROCEDURE" << endl;
    }
}

void RecursiveDescentParser::procedureMainState() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_PROCEDURE) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_PROCEDURE);
        cout << "Consumed KEYWORD_PROCEDURE" << endl;
        if (tokens[currentTokenIndex].getType() == KEYWORD_MAIN) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(KEYWORD_MAIN);
            cout << "Consumed KEYWORD_MAIN" << endl;
        }
    }
}

bool RecursiveDescentParser::checkifNumericalExpression() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER ||
        tokens[currentTokenIndex].getType() == INTEGER ||
        tokens[currentTokenIndex].getType() == L_PAREN) {
        return true;
    }
    return false;
}

bool RecursiveDescentParser::checkifBooleanExpression() {
    if (tokens[currentTokenIndex].getType() == BOOLEAN_AND ||
                    tokens[currentTokenIndex].getType() == BOOLEAN_OR ||
                    tokens[currentTokenIndex].getType() == IDENTIFIER ||
                    (tokens[currentTokenIndex].getType() == L_PAREN &&
                     tokens[currentTokenIndex].getType() == IDENTIFIER) ||
                     (checkifNumericalExpression() && tokens[currentTokenIndex+2].getType() == BOOLEAN_EQUAL) ||
                     (checkifNumericalExpression() && tokens[currentTokenIndex+2].getType() == LT_EQUAL) ||
                     (checkifNumericalExpression() && tokens[currentTokenIndex+2].getType() == GT_EQUAL)) {
        return true;
    }
    return false;
}

/*
void RecursiveDescentParser::parseProgram() {
    TokenType type;
    CSTNode* programNode = Root;
    currentNode = programNode;
    int whileLoopIndex = 0;


    while (currentTokenIndex < tokens.size() && state != ACCEPT && whileLoopIndex < 200) {
        whileLoopIndex++;
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
                       } else if (prevState == INITIALIZATION_EXPRESSION) {
                           updateState(ITERATION_STATEMENT);
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

                if(type == L_PAREN && tokens[currentTokenIndex + 1].getType() == IDENTIFIER) {
                    insertNode(currentToken);
                    consumeToken(type);
                    updateState(BOOLEAN_EXPRESSION);
                }

            case INITIALIZATION_EXPRESSION:
                type = currentToken.getType();
                if (type == IDENTIFIER && tokens[currentTokenIndex + 1].getType() == ASSIGNMENT_OPERATOR) {
                    insertNode(currentToken);
                    consumeToken(type);
                } else if (type == ASSIGNMENT_OPERATOR && tokens[currentTokenIndex + 1].getType() ==
                    IDENTIFIER) {
                    insertNode(currentToken);
                    consumeToken(type);
                    updateState(BOOLEAN_EXPRESSION);
                }


        }
        }
    }
*/

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