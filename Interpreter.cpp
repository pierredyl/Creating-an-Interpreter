//
// Created by dylan on 4/19/2025.
//


#include "Interpreter.h"

#include <algorithm>
#include <functional>
#include <stack>

using namespace std;


Interpreter::Interpreter(AbstractSyntaxTree AST, SymbolTable symbolTable) {
    ASTRoot = AST.getRoot();
    symbolRoot = symbolTable.getHead();
    mainBraceDepth = 0;
    mainScope = 0;
    ifStatementDepth = 0;
    functionBraceDepth = 0;
}

void Interpreter::interpret() {
    //Find the main procedure and set its scope
    mainProcedureDeclarationNode = findMainProcedure(ASTRoot);
    mainScope = mainProcedureDeclarationNode->symbolPointer->scope;
    currentMainProcedureNode = mainProcedureDeclarationNode;

    //Skip one ahead for the node after the declaration
    currentMainProcedureNode = mainProcedureDeclarationNode->leftChild;
    //Begin executing from main
    executeMain();
}
bool isNumeric(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

bool containsNonDigit(const std::string& str) {
    return !isNumeric(str);
}

bool Interpreter::isFunctionCall(AbstractSyntaxTree::ASTNode* node) {
    return node && node->rightSibling && node->rightSibling->name == "(";
}

void Interpreter::executeAssignment(AbstractSyntaxTree::ASTNode* node, int scope) {
    stack<string> evalStack;
    AbstractSyntaxTree::ASTNode* current = node;

    while (current != nullptr) {
        string token = current->name;
        string type = current->type;

        if (token == "=") {
            // Right-hand side and left-hand side variable names
            string rightValStr = evalStack.top();
            evalStack.pop();
            string varName = evalStack.top();
            evalStack.pop();


            int rightVal = stoi(rightValStr); // Convert the string to integer

            // Look for variable in the symbol table
            SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, varName, scope);
            if (variable != nullptr) {
                variable->currentValue = rightVal;
                cout << "Assigned " << rightVal << " to " << varName << " in scope " << scope << endl;
            } else {
                cerr << "Error: Variable '" << varName << "' not declared." << endl;
            }
        }
        else if (token == "+") {
            // Pop two operands for addition
            string operand2Str = evalStack.top();
            evalStack.pop();
            string operand1Str = evalStack.top();
            evalStack.pop();


            if (containsNonDigit(operand1Str)) {
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand1Str, scope);
                int operand1 = variable->currentValue;
                if (containsNonDigit(operand2Str)) {
                    SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand2Str, scope);
                    int operand2 = variable->currentValue;
                    int result = operand1 + operand2;
                    evalStack.push(to_string(result));
                } else {
                    int operand2 = stoi(operand2Str);
                    int result = operand1 + operand2;
                    evalStack.push(to_string(result));
                }
            } else if (containsNonDigit(operand2Str)) {
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand2Str, scope);
                int operand2 = variable->currentValue;
                int operand1 = stoi(operand1Str);
                int result = operand1 + operand2;
                evalStack.push(to_string(result));
            } else {
                int operand1 = stoi(operand1Str);
                int operand2 = stoi(operand2Str);
                int result = operand1 + operand2;
                evalStack.push(to_string(result));
            }
        } else if (token == "*") {
            // Pop two operands for multiplication
            string operand2Str = evalStack.top();
            evalStack.pop();
            string operand1Str = evalStack.top();
            evalStack.pop();


            if (containsNonDigit(operand1Str)) {
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand1Str, scope);
                int operand1 = variable->currentValue;
                if (containsNonDigit(operand2Str)) {
                    SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand2Str, scope);
                    int operand2 = variable->currentValue;
                    int result = operand1 * operand2;
                    evalStack.push(to_string(result));
                } else {
                    int operand2 = stoi(operand2Str);
                    int result = operand1 * operand2;
                    evalStack.push(to_string(result));
                }
            } else if (containsNonDigit(operand2Str)) {
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand2Str, scope);
                int operand2 = variable->currentValue;
                int operand1 = stoi(operand1Str);
                int result = operand1 * operand2;
                evalStack.push(to_string(result));
            } else {
                int operand1 = stoi(operand1Str);
                int operand2 = stoi(operand2Str);
                int result = operand1 * operand2;
                evalStack.push(to_string(result));
            }
        } else if (token == "-") {
            // Pop two operands for subtraction
            string operand2Str = evalStack.top();
            evalStack.pop();
            string operand1Str = evalStack.top();
            evalStack.pop();

            if (containsNonDigit(operand1Str)) {
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand1Str, scope);
                int operand1 = variable->currentValue;
                if (containsNonDigit(operand2Str)) {
                    SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand2Str, scope);
                    int operand2 = variable->currentValue;
                    int result = operand1 - operand2;
                    evalStack.push(to_string(result));
                } else {
                    int operand2 = stoi(operand2Str);
                    int result = operand1 - operand2;
                    evalStack.push(to_string(result));
                }
            } else if (containsNonDigit(operand2Str)) {
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand2Str, scope);
                int operand2 = variable->currentValue;
                int operand1 = stoi(operand1Str);
                int result = operand1 - operand2;
                evalStack.push(to_string(result));
            } else {
                int operand1 = stoi(operand1Str);
                int operand2 = stoi(operand2Str);
                int result = operand1 - operand2;
                evalStack.push(to_string(result));
            }
        } else if (token == "/") {
            // Pop two operands for division
            string operand2Str = evalStack.top();
            evalStack.pop();
            string operand1Str = evalStack.top();
            evalStack.pop();

            if (containsNonDigit(operand1Str)) {
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand1Str, scope);
                int operand1 = variable->currentValue;
                if (containsNonDigit(operand2Str)) {
                    SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand2Str, scope);
                    int operand2 = variable->currentValue;
                    int result = operand1 / operand2;
                    evalStack.push(to_string(result));
                } else {
                    int operand2 = stoi(operand2Str);
                    int result = operand1 / operand2;
                    evalStack.push(to_string(result));
                }
            } else if (containsNonDigit(operand2Str)) {
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, operand2Str, scope);
                int operand2 = variable->currentValue;
                int operand1 = stoi(operand1Str);
                int result = operand1 / operand2;
                evalStack.push(to_string(result));
            } else {
                int operand1 = stoi(operand1Str);
                int operand2 = stoi(operand2Str);
                int result = operand1 / operand2;
                evalStack.push(to_string(result));
            }
        } else if (isFunctionCall(current)) {
            string functionName = current->name;
            vector<int> args;
            auto* functionSym = SymbolTable::findFunctionSymbol(symbolRoot, functionName);
            functionScope = functionSym->scope;
            functionDeclarationNode = findFunctionNode(ASTRoot, functionName);
            currentFunctionNode = functionDeclarationNode;

            AbstractSyntaxTree::ASTNode* argNode = current->rightSibling->rightSibling; // skip '('
            while (argNode && argNode->name != ")") {
                if (containsNonDigit(argNode->name)) {
                    auto* sym = SymbolTable::findSymbol(symbolRoot, argNode->name, scope);
                    functionSym = functionSym -> next;
                    functionSym->currentValue = sym->currentValue;
                }

                argNode = argNode->rightSibling;
            }

            int result = executeFunctionCall(functionName);
            evalStack.push(to_string(result));

            // Skip the entire function call subtree
            current = argNode; // This points to ')'
        } else {
            evalStack.push(token);
        }

        current = current->rightSibling;
    }
}

