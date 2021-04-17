#include "tokens.h"
#include <string>
#include <regex>
#include <iostream>

using namespace std;
string varRegex = "(\\s{0,}[a-zA-Z]{1,}[a-zA-Z0-9]{0,}\\s{0,})";
string integerRegex = "\\s{0,}([0-9]{1,})\\s{0,}";
string paranRegex = "(\\(.{1,}\\))";
string termRegex = "(.*[\\*\\/]{1,1})+.*";
string muldivRegex = "[\\*|\\/]{1,1}";

regex var(varRegex);
regex integer(integerRegex);
regex parenthesis(paranRegex);
regex term(termRegex);
regex addsub("[+-]");
regex muldiv(muldivRegex);
regex equals("=");

bool isVariable(string s)
{
    return regex_match(s, var);
}

bool isFactor(string s)
{
    bool isNumber = regex_match(s, integer);
    if (isNumber)
        return true;

    bool isVar = isVariable(s);
    if (isVar)
        return true;
    return false;
}

bool isTerm(string s)
{
    smatch m;
    bool multi = regex_match(s, term);

    if (multi)
    {
        regex_search(s, m, muldiv);
        bool term = isTerm(m.suffix());
        bool factor = isFactor(m.prefix());
        return term && factor;
    }
    if (isFactor(s))
        return true;
    return false;
}
void findAndReplace(string &s, string &toReplace, string &replacement)
{
    size_t index = 0;
    index = s.find(toReplace, index);
    while (index!= -1)
    {
        /* Locate the substring to replace. */
        // index = s.find(toReplace, index);
        if (index == std::string::npos)
            break;

        /* Make the replacement. */
        s.replace(index, toReplace.length(), replacement);

        /* Advance index forward so the next iteration doesn't pick it up as well. */
        // index += replacement.length();
        index = s.find(toReplace, index);
    }
}
bool checkBetweenparenthesis(string s)
{
    bool isparenthesis = regex_match(s, parenthesis);
    if (isparenthesis)
        return (isExpr(s.substr(1, s.length() - 2)));
    
    cout << "checking between parenthesis without parenthesis\n";
    return false;
}
bool eliminateParenthesis(string &s)
{
    smatch m;
    string regex = "\\((?:[^)(]*?)*+\\)";
    std::regex e(regex);
    int index = 0;
    string replacement = "temp";
    while (std::regex_search(s, m, e))
    {
        for (auto x : m)
        {
            string xx = x;
            if (checkBetweenparenthesis(xx)){
            string rep = replacement + to_string(index);
            findAndReplace(s, xx, rep);
            index += 1;
            }
            else return false;
        }
        // cout << "parenthesis eliminated" << endl;
        // cout << s << endl;
        // std::cout << std::endl;
    }
    return true;
}
bool isExpr(string s)
{
    // if(s.empty())
    // return false;
    // cout << s<< endl;
    bool validParenthesis = eliminateParenthesis(s);
    if(!validParenthesis) return false;
    // cout << s << endl;
    if (isTerm(s))
        return true;

    smatch m;
    bool multi = regex_search(s, m, addsub);
    if (multi)
    {
        bool expr = isExpr(m.suffix());
        bool term = isTerm(m.prefix());
        return term && expr;
    }
    return false;
}

bool isAssign(string s)
{
    smatch m;
    bool multi = regex_search(s, m, equals);
    // cout << m.suffix() << endl;
    // cout << m.prefix() << endl;
    if (multi)
    {
        bool expr = isExpr(m.suffix());
        bool var = isTerm(m.prefix());
        return var && expr;
    }
    return false;
}