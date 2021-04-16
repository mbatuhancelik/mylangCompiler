#ifndef tokens
#define tokens
#include <string>
#include <regex>
using namespace std;



bool isVariable(string s);
bool isFactor(string s);
bool isTerm(string s);
bool isExpr(string s);
bool isAssign(string s);


#endif