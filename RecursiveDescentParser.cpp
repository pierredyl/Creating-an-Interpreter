//
// Created by dylan pierre on 3/6/2025.
//

#include "RecursiveDescentParser.h"

#include <iostream>
#include "RecursiveDescentParser.h"
using namespace std;


RecursiveDescentParser::RecursiveDescentParser(const vector<Token>& tokenList) {
    tokens = tokenList;
    currentTokenIndex = 0;
    Token programToken(PROGRAM_TOKEN, "PROGRAM", -1);
    Root = new CSTNode(programToken);
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

    //Main
    if (tokens[currentTokenIndex].getType() == KEYWORD_PROCEDURE &&
        tokens[currentTokenIndex + 1].getType() == KEYWORD_MAIN) {
        cout << "\n\nStarting MAIN\n\n" << endl;
        procedureMainState();
        }


    //Function
    if (tokens[currentTokenIndex].getType() == KEYWORD_FUNCTION ||
        currentNode->token.getType() == KEYWORD_FUNCTION) {
        cout << "\n\nStarting FUNCTION\n\n" << endl;
        functionDeclaration();
        parseProgram();
        }

    //Procedure
    if (tokens[currentTokenIndex].getType() == KEYWORD_PROCEDURE ||
    currentNode->token.getType() == KEYWORD_PROCEDURE) {
        cout << "\n\nStarting PROCEDURE\n\n" << endl;
        procedureState();
        parseProgram();
    }

    if (isDatatypeSpecifier()) {
        if (tokens[currentTokenIndex+1].getType() == IDENTIFIER) {
            cout << "\n\nStarting Declaration\n\n" << endl;
            declarationStatement();
            parseProgram();
        }
    }



    cout << "Exiting program" << endl;
}


void RecursiveDescentParser::functionDeclaration() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_FUNCTION) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_FUNCTION);
        cout << "Consumed KEYWORD_FUNCTION" << endl;

        if (isDatatypeSpecifier()) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(tokens[currentTokenIndex].getType());
            cout << "Consumed DATATYPE_SPECIFIER" << endl;

            if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(tokens[currentTokenIndex].getType());
                cout << "Consumed IDENTIFIER" << endl;

                if (tokens[currentTokenIndex].getType() == L_PAREN) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(tokens[currentTokenIndex].getType());
                    cout << "Consumed L_PAREN" << endl;

                    if (tokens[currentTokenIndex].getType() == KEYWORD_VOID) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(tokens[currentTokenIndex].getType());
                        cout << "Consumed KEYWORD_VOID" << endl;

                    } else {
                        parameterList();

                        if (tokens[currentTokenIndex].getType() == R_PAREN) {
                            insertNode(tokens[currentTokenIndex]);
                            consumeToken(tokens[currentTokenIndex].getType());
                            cout << "Consumed R_PAREN" << endl;

                            if (tokens[currentTokenIndex].getType() == L_BRACE) {
                                insertNode(tokens[currentTokenIndex]);
                                consumeToken(tokens[currentTokenIndex].getType());
                                cout << "Consumed L_BRACE" << endl;

                                compoundStatement();

                                if (tokens[currentTokenIndex].getType() == R_BRACE) {
                                    insertNode(tokens[currentTokenIndex]);
                                    consumeToken(tokens[currentTokenIndex].getType());
                                    cout << "Consumed R_BRACE" << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /*

    if (isDatatypeSpecifier()) {
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

    if (tokens[currentTokenIndex].getType() == R_BRACE) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(R_BRACE);
        cout << "Consumed R_BRACE" << endl;
        cout << "Exiting function declaration" << endl;
    }
    */

}
void RecursiveDescentParser::compoundStatement() {
    cout << "Moving to STATEMENT" << endl;
    statement();
    if (isStatement()) {
        compoundStatement();
    }

}

void RecursiveDescentParser::statement() {

    if (isDatatypeSpecifier()) {
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
        cout << "Moving to RETURN_STATEMENT" << endl;
        returnStatement();
    }

    if (tokens[currentTokenIndex].getType() == IDENTIFIER &&
        tokens[currentTokenIndex+1].getType() == L_PAREN) {
        userDefinedFunction();
        if (tokens[currentTokenIndex].getType() == SEMICOLON) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(SEMICOLON);
            cout << "Consumed SEMICOLON" << endl;
        }
    }
}



bool RecursiveDescentParser::isStatement() const {
    TokenType currentType = tokens[currentTokenIndex].getType();

    if (currentType == KEYWORD_INT || currentType == KEYWORD_CHAR || currentType == KEYWORD_BOOL) {
        return true; // Matches <DECLARATION_STATEMENT>
    }

    if (currentType == IDENTIFIER) {
        if (tokens[currentTokenIndex + 1].getType() == ASSIGNMENT_OPERATOR) {
            return true; // Matches <ASSIGNMENT_STATEMENT>
        }
    }

    if (currentType == KEYWORD_FOR || currentType == KEYWORD_WHILE) {
        return true; // Matches <ITERATION_STATEMENT>
    }

    if (currentType == KEYWORD_IF) {
        return true; // Matches <SELECTION_STATEMENT>
    }

    if (currentType == KEYWORD_PRINTF) {
        if (tokens[currentTokenIndex + 1].getType() == L_PAREN) {
            return true; // Matches <PRINTF_STATEMENT>
        }
    }

    if (currentType == KEYWORD_RETURN) {
        TokenType nextType = tokens[currentTokenIndex + 1].getType();
        if (nextType == SINGLE_QUOTED_STRING || nextType == DOUBLE_QUOTED_STRING) {
            return true; // Matches <RETURN_STATEMENT>
        }
    }

    return false; // Not a valid statement
}

bool RecursiveDescentParser::isDatatypeSpecifier() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_CHAR ||
        tokens[currentTokenIndex].getType() == KEYWORD_BOOL ||
        tokens[currentTokenIndex].getType() == KEYWORD_INT) {
        return true;
        }
    return false;
}

