#include <string>
#include <regex>

#ifndef tokens
#define tokens

using namespace std;
bool isValidVariable(string s);
bool isValidInteger(string s);
bool isValidAssignment(string s);
bool isValidWhile(string s);
bool isValidIf(string s);
bool isCurvParanClose(string s);
bool isValidPrint(string s);

string getType(string s);

#endif