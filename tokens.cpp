#include "tokens.h"
#include <string>
#include <regex>
#include <iostream>

string varRegex = "([a-zA-Z]{1,}[a-zA-Z0-9]{0,})";
string integerRegex ="([0-9]{1,})";
string paranRegex = "(\\(.{1,}\\))";
string termRegex = "(.*[\\*\\/]{1,1})+.*";
string muldivRegex = "[\\*|\\/]{1,1}";

regex var(varRegex);
regex integer(integerRegex);
regex paranthesis(paranRegex);
regex term(termRegex);
regex addsub ("[+-]");
regex muldiv (muldivRegex);
regex equals ("=");

bool isVariable(string s){
    return regex_match(s,var);
}

bool isFactor(string s){
    bool isNumber = regex_match(s,integer);
    if(isNumber) return true;

    bool isVar= isVariable(s);
    if (isVar) return true;
    bool isParanthesis = regex_match(s,paranthesis);
    if(isParanthesis)
        if(isExpr(s.substr(1,s.length() - 2)))
            return true;
    return false;   

}

bool isTerm(string s){
    if (isFactor(s)) return true;

    smatch m;
    bool multi = regex_match(s, term);

    if(multi){
        regex_search(s,m,muldiv);
        bool term = isTerm(m.suffix());
        bool factor = isFactor(m.prefix());
        return term && factor;
    }
    return false;
}

bool isExpr(string s){
    if (isTerm(s)) return true;

    
    smatch m;
    bool multi = regex_search(s, m, addsub);
    if(multi){
        bool expr = isExpr(m.suffix());
        bool term = isTerm(m.prefix());
        return term && expr;
    }
    return false;
}

bool isAssign(string s){
    smatch m;
    bool multi = regex_search(s, m, equals);
    cout <<m.suffix()<<endl;
    cout <<m.prefix() << endl;
    if(multi){
        bool expr = isExpr(m.suffix());
        bool var = isTerm(m.prefix());
        return var && expr;
    }
    return false;
}