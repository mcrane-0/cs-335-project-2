#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

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
Token token0;
Token token1;
Token token2;
Token token3;

vector<Token> tokenVector = {token0, token1, token2, token3};
int n = 0;//tokenVector index
//Parsing Declarations
bool isExpr();
bool isTerm();
bool isFactor();
bool isStatement();

bool isAddOp()
{
    if (tokenVector[n].specificType == "ADD" || tokenVector[n].specificType == "SUB")
    {
        cout << "Token: " << tokenVector[n] << " consumed " << endl;
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
    if (tokenVector[n].specificType == "MULT" || tokenVector[n].specificType == "DIV")
    {
        cout << "Token: " << tokenVector[n] << " consumed " << endl;
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
if (tokenVector[n].type == TokenType::Identifier || tokenVector[n].type == TokenType::Number)
{
    cout << "Token: " << tokenVector[n] << " consumed " << endl;
    n++;
    return true;
}
else if (tokenVector[n].specificType == "LPAREN")
{
    //Check this logic.
    cout << "Token: " << tokenVector[n] << " consumed " << endl;
    n++;
    if (!isExpr())
    {
        return false;
    }
    if (tokenVector[n].specificType != "RPAREN")
    {
        return false;
    }
    cout << "Token: " << tokenVector[n] << " consumed " << endl;
    n++;
    return true;
}
return false;
}
bool isFactorTail()
{
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
    if (isFactor())
    {
        return (isFactorTail());
    }
    return false;
}
bool isTermTail()
{
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
    if (isTerm())
    {
        return isTermTail();
    }
    return false;
}
bool isStatement()
{
    if (tokenVector[n].type == TokenType::Identifier)
    {
        cout << "Token: " << tokenVector[n] << " consumed " << endl;
        n++;
        if (tokenVector[n].specificType == "ASSIGN")
        {
            cout << "Token: " << tokenVector[n] << " consumed " << endl;
            n++;
            return (isExpr());
        }
    }
    else if (tokenVector[n].type == TokenType::Keyword)
    {
        if (tokenVector[n].specificType == "WRITE")
        {
            cout << "Token: " << tokenVector[n] << " consumed " << endl;
            n++;
            return (isExpr());
        }
        else if (tokenVector[n].specificType == "READ")
        {
            cout << "Token: " << tokenVector[n] << " consumed " << endl;
            n++;
            if (tokenVector[n].type == TokenType::Identifier)
            {
                cout << "Token: " << tokenVector[n] << " consumed " << endl;
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
    if (tokenVector[n].type == TokenType::End)
    {
        cout << "Token: " << tokenVector[n] << " consumed " << endl;
        return true;
    }
    if (!isStatement())
    {
        return false;
    }
    return isStatementList();
}

int main(){
    
    // "Write main, which entails using the recursive descent logic PLUS conditionals that will determine the output"
    //The recursive descent logic is called. The cout statements above need review 
    // and additional statements should be added for execution order. 
    // Not sure how to format that on one line.
    isStatementList();
    }
}