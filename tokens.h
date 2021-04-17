#ifndef tokens
#define tokens
#include <string>
#include <regex>
using namespace std;


// three commented functions bellow does not work with paranthesis
//yet, paranthesis are eliminated in isExpr method so overall result is true!
// bool isVariable(string s);
// bool isFactor(string s);
// bool isTerm(string s);
bool isExpr(string s);
bool isAssign(string s);
// checks if a line consists of a while beginning
bool isWhile(string s);
// checks if a line consists of a if beginning
bool isIf(string s);
// checks if a line consists of a curved paranthesis
bool isCurvParanClose(string s);
//checks if a line is print statement
bool isPrint(string s);
// returns type of a line
string getType(string s);

#endif