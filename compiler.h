#include "printer.h"
#include "tokens.h"
#include "infixtopostfix.h"
#include "utilities.h"
#include <sstream>
#include <unordered_map>

#ifndef compiler
#define compiler

// an object which takes valid mylang code and produces llvm code
class Compiler{
    private: 
    int pointers; //number of pointers used in llvm code
    int variables; // number of variables used in llvm code
    int whiles; //number of while structures
    int ifs;//number of if structures
    int chooses; //number of choose structures
    int chooseVariables; //number of variables used to implement choose
    bool inWhile; // true when while loop is compiling
    bool inIf;// true when if construct is compiling
    Printer &p; //printing interface to write output file
    unordered_map<string, string> variableMap; // maps variables in mylang code to pointers in llvm
    string getTemp(); // returns a valid temporary variable
    public:
    Compiler(Printer &p); //constructor
    string compilePoint(string s); // generates a valid pointer, maps its corresponding variable in variableMap
    string loadPoint(string s); //loads a pointer and assigns the result to a temporary variable
    void compileAssignment(string s); //compiles assignment statements 
    string compileExpression(string s); // compiles expression and returns a temporary variable that stores the result
    void compilePrint(string s); //compiles print statements
    bool compileWhile(string s);// starts while loops
    bool compileIf(string s);// starts if statements
    bool compileCurv(string s);// ends if statements or while loops
    string compileChoose(string s);// compiles choose functions
    void replaceChoose(string &s);// replaces choose functions in expressions
    void finalize(int line, bool syntaxError = false); //finalizes the compilation and forms the output file 
};


#endif // parser
