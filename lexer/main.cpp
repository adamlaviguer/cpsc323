#include <fstream>  //for the reading/writing files
#include <iomanip>  //for formatting I/O
#include <iostream> //for input/output in the command line
#include <cstring>  //for comparing strings with "strcmp"

using namespace std;    //include the standard (std) namespace by default

int isKeyword(char buffer[]) {
    char keywords[1][10] = {"int"};
    int flag = 0;

    for (int i = 0; i<sizeof(keywords); i++) {
        if (strcmp(keywords[i], buffer) == 0) {
            flag = 1;
            break;
        }
    }
    return flag;
}

void isOperator(char ch, char operators[]) {
    for (int i = 0; i < sizeof(&operators); i++) {
        if (ch == operators[i]) {
            cout<<ch<<" is operator\n";
        }
    }
}

void isSeparator(char ch, char separators[]) {
    for (int i = 0; i < sizeof(&separators); i++) {
        if (ch == separators[i]) {
            cout<<ch<<" is separator\n";
        }
    }
}

int lexer(char ch, int j) {
    char buffer[100];
    char operators[] = "+-*/%=!()";
    char separators[] = ";.,";
    char integers[] = "0123456789";

    if (isalnum(ch)) {  //checks whether "ch" is a decimal digit or an uppercase or lowercase letter
        buffer[j] = ch;
        j++;
    }
    else if ((ch == ' ' || ch == '\n') && (j != 0)) {
        buffer[j] = '\0';
        j = 0;

        if (isKeyword(buffer) == 1) {
            cout<<buffer<<" is keyword\n";
        }
        else {
            cout<<buffer<<" is identifier\n";
        }
    }
    isOperator(ch, operators);
    isSeparator(ch, separators);

    return j;
}

int main() {
    char ch;
    int j=0;

    ifstream readFile;      //create the readFile object to read the contents of the input file
    cout<<"Now reading the input file...\n";
    readFile.open("simpleInput.txt", ios::in);      //open the readFile object for input
    if (readFile.fail()) {
        cout<<"There was an error reading the file. Please check the file then try again.\n";
        return 1;
    }
    else {
        cout<<"Finished reading the input file.\n\n";
    }
    //read from the file until the end of the file is reached
    while (!readFile.eof()) {
        ch = readFile.get();
        j = lexer(ch, j);
    }

    cout<<"\nNow closing the input file...\n";
    readFile.close();
    cout<<"Finished closing the input file.\n";
    /*
    if (readFile.fail()) {
        cout<<"There was an error closing the file. Please check the file then try again.\n";
        return 1;
    }
    else {
        cout<<"Finished closing the input file.\n";
    }
    */
    return 0;
}