bool RecursiveDescentParser::isIdentifierList() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        if (tokens[currentTokenIndex + 1].getType() == COMMA) {
            return true;
        }
    }
    return false;
}

bool RecursiveDescentParser::isIdentifierArrayList() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        if (tokens[currentTokenIndex + 1].getType() == L_BRACKET) {
            return true;
        }
    }
    return false;
}

void RecursiveDescentParser::declarationStatement() {

    if (isDatatypeSpecifier()) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed DATATYPE_SPECIFIER" << endl;

        if (isIdentifierArrayList()) {
            identifierArrayList();

            if (tokens[currentTokenIndex].getType() == SEMICOLON) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(tokens[currentTokenIndex].getType());
                cout << "Consumed SEMICOLON" << endl;
                return;
            }
        }

        if (isIdentifierList()) {
            identifierList();

            if (tokens[currentTokenIndex].getType() == SEMICOLON) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(tokens[currentTokenIndex].getType());
                cout << "Consumed SEMICOLON" << endl;
                return;
            }
        }

        if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(tokens[currentTokenIndex].getType());
            cout << "Consumed IDENTIFIER" << endl;

            if (tokens[currentTokenIndex].getType() == SEMICOLON) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(tokens[currentTokenIndex].getType());
                cout << "Consumed SEMICOLON" << endl;
                return;
            }

        }
    }
}

void RecursiveDescentParser::iterationStatement() {
    inIterationStatement = true;

    if (tokens[currentTokenIndex].getType() == KEYWORD_FOR) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_FOR);
        cout << "Consumed KEYWORD_FOR" << endl;

        if (tokens[currentTokenIndex].getType() == L_PAREN) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(L_PAREN);
            cout << "Consumed L_PAREN" << endl;

            cout << "Moving to INITIALIZATION_EXPRESSION" << endl;
            initializationExpression();

            if (tokens[currentTokenIndex].getType() == SEMICOLON) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(SEMICOLON);
                cout << "Consumed SEMICOLON after INITIALIZATION_EXPRESSION" << endl;

                cout << "Moving to BOOLEAN_EXPRESSION" << endl;
                booleanExpression();

                if (tokens[currentTokenIndex].getType() == SEMICOLON) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(SEMICOLON);
                    cout << "Consumed SEMICOLON after BOOLEAN_EXPRESSION" << endl;

                    cout << "Moving to ITERATION_ASSIGMENT" << endl;
                    iterationAssignment();


                    if (tokens[currentTokenIndex].getType() == R_PAREN) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(R_PAREN);
                        cout << "Consumed R_PAREN" << endl;

                        if (tokens[currentTokenIndex].getType() == L_BRACE) {
                            blockStatement();
                        } else {
                            statement();

                        }
                    }
                }
            }
        }
    }

    if (tokens[currentTokenIndex].getType() == KEYWORD_WHILE) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_WHILE);
        cout << "Consumed KEYWORD_WHILE" << endl;
        if (tokens[currentTokenIndex].getType() == L_PAREN) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(L_PAREN);
            cout << "Consumed L_PAREN" << endl;

            booleanExpression();

            if (tokens[currentTokenIndex].getType() == R_PAREN) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(R_PAREN);
                cout << "Consumed R_PAREN" << endl;

                if (tokens[currentTokenIndex].getType() == L_BRACE) {
                    blockStatement();
                } else {
                    statement();
                }
            }
        }
    }
}



