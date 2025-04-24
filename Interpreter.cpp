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

            if (isNumeric(rightValStr)) {
                int rightVal = stoi(rightValStr);
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, varName, scope);
                if (variable != nullptr) {
                    variable->currentValue = rightVal;
                } else {
                    cerr << "Error: Variable '" << varName << "' not declared." << endl;
                }
            } else {
                SymbolTable::Symbol* variable = SymbolTable::findSymbol(symbolRoot, varName, scope);
                if (variable != nullptr) {
                    variable->currentStringValue = rightValStr;
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
                if (containsNonDigit(argNode->name) && argNode->rightSibling->name != "[" && argNode->name != "]") {
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
                    functionSym = functionSym -> next;
                    functionSym->currentCharValue = valAtIndexInVar;
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

AbstractSyntaxTree::ASTNode* Interpreter::movePastIfBlock(AbstractSyntaxTree::ASTNode* node, int scope) {
    AbstractSyntaxTree::ASTNode* current = node;
    //move to if's begin block
    current = moveNodeDownOneStatement(current);
    int braceDepth = 0;
    if (current->name == "BEGIN BLOCK") {
        braceDepth++;
        current = moveNodeDownOneStatement(current);
    }

    while (braceDepth > 0) {
        if (current->name == "END BLOCK") {
            braceDepth--;
            if (braceDepth == 0) {
                current = current->leftChild;
                break;
            }
        } else if (current->name == "BEGIN BLOCK") {
            braceDepth++;
        } else {
            current = moveNodeDownOneStatement(current);
        }
    }

    return current;
}


AbstractSyntaxTree::ASTNode* Interpreter::executeIfStatement(AbstractSyntaxTree::ASTNode* node, int scope) {
    AbstractSyntaxTree::ASTNode* current = node;

    //Node passed in right past the if. For example, if -> counter -> 100 -> <=. Starts at counter

    //Check if the if expression is true. if it is, I should evaluate the if statement.
    if (isIfStatementTrue(current, scope)) {
        //Moving one down should mark the begin block before the inside body of the if expression.
        current = moveNodeDownOneStatement(current);

        //executeBlock should be able to handle this.
        current = executeBlock(current, scope);

        //Execute block should return the node marking the next statement after the block has ended.
        //If this is an if statement, we can re call executeIfStatement, since it is not nested.
        if (current->name == "if") {
            current = current->rightSibling;
            current = executeIfStatement(current, scope);
        }

        //however, if it is an else, we're dealing with an if else expression. since the first if statement succeeded,
        //everything under the else must be skipped.
        if (current->name == "ELSE") {
            //Move passed the else statement.
            current = current->leftChild;
            int braceDepth = 0;
            if (current->name == "BEGIN BLOCK") {
                braceDepth++;
                current = moveNodeDownOneStatement(current);
                while (braceDepth > 0) {
                    if (current->name == "END BLOCK") {
                        braceDepth--;
                        if (braceDepth == 0) {
                            current = current->leftChild;
                            break;
                        }
                    } else if (current->name == "BEGIN BLOCK") {
                        braceDepth++;
                    }
                    current = moveNodeDownOneStatement(current);
                }
            }
        }

    } else {
        //if statement is false. It should move passed the entire if block, and check if there is another if or else.
        current = movePastIfBlock(current, scope);

        if (current->name == "if") {
            current = current->rightSibling;
            current = executeIfStatement(current, scope);
        }

        if (current->name == "ELSE") {
            //Move passed else
            current = current->leftChild;
            //begin executing the block
            current = executeBlock(current, scope);
        }
    }

    //if final statement isnt else, just return

    return current;
}

    /*
    AbstractSyntaxTree::ASTNode* current = node;
    if (isIfStatementTrue(current, scope)) {
        current = moveNodeDownOneStatement(current);
        current = executeBlock(current, scope);

    } else {
        current = moveNodeDownOneStatement(current);
        int currentBraceDepth = 0;

        if (current->name == "BEGIN BLOCK") {
            currentBraceDepth++;
            current = current->leftChild;

            while (currentBraceDepth > 0) {
                if (current->name == "BEGIN BLOCK") {
                    currentBraceDepth++;
                }
                if (current->name == "END BLOCK") {
                    currentBraceDepth--;
                    if (currentBraceDepth == 0) {
                        current = moveNodeDownOneStatement(current);
                        break;
                    }
                }

                if (current->name == "ELSE") {
                    current = current->leftChild;

                    int blockDepth = 0;
                    if (current->name == "BEGIN BLOCK") {
                        blockDepth++;

                        while (blockDepth > 0) {
                            if (current->name == "END BLOCK") {
                                blockDepth--;
                                if (blockDepth == 0) {
                                    break;
                                }
                            } else if (current->name == "BEGIN BLOCK") {
                                blockDepth++;
                            } else if (current->name == "if") {
                                current = current->rightSibling;
                                current = executeIfStatement(current, scope);
                            }
                        }
                    }
                }
                current = moveNodeDownOneStatement(current);
            }
        }
    }
    return current;
}
*/

AbstractSyntaxTree::ASTNode* Interpreter::executeBlock(AbstractSyntaxTree::ASTNode* node, int scope) {
    AbstractSyntaxTree::ASTNode* current = node;
    int blockBraceDepth = 0;
    if (current->name == "BEGIN BLOCK") {
        blockBraceDepth++;
        current = current->leftChild;
    }

    while (blockBraceDepth > 0) {
        if (current->name == "ASSIGNMENT") {
            current = current->rightSibling;
            executeAssignment(current, scope);
        } else if (current->name == "END BLOCK") {
            blockBraceDepth--;
            if (blockBraceDepth == 0) {
                current = current->leftChild;
                break;
            }
        } else if (current->name == "BEGIN BLOCK") {
            blockBraceDepth++;
        } else if (current->name == "if") {
            //Move past if
            current = current->rightSibling;
            current = executeIfStatement(current, scope);
        } else if (current->name == "PRINTF") {
            current = current->rightSibling;
            current = executePrintf(current, scope);
        } else if (current->name == "CALL") {
            AbstractSyntaxTree::ASTNode* nodeTracker;

            nodeTracker = current;

            current = current->rightSibling;
            current = executeProcedureCall(current, scope);

            //return current to its proper place
            current = nodeTracker;
        }

        //bug fix for some methods that close end blocks. this will prevent an end block from being skipped.

        if (current->name == "END BLOCK") {
            blockBraceDepth--;
        }

        current = moveNodeDownOneStatement(current);
    }
        return current;
}

AbstractSyntaxTree::ASTNode* Interpreter::executeProcedureCall(AbstractSyntaxTree::ASTNode* node, int scope) {
    int procedureScope = -1;
    AbstractSyntaxTree::ASTNode* current = node;

    string procedureName = current->name;
    //move past (
    current = current->rightSibling->rightSibling;

    string paramName = current->name;

    AbstractSyntaxTree::ASTNode* procedureDeclNode = nullptr;

    procedureDeclNode = findProcedureNode(ASTRoot, procedureName);
    procedureScope = procedureDeclNode->symbolPointer->scope;

    SymbolTable::Symbol* procParam = procedureDeclNode->symbolPointer->next;
    auto* sym = SymbolTable::findSymbol(symbolRoot, paramName, scope);
    procParam->currentValue = sym->currentValue;


    procedureDeclNode = procedureDeclNode->leftChild;
    current = procedureDeclNode;

    //Begin procedure call
    int braceDepth = 0;

    if (current->name == "BEGIN BLOCK") {
        braceDepth++;
        current = current->leftChild;
    }

    while (braceDepth > 0) {
        //cout << "current name: " << current->name << endl;
        if (current->name == "END BLOCK") {
            braceDepth--;
            if (braceDepth == 0) {
                break;
            }
        }

        if (current->name == "BEGIN BLOCK") {
            braceDepth++;
        }

        if (current->name == "if") {
            current = current->rightSibling;
            current = executeIfStatement(current, procedureScope);
        }

        if (current->name == "PRINTF") {
            current = current->rightSibling;
            current = executePrintf(current, procedureScope);
        }

        if (current->name == "ASSIGNMENT") {
            current = current->rightSibling;
            executeAssignment(current, procedureScope);
        }

        //bug fix for some methods that close end braces.
        if (current->name == "END BLOCK") {
            braceDepth--;
        }

        current = moveNodeDownOneStatement(current);
    }

    return current;
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
            token == "&&" || token == "%")
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
            else if (token == "%")  cmpResult = (left % right);
            else if (token == "<")  cmpResult = (left <  right);
            else if (token == "<=") cmpResult = (left <= right);
            else if (token == ">")  cmpResult = (left >  right);
            else if (token == ">=") cmpResult = (left >= right);
            else if (token == "&&") cmpResult = (left && right);
            //cout << "Evaluating: " << left << " " << token << " " << right << " = " << cmpResult << endl;
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
            //cout << "Pushing: " << value << endl;
            evalStack.push(value);
        }

        node = node->rightSibling;
    }

    if (evalStack.empty()) {
        cerr << "Error: no result from if condition evaluation." << endl;
        return false;
    }

    //cout << "EvalStack (top): " << evalStack.top() << endl;
    if (evalStack.top() != 0) {
        return true;
    } else {
        return false;
    }
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
            executePrintf(currentMainProcedureNode, mainScope);
        }

        if (currentMainProcedureNode->name == "FOR EXPRESSION 1") {
            currentMainProcedureNode = currentMainProcedureNode->leftChild;
            currentMainProcedureNode = executeForLoop(currentMainProcedureNode, mainScope);
        }

        if (currentMainProcedureNode->name == "WHILE") {
            currentMainProcedureNode = currentMainProcedureNode->rightSibling;
            currentMainProcedureNode = executeWhileLoop(currentMainProcedureNode, mainScope);
        }


        //AST Traversal
        while (currentMainProcedureNode->rightSibling != nullptr) {
            currentMainProcedureNode = currentMainProcedureNode->rightSibling;
        }
        currentMainProcedureNode = currentMainProcedureNode->leftChild;
    }
}

