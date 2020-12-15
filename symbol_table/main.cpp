#include <fstream>  //for the reading/writing files
#include <iomanip>  //for formatting I/O
#include <iostream> //for input/output in the command line
#include <cstring>  //for comparing strings with "strcmp"

using namespace std;    //include the standard (std) namespace by default

//GLOBAL VARIABLES
int Memory_Address = 5000;
int tableRow = 0;
string assemblyArray[18][2] = {{"1", "PUSHI"},{"2", "PUSHM"},{"3", "POPM"},{"4", "STDOUT"},{"5", "STDIN"},{"6", "ADD"},
                                {"7", "SUB"},{"8", "MUL"},{"9", "DIV"},{"10", "GRT"},{"11", "LES"},{"12", "EQU"},
                                {"13", "NEQ"},{"14", "GEQ"},{"15", "LEQ"},{"16", "JUMPZ"},{"17", "JUMP"},{"18", "LABEL"}};
string symbol_table[3][2] = {{"", ""},{"",""},{"",""}};
bool lastWasKeyword = false;

//FUNCTION DEFINITIONS

bool checkSymbolTable(char charBuffer[]) {
    for (int row = 0; row < 3; row++) {
        if (to_string(*charBuffer) == symbol_table[row][0]) {
            return false;
        }
    }
    return true;
}
void updateSymbolTable(char charBuffer[]) {
    symbol_table[tableRow][0] = charBuffer;
    symbol_table[tableRow][1] = to_string(Memory_Address);
    Memory_Address += 1;
    tableRow += 1;
}
bool isIdentifier(char charBuffer[]) {
    char keywords[1][10] = {"int"};
    for (int i = 0; i<sizeof(keywords); i++) {
        if (strcmp(keywords[i], charBuffer) == 0) {
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
    char separators[] = ";,";
    for (int i = 0; i < sizeof(separators); i++) {
        if (ch == separators[i]) {
            return true;
            break;
        }
    }
    return false;
}
bool isOperator(char ch) {
    //char operators[] = {'=','+','-','%','*'};
    char operators[] = "=+-%";
    for (int i = 0; i < sizeof(operators); i++) {
        if (ch == operators[i]) {
            return true;
            break;
        }
    }
    return false;
}
string syntaxAnalyzer(char *synBuffer) {
    //define the Grammar
    string result = "Error\n";
    int pos = sizeof(*synBuffer);
    if (synBuffer[pos] == '=') {
        result = "\t<Statement> --> <Assign>\n\t<Assign> --> <Identifier> = <Expression>\n\n";
        return result;
    }
    if (synBuffer[pos] == '+') {
        result = "\t<Expression> --> <Term> <Expression Prime>\n\t<Term> --> <Factor> <TermPrime>\n\t<Factor> --> <Identifier>\n\n";
        return result;
    }
    if (isLetter(synBuffer[pos])) {
        result = "\t<TermPrime> --> <Empty>\n\t<ExpressionPrime> --> + <Term> <ExpressionPrime>\n\t<Empty> --> <Epsilon>\n\n";
        return result;
    }
    if (isSeparator(synBuffer[pos])) {
        result = "\t<Term> --> <Factor> <TermPrime>\n\t<Factor> --> <Identifier>\n\n";
        return result;
    }
    return result;
}
void writeAssembly(ofstream &writeFile) {
    
}
void writeSymbolTable(ofstream &writeFile) {
    writeFile<<"\n------------------SYMBOL TABLE------------------"<<endl<<setw(10)<<left<<"Identifier"<<"\t\t"<<"Memory Location"<<endl;
    for (int row = 0; row < 4; row++) {
        writeFile<<setw(10)<<left<<symbol_table[row][0]<<"\t\t"<<symbol_table[row][1]<<endl;
    }
}
void lexer(ifstream &readFile, ofstream &writeFile) {
    int state = 1;
    char charBuffer[100];
    char synBuffer[100];
    char commentBuffer[1000];
    int j = 0, k = 0;
    char ch;


    //add headers to the output file
    //writeFile<<setw(20)<<left<<"TOKEN"<<"\t\t"<<"LEXEMES"<<endl<<endl;

    //read from the file until the end of the file is reached
    while (!readFile.eof()) {
        ch = readFile.get();
        switch (state) {
            case 1:
                if (isLetter(ch)) {
                    charBuffer[j] = ch;
                    synBuffer[j] = ch;
                    j++;
                    state = 2;
                }
                if (isInteger(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 4;
                }
                if (isComment(ch)) {
                    writeFile<<setw(20)<<left<<"Token: COMMENT"<<"\t"<<"Lexeme: "<<ch<<endl;
                    state = 6;
                }
                if (isSeparator(ch)) {
                    synBuffer[j] = ch;
                    if (isIdentifier(charBuffer)) {
                        writeFile<<setw(20)<<left<<"Token: IDENTIFIER"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        if (lastWasKeyword && ch != ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            else {
                                cout<<"Error: That Identifier Already Exists"<<endl;
                            }
                            lastWasKeyword = false;
                        }
                        else if (lastWasKeyword && ch == ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            lastWasKeyword = true;
                        }
                        //writeFile<<syntaxAnalyzer(synBuffer);
                    }
                    writeFile<<setw(20)<<left<<"Token: SEPARATOR"<<"\t"<<"Lexeme: "<<ch<<endl;
                    //writeFile<<"\t<Separator> --> <EndSeparator>\n\n";
                    state = 8;
                }
                if (isOperator(ch)) {
                    charBuffer[j] = '\0';
                    synBuffer[j] = ch;
                    if (isIdentifier(charBuffer)) {
                        writeFile<<setw(20)<<left<<"Token: IDENTIFIER"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        if (lastWasKeyword && ch != ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            else {
                                cout<<"Error: That Identifier Already Exists"<<endl;
                            }
                            lastWasKeyword = false;
                        }
                        else if (lastWasKeyword && ch == ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            lastWasKeyword = true;
                        }
                        //writeFile<<syntaxAnalyzer(synBuffer);
                    }
                    writeFile<<setw(20)<<left<<"Token: OPERATOR"<<"\t"<<"Lexeme: "<<ch<<endl;
                    //writeFile<<"\t<TermPrime> --> <Empty>\n\t<ExpressionPrime> --> + <Term> <ExpressionPrime>\n\t<Empty> --> <Epsilon>\n\n";
                    for (int i = 0; i < sizeof(*synBuffer); i++) {
                        synBuffer[i] = '\0';
                    }
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    j = 0;
                    state = 9;
                }
                if (isOther(ch)) {
                    charBuffer[j] = '\0';
                    j = 0;
                    if (isIdentifier(charBuffer)) {
                        writeFile<<setw(20)<<left<<"Token: IDENTIFIER"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        if (lastWasKeyword && ch != ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            else {
                                cout<<"Error: That Identifier Already Exists"<<endl;
                            }
                            lastWasKeyword = false;
                        }
                        else if (lastWasKeyword && ch == ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            lastWasKeyword = true;
                        }
                    }
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    state = 1;
                }
                break;
            case 2:
                if (isLetter(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 2;
                }
                if (isInteger(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 2;
                }
                if (isComment(ch)) {
                    charBuffer[j] = '\0';
                    j = 0;
                    if (isIdentifier(charBuffer)) {
                        writeFile<<setw(20)<<left<<"Token: IDENTIFIER"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        if (lastWasKeyword && ch != ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            else {
                                cout<<"Error: That Identifier Already Exists"<<endl;
                            }
                            lastWasKeyword = false;
                        }
                        else if (lastWasKeyword && ch == ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            lastWasKeyword = true;
                        }
                    }
                    else {
                        writeFile<<setw(20)<<left<<"Token: KEYWORD"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        lastWasKeyword = true;
                    }
                    state = 3;
                }
                if (isSeparator(ch)) {
                    charBuffer[j] = '\0';
                    synBuffer[j] = ch;
                    if (isIdentifier(charBuffer)) {
                        writeFile<<setw(20)<<left<<"Token: IDENTIFIER"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        if (lastWasKeyword && ch != ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            else {
                                cout<<"Error: That Identifier Already Exists"<<endl;
                            }
                            lastWasKeyword = false;
                        }
                        else if (lastWasKeyword && ch == ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            lastWasKeyword = true;
                        }
                        //writeFile<<syntaxAnalyzer(synBuffer);
                    }
                    else {
                        writeFile<<setw(20)<<left<<"Token: KEYWORD"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        lastWasKeyword = true;
                    }
                    writeFile<<setw(20)<<left<<"Token: SEPARATOR"<<"\t"<<"Lexeme: "<<ch<<endl;
                    for (int i = 0; i < sizeof(*synBuffer); i++) {
                        synBuffer[i] = '\0';
                    }
                    j = 0;
                    state = 3;
                }
                if (isOperator(ch)) {
                    charBuffer[j] = '\0';
                    synBuffer[j] = ch;
                    if (isIdentifier(charBuffer)) {
                        writeFile<<setw(20)<<left<<"Token: IDENTIFIER"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        if (lastWasKeyword && ch != ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            else {
                                cout<<"Error: That Identifier Already Exists"<<endl;
                            }
                            lastWasKeyword = false;
                        }
                        else if (lastWasKeyword && ch == ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            lastWasKeyword = true;
                        }
                        //writeFile<<syntaxAnalyzer(synBuffer);
                    }
                    else {
                        writeFile<<setw(20)<<left<<"Token: KEYWORD"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        lastWasKeyword = true;
                    }
                    writeFile<<setw(20)<<left<<"Token: OPERATOR"<<"\t"<<"Lexeme: "<<ch<<endl;
                    for (int i = 0; i < sizeof(*synBuffer); i++) {
                        synBuffer[i] = '\0';
                    }
                    j = 0;
                    state = 3;
                }
                if (isOther(ch)) {
                    charBuffer[j] = '\0';
                    j = 0;
                    if (isIdentifier(charBuffer)) {
                        writeFile<<setw(20)<<left<<"Token: IDENTIFIER"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        if (lastWasKeyword && ch != ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            else {
                                cout<<"Error: That Identifier Already Exists"<<endl;
                            }
                            lastWasKeyword = false;
                        }
                        else if (lastWasKeyword && ch == ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            lastWasKeyword = true;
                        }
                    }
                    else {
                        writeFile<<setw(20)<<left<<"Token: KEYWORD"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        lastWasKeyword = true;
                    }
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    state = 3;
                }
                break;
            case 3:
                if (isLetter(ch)) {
                    charBuffer[j] = ch;
                    synBuffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isInteger(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isComment(ch)) {
                    state = 1;
                }
                if (isSeparator(ch)) {
                    state = 1;
                }
                if (isOperator(ch)) {
                    charBuffer[j] = '\0';
                    synBuffer[j] = ch;
                    writeFile<<setw(20)<<left<<"Token: OPERATOR"<<"\t"<<"Lexeme: "<<ch<<endl;
                    //writeFile<<"\t<TermPrime> --> <Empty>\n\t<ExpressionPrime> --> + <Term> <ExpressionPrime>\n\t<Empty> --> <Epsilon>\n\n";
                    for (int i = 0; i < sizeof(*synBuffer); i++) {
                        synBuffer[i] = '\0';
                    }
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    j = 0;
                    state = 1;
                }
                if (isOther(ch)) {
                    charBuffer[j] = '\0';
                    j = 0;
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    state = 1;
                }
                break;
            case 4:
                if (isInteger(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 4;
                }
                else {
                    charBuffer[j] = '\0';
                    j = 0;
                    writeFile<<setw(20)<<left<<"Token: INTEGER"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                    if (isSeparator(ch)) {
                        writeFile<<setw(20)<<left<<"Token: SEPARATOR"<<"\t"<<"Lexeme: "<<ch<<endl;
                    }
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    state = 5;
                }
                break;
            case 5:
                if (isLetter(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isInteger(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isComment(ch)) {
                    state = 1;
                }
                if (isSeparator(ch)) {
                    state = 1;
                }
                if (isOperator(ch)) {
                    charBuffer[j] = '\0';
                    synBuffer[j] = ch;
                    writeFile<<setw(20)<<left<<"Token: OPERATOR"<<"\t"<<"Lexeme: "<<ch<<endl;
                    //writeFile<<"\t<TermPrime> --> <Empty>\n\t<ExpressionPrime> --> + <Term> <ExpressionPrime>\n\t<Empty> --> <Epsilon>\n\n";
                    for (int i = 0; i < sizeof(*synBuffer); i++) {
                        synBuffer[i] = '\0';
                    }
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    j = 0;
                    state = 1;
                }
                if (isOther(ch)) {
                    charBuffer[j] = '\0';
                    j = 0;
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    state = 1;
                }
                break;
            case 6:
                if (isComment(ch)) {
                    state = 7;
                    writeFile<<setw(20)<<left<<"Comment Buffer:"<<"\t"<<commentBuffer<<endl;
                    writeFile<<setw(20)<<left<<"Token: COMMENT"<<"\t"<<"Lexeme: "<<ch<<endl;
                }
                else {
                    commentBuffer[k] = ch;
                    k++;
                    state = 6;
                }
                break;
            case 7:
                if (isLetter(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isInteger(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isComment(ch)) {
                    state = 1;
                }
                if (isSeparator(ch)) {
                    state = 1;
                }
                if (isOperator(ch)) {
                    state = 1;
                }
                if (isOther(ch)) {
                    charBuffer[j] = '\0';
                    j = 0;
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    state = 1;
                }
                break;
            case 8:
                if (isLetter(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isInteger(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isComment(ch)) {
                    state = 1;
                }
                if (isSeparator(ch)) {
                    state = 1;
                }
                if (isOperator(ch)) {
                    state = 1;
                }
                if (isOther(ch)) {
                    charBuffer[j] = '\0';
                    j = 0;
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    state = 1;
                }
                break;
            case 9:
                if (isLetter(ch)) {
                    charBuffer[j] = ch;
                    synBuffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isInteger(ch)) {
                    charBuffer[j] = ch;
                    j++;
                    state = 1;
                }
                if (isComment(ch)) {
                    state = 1;
                }
                if (isSeparator(ch)) {
                    state = 1;
                }
                if (isOperator(ch)) {
                    charBuffer[j] = '\0';
                    j = 0;
                    if (isIdentifier(charBuffer)) {
                        writeFile<<setw(20)<<left<<"Token: IDENTIFIER"<<"\t"<<"Lexeme: "<<charBuffer<<endl;
                        if (lastWasKeyword && ch != ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            else {
                                cout<<"Error: That Identifier Already Exists"<<endl;
                            }
                            lastWasKeyword = false;
                        }
                        else if (lastWasKeyword && ch == ',') {
                            if (checkSymbolTable(charBuffer)) {
                                updateSymbolTable(charBuffer);
                            }
                            lastWasKeyword = true;
                        }
                    }
                    writeFile<<setw(20)<<left<<"Token: OPERATOR"<<"\t"<<"Lexeme: "<<ch<<endl;
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
                    state = 1;
                }
                if (isOther(ch)) {
                    charBuffer[j] = '\0';
                    j = 0;
                    for (int i = 0; i < sizeof(charBuffer); i++) {
                        charBuffer[i] = '\0';
                    }
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
    writeAssembly(writeFile);
    writeSymbolTable(writeFile);
    cout<<"Now writing to the output file...\n";
    cout<<"\nNow closing all files...\n";
    readFile.close();   //close the readFile object
    writeFile.close();   //close the writeFile object
    cout<<"Program finished.\n";
    return 0;
}
