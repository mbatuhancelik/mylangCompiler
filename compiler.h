#include "printer.h"
#include "tokens.h"
#include "infixtopostfix.h"
#include "utilities.h"
#include <sstream>
#include <unordered_map>

#ifndef COMPILER_H
#define COMPILER_H

// an object which takes valid mylang code and produces llvm code
//It is responsible for coherent naming of llvm variables, labels, pointers
class Compiler{

    private:

        int pointerCount; //number of pointerCount used in llvm code
        int variableCount; // number of variableCount used in llvm code
        int whileCount; //number of while structures
        int ifCount;//number of if structures
        int chooseCount; //number of choose structures
        int choosePointerCount; //number of variableCount used to implement choose

        bool inWhile; // true when while loop is compiling
        bool inIf;// true when if construct is compiling

        Printer &p; //printing interface to write output file

        unordered_map<string, string> pointerMap; // maps variableCount in mylang code to pointerCount in llvm

        string getTemp(); // returns a valid temporary variable

    public:

        Compiler(Printer &p); //constructor
        
        string compilePoint(string s); // generates a valid pointer, maps its corresponding variable in pointerMap
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
