#include <string>
#include <stack>
#include <iostream>
#include <algorithm>

using namespace std;

#ifndef INFIX_TO_POSTFIX_H
#define INFIX_TO_POSTFIX_H

void printstack(stack<int> myStack);

int operatorprecedence(char op);

string infixtopostfix(string expression);

#endif