void RecursiveDescentParser::iterationAssignment() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed IDENTIFIER" << endl;
        if (tokens[currentTokenIndex].getType() == ASSIGNMENT_OPERATOR) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(tokens[currentTokenIndex].getType());
            cout << "Consumed ASSIGNMENT_OPERATOR" << endl;

            if (tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(tokens[currentTokenIndex].getType());
                cout << "Consumed SINGLE_QUOTE" << endl;
                if (tokens[currentTokenIndex].getType() == SINGLE_QUOTED_STRING) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(tokens[currentTokenIndex].getType());
                    cout << "Consumed SINGLE_QUOTED_STRING" << endl;
                    if (tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(tokens[currentTokenIndex].getType());
                        cout << "Consumed SINGLE_QUOTED_STRING" << endl;
                    }
                }
            } else if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(tokens[currentTokenIndex].getType());
                cout << "Consumed SINGLE_QUOTE" << endl;
                if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTED_STRING) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(tokens[currentTokenIndex].getType());
                    cout << "Consumed SINGLE_QUOTED_STRING" << endl;
                    if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTE) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(tokens[currentTokenIndex].getType());
                        cout << "Consumed SINGLE_QUOTED_STRING" << endl;
                    }
                }
            } else {
                expression();
            }
        }
    }
}




void RecursiveDescentParser::initializationExpression() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consumed IDENTIFIER" << endl;

        if (tokens[currentTokenIndex].getType() == ASSIGNMENT_OPERATOR) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(ASSIGNMENT_OPERATOR);
            cout << "Consumed ASSIGNMENT_OPERATOR" << endl;

            cout << "Moving to EXPRESSION" << endl;
            expression();

            if (tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(SINGLE_QUOTE);
                cout << "Consumed SINGLE_QUOTE" << endl;

                if (tokens[currentTokenIndex].getType() == SINGLE_QUOTED_STRING) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(SINGLE_QUOTED_STRING);
                    cout << "Consumed SINGLE_QUOTED_STRING" << endl;

                    if (tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(SINGLE_QUOTE);
                        cout << "Consumed SINGLE_QUOTE" << endl;
                    }
                }
            } else if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(DOUBLE_QUOTE);
                cout << "Consumed DOUBLE_QUOTE" << endl;

                if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTED_STRING) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(DOUBLE_QUOTED_STRING);
                    cout << "Consumed DOUBLE_QUOTED_STRING" << endl;

                    if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTE) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(DOUBLE_QUOTE);
                        cout << "Consumed DOUBLE_QUOTE" << endl;
                    }
                }
            }
        }
    }
}


void RecursiveDescentParser::returnStatement() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_RETURN) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_RETURN);
        cout << "Consumed KEYWORD_RETURN" << endl;
        cout << "Moving to EXPRESSION" << endl;
        expression();
    }

    if (tokens[currentTokenIndex].getType() == SEMICOLON) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(SEMICOLON);
        cout << "Finished return statement" << endl;
    }
}

void RecursiveDescentParser::expression() {
    if (checkifNumericalExpression()) {
        cout << "Moving to NUMERICAL_EXPRESSION" << endl;
        numericalExpression();
    }

    else if (checkifBooleanExpression()){
        cout << "Moving to BOOLEAN_EXPRESSION" << endl;
        booleanExpression();
    }

}

void RecursiveDescentParser::printfStatement() {
    if (tokens[currentTokenIndex].getType() == KEYWORD_PRINTF) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_PRINTF);
        cout << "Consumed KEYWORD_PRINTF" << endl;
    }

    if (tokens[currentTokenIndex].getType() == L_PAREN) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(L_PAREN);
        cout << "Consumed L_PAREN" << endl;

        // Handle quoted string (either double or single)
        if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTE) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(DOUBLE_QUOTE);
            cout << "Consumed DOUBLE_QUOTE" << endl;

            if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTED_STRING) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(DOUBLE_QUOTED_STRING);
                cout << "Consumed DOUBLE_QUOTED_STRING" << endl;
            }

            if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(DOUBLE_QUOTE);
                cout << "Consumed DOUBLE_QUOTE" << endl;
            }

        } else if (tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(SINGLE_QUOTE);
            cout << "Consumed SINGLE_QUOTE" << endl;

            if (tokens[currentTokenIndex].getType() == SINGLE_QUOTED_STRING) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(SINGLE_QUOTED_STRING);
                cout << "Consumed SINGLE_QUOTED_STRING" << endl;
            }

            if (tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(SINGLE_QUOTE);
                cout << "Consumed SINGLE_QUOTE" << endl;
            }
        }

        // Handle optional identifier list after a comma
        if (tokens[currentTokenIndex].getType() == COMMA) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(COMMA);
            cout << "Consumed COMMA" << endl;

            cout << "Moving to IDENTIFIER_AND_ARRAY_LIST FROM PRINTF" << endl;
            identifierAndArrayList();
        }

        if (tokens[currentTokenIndex].getType() == R_PAREN) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(R_PAREN);
            cout << "Consumed R_PAREN" << endl;

            if (tokens[currentTokenIndex].getType() == SEMICOLON) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(SEMICOLON);
                cout << "Consumed SEMICOLON" << endl;
            } else {
                cout << "Syntax Error: Expected SEMICOLON after PRINTF statement" << endl;
            }
        } else {
            cout << "Syntax Error: Expected R_PAREN after PRINTF statement" << endl;
        }
    } else {
        cout << "Syntax Error: Expected L_PAREN after PRINTF" << endl;
    }
}


