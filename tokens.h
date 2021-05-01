#include <string>
#include <regex>

#ifndef TOKENS_H
#define TOKENS_H

using namespace std;
//checks if a variable is valid
bool isValidVariable(string s);
//checks if an integer is valid
bool isValidInteger(string s);
//checks for if given string is a valid assignment statement
bool isValidAssignment(string s);
//checks for if given string is a valid while statement
bool isValidWhile(string s);
//checks for if given string is a valid if statement
bool isValidIf(string s);
//checks for if given string is a closing curving parenthesis
bool isCurvParanClose(string s);
//checks for if given string is a valid print statement
bool isValidPrint(string s);
// returns the type of the given string
string getType(string s);

#endif