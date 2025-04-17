//
// Created by dylan on 4/12/2025.
//


#include "AbstractSyntaxTree.h"

#include <deque>

#include "Tokenizer.h"
#include <iostream>
#include <stack>
#include <unordered_map>


using namespace std;



AbstractSyntaxTree::AbstractSyntaxTree(RecursiveDescentParser& CST, SymbolTable& ST) {
    this->CSTRoot = CST.getRoot();
    this->currentSymbolPointer = ST.getHead();
};

bool isNegativeNumber(const string& token) {
    // Check if the token starts with a '-' and the rest is a digit
    if (token[0] == '-' && token.length() > 1 && isdigit(token[1])) {
        return true;
    }
    return false;
}

// Function to get the precedence of operators
int precedence(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/" || op == "&&") return 2;
    if (op == "||" || op == ">=" || op == "<=" || op == ">" || op == "<"
        || op == "!") return 3;  // Add precedence for boolean operators
    return 0;  // For parentheses or unsupported operators
}


deque<string> infixToPostfix(deque<string>& expression) {
    stack<string> opStack;  // Stack for operators
    deque<string> output;   // Output deque for postfix expression
    bool isFunctionCall = true;


    //Function Call Assignment
    for (deque<string>::iterator it = expression.begin(); it != expression.end(); it++) {
        if (*it == "*" || *it == "/" || *it == "+" || *it == "-" || *it == "%") {
            isFunctionCall = false;
            break;
        }
    }


    // Process each token in the expression
    for (const string& token : expression) {
        if (isdigit(token[0]) || isalpha(token[0]) || isNegativeNumber(token) || token[0] == '\'' || token[0] == '"' ||
            token[0] == '\\') {
            output.push_back(token);  // Add operands (numbers or variables) to output
        } else if ((token == "("  || token == ")" || token == "[" || token == "]") && isFunctionCall) {
            output.push_back(token);
        } else if (token == "(") {
            opStack.push(token);  // Push left parenthesis to stack
        } else if (token == ")") {
            // Pop operators from stack until left parenthesis is encountered
            while (!opStack.empty() && opStack.top() != "(") {
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty() && opStack.top() == "(") {
                opStack.pop();  // Discard the left parenthesis
            }
        } else {  // It's an operator (+, -, *, /)
            // Handle operators with precedence
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(token)) {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);  // Push the current operator to stack
        }
    }

    // Pop any remaining operators from the stack
    while (!opStack.empty()) {
        output.push_back(opStack.top());
        opStack.pop();
    }

    return output;
}


void AbstractSyntaxTree::printAST() const {
    if (ASTRoot) {
        printASTHelper(ASTRoot, false);
    } else {
        cout << "AST is empty." << endl;
    }
}

void AbstractSyntaxTree::printASTHelper(ASTNode* node, bool isLeftChild) const {
    // Base case: If the node is null, return
    if (node == nullptr) {
        return;
    }

    if (isLeftChild) {
        cout << "\n";
        cout << "v\n";
    }

    /*
    if (node->name == "DECLARATION") {
        cout << node->name << ", " << node->type << ": " << node->typeName;
    }
    else {
        cout << node->name;
    }
    */

    cout << node->name;

    if (node->rightSibling != nullptr) {
        cout << " -> ";
    }

    // If there's a left child, print it recursively
    if (node->leftChild != nullptr) {
        printASTHelper(node->leftChild, true);
    }

    // If there's a right sibling, print it recursively
    if (node->rightSibling != nullptr) {
        printASTHelper(node->rightSibling, false);
    }
}

void AbstractSyntaxTree::insertNode(RecursiveDescentParser::CSTNode* currentCSTNode, string name, string type,
    string typeName) {
    ASTNode* newNode = new ASTNode(name, type, typeName);

    if (ASTRoot == nullptr) {
        ASTRoot = newNode;
        ASTRoot->leftChild = nullptr;
        ASTRoot->rightSibling = nullptr;
        currentASTNode = ASTRoot;
        return;
    }

    if (name == "DECLARATION") {
        currentASTNode->leftChild = newNode;
        currentASTNode->rightSibling = nullptr;
        currentASTNode = newNode;
        return;
    }


    if (name.substr(0,3) == "FOR") {
        currentASTNode->leftChild = newNode;
        currentASTNode->rightSibling = nullptr;
        currentASTNode = newNode;
        return;
    }

    newNode->lineNumber = currentCSTNode->token.getLineNumber();
    if (newNode->lineNumber == currentASTNode->lineNumber) {
        currentASTNode->rightSibling = newNode;
        currentASTNode->leftChild = nullptr;
    } else {
        currentASTNode->leftChild = newNode;
        currentASTNode->rightSibling = nullptr;
    }
    currentASTNode = newNode;
}
void AbstractSyntaxTree::printSymbolTableForDeclaration(SymbolTable::Symbol* s) {
    std::cout << "IDENTIFIER_NAME: " << s->identifierName << "\n";
    std::cout << "IDENTIFIER_TYPE: " << s->identifierType << "\n";
    std::cout << "DATATYPE: " << s->dataType << "\n";
    std::cout << "DATATYPE_IS_ARRAY: " << (s->isArray ? "yes" : "no") << "\n";
    std::cout << "DATATYPE_ARRAY_SIZE: " << s->arraySize << "\n";
    std::cout << "SCOPE: " << s->scope << "\n\n";
}