void RecursiveDescentParser::assignmentStatement() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consumed IDENTIFIER" << endl;

        if (tokens[currentTokenIndex].getType() == ASSIGNMENT_OPERATOR) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(ASSIGNMENT_OPERATOR);
            cout << "Consumed ASSIGNMENT_OPERATOR" << endl;

            cout << "Moving to EXPRESSION" << endl;
            expression();

            if (tokens[currentTokenIndex].getType() == SEMICOLON) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(SEMICOLON);
                cout << "Consumed SEMICOLON" << endl;
                return;
            }

            if (tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(SINGLE_QUOTE);
                cout << "Consumed SINGLE_QUOTE" << endl;

                if (tokens[currentTokenIndex].getType() == SINGLE_QUOTED_STRING) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(SINGLE_QUOTED_STRING);
                    cout << "Consumed SINGLE_QUOTED_STRING" << endl;

                    if (tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(SINGLE_QUOTE);
                        cout << "Consumed SINGLE_QUOTE" << endl;

                        if (tokens[currentTokenIndex].getType() == SEMICOLON) {
                            insertNode(tokens[currentTokenIndex]);
                            consumeToken(SEMICOLON);
                            cout << "Consumed SEMICOLON" << endl;
                            return;
                        }
                    }
                }
            }


            if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(DOUBLE_QUOTE);
                cout << "Consumed DOUBLE_QUOTE" << endl;

                if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTED_STRING) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(DOUBLE_QUOTED_STRING);
                    cout << "Consumed DOUBLE_QUOTED_STRING" << endl;

                    if (tokens[currentTokenIndex].getType() == DOUBLE_QUOTE) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(DOUBLE_QUOTE);
                        cout << "Consumed DOUBLE_QUOTE" << endl;

                        if (tokens[currentTokenIndex].getType() == SEMICOLON) {
                            insertNode(tokens[currentTokenIndex]);
                            consumeToken(SEMICOLON);
                            cout << "Consumed SEMICOLON" << endl;
                            return;
                        }
                    }
                }
            }
        }
    }
}


void RecursiveDescentParser::numericalExpression() {
    if (checkNumericalOperator()) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed NUMERICAL_OPERATOR" << endl;

        numericalExpression();
    }

    if (tokens[currentTokenIndex].getType() == L_PAREN) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(L_PAREN);
        cout << "Consumed L_PAREN" << endl;

        numericalOperand();

        if (tokens[currentTokenIndex].getType() == R_PAREN) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(tokens[currentTokenIndex].getType());
            cout << "Consumed R_PAREN" << endl;
        }

        if (checkNumericalOperator()) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(tokens[currentTokenIndex].getType());
            cout << "Consumed NUMERICAL OPERATOR" << endl;

            cout << "Moving to numerical expression" << endl;
            numericalExpression();

            if (tokens[currentTokenIndex].getType() == R_PAREN) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(tokens[currentTokenIndex].getType());
                cout << "Consumed R_PAREN" << endl;

                if (checkNumericalOperator()) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(tokens[currentTokenIndex].getType());
                    cout << "Consumed R_PAREN" << endl;

                    numericalExpression();
                }
            }
        }
    }


    numericalOperand();

    if (checkNumericalOperator() && tokens[currentTokenIndex].getType() != L_PAREN) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed L_PAREN" << endl;

        numericalExpression();
        return;
    }

    if (checkNumericalOperator()) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed NUMERICAL_OPERATOR" << endl;


        if (tokens[currentTokenIndex].getType() == L_PAREN) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(L_PAREN);
            cout << "Consumed L_PAREN" << endl;

            cout << "Moving to NUMERICAL_EXPRESSION" << endl;
            numericalExpression();
        } else {
            numericalExpression();

            if (tokens[currentTokenIndex].getType() == R_PAREN) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(R_PAREN);
                cout << "Consumed R_PAREN" << endl;

                if (checkNumericalOperator()) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(tokens[currentTokenIndex].getType());
                    cout << "Consumed NUMERICAL OPERATOR" << endl;
                    cout << "Moving to NUMERICAL_EXPRESSION" << endl;
                    numericalExpression();
                }

                numericalOperand();

                if (checkNumericalOperator()) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(tokens[currentTokenIndex].getType());
                    cout << "Consumed NUMERICAL OPERATOR" << endl;

                    cout << "Moving to NUMERICAL_EXPRESSION" << endl;
                    numericalExpression();
                }
            }
        }
    }
        }

bool RecursiveDescentParser::isIdentifierArrayParameterList() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        if (tokens[currentTokenIndex+1].getType() == L_BRACKET) {
            return true;
        }
    }
    return false;
}

