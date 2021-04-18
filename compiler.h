#include <fstream>
#include <string>
#include "infixtopostfix.h"
#include "tokens.h"
#include <stack>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include "printer.h"
#if !defined(compiler)
#define compiler

class Compiler{
    private: 
    int pointers;
    int variables;
    int whiles; 
    int ifs;
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
    bool compileCurv(string s);
    void finalize();
    
};


#endif // parser
