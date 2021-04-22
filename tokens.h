#include <string>
#include <regex>

#ifndef tokens
#define tokens

using namespace std;
regex chooseRegex("choose\\s{0,}\\(([a-zA-Z0-9+\\*\\-\\/\\s()]*,){3,3}[a-zA-Z0-9+\\*\\-\\/\\s()]*?\\)");
bool isValidVariable(string s);
bool isValidInteger(string s);
bool isValidAssignment(string s);
bool isValidWhile(string s);
bool isValidIf(string s);
bool isCurvParanClose(string s);
bool isValidPrint(string s);

string getType(string s);

#endif