void RecursiveDescentParser::userDefinedFunction() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consumed IDENTIFIER" << endl;

        if (tokens[currentTokenIndex].getType() == L_PAREN) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(L_PAREN);
            cout << "Consumed L_PAREN" << endl;

            if (isIdentifierArrayParameterList()) {
                identifierAndIdentifierArrayParameterList();

                if (tokens[currentTokenIndex].getType() == R_PAREN) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(tokens[currentTokenIndex].getType());
                    cout << "Consumed R_PAREN" << endl;
                }
            } else {
                expression();

                if (tokens[currentTokenIndex].getType() == R_PAREN) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(tokens[currentTokenIndex].getType());
                    cout << "Consumed R_PAREN" << endl;
                }
            }
        }
    }
}

void RecursiveDescentParser::identifierAndIdentifierArrayParameterList() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed IDENTIFIER" << endl;

        if (tokens[currentTokenIndex].getType() == COMMA) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(tokens[currentTokenIndex].getType());
            cout << "Consumed COMMA" << endl;
            identifierAndIdentifierArrayParameterList();
        }


        if (tokens[currentTokenIndex].getType() == L_BRACKET) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(L_BRACKET);
            cout << "Consumed L_BRACKET here" << endl;

            if (checkifNumericalExpression()) {
                numericalExpression();

                if (tokens[currentTokenIndex].getType() == R_BRACKET) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(tokens[currentTokenIndex].getType());
                    cout << "Consumed R_BRACKET" << endl;
                    return;
                }
            }

            if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(tokens[currentTokenIndex].getType());
                cout << "Consumed IDENTIFIER" << endl;

                if (tokens[currentTokenIndex].getType() == R_BRACKET) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(R_BRACKET);
                    cout << "Consumed R_BRACKET" << endl;

                    if (tokens[currentTokenIndex].getType() == COMMA) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(tokens[currentTokenIndex].getType());
                        cout << "Consumed COMMA" << endl;

                        identifierAndIdentifierArrayParameterList();
                    }
                }
            }
        }
    }
}

bool RecursiveDescentParser::checkNumericalOperand() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER ||
        tokens[currentTokenIndex].getType() == INTEGER ||
        tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
        return true;
    }
    return false;
}

bool RecursiveDescentParser::checkNumericalOperator() {
    if (tokens[currentTokenIndex].getType() == PLUS ||
        tokens[currentTokenIndex].getType() == MINUS ||
        tokens[currentTokenIndex].getType() == ASTERISK ||
        tokens[currentTokenIndex].getType() == DIVIDE ||
        tokens[currentTokenIndex].getType() == MODULO) {
        return true;
    }
    return false;
}




bool RecursiveDescentParser::isRelationalExpression() {
    if (tokens[currentTokenIndex].getType() == BOOLEAN_EQUAL ||
    tokens[currentTokenIndex].getType() == LT_EQUAL ||
    tokens[currentTokenIndex].getType() == GT_EQUAL ||
    tokens[currentTokenIndex].getType() == LT ||
    tokens[currentTokenIndex].getType() == GT) {
        return true;
    }

    return false;
}

void RecursiveDescentParser::booleanExpression() {

    if ((tokens[currentTokenIndex+1].getType() == BOOLEAN_EQUAL ||
        tokens[currentTokenIndex+1].getType() == LT_EQUAL ||
        tokens[currentTokenIndex+1].getType() == GT_EQUAL ||
        tokens[currentTokenIndex+1].getType() == LT ||
        tokens[currentTokenIndex+1].getType() == GT) ||
        (tokens[currentTokenIndex].getType() == L_PAREN &&
        tokens[currentTokenIndex+1].getType() == IDENTIFIER &&
        (tokens[currentTokenIndex+2].getType() == PLUS) ||
        tokens[currentTokenIndex+2].getType() == MINUS ||
        tokens[currentTokenIndex+2].getType() == ASTERISK ||
        tokens[currentTokenIndex+2].getType() == DIVIDE ||
        tokens[currentTokenIndex+2].getType() == MODULO)) {
        numericalExpression();

        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed RELATIONAL_OPERATOR" << endl;
            numericalExpression();
    }

    if (tokens[currentTokenIndex].getType() == BOOLEAN_TRUE) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed BOOLEAN_TRUE" << endl;
        return;
    }

    if (tokens[currentTokenIndex].getType() == BOOLEAN_FALSE) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed BOOLEAN_FALSE" << endl;
    }


    if (tokens[currentTokenIndex].getType() == L_PAREN &&
        tokens[currentTokenIndex+1].getType() == IDENTIFIER &&
        tokens[currentTokenIndex+2].getType() == BOOLEAN_AND ||
        tokens[currentTokenIndex+2].getType() == BOOLEAN_OR ) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed L_PAREN" << endl;

        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed IDENTIFIER" << endl;

        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());

        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());

        booleanExpression();

            if (tokens[currentTokenIndex].getType() == R_PAREN) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(tokens[currentTokenIndex].getType());
                cout << "Consumed R_PAREN" << endl;
            }
        }


    if (tokens[currentTokenIndex].getType() == L_PAREN) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(L_PAREN);
        cout << "Consumed L_PAREN" << endl;

        numericalOperand();

            if (isRelationalExpression()) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(tokens[currentTokenIndex].getType());
                cout << "Consumed RELATIONAL_OPERATOR" << endl;

                numericalOperand();

                    if (tokens[currentTokenIndex].getType() == R_PAREN) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(R_PAREN);
                        cout << "Consumed R_PAREN" << endl;

                        if (tokens[currentTokenIndex].getType() == BOOLEAN_AND ||
                            tokens[currentTokenIndex].getType() == BOOLEAN_OR) {
                            insertNode(tokens[currentTokenIndex]);
                            consumeToken(tokens[currentTokenIndex].getType());
                            cout << "Consumed BOOLEAN_OPERATOR" << endl;
                            booleanExpression();
                            }
                    }
                }
            }

    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed IDENTIFIER" << endl;

        if (tokens[currentTokenIndex].getType() == BOOLEAN_AND ||
            tokens[currentTokenIndex].getType() == BOOLEAN_OR) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(tokens[currentTokenIndex].getType());
            cout << "Consumed BOOLEAN_OPERATOR" << endl;
            cout << "Moving to BOOLEAN_EXPRESSION" << endl;
            booleanExpression();
            }
    }
}

