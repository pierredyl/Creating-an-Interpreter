//
// Created by Dylan Pierre on 4/2/25.
//
#include "SymbolTable.h"
#include "RecursiveDescentParser.h"
#include "Token.h"
#include <iostream>

// Default constructor for SymbolTable
SymbolTable::SymbolTable(RecursiveDescentParser& parser) {
    this->Root = parser.getRoot();  // Now correctly getting the root from an instance
    head = nullptr;
    globalScope = 0;
}

// Insert a new symbol (variable, function, or parameter)
void SymbolTable::insertSymbol(std::string name, std::string type, std::string dtype, int scope, bool isArray, int arrSize, int lineNumber) {
    checkForDuplicate(name, scope, lineNumber);
    Symbol* newSymbol = new Symbol(name, type, dtype, scope, isArray, arrSize, lineNumber);
    newSymbol->next = head;
    head = newSymbol;
}

void SymbolTable::checkForDuplicate(const string& identifierName, int scope, int currentLineNumber) {
    Symbol* temp = head;
    while (temp) {
        // Check if the identifier already exists in the symbol table
        if (temp->identifierName == identifierName) {
            // If the identifier is already declared globally (scope 0)
            if (temp->scope == 0 && scope > 0) {
                throw runtime_error("Error on line " + to_string(currentLineNumber) +
                    ": '" + identifierName + "' is already defined globally.");
            }
            // If the identifier is already declared in the same local scope
            else if (temp->scope > 0 && temp->scope == scope) {
                throw runtime_error("Error on line " + to_string(currentLineNumber) +
                    ": '" + identifierName + "' is already defined locally in scope " +
                    to_string(temp->scope) + ".");
            }
        }
        temp = temp->next;
    }
}


// Insert function entry
void SymbolTable::insertFunction(std::string identifierName, string identifierType, std::string datatype, int scope, int lineNumber) {
    insertSymbol(identifierName, identifierType, datatype, scope, false, 0, lineNumber);
}
//insert procedure
void SymbolTable::insertProcedure(string identifierName, string identifierType, int scope, int lineNumber) {
    insertSymbol(identifierName, identifierType, "NOT APPLICABLE", scope, false, 0, lineNumber);
}


// Insert variable entry
void SymbolTable::insertVariable(std::string varName, std::string varType, int scope, bool isArray, int arrSize, int lineNumber) {
    insertSymbol(varName, "datatype", varType, scope, isArray, arrSize, lineNumber);
}

void SymbolTable::insertParameter(string identifierName, std::string varType, int scope, bool isArray, int arrSize, int lineNumber) {
    insertSymbol(identifierName, "parameter", varType, scope, isArray, arrSize, lineNumber);
}

RecursiveDescentParser::CSTNode* SymbolTable::variableListHelper(RecursiveDescentParser::CSTNode* node,
    string datatype) {

    if (node->token.getValue() == ";" ||
        node->token.getValue() == ")") {
        return node;
    }

    if (node->rightSibling != nullptr && node->token.getValue() != ",") {
        string identifier_name = node->token.getValue();
        if (inFuncOrProc) {
            insertVariable(identifier_name, datatype, globalScope, false, 0, node->token.getLineNumber());
        } else {
            insertVariable(identifier_name, datatype, 0, false, 0, node->token.getLineNumber());

        }
    }

    if (node->rightSibling != nullptr) {
        return variableListHelper(node->rightSibling, datatype);
    }
}



RecursiveDescentParser::CSTNode* SymbolTable::insertParameterHelper(RecursiveDescentParser::CSTNode* node, string identifierName) {
    if (node->token.getValue() == ")") {
        return node;
    }

    if (node->rightSibling != nullptr) {
        if (node->token.getValue() == "int" ||
            node->token.getValue() == "bool" ||
            node->token.getValue() == "char") {
            string datatype = node->token.getValue();

            node = node->rightSibling;

            string identifier_name = node->token.getValue();

            if (node->rightSibling->token.getValue() == "[") {
                node = node->rightSibling;
                node = node->rightSibling;
                int arraySize = stoi(node->token.getValue());
                insertParameter(identifier_name, datatype, globalScope, true, arraySize, node->token.getLineNumber());
            } else {
                insertParameter(identifier_name, datatype, globalScope, false, 0, node->token.getLineNumber());
            }
        }
    }

    if (node->rightSibling != nullptr) {
        return insertParameterHelper(node->rightSibling, identifierName);
    }
}


