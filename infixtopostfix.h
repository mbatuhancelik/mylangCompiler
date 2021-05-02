#include <string>
#include <stack>
#include <algorithm>

using namespace std;

#ifndef INFIX_TO_POSTFIX_H
#define INFIX_TO_POSTFIX_H

// returns 1 for + or -, returns 2 for * or /, returns 0 for neither
int operatorprecedence(char op);
//Well known infix to postfix algorithm
string infixtopostfix(string expression);

#endif