void RecursiveDescentParser::numericalOperator() {
    TokenType type = tokens[currentTokenIndex].getType();
}

void RecursiveDescentParser::numericalOperand() {

    if (tokens[currentTokenIndex].getType() == IDENTIFIER &&
    tokens[currentTokenIndex+1].getType() == L_PAREN) {
        userDefinedFunction();
    }


    if (tokens[currentTokenIndex].getType() == INTEGER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(INTEGER);
    }

    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);;
    }

    if (tokens[currentTokenIndex].getType() == IDENTIFIER &&
        tokens[currentTokenIndex+1].getType() == L_PAREN) {
        userDefinedFunction();
    }

    if (tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(SINGLE_QUOTE);
        cout << "Consumed SINGLE_QUOTE" << endl;

        if (tokens[currentTokenIndex].getType() == INTEGER ||
            tokens[currentTokenIndex].getType() == IDENTIFIER ||
            tokens[currentTokenIndex].getType() == SINGLE_QUOTED_STRING) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(tokens[currentTokenIndex].getType());
            cout << "Consumed INTEGER" << endl;

            if (tokens[currentTokenIndex].getType() == SINGLE_QUOTE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(SINGLE_QUOTE);
                cout << "Consumed SINGLE_QUOTE" << endl;\
                return;
            }
        }
    }

}

void RecursiveDescentParser::selectionStatement() {

    inSelectionStatement = true;
    if (tokens[currentTokenIndex].getType() == KEYWORD_IF) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_IF);
        cout << "Consumed KEYWORD_IF" << endl;

        if (tokens[currentTokenIndex].getType() == L_PAREN) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(L_PAREN);
            cout << "Consumed L_PAREN" << endl;

            cout << "Moving to BOOLEAN_EXPRESSION" << endl;
            booleanExpression();

            if (tokens[currentTokenIndex].getType() == R_PAREN) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(R_PAREN);
                cout << "Consumed R_PAREN" << endl;

                if (tokens[currentTokenIndex].getType() == L_BRACE) {
                    cout << "Moving to BLOCK_STATEMENT" << endl;
                    blockStatement();
                } else {
                    cout << "Moving to STATEMENT" << endl;
                    statement();
                }

                if (tokens[currentTokenIndex].getType() == KEYWORD_ELSE) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(KEYWORD_ELSE);
                    cout << "Consumed KEYWORD_ELSE" << endl;

                    if (tokens[currentTokenIndex].getType() == L_BRACE) {
                        cout << "Moving to BLOCK_STATEMENT" << endl;
                        blockStatement();
                    } else {
                        cout << "Moving to STATEMENT" << endl;
                        statement();
                    }
                }
            }
        }
    }
    inSelectionStatement = false;
}


void RecursiveDescentParser::blockStatement() {

    cout << tokens[currentTokenIndex].getValue() << endl;

    if (currentTokenIndex < tokens.size() && tokens[currentTokenIndex].getType() == L_BRACE) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(L_BRACE);
        cout << "Consumed L_BRACE" << endl;
        cout << "Moving to COMPOUND_STATEMENT" << endl;
        compoundStatement();

        if (currentTokenIndex < tokens.size() && tokens[currentTokenIndex].getType() == R_BRACE) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(R_BRACE);
            cout << "Consumed R_BRACE" << endl;

        }
    }
    cout << "Exiting blockStatement()" << endl;
}

