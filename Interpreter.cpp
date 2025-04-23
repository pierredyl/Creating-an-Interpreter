//
// Created by dylan on 4/19/2025.
//


#include "Interpreter.h"

#include <algorithm>
#include <sstream>
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
    if (str.empty()) return false;

    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') {
        if (str.size() == 1) return false; // Only a sign, no digits
        start = 1;
    }

    return std::all_of(str.begin() + start, str.end(), ::isdigit);
}

int hexCharToInt(char c) {
    if (c >= '0' && c <= '9')        return c - '0';           // '0'→0 … '9'→9
    else if (c >= 'A' && c <= 'F')   return 10 + (c - 'A');    // 'A'→10 … 'F'→15
    else if (c >= 'a' && c <= 'f')   return 10 + (c - 'a');    // 'a'→10 … 'f'→15
    else                             return -1;                 // not a hex digit
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

        // Start of string literal
        if (token == "\"") {
            string stringLiteral;
            current = current->rightSibling;
            while (current != nullptr && current->name != "\"") {
                stringLiteral += current->name;
                current = current->rightSibling;
            }

            size_t pos = stringLiteral.find("\\x0");
            if (pos != string::npos) {
                stringLiteral = stringLiteral.substr(0, pos);
            }

            evalStack.push(stringLiteral);

            if (current != nullptr) current = current->rightSibling;
            continue;
        }

        // Start of char literal
        if (token == "'") {
            current = current->rightSibling;
            if (current != nullptr && current->rightSibling != nullptr && current->rightSibling->name == "'") {
                char charLiteral = current->name[0];
                evalStack.push(to_string(static_cast<int>(charLiteral)));
                current = current->rightSibling->rightSibling;
                continue;
            } else {
                cerr << "Error: Invalid character literal." << endl;
                break;
            }
        }

        if (token == "=") {
            string rightValStr = evalStack.top();
            evalStack.pop();
            string varName = evalStack.top();
            evalStack.pop();

            cout << "currently working on " << rightValStr << " and " << varName << endl;

            if (isNumeric(rightValStr)) {
                int rightVal = stoi(rightValStr);
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, varName, scope);
                if (variable != nullptr) {
                    variable->currentValue = rightVal;
                    cout << "Assigned " << rightVal << " to " << varName << " in scope " << scope << endl;
                } else {
                    cerr << "Error: Variable '" << varName << "' not declared." << endl;
                }
            } else {
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, varName, scope);
                if (variable != nullptr) {
                    variable->currentStringValue = rightValStr;
                    cout << "Assigned " << rightValStr << " to " << varName << " in scope " << scope << endl;
                } else {
                    cerr << "Error: Variable '" << varName << "' not declared." << endl;
                }
            }
        }

        else if (token == "+" || token == "-" || token == "*" || token == "/") {
            string operand2Str = evalStack.top(); evalStack.pop();
            string operand1Str = evalStack.top(); evalStack.pop();
            int operand1, operand2;

            if (containsNonDigit(operand1Str)) {
                SymbolTable::Symbol* var1 = SymbolTable::findSymbol(symbolRoot, operand1Str, scope);
                operand1 = (var1->currentCharValue != '\0') ? static_cast<int>(var1->currentCharValue) : var1->currentValue;
            } else {
                operand1 = stoi(operand1Str);
            }

            if (containsNonDigit(operand2Str)) {
                SymbolTable::Symbol* var2 = SymbolTable::findSymbol(symbolRoot, operand2Str, scope);
                operand2 = (var2->currentCharValue != '\0') ? static_cast<int>(var2->currentCharValue) : var2->currentValue;
            } else {
                operand2 = stoi(operand2Str);
            }

            int result;
            if (token == "+") result = operand1 + operand2;
            else if (token == "-") result = operand1 - operand2;
            else if (token == "*") result = operand1 * operand2;
            else result = operand1 / operand2;

            evalStack.push(to_string(result));
        }

        else if (isFunctionCall(current)) {
            string functionName = current->name;
            vector<int> args;
            auto* functionSym = SymbolTable::findFunctionSymbol(symbolRoot, functionName);
            functionScope = functionSym->scope;
            functionDeclarationNode = findFunctionNode(ASTRoot, functionName);
            currentFunctionNode = functionDeclarationNode;

            AbstractSyntaxTree::ASTNode* argNode = current->rightSibling->rightSibling;
            while (argNode && argNode->name != ")") {
                if (containsNonDigit(argNode->name) && argNode->rightSibling->name != "[") {
                    auto* sym = SymbolTable::findSymbol(symbolRoot, argNode->name, scope);
                    functionSym = functionSym -> next;
                    functionSym->currentValue = sym->currentValue;
                } else if (containsNonDigit(argNode->name) && argNode->rightSibling->name == "[") {
                    auto* var = SymbolTable::findSymbol(symbolRoot, argNode->name, scope);
                    argNode = argNode->rightSibling->rightSibling;
                    auto* varInsideIndex = SymbolTable::findSymbol(symbolRoot, argNode->name, scope);
                    int varIndexValue = varInsideIndex->currentValue;
                    string varValue = var->currentStringValue;
                    char valAtIndexInVar = varValue[varIndexValue];
                    cout << "current char being processed is: " << valAtIndexInVar << endl;
                    functionSym = functionSym -> next;
                    functionSym->currentCharValue = valAtIndexInVar;
                    cout << "just assigned " << functionSym->identifierName << ": " << functionSym->currentCharValue << endl;
                }
                argNode = argNode->rightSibling;
            }

            int result = executeFunctionCall(functionName);
            evalStack.push(to_string(result));
            current = argNode;
        } else {
            evalStack.push(token);
        }

        current = current->rightSibling;
    }
}