void AbstractSyntaxTree::buildAST() {
    if (CSTRoot != nullptr) {
        BuildASTHelper(CSTRoot);  // Call the helper function to traverse the tree
    }

}

void AbstractSyntaxTree::testLinking() {
    cout << "Testing Symbol Table and AST Linking\n\n" << endl;

    ASTNode* node = ASTRoot;

    while (node != nullptr && currentSymbolPointer != nullptr) {
        if (node->name == "DECLARATION") {
            cout << "Declaring: " << node->typeName << ", which is a " << node->type << "." << endl;
            cout << "Symbol Table for " << node->typeName << ": " << endl;
            printSymbolTableForDeclaration(node->symbolPointer);

            if (node->rightSibling == nullptr) {
                node = node->leftChild;
            } else if (node->leftChild == nullptr) {
                node = node->rightSibling;
            } else {
                break;
            }

        } else {
            if (node->rightSibling == nullptr) {
                node = node->leftChild;
            } else if (node->leftChild == nullptr) {
                node = node->rightSibling;
            } else {
                break;
            }
        }

    }
}


void AbstractSyntaxTree::linkASTandSymbolTable() {
    ASTNode* node = ASTRoot;
    while (node != nullptr) {
        if (node->name == "DECLARATION") {
            string type = node->type;
            string typeName = node->typeName;
            cout << "attempting to link: " << typeName << " " << type << " to " <<
                currentSymbolPointer->identifierName << " " << currentSymbolPointer->identifierType << "." << endl;
            if (currentSymbolPointer->identifierName == typeName) {
                node->symbolPointer = currentSymbolPointer;
                cout << "Just Linked: " << typeName << " in the AST to " << currentSymbolPointer->identifierName
                << " in the Symbol Table" << endl;
                if (currentSymbolPointer->next != nullptr) {
                    currentSymbolPointer = currentSymbolPointer->next;
                } else if (currentSymbolPointer->next == nullptr) {
                    break;
                }

                while (currentSymbolPointer->identifierType == "parameter" && currentSymbolPointer->next != nullptr) {
                    currentSymbolPointer = currentSymbolPointer->next;
                }
                }

            if (node->rightSibling == nullptr) {
                node = node->leftChild;
            } else if (node->leftChild == nullptr) {
                node = node->rightSibling;
            } else {
                break;
            }

        } else {

            if (node->rightSibling != nullptr) {
                node = node->rightSibling;
            } else if (node->rightSibling == nullptr && node->leftChild != nullptr) {
                node = node->leftChild;
            } else {
                break;
            }
        }
    }
}

