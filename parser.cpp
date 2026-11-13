/*
CS 335 Programming Assignment 2
Chloe Cantrell, Miles Crane, Trevor Past

Generative AI was NOT used for this assignment.
*/


#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;


// Input stuff
ifstream infile("input.txt"); // input file
vector<string> infileVector = {};
int currentLine;
vector<char> input = {}; // vector to store line from file

void clearInputVector(){
    while (input.empty() == 0){
        input.pop_back();
    }
}

void inputFileToVector(){
    string line = "";
    while (getline (infile, line)) {
        infileVector.push_back(line);
    }
    //cout << "DEBUG:\tImported infile to infileVector!" << endl;
}

void nextLineAsVector(int lineNum){
    string line = infileVector[lineNum];
    const char* cline = line.c_str();

    clearInputVector();
    for (int i = 0; i < line.length(); i++){
        input.push_back(cline[i]);
    }
}


// open output file
ofstream output("output.txt");


// Recursive descent logic
enum class TokenType
{
    Default,
    Keyword,//Token.specificType values: READ, WRITE
    Identifier,
    Number,
    Operator,//Token.specificType values: ADD, SUB, ASSIGN, DIV, MULT
    Punctuation,//Token.specificType: LPAREN, RPAREN, PERIOD
    Comment,
    End,// CREATE "$$" TOKEN
};
struct Token
{
    TokenType type = TokenType::Default;
    string specificType = "";
};
/*Token token0;
Token token1;
Token token2;
Token token3;

vector<Token> tokenVector = {token0, token1, token2, token3};*/
vector<Token> tokenVector = {};
int n = 0;//tokenVector index

int isExecuting = 1; // 0 = no, 1 = yes, this is the first one after consuming a token, 2 = yes, but not the first after token
void executing(string grammar){
    if (isExecuting == 2 || isExecuting == 1){ // if
        if (isExecuting == 1){
            cout << endl << "Executing:\t";
            output << endl << "Executing:\t";
            isExecuting = 2;
        }
        cout << grammar << "() ";
        output << grammar << "() ";
    }
}

//Parsing Declarations
bool isExpr();
bool isTerm();
bool isFactor();
bool isStatement();