AbstractSyntaxTree::ASTNode* Interpreter::executeIfStatement(AbstractSyntaxTree::ASTNode* node, int scope) {
    if (isIfStatementTrue(node, scope)) {
        node = moveNodeDownOneStatement(node);

        node = executeBlock(node, scope);

    } else {
        node = moveNodeDownOneStatement(node);
        int currentBraceDepth = 0;

        if (node->name == "BEGIN BLOCK") {
            currentBraceDepth++;
            node = node->leftChild;

            while (currentBraceDepth > 0) {
                if (node->name == "END BLOCK") {
                    currentBraceDepth--;
                }
                node = moveNodeDownOneStatement(node);
            }

            if (node->name == "ELSE") {
                //move ahead
                node = moveNodeDownOneStatement(node);
                executeBlock(node, scope);
            }
        }
    }
    return node;
}

AbstractSyntaxTree::ASTNode* Interpreter::executeBlock(AbstractSyntaxTree::ASTNode* node, int scope) {
    int blockBraceDepth = 0;
    if (node->name == "BEGIN BLOCK") {
        blockBraceDepth++;
        node = node->leftChild;
    }

    while (blockBraceDepth > 0) {
        if (node->name == "ASSIGNMENT") {
            node = node->rightSibling;
            executeAssignment(node, scope);
        } else if (node->name == "END BLOCK") {
            blockBraceDepth--;
            if (blockBraceDepth == 0) {
                break;
            }
        } else if (node->name == "BEGIN BLOCK") {
            blockBraceDepth++;
        } else if (node->name == "if") {
            //Move past if
            node = node->rightSibling;
            node = executeIfStatement(node, scope);
        }

        node = moveNodeDownOneStatement(node);
    }
        return node;
}

