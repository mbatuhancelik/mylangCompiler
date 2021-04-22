#include "tokens.h"
#include <string>
#include <regex>
#include <iostream>
#include "utilities.h"
using namespace std;

string varRegexStr = "(\\s{0,}[a-zA-Z]{1,}[a-zA-Z0-9]{0,}\\s{0,})";
string intRegexStr = "\\s{0,}([0-9]{1,})\\s{0,}";
string paranRegexStr = "\\((?:[^)(,]*?)*+\\)";
string termRegexStr = "(.*[\\*\\/]{1,1})+.*";
string multdivRegexStr = "[\\*|\\/]{1,1}";

regex varRegex(varRegexStr);
regex integerRegex(intRegexStr);
regex parenthesisRegex(paranRegexStr);
regex termRegex(termRegexStr);
regex addsubRegex("[+-]");
regex multdivRegex(multdivRegexStr);
regex equalsRegex("=");
regex whileRegex("\\s{0,}while(\\(.{1,}\\))\\s{0,}\\{");
regex ifRegex("\\s{0,}if(\\(.{1,}\\))\\s{0,}\\{");
regex printRegex("\\s{0,}print(\\(.{1,}\\))\\s{0,}");
regex closeCurvedParanRegex("\\s{0,}\\}\\s{0,}");
regex commaRegex(",");

bool isValidExpression(string s);

bool isValidVariable(string s){

    return regex_match(s, varRegex);

}
bool isValidInteger(string s){

    return regex_match(s, integerRegex);

}

bool isValidFactor(string s){

    bool isNumber = isValidInteger(s);
    if (isNumber)
        return true;

    bool isVar = isValidVariable(s);
    if (isVar)
        return true;
    return false;

}

bool isValidTerm(string s){

    smatch m;
    bool multi = regex_match(s, termRegex);

    if (multi){
        regex_search(s, m, multdivRegex);
        bool isTerm = isValidTerm(m.suffix());
        bool isFactor = isValidFactor(m.prefix());
        return isTerm && isFactor;
    }

    if (isValidFactor(s))
        return true;
    return false;

}

bool checkBetweenParenthesis(string s){

    bool isparenthesis = regex_match(s, parenthesisRegex);
    if (isparenthesis)
        return (isValidExpression(s.substr(1, s.length() - 2)));
    return false;

}

bool removeParenthesis(string &s){

    smatch m;
    int index = 0;
    string replacement = " temp";

    while (std::regex_search(s, m, parenthesisRegex)){

        for (auto x : m){

            string xx = x;
            
            if (checkBetweenParenthesis(xx)){
            string rep = replacement + to_string(index) + " ";
            findAndReplace(s, xx, rep);
            index += 1;
            }
            else return false;
        }

    }
    return true;
}

bool isValidChoose(string s){

    s = s.substr(7,s.length()-8);
    s += ",";

    smatch m;
    while (std::regex_search(s, m, commaRegex) && s.length() != 1){

        if(!isValidExpression(m.prefix()))
            return false;
        s = m.suffix();

    }

    return true;

}
// removes syntatically correct chooses
bool removeChoose(string &s){

    smatch m;
    int index = 0;

    while (std::regex_search(s, m, chooseRegex)){

        string xx = m[0].str();

        int parantval = 0;
        for(int i = 0; i < m.size(); i++){

        	cout << m[i] << endl;

        }
        for(int i = 0; i < xx.size(); i++){

        	if(xx[i] == '(') parantval++;
        	else if(xx[i] == ')') parantval--;

        }
        int index = s.find(xx);
        while(parantval > 0){
            int len = xx.length();
        	string temp = s.substr(index+len);

            int t = temp.find_first_of(")");

            xx += temp.substr(0,t+1);
            parantval -- ;

        }
        // find xx
        if (isValidChoose(xx)){

			string rep = " tempc" + to_string(index) + " ";
			findAndReplace(s, xx, rep);

			index += 1;
        }

        else return false;

    }

    return true;

}

bool isValidExpression(string s){

    bool validChoose = removeChoose(s);

    if(!validChoose) return false;

    bool validParenthesis = removeParenthesis(s);

    if(!validParenthesis) return false;

    if (isValidTerm(s))
        return true;

    smatch m;
    bool multi = regex_search(s, m, addsubRegex);
    if (multi)
    {
        bool isExpression = isValidExpression(m.suffix());
        bool isTerm = isValidTerm(m.prefix());
        return isTerm && isExpression;
    }

    return false;

}

bool isValidAssignment(string s){
    smatch m;
    bool multi = regex_search(s, m, equalsRegex);

    if (multi)
    {
        bool expr = isValidExpression(m.suffix());
        bool var = isValidTerm(m.prefix());
        return var && expr;
    }

    return false;

}

bool isValidWhile(string s){

    bool syntax = regex_match(s, whileRegex);
    if(!syntax) return false;
    int beginString = s.find_first_of("(");
    int endString = s.find_last_of(")");

    return isValidExpression(s.substr(beginString+1, endString - beginString -1));

}

bool isValidIf(string s){

    bool syntax = regex_match(s, ifRegex);
    if(!syntax) return false;
    int beginString = s.find_first_of("(");
    int endString = s.find_last_of(")");

    return isValidExpression(s.substr(beginString+1, endString - beginString -1));

}

bool isCurvParanClose(string s){

    return regex_match(s, closeCurvedParanRegex);

}

bool isValidPrint(string s){

    bool syntax = regex_match(s,printRegex);
    if(!syntax) return false;
    int beginString = s.find_first_of("(");
    int endString = s.find_last_of(")");
    return isValidExpression(s.substr(beginString+1, endString - beginString -1));

}

string getType(string s){

    if(isCurvParanClose(s)) return "curv";
    if(isValidWhile(s)) return "while";
    if(isValidIf(s)) return "if";
    if(isValidPrint(s)) return "print";
    if(isValidAssignment(s)) return "assignment";
    return "syntax error";

}