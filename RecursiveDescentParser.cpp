//
// Created by dylan on 3/6/2025.
//

#include <iostream>
#include "RecursiveDescentParser.h"
using namespace std;


RecursiveDescentParser::RecursiveDescentParser(const vector<Token>& tokenList) {
    tokens = tokenList;

    if (!tokens.empty()) {
        // Initialize root node
        Root = new CSTNode{tokens[0], nullptr, nullptr};


        // Build the tree from the remaining tokens
        CSTNode* currentNode = Root;
        for (size_t i = 1; i < tokens.size(); ++i) {
            // Create a new node
            CSTNode* newNode = new CSTNode{tokens[i], nullptr, nullptr};

            if (currentNode->token.getLineNumber() == newNode->token.getLineNumber()) {
                currentNode->rightSibling = newNode;
            } else {
                currentNode->leftChild = newNode;
                currentNode->rightSibling = nullptr;
            }
            // Move to the new node
            currentNode = newNode;
        }
    } else {
        Root = nullptr;
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