bool isAddOp()
{
    if (tokenVector[n].specificType == "+" || tokenVector[n].specificType == "-")
    {
        if (isExecuting == 0){ // if not already executing,
            isExecuting = 1; // it is now
        }
        executing("add_op");

        isExecuting = 0;
        cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        n++;
        return true;
    }
    else
    {
        return false;
    }
}
bool isMultOp()
{
    if (tokenVector[n].specificType == "*" || tokenVector[n].specificType == "/")
    {
        if (isExecuting == 0){ // if not executing
            isExecuting = 1; // it is now
        }
        executing("mult_op");

        isExecuting = 0;
        cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";        
        output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        n++;
        return true;
    }
    else
    {
        return false;
    }
}
bool isFactor()
{
    if (isExecuting == 0){ // if not executing
        isExecuting = 1; // it is now
    }
    executing("factor");

    if (tokenVector[n].type == TokenType::Identifier || tokenVector[n].type == TokenType::Number)
    {
        isExecuting = 0;

        if (tokenVector[n].specificType == "$$"){
            cout << endl << "    Token:\t$$ consumed ";
            output << endl << "    Token:\t$$ consumed ";
            return true;
        }

        cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        n++;
        return true;
    }
    else if (tokenVector[n].specificType == "(")
    {
        //Check this logic.
        isExecuting = 0;
        cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        n++;
        if (!isExpr())
        {
            return false;
        }
        if (tokenVector[n].specificType != ")")
        {
            return false;
        }
        isExecuting = 0;
        cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        n++;
        return true;
    }
    return false;
}
bool isFactorTail()
{
    if (isExecuting == 0){ // if not executing
        isExecuting = 1; // it is now
    }
    executing("fact_tail");

    if (isMultOp())
    {
        if (!isFactor())
        {
            return false;
        }
        return (isFactorTail());
    }
    return true;
}
bool isTerm()
{
    if (isExecuting == 0){ // if not executing
        isExecuting = 1; // it is now
    }
    executing("term");

    if (isFactor())
    {
        return (isFactorTail());
    }
    return false;
}
bool isTermTail()
{
    if (isExecuting == 0){ // if not executing
        isExecuting = 1; // it is now
    }
    executing("term_tail");

    if (isAddOp())
    {
        if (!isTerm())
        {
            return false;
        }
        return (isTermTail());
    }
    return true;
}
bool isExpr()
{
    if (isExecuting == 0){ // if not executing
        isExecuting = 1; // it is now
    }
    executing("expr");

    if (isTerm())
    {
        return isTermTail();
    }
    return false;
}
bool isStatement()
{
    if (isExecuting == 0){ // if not executing
        isExecuting = 1; // it is now
    }
    executing("stmt");

    if (tokenVector[n].type == TokenType::Identifier)
    {
        
        isExecuting = 0;
        cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        n++;
        if (tokenVector[n].specificType == ":=")
        {
            isExecuting = 0;
            cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
            output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
            n++;
            return (isExpr());
        }
    }
    else if (tokenVector[n].type == TokenType::Keyword)
    {
        if (tokenVector[n].specificType == "WRITE")
        {
            isExecuting = 0;
            cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
            output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
            n++;
            return (isExpr());
        }
        else if (tokenVector[n].specificType == "READ")
        {
            isExecuting = 0;
            cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
            output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
            n++;
            if (tokenVector[n].type == TokenType::Identifier)
            {
                isExecuting = 0;
                cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
                output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
                n++;
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}
bool isStatementList()
{
    isExecuting = 1;
    executing("stmt_list");

    if (tokenVector[n].type == TokenType::End)
    {
        isExecuting = 0;
        cout << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        output << endl << "    Token:\t" << tokenVector[n].specificType << " consumed ";
        return true;
    }
    if (!isStatement())
    {
        return false;
    }
    return isStatementList();
}



int main() {

    // Input Stuff
    class LexAnalyzer
    {
        private:
            int position;

        public:
            LexAnalyzer(const vector<char>& inputVector)
            {
                position = 0;
            }
            void advance()
            {
                position++;
            }
            int getPosition()
            {
                return position;
            }
            void resetPosition()
            {
                position = 0;
            }

            char peek()
            {
                if (position + 1 < input.size())
                    return input[position + 1];

                return '\0';
            }

            char current()
            {
                if (position < input.size())
                    return input[position];

                return '\0';
            }

            void ignoreWhiteSpace()
            {
                while (inputRemaining() && current() == ' ')
                {
                    advance();
                }
            }
            bool isDigit(char input)
            {
                if (input == '0')
                    return true;
                else if (input == '1')
                    return true;
                else if (input == '2')
                    return true;
                else if (input == '3')
                    return true;
                else if (input == '4')
                    return true;
                else if (input == '5')
                    return true;
                else if (input == '6')
                    return true;
                else if (input == '7')
                    return true;
                else if (input == '8')
                    return true;
                else if (input == '9')
                    return true;
                else
                    return false;
            }
            bool isLetter(char input)
            {
                return (input >= 'a' && input <= 'z') ||
                    (input >= 'A' && input <= 'Z');
            }
            bool isKeyword(string input)
            {
                return (input == "read" || input == "write");
            }

            bool inputRemaining()
            {
                return position < input.size();
            }
    };

    // Load input file and store each line as a token in "tokens" vector. Logic mostly reused from Project 1
    cout << "LOADING TOKENS FROM INPUT FILE..." << endl << endl;
    inputFileToVector();
    currentLine = 0;
    while (currentLine < infileVector.size()){
        nextLineAsVector(currentLine);
        //cout << endl << "STARTING AT NEW LINE: " << currentLine << endl;
            
        string number = "";
        string word = "";
        
        LexAnalyzer lexer(input);
        Token tkn;

        //Loop processes 1 line from input file
        char currentChar = lexer.current();
        switch (currentChar)
        {
        case ('+'):
            tkn.type = TokenType::Operator;
            tkn.specificType = "+";
            cout << "add token stored!" << endl;
            tokenVector.push_back(tkn); // push token onto vector
            lexer.advance();
            break;
        case ('-'):
            tkn.type = TokenType::Operator;
            tkn.specificType = "-";
            cout << "sub token stored!" << endl;
            tokenVector.push_back(tkn); // push token onto vector
            lexer.advance();
            break;

        case ('*'):
            tkn.type = TokenType::Operator;
            tkn.specificType = "*";
            cout << "mult token stored!" << endl;
            tokenVector.push_back(tkn); // push token onto vector
            lexer.advance();;
            break;
        case ('/'):
            tkn.type = TokenType::Operator;
            tkn.specificType = "/";
            cout << "div token stored!" << endl;
            tokenVector.push_back(tkn); // push token onto vector
            lexer.advance();
            break;

        case (':'):
            if (lexer.peek() == '=')
            {
                tkn.type = TokenType::Operator;
                tkn.specificType = ":=";
                cout << "assign token stored!" << endl;
                tokenVector.push_back(tkn); // push token onto vector
                lexer.advance();
                lexer.advance();
            }
            else
            {
                cout << "unhandled case" << endl;
                lexer.advance();
            }
            break;
        case ('('):
            tkn.type = TokenType::Punctuation;
            tkn.specificType = "(";
            cout << "lparen token stored!" << endl;
            tokenVector.push_back(tkn); // push token onto vector
            lexer.advance();
            break;
        case (')'):
            tkn.type = TokenType::Punctuation;
            tkn.specificType = ")";
            cout << "rparen token stored!" << endl;
            tokenVector.push_back(tkn); // push token onto vector
            lexer.advance();
            break;
        case ('.'):
            if (lexer.isDigit(lexer.peek()))
            {
                number = ".";

                lexer.advance();

                while (lexer.isDigit(lexer.current()))
                {
                    number += lexer.current();
                    lexer.advance();
                }

                tkn.type = TokenType::Number;
                tkn.specificType = number;
                cout << number << " stored as token!" << endl;
                tokenVector.push_back(tkn); // push token onto vector
                number = "";
                cout << " number cleared!";
            }
            else
            {
                tkn.type = TokenType::Punctuation;
                tkn.specificType = "PERIOD";
                cout << "period stored!" << endl;
                tokenVector.push_back(tkn); // push token onto vector
                lexer.advance();
            }
            break;
        case ('$'):
            if (lexer.peek() == '$')
            {
                tkn.type = TokenType::End;
                tkn.specificType = "$$";
                tokenVector.push_back(tkn); // push token onto vector
                lexer.advance();
                lexer.advance();
            }
            else
            {
                cout << "Error, $ is not a valid input";
            }
            break;
        case (' '):
            lexer.ignoreWhiteSpace();
            break;
        default:
            if (lexer.isDigit(currentChar))
            {
                bool hasDecimal = false;
                while (lexer.isDigit(lexer.current()) ||
                    (lexer.current() == '.' && !hasDecimal))
                {
                    if (lexer.current() == '.')
                    {
                        hasDecimal = true;
                    }
                    number += lexer.current();
                    lexer.advance();
                }
                tkn.type = TokenType::Number;
                tkn.specificType = number;
                cout << number << " stored as token!\t";
                tokenVector.push_back(tkn); // push token onto vector
                number = "";
                cout << "number cleared!" << endl;
            }
            else if (lexer.isLetter(currentChar))
            {
                while (lexer.isLetter(lexer.current()) || lexer.isDigit(lexer.current()))
                {
                    word += lexer.current();
                    lexer.advance();
                }
                if (lexer.isKeyword(word))
                {
                    tkn.type = TokenType::Keyword;
                    if (word == "read")
                    {
                        tkn.specificType = "READ";
                        cout << "keyword read stored\t";
                        tokenVector.push_back(tkn); // push token onto vector

                    }
                    else if (word == "write")
                    {
                        tkn.specificType = "WRITE";
                        cout << "keyword write stored\t";
                        tokenVector.push_back(tkn); // push token onto vector
                    }
                    else
                    {
                        cout << "how did we get here\t";
                    }
                }
                else
                {
                    tkn.type = TokenType::Identifier;
                    tkn.specificType = word;
                    cout << "identifier \"" << word << "\" stored!\t";
                    tokenVector.push_back(tkn); // push token onto vector
                }
                word = "";
                cout << " word cleared!" << endl;
            }
        }
        currentLine++;
    }
    cout << endl << "LOADING TOKENS COMPLETE!" << endl << "--------------------------------------------------" << endl;


    // main using the recursive descent logic
    executing("program");

    bool parsed = false;
    parsed = isStatementList();

    if (parsed){
        cout << endl << "--------------------------------------------------" << endl << "No Error was generated." << endl;
        output << endl << "--------------------------------------------------" << endl << "No Error was generated." << endl;
    }
    else {
        cout << endl << "--------------------------------------------------" << endl << "An Error has occured." << endl;
        output << endl << "--------------------------------------------------" << endl << "An Error has occured." << endl;
    }
    
    output.close();
    cout << "--------------------------------------------------" << endl << "Output saved to output.txt." << endl;

    return 0;
}