bool Interpreter::isIfStatementTrue(AbstractSyntaxTree::ASTNode* node, int scope) {
    stack<int> evalStack;

    while (node != nullptr) {
        string token = node->name;

        // Handle character literals: ' -> c -> '
        if (token == "'") {
            if (node->rightSibling && node->rightSibling->rightSibling &&
                node->rightSibling->rightSibling->name == "'") {

                string charStr = node->rightSibling->name;
                if (charStr.length() != 1) {
                    cerr << "Error: Invalid character literal: " << charStr << endl;
                    return false;
                }

                // Push ASCII value of the character
                char c = charStr[0];
                evalStack.push(static_cast<int>(c));

                // Skip over the char literal parts: ' c '
                node = node->rightSibling->rightSibling->rightSibling;
                continue;
            } else {
                cerr << "Error: Malformed character literal in condition." << endl;
                return false;
            }
        }

        // Operators
        if (token == "==" || token == "!=" ||
            token == "<"  || token == "<=" ||
            token == ">"  || token == ">=" ||
            token == "&&")
        {
            if (evalStack.size() < 2) {
                cerr << "Error: not enough operands in if condition." << endl;
                return false;
            }

            int right = evalStack.top(); evalStack.pop();
            int left  = evalStack.top(); evalStack.pop();

            int cmpResult = 0;
            if      (token == "==") cmpResult = (left == right);
            else if (token == "!=") cmpResult = (left != right);
            else if (token == "<")  cmpResult = (left <  right);
            else if (token == "<=") cmpResult = (left <= right);
            else if (token == ">")  cmpResult = (left >  right);
            else if (token == ">=") cmpResult = (left >= right);
            else if (token == "&&") cmpResult = (left && right);

            evalStack.push(cmpResult);
        }
        else {
            int value = 0;
            if (containsNonDigit(token)) {
                SymbolTable::Symbol* sym = SymbolTable::findSymbol(symbolRoot, token, scope);
                if (!sym) {
                    cerr << "Error: Variable " << token << " not declared." << endl;
                    return false;
                }

                if (sym->currentCharValue != '\0') {
                    value = static_cast<int>(sym->currentCharValue); // ASCII value
                } else {
                    value = sym->currentValue;
                }
            } else {
                value = stoi(token);
            }
            evalStack.push(value);
        }

        node = node->rightSibling;
    }

    if (evalStack.empty()) {
        cerr << "Error: no result from if condition evaluation." << endl;
        return false;
    }

    return evalStack.top() != 0;
}




