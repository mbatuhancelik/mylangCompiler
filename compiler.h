#include "printer.h"
#include "tokens.h"
#include "infixtopostfix.h"
#include "utilities.h"

#include <string>
#include <sstream>
#include <unordered_map>

#ifndef compiler
#define compiler


class Compiler{
    private: 
    int pointers;
    int variables;
    int whiles; 
    int ifs;
    int chooses;
    int chooseVariables;
    bool inWhile;
    bool inIf;
    Printer &p;
    unordered_map<string, string> variableMap;
    string getTemp();
    public:
    Compiler(Printer &p);
    string compilePoint(string s);
    string loadPoint(string s);
    bool compileAssignment(string s);
    string compileExpression(string s);
    void compilePrint(string s);
    bool compileWhile(string s);
    bool compileIf(string s);
    bool compileCurv(string s);
    string compileChoose(string s);
    void replaceChoose(string &s);
    void finalize(int line, bool syntaxError = false);
    
};


#endif // parser
