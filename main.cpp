#include <iostream>
#include <fstream>
#include "remove_comments.h"
#include "Token.h"
#include "Tokenizer.h"
#include "RecursiveDescentParser.h"
#include <sstream>

using namespace std;


int main() {
    ifstream inputFile("C:/Users/dylan/CLionProjects/Creating-an-Interpreter/test_files/programming_assignment_2-test_file_1.c");
    ofstream outputFile("/Users/dylanpierre/Desktop/Creating-an-Interpreter/output.txt");
    remove_comments(inputFile, outputFile);
    inputFile.close();

    inputFile.open("C:/Users/dylan/CLionProjects/Creating-an-Interpreter/output.txt");
    stringstream buffer;
    buffer << inputFile.rdbuf();
    string code = buffer.str();

    Tokenizer tokenizer(code);

    vector<Token> tokens = tokenizer.tokenize();
    // Print out all tokens
    for (const Token& token : tokens) {
        cout << "Token Value: " << token.getValue() << ", Type: " << token.typeToString(token.getType()) << endl;
    }

    //Pass tokens to Parser
    RecursiveDescentParser parser(tokens);
    parser.printCST();

    return 0;
}