AbstractSyntaxTree::ASTNode* Interpreter::executeIfStatement(AbstractSyntaxTree::ASTNode* node) {
    if (isIfStatementTrue(node)) {
        cout << "if statement is true. moving to the next statement and evaluating" << endl;
        while (node->leftChild == nullptr) {
            node = node->rightSibling;
        }
        node = node->leftChild;

        int ifBlockBraceDepth = 0;
        if (node->name == "BEGIN BLOCK") {
            ifBlockBraceDepth++;
            node = node->leftChild;
        }

        while (ifBlockBraceDepth > 0) {
            if (node->name == "ASSIGNMENT") {
                node = node->rightSibling;
                executeAssignment(node, functionScope);
                //move to next statement
                while (node->leftChild == nullptr) {
                    node = node->rightSibling;
                }

            } else if (node->name == "END BLOCK") {
                ifBlockBraceDepth--;
            }
            node = node->leftChild;
        }
    } else {
        cout << "If statement is false. Skipping over" << endl;
    }

    return node;

}

bool Interpreter::isIfStatementTrue(AbstractSyntaxTree::ASTNode* node) {
    stack<int> evalStack;

    while (node != nullptr) {
        string token = node->name;

        if (token == "==" || token == "!=" || token == "<" || token == "<=" || token == ">" || token == ">=") {
            if (evalStack.size() < 2) {
                cerr << "Error: not enough operands in if condition." << endl;
                return false;
            }

            int right = evalStack.top(); evalStack.pop();
            int left = evalStack.top(); evalStack.pop();

            if (token == "==") return left == right;
            if (token == "!=") return left != right;
            if (token == "<")  return left < right;
            if (token == "<=") return left <= right;
            if (token == ">")  return left > right;
            if (token == ">=") return left >= right;
        } else {

            int value;
            if (containsNonDigit(token)) {
                SymbolTable::Symbol* sym = SymbolTable::findSymbol(symbolRoot, token, functionScope);
                if (!sym) {
                    cerr << "Error: Variable " << token << " not declared." << endl;
                    return false;
                }
                value = sym->currentValue;
            } else {
                value = stoi(token);
            }
            evalStack.push(value);
        }

        node = node->rightSibling;
    }

    cerr << "Error: no operator found in if condition." << endl;
    return false;
}