int Interpreter::executeFunctionCall(string) {
    currentFunctionNode = currentFunctionNode->leftChild;
    if (currentFunctionNode->name == "BEGIN BLOCK") {
        functionBraceDepth++;
        currentFunctionNode = currentFunctionNode->leftChild;
    }

    while (functionBraceDepth > 0 && currentFunctionNode != nullptr) {
        if (currentFunctionNode->name == "BEGIN BLOCK") {
            functionBraceDepth++;
        }

        if (currentFunctionNode->name == "END BLOCK") {
            functionBraceDepth--;
        }

        if (currentFunctionNode->name == "if") {
            currentFunctionNode = currentFunctionNode->rightSibling;
            currentFunctionNode = executeIfStatement(currentFunctionNode, functionScope);
        }


        if (currentFunctionNode->name == "ASSIGNMENT") {
            //move past the assignment node
            currentFunctionNode = currentFunctionNode->rightSibling;
            //begin executing the assignment
            executeAssignment(currentFunctionNode, functionScope);
        }

        if (currentFunctionNode->name == "RETURN") {
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
        currentMainProcedureNode = currentMainProcedureNode->leftChild;
    }

    while (mainBraceDepth > 0 && currentMainProcedureNode != nullptr) {
        if (currentMainProcedureNode->name == "BEGIN BLOCK") {
            mainBraceDepth++;
        }

        if (currentMainProcedureNode->name == "END BLOCK") {
            mainBraceDepth--;
        }


        if (currentMainProcedureNode->name == "ASSIGNMENT") {
            //move past the assignment node
            currentMainProcedureNode = currentMainProcedureNode->rightSibling;
            //begin executing the assignment
            executeAssignment(currentMainProcedureNode, mainScope);
        }

        if (currentMainProcedureNode->name == "PRINTF") {
            currentMainProcedureNode = currentMainProcedureNode->rightSibling;
            executePrintf();
        }

        if (currentMainProcedureNode->name == "FOR EXPRESSION 1") {
            currentMainProcedureNode = currentMainProcedureNode->leftChild;
            currentMainProcedureNode = executeForLoop(currentMainProcedureNode, mainScope);
        }


        //AST Traversal
        while (currentMainProcedureNode->rightSibling != nullptr) {
            currentMainProcedureNode = currentMainProcedureNode->rightSibling;
        }
        currentMainProcedureNode = currentMainProcedureNode->leftChild;
    }
}

AbstractSyntaxTree::ASTNode* Interpreter::moveNodeDownOneStatement(AbstractSyntaxTree::ASTNode* node) {
    while (node != nullptr && node->rightSibling != nullptr) {
        node = node->rightSibling;
    }
    node = node -> leftChild;
    return node;
}

AbstractSyntaxTree::ASTNode* Interpreter::executeForLoop(AbstractSyntaxTree::ASTNode* node, int scope) {
    AbstractSyntaxTree::ASTNode* current = node;
    //Execute For Expression 1
    executeAssignment(current, scope);

    //Move to For Expression 2
    current = moveNodeDownOneStatement(current);

    //Move past FOR EXPRESSION 2 Node
    current = current->leftChild;
    //Create a node that doesn't move to repeatedly check the condition of the loop
    AbstractSyntaxTree::ASTNode* conditionalNode = current;
    if (isIfStatementTrue(conditionalNode, scope)) {
        current = moveNodeDownOneStatement(current);
        //Move past FOR EXPRESSION 3
        current = current->leftChild;

        //Create a new node to hold assignment operation
        AbstractSyntaxTree::ASTNode* forAssignmentNode = current;


        //Begin executing after the for expression
        current = moveNodeDownOneStatement(current);
        AbstractSyntaxTree::ASTNode* loopBodyStart = current;
        while (isIfStatementTrue(conditionalNode, scope)) {
            executeBlock(loopBodyStart, scope);
            cout << "+1 iteration in for loop" << endl;
            executeAssignment(forAssignmentNode, scope);
        }
    }

    current = FindEndOfForLoopBody(current);
    return current;
}

AbstractSyntaxTree::ASTNode* Interpreter::FindEndOfForLoopBody(AbstractSyntaxTree::ASTNode* node) {
    int braceDepth = 0;
    if (node->name == "BEGIN BLOCK") {
        braceDepth++;
    }

    node = moveNodeDownOneStatement(node);
    while (braceDepth > 0) {
        if (node->name == "BEGIN BLOCK") {
            braceDepth++;
        } else if (node->name == "END BLOCK") {
            braceDepth--;
            if (braceDepth == 0) {
                break;
            }
        }
        node = moveNodeDownOneStatement(node);
    }
    return node;
}

void Interpreter::executePrintf() {
    string printString = currentMainProcedureNode->name;
    AbstractSyntaxTree::ASTNode* argNode = currentMainProcedureNode->rightSibling;

    // Store both ints and strings
    vector<string> formattedArgs;

    // Collect all arguments
    while (argNode != nullptr) {
        string token = argNode->name;

        // Try to determine if it's a variable or a literal
        if (containsNonDigit(token)) {
            SymbolTable::Symbol* sym = SymbolTable::findSymbol(symbolRoot, token, mainScope);
            if (!sym) {
                cerr << "Error: Variable " << token << " not declared." << endl;
                return;
            }

            if (!sym->currentStringValue.empty()) {
                formattedArgs.push_back(sym->currentStringValue);
            } else {
                formattedArgs.push_back(to_string(sym->currentValue));
            }
        } else {
            // It's a literal (either string or int in quotes)
            if (token.front() == '"' && token.back() == '"') {
                formattedArgs.push_back(token.substr(1, token.length() - 2)); // Remove quotes
            } else {
                formattedArgs.push_back(token); // Just a number
            }
        }

        argNode = argNode->rightSibling;
    }

    // Replace each format specifier with argument
    size_t pos = 0;
    int argIndex = 0;
    while (argIndex < formattedArgs.size() &&
           (pos = printString.find_first_of('%', pos)) != string::npos) {
        if (pos + 1 >= printString.size()) break; // Safety check
        char specifier = printString[pos + 1];

        if (specifier == 'd' || specifier == 's') {
            printString.replace(pos, 2, formattedArgs[argIndex++]);
        }
        pos += 1; // Move forward
    }

    // Replace \n with actual newlines
    size_t newlinePos;
    while ((newlinePos = printString.find("\\n")) != string::npos) {
        printString.replace(newlinePos, 2, "\n");
    }

    cout << "\n\n\n\n\n\n\n" << endl;
    cout << printString << endl;
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
