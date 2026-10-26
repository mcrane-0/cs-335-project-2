/*
CS 335 Programming Assignment 1
Chloe Cantrell, Miles Crane, Trevor Past

Generative AI was NOT used for this assignment.
*/


#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

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


int main() {
    enum class TokenType
    {
        Default,
        Keyword,//Token.specificType values: READ, WRITE
        Identifier,
        Number,
        Operator,//Token.specificType values: ADD, SUB, ASSIGN, DIV, MULT
        Punctuation,//Token.specificType: LPAREN, RPAREN, PERIOD
    };
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

    struct Token
    {
        TokenType type = TokenType::Default;
        string specificType = "";
    };


    vector<Token> tokens = {}; // vector to store tokens from input

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
            tkn.specificType = "ADD";
            cout << "add token stored!" << endl;
            //output << "+" << endl;
            lexer.advance();


            break;
        case ('-'):
            tkn.type = TokenType::Operator;
            tkn.specificType = "SUB";
            cout << "sub token stored!" << endl;
            //output << "-" << endl;
            lexer.advance();
            break;

        case ('*'):
            tkn.type = TokenType::Operator;
            tkn.specificType = "MULT";
            cout << "mult token stored!" << endl;
            //output << "*" << endl;
            lexer.advance();;
            break;
        case ('/'):
            tkn.type = TokenType::Operator;
            tkn.specificType = "DIV";
            cout << "div token stored!" << endl;
            //output << "/" << endl;
            lexer.advance();
            break;

        case (':'):
            if (lexer.peek() == '=')
            {
                tkn.type = TokenType::Operator;
                tkn.specificType = "ASSIGN";
                cout << "assign token stored!" << endl;
                //output << ":=" << endl;
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
            tkn.specificType = "LPAREN";
            cout << "lparen token stored!" << endl;
            //output << "(" << endl;
            lexer.advance();
            break;
        case (')'):
            tkn.type = TokenType::Punctuation;
            tkn.specificType = "RPAREN";
            cout << "rparen token stored!" << endl;
            //output << ")" << endl;
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
                cout << number << " stored as token!" << endl;
                //output << number << endl;
                number = "";
                cout << " number cleared!";
            }
            else
            {
                tkn.type = TokenType::Punctuation;
                tkn.specificType = "PERIOD";
                cout << "period stored!" << endl;
                //output << "." << endl;
                lexer.advance();
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
                cout << number << " stored as token!\t";
                //output << number << endl;
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
                        //output << "read" << endl;

                    }
                    else if (word == "write")
                    {
                        tkn.specificType = "WRITE";
                        cout << "keyword write stored\t";
                        //output << "write" << endl;
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
                    //output << word << endl;
                }
                word = "";
                cout << " word cleared!" << endl;
            }
        }
        currentLine++;
    }
    cout << endl << "LOADING TOKENS COMPLETE!" << endl << endl;

    // now the parsing begins...

    
    return 0;
}
