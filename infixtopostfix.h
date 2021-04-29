#include <string>
#include <stack>
#include <algorithm>

using namespace std;

#ifndef INFIX_TO_POSTFIX_H
#define INFIX_TO_POSTFIX_H

int operatorprecedence(char op);

string infixtopostfix(string expression);

#endif