#include <fstream>  //for the reading/writing files
#include <iomanip>  //for formatting I/O
#include <iostream> //for input/output in the command line
#include <cstring>  //for comparing strings with "strcmp"

using namespace std;    //include the standard (std) namespace by default

int isKeyword(char buffer[]) {
    char keywords[1][10] = {"int"};
    int i, flag = 0;

    for (i = 0; i<sizeof(keywords); i++) {
        if (strcmp(keywords[i], buffer) == 0) {
            flag = 1;
            break;
        }
    }
    return flag;
}

void isOperator(char ch, char operators[]) {
    int i;
    for (i = 0; i < sizeof(&operators); i++) {
        if (ch == operators[i]) {
            cout<<ch<<" is operator\n";
        }
    }
}

void isSeparator(char ch, char separators[]) {
    int i;
    for (i = 0; i < sizeof(&separators); i++) {
        if (ch == separators[i]) {
            cout<<ch<<" is separator\n";
        }
    }
}

void lexer(char ch) {
    char buffer[15];
    char operators[] = "+-*/%=!";
    char separators[] = ";.";
    char integers[] = "0123456789";
    int j=0;

    isOperator(ch, operators);
    isSeparator(ch, separators);
    //cout<<"Test of ch: "<<ch<<"\n"; //REMOVE THIS BEFORE SUBMISSION

    if (isalnum(ch)) {  //checks whether "ch" is a decimal digit or an uppercase or lowercase letter
        buffer[j] = ch;
        //cout<<"Buffer Test: "<<buffer[j]<<"\n"; //REMOVE THIS BEFORE SUBMISSION
        j++;
        cout<<j<<"\n";
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
}

int main() {
    char ch;

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
        lexer(ch);
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