// Print symbol table
void SymbolTable::printTable() {
    Symbol* temp = head;
    while (temp) {
        std::cout << "IDENTIFIER_NAME: " << temp->identifierName << "\n";
        std::cout << "IDENTIFIER_TYPE: " << temp->identifierType << "\n";
        std::cout << "DATATYPE: " << temp->dataType << "\n";
        std::cout << "DATATYPE_IS_ARRAY: " << (temp->isArray ? "yes" : "no") << "\n";
        std::cout << "DATATYPE_ARRAY_SIZE: " << temp->arraySize << "\n";
        std::cout << "SCOPE: " << temp->scope << "\n\n";

        temp = temp->next;
    }
}
void SymbolTable::buildSymbolTableHelper(RecursiveDescentParser::CSTNode* node) {
    if (node == nullptr) {
        return; // Base case: if the node is null, return.
    }

    if (node->rightSibling == nullptr) {
        lineNumber++;
    }

    //Mark the end of a function or procedure
    if (node->token.getValue() == "}" && (inFuncOrProc && depth == 1)) {
        inFuncOrProc = false;
        depth = 0;
    }

    //If in a function or procedure, and an { is encountered, increment depth.
    //Else if an } is encountered, increment depth.
    if (inFuncOrProc && node->token.getValue() == "{") {
        depth++;
    } else if (inFuncOrProc && node->token.getValue() == "}") {
        depth--;
    }

    //Variable
    if (node->rightSibling != nullptr) {
        if (node->token.getValue() == "int" ||
            node->token.getValue() == "bool" ||
            node->token.getValue() == "char") {
            string datatype = node->token.getValue();

            node = node->rightSibling;

            string identifier_name = node->token.getValue();

            if (node->rightSibling->token.getValue() == "[") {
                node = node->rightSibling;
                node = node->rightSibling;

                int arraySize = stoi(node->token.getValue());

                if (!inFuncOrProc) {
                    insertVariable(identifier_name, datatype, 0, true, arraySize, node->token.getLineNumber());
                } else {
                    insertVariable(identifier_name, datatype, globalScope, true, arraySize, node->token.getLineNumber());
                }

            } else if (node->rightSibling->token.getValue() == ",") {
                node = variableListHelper(node, datatype);
            } else {
                if (!inFuncOrProc) {
                    insertVariable(identifier_name, datatype, 0,false, 0, node->token.getLineNumber());
                } else {
                    insertVariable(identifier_name, datatype, globalScope, false, 0, node->token.getLineNumber());
                }
            }
        }
    }

    //Function
    if (node->rightSibling != nullptr) {
        if (node->token.getValue() == "function") {
            globalScope++;
            inFuncOrProc = true;
            string identifier_type = node->token.getValue();
            node = node->rightSibling;

            if (node->token.getValue() == "int" ||
                node->token.getValue() == "bool" ||
                node->token.getValue() == "char") {
                string datatype = node->token.getValue();

                node = node->rightSibling;

                if (node->token.getType() == IDENTIFIER) {
                    string identifier_name = node->token.getValue();
                    int tokenLineNumber = node->token.getLineNumber();
                    insertFunction(identifier_name, identifier_type, datatype, globalScope, tokenLineNumber);
                    node = insertParameterHelper(node, identifier_name);
                }
            }
        }
    }

    //Procedure
    if (node->rightSibling != nullptr) {
        if (node->token.getValue() == "procedure") {
            globalScope++;
            inFuncOrProc = true;
            string identifier_type = node->token.getValue();
            node = node->rightSibling;

            if (node->token.getType() == IDENTIFIER ||
                node->token.getType() == KEYWORD_MAIN) {
                string identifier_name = node->token.getValue();
                int tokenLineNumber = node->token.getLineNumber();
                insertProcedure(identifier_name, identifier_type, globalScope, tokenLineNumber);
                node = insertParameterHelper(node, identifier_name);
                }
            }
    }


    if (node->leftChild != nullptr) {
        buildSymbolTableHelper(node->leftChild);
    }

    if (node->rightSibling != nullptr) {
        buildSymbolTableHelper(node->rightSibling);
    }
}


void SymbolTable::buildSymbolTable() {
    if (Root != nullptr) {
        buildSymbolTableHelper(Root);  // Call the helper function to traverse the tree
    }
}