void RecursiveDescentParser::identifierAndArrayList() {
    if (currentTokenIndex < tokens.size()) {
        if (tokens[currentTokenIndex].getType() == L_BRACKET) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(L_BRACKET);
            cout << "Consumed L_BRACKET" << endl;
            if (tokens[currentTokenIndex].getType() == R_BRACKET) {
            }
        }
        if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
            // Check if the identifier is followed by a '[' indicating an array.
            if (currentTokenIndex + 1 < tokens.size() && tokens[currentTokenIndex + 1].getType() == L_BRACKET) {
                cout << "Moving to IDENTIFIER_ARRAY_LIST" << endl;
                identifierArrayList();

                // After an array list, there may still be a normal identifier list.
                if (currentTokenIndex < tokens.size() && tokens[currentTokenIndex].getType() == IDENTIFIER) {
                    cout << "Moving to IDENTIFIER_LIST after ARRAY_LIST" << endl;
                    identifierList();
                }
            } else {
                cout << "Moving to IDENTIFIER_LIST" << endl;
                identifierList();

                // After an identifier list, there may still be an array list.
                if (currentTokenIndex < tokens.size() && tokens[currentTokenIndex].getType() == IDENTIFIER &&
                    currentTokenIndex + 1 < tokens.size() && tokens[currentTokenIndex + 1].getType() == L_BRACKET) {
                    cout << "Moving to IDENTIFIER_ARRAY_LIST after IDENTIFIER_LIST" << endl;
                    identifierArrayList();
                }
            }
        }
    }
}

void RecursiveDescentParser::identifierList() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consumed IDENTIFIER" << endl;

        if (tokens[currentTokenIndex].getType() == COMMA) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(COMMA);
            cout << "Consumed COMMA" << endl;
            cout << "moving to IDENTIFIER_LIST" << endl;
            identifierList();
        }
    }
    cout << "Exiting identifierList()" << endl;
}

void RecursiveDescentParser::identifierArrayList() {
    if (tokens[currentTokenIndex].getType() == IDENTIFIER) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consumed IDENTIFIER (Array)" << endl;

        if (tokens[currentTokenIndex].getType() == L_BRACKET) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(L_BRACKET);
            cout << "Consumed L_BRACKET" << endl;

            if (tokens[currentTokenIndex].getType() == INTEGER) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(INTEGER);
                cout << "Consumed WHOLE_NUMBER (Array Index)" << endl;

                if (tokens[currentTokenIndex].getType() == R_BRACKET) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(R_BRACKET);
                    cout << "Consumed R_BRACKET" << endl;

                    if (tokens[currentTokenIndex].getType() == COMMA) {
                        insertNode(tokens[currentTokenIndex]);
                        consumeToken(COMMA);
                        cout << "Consumed COMMA" << endl;
                        cout << "Moving to IDENTIFIER_ARRAY_LIST" << endl;
                        identifierArrayList();
                    }
                }
            }
        }
    }
    cout << "Exiting identifierArrayList()" << endl;
}



void RecursiveDescentParser::parameterList() {

    if (isDatatypeSpecifier()) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(tokens[currentTokenIndex].getType());
        cout << "Consumed DATATYPE_SPECIFIER" << endl;
        if (tokens[currentTokenIndex].getType() == IDENTIFIER &&
            tokens[currentTokenIndex + 1].getType() == L_BRACKET) {
            identifierAndIdentifierArrayParameterList();
        } else {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(IDENTIFIER);
            cout << "Consumed IDENTIFIER" << endl;
            if (isDatatypeSpecifier()) {
                cout << "moving to PARAMETER_LIST" << endl;
                parameterList();
            }
        }
    }


}

