#include "tokens.h"
#include "utilities.h"

using namespace std;

regex varRegex("(\\s{0,}[a-zA-Z]{1,}[a-zA-Z0-9]{0,}\\s{0,})");
regex integerRegex("\\s{0,}([0-9]{1,})\\s{0,}");
regex parenthesisRegex("\\([^(),]{1,}\\)");
regex termRegex("(.*[\\*\\/]{1,1})+.*");
regex addsubRegex("[+-]");
regex multdivRegex("[\\*|\\/]{1,1}");
regex equalsRegex("=");
regex whileRegex("\\s{0,}while\\s{0,}(\\(.{1,}\\))\\s{0,}\\{\\s{0,}");
regex ifRegex("\\s{0,}if\\s{0,}(\\(.{1,}\\))\\s{0,}\\{\\s{0,}");
regex printRegex("\\s{0,}print(\\(.{1,}\\))\\s{0,}");
regex closeCurvedParanRegex("\\s{0,}\\}\\s{0,}");
regex chooseRegex("choose\\s{0,}\\((((?!choose\\()[a-zA-Z0-9+\\*\\-\\/\\s()])*,){3,3}((?!choose\\()[a-zA-Z0-9+\\*\\-\\/\\s()])*?\\)");
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

    smatch matches;
    bool multi = regex_match(s, termRegex);

    if (multi){
        regex_search(s, matches, multdivRegex);
        bool isTerm = isValidTerm(matches.suffix());
        bool isFactor = isValidFactor(matches.prefix());
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

    smatch matches;
    int index = 0;
    string replacement = " temp";

    while (std::regex_search(s, matches, parenthesisRegex)){

            string match = matches[0];
            
            if (checkBetweenParenthesis(match)){
            replacement = replacement + to_string(index) + " ";
            findAndReplace(s, match, replacement);
            index += 1;
            }
            else return false;
    }
    return true;
}

bool isValidChoose(string s){

    s = s.substr(7,s.length()-8) + ",";

    smatch matches;
    while (std::regex_search(s, matches, commaRegex) && s.length() != 1){

        if(!isValidExpression(matches.prefix()))
            return false;
        s = matches.suffix();

    }

    return true;

}
// removes syntatically correct chooses
bool removeChoose(string &s){

    smatch matches;
    int index = 0;

    while (std::regex_search(s, matches, chooseRegex)){

        string match = matches[0].str();

        int parantval = 0;

        for(int i = 0; i < match.size(); i++){

        	if(match[i] == '(') parantval++;
        	else if(match[i] == ')') parantval--;

        }

        int index = s.find(match);
        while(parantval > 0){
            int len = match.length();
        	string temp = s.substr(index+len);

            int t = temp.find_first_of(")");

            match += temp.substr(0,t+1);
            parantval -- ;

        }

        if (isValidChoose(match)){

			string replacement = " tempc" + to_string(index) + " ";
			findAndReplace(s, match, replacement);

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

    smatch matches;
    bool multi = regex_search(s, matches, addsubRegex);

    if (multi){
        bool isExpression = isValidExpression(matches.suffix());
        bool isTerm = isValidTerm(matches.prefix());
        return isTerm && isExpression;
    }

    return false;

}

bool isValidAssignment(string s){

    smatch matches;
    bool multi = regex_search(s, matches, equalsRegex);

    if (multi){
        bool expr = isValidExpression(matches.suffix());
        bool var = isValidTerm(matches.prefix());
        return var && expr;
    }

    return false;

}

bool isValidWhile(string s){

    bool syntax = regex_match(s, whileRegex);

    if(!syntax) 
        return false;
    
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