void AbstractSyntaxTree::BuildASTHelper(RecursiveDescentParser::CSTNode* currentCSTNode) {
    string typeName;

    if (currentCSTNode == nullptr) {
        return;
    }
    string tokenValue = currentCSTNode->token.getValue();
    TokenType tokenType = currentCSTNode->token.getType();


    if (tokenValue == "{") {
        insideFuncDeclaration = false;
    }

    if (tokenValue == ";") {
        inStatement = false;
        inVariableList = false;
    }

    //Function or Procedure Declaration
    if (tokenValue == "function" || tokenValue == "procedure") {
        if (currentCSTNode->rightSibling->token.getValue() == "main") {
            insertNode(currentCSTNode, "DECLARATION", "procedure", "main");
        } else if (tokenValue == "procedure" && currentCSTNode->rightSibling->token.getType() == IDENTIFIER) {
            typeName = currentCSTNode->rightSibling->token.getValue();
            insertNode(currentCSTNode, "DECLARATION", "procedure", typeName);
        } else {
            typeName = currentCSTNode->rightSibling->rightSibling->token.getValue();
            insertNode(currentCSTNode, "DECLARATION", "function", typeName);
        }
        insideFuncDeclaration = true;

    } else if (tokenType == IDENTIFIER && currentCSTNode->rightSibling->token.getValue() == "(" && !insideFuncDeclaration) {
        insertNode(currentCSTNode, "CALL", "N/A", "N/A");
        insertNode(currentCSTNode, currentCSTNode->token.getValue(), "N/A", "N/A");
        currentCSTNode = currentCSTNode->rightSibling;
        insertNode(currentCSTNode, currentCSTNode->token.getValue(), "N/A", "N/A");
        currentCSTNode = currentCSTNode->rightSibling;
        insertNode(currentCSTNode, currentCSTNode->token.getValue(), "N/A", "N/A");
        currentCSTNode = currentCSTNode->rightSibling;
        insertNode(currentCSTNode, currentCSTNode->token.getValue(), "N/A", "N/A");


    //Variable Declaration
    } else if ((tokenValue == "int" || tokenValue == "char" || tokenValue == "bool")  && !insideFuncDeclaration &&
        !inForStatement) {
        inStatement = true;
        typeName = currentCSTNode->rightSibling->token.getValue();
        insertNode(currentCSTNode, "DECLARATION", "variable", typeName);
        if (currentCSTNode->rightSibling->rightSibling->token.getValue() == ",") {
            inVariableList = true;
            currentCSTNode = currentCSTNode->rightSibling;
        }
    } else if (inVariableList && tokenType == IDENTIFIER && !inForStatement) {
        typeName = currentCSTNode->token.getValue();
        insertNode(currentCSTNode, "DECLARATION", "variable", typeName);

    //Begin Block '{'
    } else if (tokenValue == "{") {
        insertNode(currentCSTNode, "BEGIN BLOCK", "N/A", "N/A");

    //End Blcok '}'
    } else if (tokenValue == "}") {
        insertNode(currentCSTNode, "END BLOCK", "N/A", "N/A");

    //Assignment
    } else if (tokenType == IDENTIFIER && !insideFuncDeclaration && !inStatement && !inForStatement && !inForStatement2 &&
        !inForStatement3) {
        inStatement = true;
        deque<string> expression;
        insertNode(currentCSTNode, "ASSIGNMENT", "N/A", "N/A");

        while (tokenValue != ";") {
            expression.push_back(tokenValue);
            currentCSTNode = currentCSTNode->rightSibling;
            tokenValue = currentCSTNode->token.getValue();
        }
        expression = infixToPostfix(expression);

        for (auto it = expression.begin(); it != expression.end(); it++) {
            insertNode(currentCSTNode, *it, "N/A", "N/A");
        }
        inStatement = false;

    //if statement loop
    } else if (tokenValue == "if") {
        deque<string> expression;
        inIfStatement = true;
        parenDepth = 1;
        bool functionCallInside = false;

        while (parenDepth > 0) {

            if (tokenValue == "(") {
                parenDepth++;
            } else if (tokenValue == ")" && parenDepth > 0) {
                parenDepth--;
            }

            if (currentCSTNode->token.getType() == IDENTIFIER && currentCSTNode->rightSibling->token.getValue() == "(") {
                functionCallInside = true;
            }


            if (tokenValue != ")" && tokenValue != "(" && !functionCallInside) {
                expression.push_back(tokenValue);
            } else if (functionCallInside && parenDepth > 1) {
                expression.push_back(tokenValue);
            }

            if (currentCSTNode->rightSibling != nullptr) {
                currentCSTNode = currentCSTNode->rightSibling;
                tokenValue = currentCSTNode->token.getValue();
            }
        }
        expression = infixToPostfix(expression);

        for (auto it = expression.begin(); it != expression.end(); it++) {
            insertNode(currentCSTNode, *it, "N/A", "N/A");
        }

        inIfStatement = false;
        parenDepth = 0;


    //else
    } else if (tokenValue == "else") {
        insertNode(currentCSTNode, "ELSE", "N/A", "N/A");

    //Return Statement
    } else if (tokenValue == "return") {
        deque<string> expression;
        insertNode(currentCSTNode, "RETURN", "N/A", "N/A");

        while (tokenValue != ";") {
            if (tokenValue != "return" && tokenValue != ")" && tokenValue != "(") {
                expression.push_back(tokenValue);
            }

            currentCSTNode = currentCSTNode->rightSibling;
            tokenValue = currentCSTNode->token.getValue();
        }
        expression = infixToPostfix(expression);

        for (auto it = expression.begin(); it != expression.end(); it++) {
            insertNode(currentCSTNode, *it, "N/A", "N/A");
        }
        inStatement = false;



    //printf
    } else if (tokenValue == "printf") {
        insertNode(currentCSTNode, "PRINTF", "N/A", "N/A");
        currentCSTNode = currentCSTNode->rightSibling;
        tokenValue = currentCSTNode->token.getValue();
        while (tokenValue != ";") {
            if (tokenValue != "," && tokenValue != "(" && tokenValue != ")" && tokenValue != "\"") {
                insertNode(currentCSTNode, tokenValue, "N/A", "N/A");
                currentCSTNode = currentCSTNode->rightSibling;
                tokenValue = currentCSTNode->token.getValue();
            } else {
                currentCSTNode = currentCSTNode->rightSibling;
                tokenValue = currentCSTNode->token.getValue();
            }
        }

    //For Loop
    } else if (tokenValue == "for") {
        inForStatement = true;
        deque<string> expression1;
        insertNode(currentCSTNode, "FOR EXPRESSION 1", "N/A", "N/A");

        while (tokenValue != ";") {
            if (tokenValue != "(" && tokenValue != ")" && tokenValue != "for") {
                expression1.push_back(tokenValue);
            }

            if (currentCSTNode->rightSibling != nullptr) {
                currentCSTNode = currentCSTNode->rightSibling;
                tokenValue = currentCSTNode->token.getValue();
            }
        }
        inForStatement2 = true;
        inForStatement = false;
        expression1 = infixToPostfix(expression1);
        for (auto it = expression1.begin(); it != expression1.end(); it++) {
            insertNode(currentCSTNode, *it, "N/A", "N/A");
        }
    } else if (inForStatement2) {
        deque<string> expression2;
        insertNode(currentCSTNode, "FOR EXPRESSION 2", "N/A", "N/A");

        while (tokenValue != ";") {
            if (tokenValue != "(" && tokenValue != ")") {
                expression2.push_back(tokenValue);
            }

            if (currentCSTNode->rightSibling != nullptr) {
                currentCSTNode = currentCSTNode->rightSibling;
                tokenValue = currentCSTNode->token.getValue();
            }
        }
        inForStatement2 = false;
        inForStatement3 = true;
        expression2 = infixToPostfix(expression2);
        for (auto it = expression2.begin(); it != expression2.end(); it++) {
            insertNode(currentCSTNode, *it, "N/A", "N/A");
        }
    } else if (inForStatement3) {
        deque<string> expression3;
        insertNode(currentCSTNode, "FOR EXPRESSION 3", "N/A", "N/A");
        parenDepth = 1;
        while (parenDepth > 0) {
            if (tokenValue == ")") {
                parenDepth--;
            }
            if (tokenValue != "(" && tokenValue != ")") {
                expression3.push_back(tokenValue);
            }

            if (currentCSTNode->rightSibling != nullptr) {
                currentCSTNode = currentCSTNode->rightSibling;
                tokenValue = currentCSTNode->token.getValue();
            }
        }
        inForStatement3 = false;
        expression3 = infixToPostfix(expression3);
        for (auto it = expression3.begin(); it != expression3.end(); it++) {
            insertNode(currentCSTNode, *it, "N/A", "N/A");
        }
        inStatement = false;

    //While Loop
    } else if (tokenValue == "while") {
        insertNode(currentCSTNode, "WHILE", "N/A", "N/A");
        deque<string> expression;
        parenDepth = 1;
        while (parenDepth > 0) {
            if (tokenValue == "(") {
                parenDepth++;
            } else if (tokenValue == ")") {
                parenDepth--;
            }

            if (tokenValue != ")" && tokenValue != "(" && tokenValue != "while") {
                expression.push_back(tokenValue);
            }
            if (currentCSTNode->rightSibling != nullptr) {
                currentCSTNode = currentCSTNode->rightSibling;
                tokenValue = currentCSTNode->token.getValue();
            }
        }

        expression = infixToPostfix(expression);
        for (auto it = expression.begin(); it != expression.end(); it++) {
            insertNode(currentCSTNode, *it, "N/A", "N/A");
        }
    }





    if (currentCSTNode->leftChild != nullptr) {
        BuildASTHelper(currentCSTNode->leftChild);
    }

    if (currentCSTNode->rightSibling != nullptr) {
        BuildASTHelper(currentCSTNode->rightSibling);
    }
}