AbstractSyntaxTree::ASTNode* Interpreter::executeWhileLoop(AbstractSyntaxTree::ASTNode* node, int scope) {
    AbstractSyntaxTree::ASTNode* current = node;
    AbstractSyntaxTree::ASTNode* whileStatement = node;
    AbstractSyntaxTree::ASTNode* startOfWhileLoop = node;

    startOfWhileLoop = moveNodeDownOneStatement(startOfWhileLoop);

    if (isIfStatementTrue(current,scope)) {
        current = moveNodeDownOneStatement(current);
        while (isIfStatementTrue(whileStatement,scope)) {
            current = executeBlock(current, scope);
            if (isIfStatementTrue(whileStatement, scope)) {
                current = startOfWhileLoop;
            }
        }
    }

    return current;
}

AbstractSyntaxTree::ASTNode* Interpreter::moveNodeDownOneStatement(AbstractSyntaxTree::ASTNode* node) {
    AbstractSyntaxTree::ASTNode* current = node;
    while (current != nullptr && current->rightSibling != nullptr) {
        current = current->rightSibling;
    }
    current = current -> leftChild;
    return current;
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
            current = executeBlock(loopBodyStart, scope);
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

AbstractSyntaxTree::ASTNode* Interpreter::executePrintf(AbstractSyntaxTree::ASTNode* node, int scope) {
    AbstractSyntaxTree::ASTNode* current = node;

    string printString = current->name;
    AbstractSyntaxTree::ASTNode* argNode = current->rightSibling;

    // Store both ints and strings
    vector<string> formattedArgs;

    // Collect all arguments
    while (argNode != nullptr) {
        string token = argNode->name;

        // Try to determine if it's a variable or a literal
        if (containsNonDigit(token)) {
            SymbolTable::Symbol* sym = SymbolTable::findSymbol(symbolRoot, token, scope);
            if (!sym) {
                cerr << "Error: Variable " << token << " not declared." << endl;
                return nullptr;
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

    cout << printString;
    return current;
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

AbstractSyntaxTree::ASTNode* Interpreter::findProcedureNode(AbstractSyntaxTree::ASTNode* currentNode, string name) {
    if (currentNode == nullptr) {
        return nullptr;
    }

    string nodeName = currentNode->name;

    // Check if this node is a "DECLARATION" of a "main" procedure
    if (nodeName == "DECLARATION") {
        string identifierType = currentNode->symbolPointer->identifierType;
        string identifierName = currentNode->symbolPointer->identifierName;

        if (identifierName == name && identifierType == "procedure") {
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
