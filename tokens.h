#ifndef tokens
#define tokens
#include <string>
#include <regex>

using namespace std;

bool isValidAssignment(string s);
bool isValidWhile(string s);
bool isValidIf(string s);
bool isCurvParanClose(string s);
bool isValidPrint(string s);

string getType(string s);

#endif