void RecursiveDescentParser::procedureState() {
    cout << tokens[currentTokenIndex].getValue() << endl;
    if (tokens[currentTokenIndex].getType() == KEYWORD_PROCEDURE) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_PROCEDURE);
        cout << "Consumed KEYWORD_PROCEDURE" << endl;
    }

    //<IDENTIFIER> <L_PAREN> void <R_PAREN> < L_BRACE> <COMPOUND_STATEMENT > < R_BRACE>
    if (tokens[currentTokenIndex].getType() == IDENTIFIER &&
        tokens[currentTokenIndex+1].getType() == L_PAREN &&
        tokens[currentTokenIndex+2].getType() == KEYWORD_VOID) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consumed IDENTIFIER" << endl;

        insertNode(tokens[currentTokenIndex]);
        consumeToken(L_PAREN);
        cout << "Consumed L_PAREN" << endl;

        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_VOID);
        cout << "Consumed KEYWORD_VOID" << endl;

        if (tokens[currentTokenIndex].getType() == L_BRACE) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(L_BRACE);
            cout << "Consumed L_BRACE" << endl;
            cout << "Moving to COMPOUND_STATEMENT" << endl;
            compoundStatement();
            if (tokens[currentTokenIndex].getType() == R_BRACE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(R_BRACE);
                cout << "Consumed R_BRACE" << endl;
                cout << "exiting procedurestate" << endl;
            }
        }
    }
    //<IDENTIFIER> <L_PAREN> <PARAMETER_LIST > < R_PAREN > < L_BRACE > < COMPOUND_STATEMENT > < R_BRACE>
    if (tokens[currentTokenIndex].getType() == IDENTIFIER &&
        tokens[currentTokenIndex+1].getType() == L_PAREN) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(IDENTIFIER);
        cout << "Consumed IDENTIFIER" << endl;

        insertNode(tokens[currentTokenIndex]);
        consumeToken(L_PAREN);
        cout << "Consumed L_PAREN" << endl;

        cout << "Moving to PARAMETER_LIST" << endl;
        parameterList();

        if (tokens[currentTokenIndex].getType() == R_PAREN) {
            insertNode(tokens[currentTokenIndex]);
            consumeToken(R_PAREN);
            cout << "Consumed R_PAREN" << endl;
            if (tokens[currentTokenIndex].getType() == L_BRACE) {
                insertNode(tokens[currentTokenIndex]);
                consumeToken(L_BRACE);
                cout << "Consumed L_BRACE" << endl;
                cout << "Moving to COMPOUND_STATEMENT" << endl;
                compoundStatement();
                if (tokens[currentTokenIndex].getType() == R_BRACE) {
                    insertNode(tokens[currentTokenIndex]);
                    consumeToken(R_BRACE);
                    cout << "Consumed R_BRACE" << endl;
                    cout << "exiting procedurestate" << endl;
                }
            }
        }
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
    //procedure main <L_PAREN> void <R_PAREN> <BLOCK_STATEMENT>
    if (tokens[currentTokenIndex].getType() == L_PAREN &&
        tokens[currentTokenIndex+1].getType() == KEYWORD_VOID &&
        tokens[currentTokenIndex+2].getType() == R_PAREN) {
        insertNode(tokens[currentTokenIndex]);
        consumeToken(L_PAREN);
        cout << "Consumed L_PAREN" << endl;
        insertNode(tokens[currentTokenIndex]);
        consumeToken(KEYWORD_VOID);
        cout << "Consumed KEYWORD_VOID" << endl;
        insertNode(tokens[currentTokenIndex]);
        consumeToken(R_PAREN);
        cout << "Consumed R_PAREN" << endl;
        cout << "Moving to BLOCK_STATEMENT" << endl;
        blockStatement();
    }
}

bool RecursiveDescentParser::checkifNumericalExpression() {

    if (tokens[currentTokenIndex].getType() == INTEGER ||
        tokens[currentTokenIndex].getType() == IDENTIFIER) {
        return true;
    }

    //numerical operand
    if (tokens[currentTokenIndex].getType() == IDENTIFIER ||
        tokens[currentTokenIndex].getType() == INTEGER ||
        tokens[currentTokenIndex].getType() == SINGLE_QUOTE  ||
        tokens[currentTokenIndex].getType() == DOUBLE_QUOTE) {

        //numerical operator
        if (tokens[currentTokenIndex+1].getType() == PLUS ||
        tokens[currentTokenIndex+1].getType() == MINUS ||
        tokens[currentTokenIndex+1].getType() == ASTERISK ||
        tokens[currentTokenIndex+1].getType() == DIVIDE ||
        tokens[currentTokenIndex+1].getType() == MODULO) {
            return true;
        }
    }

    if (tokens[currentTokenIndex].getType() == L_PAREN) {

        //numerical opernad
        if (tokens[currentTokenIndex+1].getType() == IDENTIFIER ||
        tokens[currentTokenIndex+1].getType() == INTEGER ||
        tokens[currentTokenIndex+1].getType() == SINGLE_QUOTE  ||
        tokens[currentTokenIndex+1].getType() == DOUBLE_QUOTE) {

                return true;
        }
    }

    if (tokens[currentTokenIndex].getType() == L_PAREN) {

        //numerical operand
        if (tokens[currentTokenIndex+1].getType() == IDENTIFIER ||
        tokens[currentTokenIndex+1].getType() == INTEGER ||
        tokens[currentTokenIndex+1].getType() == SINGLE_QUOTE  ||
        tokens[currentTokenIndex+1].getType() == DOUBLE_QUOTE)
        {
            if (tokens[currentTokenIndex+2].getType() == PLUS ||
                tokens[currentTokenIndex+2].getType() == MINUS ||
                tokens[currentTokenIndex+2].getType() == ASTERISK ||
                tokens[currentTokenIndex+2].getType() == DIVIDE ||
                tokens[currentTokenIndex+2].getType() == MODULO) {
                return true;
            }
        }
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


void RecursiveDescentParser::printCSTHelper(CSTNode* node, bool isLeftChild) const {
    // Base case: If the node is null, return
    if (node == nullptr) {
        return;
    }

    if (isLeftChild) {
        cout << "\n";
        cout << "v\n";
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