int Interpreter::executeFunctionCall(string) {
    currentFunctionNode = currentFunctionNode->leftChild;
    if (currentFunctionNode->name == "BEGIN BLOCK") {
        functionBraceDepth++;
        cout << "New BEGIN BLOCK. functionBraceDepth = " << functionBraceDepth << endl;
        currentFunctionNode = currentFunctionNode->leftChild;
    }

    while (functionBraceDepth > 0 && currentFunctionNode != nullptr) {
        if (currentFunctionNode->name == "BEGIN BLOCK") {
            functionBraceDepth++;
            cout << "New BEGIN BLOCK. functionBraceDepth = " << functionBraceDepth << endl;
        }

        if (currentFunctionNode->name == "END BLOCK") {
            functionBraceDepth--;
            cout << "New END BLOCK. functionBraceDepth = " << functionBraceDepth << endl;
        }

        if (currentFunctionNode->name == "if") {
            currentFunctionNode = currentFunctionNode->rightSibling;
            currentFunctionNode = executeIfStatement(currentFunctionNode);
            cout << "returned" << endl;
        }


        if (currentFunctionNode->name == "ASSIGNMENT") {
            cout << "New ASSIGNMENT." << endl;
            //move past the assignment node
            currentFunctionNode = currentFunctionNode->rightSibling;
            //begin executing the assignment
            executeAssignment(currentFunctionNode, functionScope);
        }

        if (currentFunctionNode->name == "RETURN") {
            cout << "Returning from function call" << endl;
            currentFunctionNode = currentFunctionNode->rightSibling;

            SymbolTable::Symbol* returnVariable = SymbolTable::findSymbol(symbolRoot, currentFunctionNode->name,
                                                                        functionScope);

            currentFunctionNode = nullptr;
            functionDeclarationNode = nullptr;
            functionScope = -1;
            return returnVariable->currentValue;
        }


        //AST Traversal
        while (currentFunctionNode->rightSibling != nullptr) {
            currentFunctionNode = currentFunctionNode->rightSibling;
        }
        currentFunctionNode = currentFunctionNode->leftChild;
    }
}




