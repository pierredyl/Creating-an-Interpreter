#include <iostream>
#include <fstream>

using namespace std;

enum State { S0, S1, S2, S3, S4, S5, S6, S7 };

int main() {
    ifstream inputFile("/Users/dylanpierre/Desktop/CS460 Assignment #1/programming_assignment_1-test_file_6.c");
    ofstream outputFile("/Users/dylanpierre/Desktop/CS460 Assignment #1/output.txt");
    char ch;
    int lineNumber = 1;
    int commentStart;
    State currentState = S0;
    string lineBuffer;

    while (inputFile.get(ch)) {
        lineBuffer += ch;

        if (ch == '\n') {
            outputFile << lineBuffer;
            lineBuffer.clear();
            lineNumber++;
        }

        switch (currentState) {
            case S0:
                if (ch == '/') {
                    currentState = S1;
                } else if (ch == '"') {
                    currentState = S5;
                } else if (ch == '*') {
                    currentState = S6;
                }

                break;

            case S1:
                if (ch == '/') {
                    lineBuffer[lineBuffer.size() - 1] = ' ';
                    lineBuffer[lineBuffer.size() - 2] = ' ';
                    currentState = S2;
                } else if (ch == '*') {
                    lineBuffer[lineBuffer.size() - 1] = ' ';
                    lineBuffer[lineBuffer.size() - 2] = ' ';
                    commentStart = lineNumber;
                    currentState = S3;
                } else {
                    currentState = S0;
                }
                break;

            case S2:
                if (ch != '\n') {
                    lineBuffer[lineBuffer.size() - 1] = ' ';
                } else {
                    currentState = S0;
                }
                break;

            case S3:
                if (ch == '*') {
                    currentState = S4;
                } else {
                    lineBuffer[lineBuffer.size() - 1] = ' ';
                }
                break;

            case S4:
                if (ch == '/') {
                    lineBuffer[lineBuffer.size() - 1] = ' ';
                    lineBuffer[lineBuffer.size() - 2] = ' ';
                    currentState = S0;
                } else if (ch == '*') {
                    lineBuffer[lineBuffer.size() - 1] = ' ';
                } else {
                    lineBuffer[lineBuffer.size() - 1] = ' ';
                    currentState = S3;
                }
                break;

            case S5:
                if (ch == '"') {
                    currentState = S0;
                }
                break;

            case S6:
                if (ch == '/') {
                    commentStart = lineNumber;
                    currentState = S7;
                }
        }
    }


    if (currentState == S3 || currentState == S4 || currentState == S7) {
        throw runtime_error("ERROR: Program contains C-style, unterminated comment starting on line " + to_string(commentStart));
    }

    outputFile << lineBuffer;

    inputFile.close();
    outputFile.close();
}
