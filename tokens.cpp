#include "tokens.h"
#include "utilities.h"

using namespace std;

regex varRegex("(\\s{0,}[a-zA-Z]{1,}[a-zA-Z0-9]{0,}\\s{0,})"); //regex expression that defines variables
regex integerRegex("\\s{0,}([0-9]{1,})\\s{0,}");//regex expression that defines integers
regex parenthesisRegex("\\([^(),]{1,}\\)");//regex expression that defines expressions between parenthesis
regex termRegex("(.*[\\*\\/]{1,1})+.*");//regex expression that defines terms
regex addsubRegex("[+-]{1,1}");//regex expression that defines addition and subtraction operators
regex multdivRegex("[\\*|\\/]{1,1}"); //regex expression that defines multiplication and division operators
regex equalsRegex("=");//regex expression that defines equals sign
regex whileRegex("\\s{0,}while\\s{0,}(\\(.{1,}\\))\\s{0,}\\{\\s{0,}");//regex expression that defines while statements
regex ifRegex("\\s{0,}if\\s{0,}(\\(.{1,}\\))\\s{0,}\\{\\s{0,}");//regex expression that defines if statements
regex printRegex("\\s{0,}print\\s{0,}(\\(.{1,}\\))\\s{0,}");//regex expression that defines print statements
regex closeCurvedParanRegex("\\s{0,}\\}\\s{0,}"); //regex expression that defines closed curved parenthesis statements
//hideous regex expression that defines choose functions
regex chooseRegex("choose\\s{0,}\\((((?!choose\\()[a-zA-Z0-9+\\*\\-\\/\\s()])*,){3,3}((?!choose\\()[a-zA-Z0-9+\\*\\-\\/\\s()])*?\\)");
regex commaRegex(","); //regex expression that defines a comma

bool isValidExpression(string s); // declare isValidExpression

//checks if a variable is valir
bool isValidVariable(string s){
    //returns if the variable matches the variable regex
    return regex_match(s, varRegex);

}
//checks if an integer is valid
bool isValidInteger(string s){
    //returns if the variable matches the integer regex
    return regex_match(s, integerRegex);

}
//checks if a factoris valid
bool isValidFactor(string s){
    // if s is a valid number, returns true
    bool isNumber = isValidInteger(s);
    if (isNumber)
        return true;   
    
    //if x is a valid variable, returns true
    bool isVar = isValidVariable(s);
    if (isVar)
        return true;

    // otherwise returns false
    return false;

}

//checks if a term is valid
bool isValidTerm(string s){

    smatch matches;
    //true if there are multiple nonterminals divided by multiplication or division
    bool multi = regex_match(s, termRegex);

    if (multi){
        //find the first multiplication or division operator
        regex_search(s, matches, multdivRegex);
        //true if right hand side of the operator is a term 
        bool isTerm = isValidTerm(matches.suffix());
        //true if left hand side of the operator is a factor
        bool isFactor = isValidFactor(matches.prefix());
        //returns true if both are true
        return isTerm && isFactor;
    }
    //if s is a valid factor, returns true
    if (isValidFactor(s))
        return true;

    //otherwise, returns false
    return false;

}

//returns true if the expression between parentesis is valid
bool checkBetweenParenthesis(string s){
    //true if s is something between parenthesis and does not contain nested parenthesis
    bool isparenthesis = regex_match(s, parenthesisRegex);

    if (isparenthesis)
        //if thats the case, return validity of the expression inside
        return (isValidExpression(s.substr(1, s.length() - 2)));

    //if s does not match parenthesis regex, return false
    return false;

}

//eliminates the parenthesis in expression by replacing them with random variables 
bool removeParenthesis(string &expr){

    smatch matches;
    int index = 0;
    string replacement = " temp"; //random variable name to replace parenthesis with

    while (std::regex_search(expr, matches, parenthesisRegex)){ //do while there are parenthesis within the string

            string match = matches[0]; //locate inermost parenthesis
            
            if (checkBetweenParenthesis(match)){ //if the expression within is valid
            replacement = replacement + to_string(index) + " "; //create a temp variable 
            findAndReplace(expr, match, replacement); //replace the innermost parenthesis with the temp variable
            index += 1;
            }
            else return false; // if all parenthesis could not be replaced, return false
    }
    //otherwise, return true
    return true;
}
//checks for if given string is a valid choose function
bool isValidChoose(string s){

    //change form of string to:
    // expr1,expr2,expr3,expr4,
    s = s.substr(7,s.length()-8) + ","; 

    smatch matches;
    //while there is something before the comma
    while (std::regex_search(s, matches, commaRegex) && s.length() != 1){
        //if the thing before the comma is not a valid expression return false
        if(!isValidExpression(matches.prefix()))
            return false;
        //remove the validated expresion from s
        s = matches.suffix();

    }
    //if all expressions are valid, returns true
    return true;

}
// removes syntatically correct chooses from string s
bool removeChoose(string &s){

    smatch matches;
    int index = 0; //beginning index of the choose function 

    //while there is something that matches the choose regex in s
    while (std::regex_search(s, matches, chooseRegex)){

        string match = matches[0].str(); // extract the choose function

        //calculate the difference between number of opening and closing parenthesis
        //paranval should be 0 for balanced parenthesis
        int parantval = 0; 

        for(int i = 0; i < match.size(); i++){

        	if(match[i] == '(') parantval++;
        	else if(match[i] == ')') parantval--;

        }

        int index = s.find(match); // beginning index of the choose sub

        //while parenthesis are not balanced, extend the match and 
        //add closing parenthesis until they are matched
        //Note that if paranval <0, the string cant match the regex
        while(parantval > 0){
            int len = match.length();
        	string temp = s.substr(index+len);

            int t = temp.find_first_of(")");

            match += temp.substr(0,t+1);
            parantval -- ;

        }
        //if the choose function is valid
        if (isValidChoose(match)){

            //create a replacement string
			string replacement = " tempc" + to_string(index) + " ";
            //replace the choose function with a variable
			findAndReplace(s, match, replacement);

			index += 1;
        }
        // if the choose function is not valid, return false
        else return false;

    }
    //when all choose functions are replaced, return true
    return true;

}

//checks for if given string is a valid expression
bool isValidExpression(string s){

    //remove choose functions
    //if one of them is not valid, return false
    bool validChoose = removeChoose(s);
    if(!validChoose) return false;
    //remove parenthesis
    //if any expression in parenthesis is invalid, return false
    bool validParenthesis = removeParenthesis(s);
    if(!validParenthesis) return false;
    
    //checks if the string is a valid term
    //If it is, returns true 
    if (isValidTerm(s))
        return true;

    smatch matches;
    //searches for + or - tokens in the string
    bool multi = regex_search(s, matches, addsubRegex);
    //if these tokens exists
    if (multi){
        // true if right hand side is an expression
        bool isExpression = isValidExpression(matches.suffix());
        // true if left hand side is an term
        bool isTerm = isValidTerm(matches.prefix());
        //return end of the results
        return isTerm && isExpression;
    }
    //otherwise return false
    return false;

}

//checks for if given string is a valid assignment statement
bool isValidAssignment(string s){

    smatch matches;
    //finds = token in the string
    bool multi = regex_search(s, matches, equalsRegex);
    //if it is found
    if (multi){
        //checks if right hand side is an expression
        bool expr = isValidExpression(matches.suffix());
        //checks if left hand side is an variable
        bool var = isValidVariable(matches.prefix());
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