void Interpreter::executeMain() {
    if (currentMainProcedureNode->name == "BEGIN BLOCK") {
        mainBraceDepth++;
        cout << "New BEGIN BLOCK. mainBraceDepth = " << mainBraceDepth << endl;
        currentMainProcedureNode = currentMainProcedureNode->leftChild;
    }

    while (mainBraceDepth > 0 && currentMainProcedureNode != nullptr) {
        if (currentMainProcedureNode->name == "BEGIN BLOCK") {
            mainBraceDepth++;
            cout << "New BEGIN BLOCK. mainBraceDepth = " << mainBraceDepth << endl;
        }

        if (currentMainProcedureNode->name == "END BLOCK") {
            mainBraceDepth--;
            cout << "New END BLOCK. mainBraceDepth = " << mainBraceDepth << endl;
        }


        if (currentMainProcedureNode->name == "ASSIGNMENT") {
            cout << "New ASSIGNMENT." << endl;
            //move past the assignment node
            currentMainProcedureNode = currentMainProcedureNode->rightSibling;
            //begin executing the assignment
            executeAssignment(currentMainProcedureNode, mainScope);
        }

        if (currentMainProcedureNode->name == "PRINTF") {
            currentMainProcedureNode = currentMainProcedureNode->rightSibling;

            string printString = currentMainProcedureNode->name;
            AbstractSyntaxTree::ASTNode* argNode = currentMainProcedureNode->rightSibling;
            vector<int> args;

            // Collect all arguments
            while (argNode != nullptr) {

                if (argNode->leftChild != nullptr) {
                    currentMainProcedureNode = argNode->leftChild;
                }

                string token = argNode->name;
                int value;

                if (containsNonDigit(token)) {
                    SymbolTable::Symbol* sym = SymbolTable::findSymbol(symbolRoot, token, mainScope);
                    if (!sym) {
                        cerr << "Error: Variable " << token << " not declared." << endl;
                        return;
                    }
                    value = sym->currentValue;
                } else {
                    value = stoi(token);
                }

                cout << "pushed " << value << endl;
                args.push_back(value);
                argNode = argNode->rightSibling;
            }

            // Replace each %d with corresponding value
            size_t pos = 0;
            int argIndex = 0;
            while ((pos = printString.find("%d", pos)) != string::npos && argIndex < args.size()) {
                printString.replace(pos, 2, to_string(args[argIndex++]));
                pos += 1;  // move forward
            }

            size_t newlinePos;
            while ((newlinePos = printString.find("\\n")) != string::npos) {
                printString.replace(newlinePos, 2, "\n");
            }
            cout << "\n\n\n\n\n\n\n" << endl;
            cout << printString << endl;
        }


        //AST Traversal
        while (currentMainProcedureNode->rightSibling != nullptr) {
            currentMainProcedureNode = currentMainProcedureNode->rightSibling;
        }
        currentMainProcedureNode = currentMainProcedureNode->leftChild;
    }
}

AbstractSyntaxTree::ASTNode* Interpreter::findMainProcedure(AbstractSyntaxTree::ASTNode* currentNode) {
    if (currentNode == nullptr) {
        return nullptr;
    }

    string nodeName = currentNode->name;

    // Check if this node is a "DECLARATION" of a "main" procedure
    if (nodeName == "DECLARATION") {
        string identifierType = currentNode->symbolPointer->identifierType;
        string identifierName = currentNode->symbolPointer->identifierName;

        if (identifierName == "main" && identifierType == "procedure") {
            return currentNode; // Found the main procedure, return it
        }
    }

    // Recursively search in left child first
    AbstractSyntaxTree::ASTNode* leftResult = findMainProcedure(currentNode->leftChild);
    if (leftResult != nullptr) {
        return leftResult; // If found in left child, return it
    }

    // If not found in left child, search in the right sibling
    return findMainProcedure(currentNode->rightSibling);
}

AbstractSyntaxTree::ASTNode* Interpreter::findFunctionNode(AbstractSyntaxTree::ASTNode* currentNode, string name) {
    if (currentNode == nullptr) {
        return nullptr;
    }

    string nodeName = currentNode->name;

    // Check if this node is a "DECLARATION" of a "main" procedure
    if (nodeName == "DECLARATION") {
        string identifierType = currentNode->symbolPointer->identifierType;
        string identifierName = currentNode->symbolPointer->identifierName;

        if (identifierName == name && identifierType == "function") {
            return currentNode; // Found the main procedure, return it
        }
    }

    // Recursively search in left child first
    AbstractSyntaxTree::ASTNode* leftResult = findMainProcedure(currentNode->leftChild);
    if (leftResult != nullptr) {
        return leftResult; // If found in left child, return it
    }

    // If not found in left child, search in the right sibling
    return findMainProcedure(currentNode->rightSibling);
}
