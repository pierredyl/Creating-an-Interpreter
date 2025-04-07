#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "remove_comments.h"
#include "Token.h"
#include "Tokenizer.h"
#include "RecursiveDescentParser.h"
#include "SymbolTable.h"

using namespace std;

int main() {
    string inputFilePath, outputFilePath;

    // Ask the user for the input and output file paths (or filenames)
    cout << "Enter the input file name (e.g., 'programming_assignment_3-test_file_1.c'): ";
    cin >> inputFilePath;
    cout << endl;

    cout << "Enter the output file name (e.g., 'output.txt'): " << endl;
    cin >> outputFilePath;
    cout << endl;

    // Open input file for reading and output file for writing
    ifstream inputFile(inputFilePath);
    if (!inputFile) {
        cerr << "Error opening input file: " << inputFilePath << endl;
        return 1;
    }

    ofstream outputFile(outputFilePath);
    if (!outputFile) {
        cerr << "Error opening output file: " << outputFilePath << endl;
        return 1;
    }

    // Remove comments from the input file and write to the output file
    remove_comments(inputFile, outputFile);
    inputFile.close(); // Close the input file

    // Reopen the output file to read the processed content
    inputFile.open(outputFilePath);
    if (!inputFile) {
        cerr << "Error reopening output file: " << outputFilePath << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string code = buffer.str();
    inputFile.close(); // Close the output file after reading

    // Tokenizer
    Tokenizer tokenizer(code);
    vector<Token> tokens = tokenizer.tokenize();

    /*
    // Print out all tokens
    for (const Token& token : tokens) {
        cout << "Token Value: " << token.getValue() << ", Type: " << token.typeToString(token.getType()) << endl;
    }

    cout << "\n\n";
    */

    // Pass tokens to Parser
    RecursiveDescentParser SyntaxTree(tokens);
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    SyntaxTree.printCST();

    //Symbol Table
    SymbolTable symbolTable(SyntaxTree);
    symbolTable.buildSymbolTable();
    cout << "\n\n";
    symbolTable.printTable();

    return 0;
}
