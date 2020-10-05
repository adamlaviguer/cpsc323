#include <fstream>  //for the reading/writing files
#include <iomanip>  //for formatting I/O
#include <iostream> //for input/output in the command line
#include <cstring>  //for comparing strings with "strcmp"

using namespace std;    //include the standard (std) namespace by default

bool isIdentifier(char buffer[]) {
    char keywords[1][10] = {"int"};

    for (int i = 0; i<sizeof(keywords); i++) {
        if (strcmp(keywords[i], buffer) == 0) {
            return false;
            break;
        }
    }
    return true;
}
bool isLetter(char ch) {
    if (isalpha(ch)) {  //checks whether "ch" is an uppercase or lowercase letter
        return true;
    }
    return false;
}
bool isInteger(char ch) {
    if (isdigit(ch)) {  //checks whether "ch" is an uppercase or lowercase letter
        return true;
    }
    return false;
}
bool isComment(char ch) {
    if (ch == '!') {  //checks whether "ch" is a comment operator
        return true;
    }
    return false;
}
bool isOther(char ch) {
    if (ch == ' ' || ch == '\n') {  //checks whether "ch" is a space or a new line character
        return true;
    }
    return false;
}
bool isSeparator(char ch) {
    char separators[] = ";";
    for (int i = 0; i < sizeof(separators); i++) {
        if (ch == separators[i]) {
            return true;
            break;
        }
    }
    return false;
}
bool isOperator(char ch) {
    char operators[] = "=";
    for (int i = 0; i < sizeof(operators); i++) {
        if (ch == operators[i]) {
            return true;
            break;
        }
    }
    return false;
}
void lexer(ifstream &readFile, ofstream &writeFile) {
    int state = 1;
    char buffer[100];
    char commentBuffer[1000];
    int j = 0, k = 0;
    char ch;

    //add headers to the output file
    writeFile<<"TOKEN\t\t\t\t\tLEXEMES"<<endl<<endl;

    //read from the file until the end of the file is reached
    while (!readFile.eof()) {
        ch = readFile.get();

        switch (state) {
            case 1:
                if (isLetter(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 2;
                }
                if (isInteger(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 4;
                }
                if (isComment(ch)) {
                    writeFile<<"COMMENT\t\t\t=\t\t"<<ch<<endl;
                    state = 6;
                }
                if (isSeparator(ch)) {
                    // cout<<"Separator: "<<ch<<endl;
                    state = 8;
                }
                if (isOperator(ch)) {
                    // cout<<"Operator: "<<ch<<endl;
                    state = 9;
                }
                if (isOther(ch)) {
                    buffer[j] = '\0';
                    j = 0;
                    state = 1;
                }
                break;
            case 2:
                if (isLetter(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 2;
                }
                if (isInteger(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 2;
                }
                if (isComment(ch)) {
                    buffer[j] = '\0';
                    j = 0;
                    if (isIdentifier(buffer)) {
                        writeFile<<"IDENTIFIER\t\t=\t\t"<<buffer<<endl;
                    }
                    else {
                        writeFile<<"KEYWORD\t\t\t=\t\t"<<buffer<<endl;
                    }
                    state = 3;
                }
                if (isSeparator(ch)) {
                    buffer[j] = '\0';
                    j = 0;
                    if (isIdentifier(buffer)) {
                        writeFile<<"IDENTIFIER\t\t=\t\t"<<buffer<<endl;
                    }
                    else {
                        writeFile<<"KEYWORD\t\t\t=\t\t"<<buffer<<endl;
                    }
                    writeFile<<"SEPARATOR\t\t=\t\t"<<ch<<endl;
                    state = 3;
                }
                if (isOperator(ch)) {
                    buffer[j] = '\0';
                    j = 0;
                    if (isIdentifier(buffer)) {
                        writeFile<<"IDENTIFIER\t\t=\t\t"<<buffer<<endl;
                    }
                    else {
                        writeFile<<"KEYWORD\t\t\t=\t\t"<<buffer<<endl;
                    }
                    writeFile<<"OPERATOR\t\t\t=\t\t"<<ch<<endl;
                    state = 3;
                }
                if (isOther(ch)) {
                    buffer[j] = '\0';
                    j = 0;
                    if (isIdentifier(buffer)) {
                        writeFile<<"IDENTIFIER\t\t=\t\t"<<buffer<<endl;
                    }
                    else {
                        writeFile<<"KEYWORD\t\t\t=\t\t"<<buffer<<endl;
                    }
                    state = 3;
                }
                break;
            case 3:
                if (isLetter(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isInteger(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isComment(ch)) {
                    state = 1;
                }
                if (isSeparator(ch)) {
                    // cout<<"Separator: "<<ch<<endl;
                    state = 1;
                }
                if (isOperator(ch)) {
                    // cout<<"Operator: "<<ch<<endl;
                    state = 1;
                }
                if (isOther(ch)) {
                    buffer[j] = '\0';
                    j = 0;
                    state = 1;
                }
                break;
            case 4:
                if (isInteger(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 4;
                }
                else {
                    buffer[j] = '\0';
                    j = 0;
                    writeFile<<"INTEGER\t\t\t=\t\t"<<buffer<<endl;
                    state = 5;
                }
                break;
            case 5:
                if (isLetter(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isInteger(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isComment(ch)) {
                    state = 1;
                }
                if (isSeparator(ch)) {
                    // cout<<"Separator: "<<ch<<endl;
                    state = 1;
                }
                if (isOperator(ch)) {
                    // cout<<"Operator: "<<ch<<endl;
                    state = 1;
                }
                if (isOther(ch)) {
                    buffer[j] = '\0';
                    j = 0;
                    state = 1;
                }
                break;
            case 6:
                if (isComment(ch)) {
                    state = 7;
                    writeFile<<"CommentBuffer\t=\t\t"<<commentBuffer<<endl;
                    writeFile<<"COMMENT\t\t\t=\t\t"<<ch<<endl;
                }
                else {
                    commentBuffer[k] = ch;
                    k++;
                    state = 6;
                }
                break;
            case 7:
                if (isLetter(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isInteger(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isComment(ch)) {
                    state = 1;
                }
                if (isSeparator(ch)) {
                    // cout<<"Separator: "<<ch<<endl;
                    state = 1;
                }
                if (isOperator(ch)) {
                    // cout<<"Operator: "<<ch<<endl;
                    state = 1;
                }
                if (isOther(ch)) {
                    buffer[j] = '\0';
                    j = 0;
                    state = 1;
                }
                break;
            case 8:
                if (isLetter(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isInteger(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isComment(ch)) {
                    state = 1;
                }
                if (isSeparator(ch)) {
                    // cout<<"Separator: "<<ch<<endl;
                    state = 1;
                }
                if (isOperator(ch)) {
                    // cout<<"Operator: "<<ch<<endl;
                    state = 1;
                }
                if (isOther(ch)) {
                    buffer[j] = '\0';
                    j = 0;
                    state = 1;
                }
                break;
            case 9:
                if (isLetter(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isInteger(ch)) {
                    buffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isComment(ch)) {
                    state = 1;
                }
                if (isSeparator(ch)) {
                    // cout<<"Separator: "<<ch<<endl;
                    state = 1;
                }
                if (isOperator(ch)) {
                    // cout<<"Operator: "<<ch<<endl;
                    state = 1;
                }
                if (isOther(ch)) {
                    buffer[j] = '\0';
                    j = 0;
                    state = 1;
                }
                break;
        }
    }
}
int main() {
    ifstream readFile;  //create the readFile object to read the contents of the input file
    ofstream writeFile; //create the writeFile object to write the output of the progrem
    cout<<"Now reading the input file...\n";
    readFile.open("simpleInput.txt", ios::in);      //open the readFile object for input
    if (readFile.fail()) {
        cout<<"There was an error reading the input file. Please check the file then try again.\n";
        return 1;
    }
    writeFile.open("output.txt", ios::out);     //open the writeFile object for output
    if (writeFile.fail()) {
        cout<<"There was an error opening the output file. Please check the file then try again.\n";
        return 1;
    }
    lexer(readFile, writeFile); //call the lexer function
    cout<<"Now writing to the output file...\n";
    cout<<"\nNow closing all files...\n";
    readFile.close();   //close the readFile object
    writeFile.close();   //close the writeFile object
    cout<<"Program finished.\n";
    return 0;
}
