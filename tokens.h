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


#endif