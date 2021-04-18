#include <fstream>
#include <string>
#include "infixtopostfix.h"
#include "tokens.h"
#include <stack>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include "compiler.h"
#include "printer.h"
#include <algorithm>
using namespace std;


Compiler::Compiler (Printer &p):p{p}{
    this->pointers = 0;
    this->variables = 0;
    this->inWhile = false;
    this->inIf = false;
    this->whiles = 0;
    this->ifs = 0;
}
string Compiler::getTemp(){
    string temp = "%t" + to_string(this->variables);
    this->variables ++ ;
    return temp;
}
string Compiler::compilePoint(string s){
    if(this->variableMap.find(s) == this->variableMap.end()){
        string pointer = "%p" + to_string(pointers);
        this->variableMap[s] = pointer;
        this->pointers += 1;
    }
    return variableMap[s];
}
string Compiler::loadPoint(string s){
    s = compilePoint(s);
    string temp = this->getTemp();
    this->p.print(temp + " = load i32* " + s);
    return temp;
}
bool Compiler::compileAssignment(string s){
    // s.erase(remove(s.begin(), s.end(), ' '), s.end());
    string point = s.substr(0,s.find_first_of("="));
    string exp = s.substr(s.find_first_of("=")+1);

    exp = compileExpression(exp);
    point = compilePoint(point);
    this->p.print("store i32 "+ exp+", i32* " + point);
    return true;
}

string Compiler::compileExpression(string s){
    // TODO: handle chooses
    s = infixtopostfix(s);

    stack<string> var;

    string token= "";
    stringstream tokenizer(s);

    while(tokenizer >> token){

        if( token[0] == '+'  ||token[0] == '-'||  
            token[0] == '*' || token[0] == '/'){

            string firstoperand;
            string secondoperand;

            secondoperand = var.top();
            var.pop();

            firstoperand = var.top();
            var.pop();

            // if(isValidVariable(firstoperand)){
            //     firstoperand = loadPoint(firstoperand);
            // }
            // if(isValidVariable(secondoperand)){
            //     secondoperand = loadPoint(secondoperand);
            // }

            if(token[0] == '+' ){
                string temp = this->getTemp();
                this->p.print(temp +" = add i32 " +firstoperand+ "," +secondoperand);
                var.push(temp);
            }
            if(token[0] == '-' ){
                string temp = this->getTemp();
                this->p.print(temp +" = sub i32 " +firstoperand+ "," +secondoperand);
                var.push(temp);
            }
            if(token[0] == '*' ){
                string temp = this->getTemp();
                this->p.print(temp +" = mul i32 " +firstoperand+ "," +secondoperand);
                var.push(temp);
            }
            if(token[0] == '/' ){
                string temp = this->getTemp();
                this->p.print(temp +" = sdiv i32 " +firstoperand+ "," +secondoperand);
                var.push(temp);
            }

        }else{
            if(isValidVariable(token))
                var.push(loadPoint(token));
            else
                var.push(token);

        }
        
    }
    string result = var.top();
    return result;

}
void Compiler::compilePrint(string s){
    //print(<expr>)
    s = s.substr(6,s.length()-7);
    string temp = this->compileExpression(s);
    this->p.print("call i32 (i8*, ...)* @printf(i8* getelementptr ([4x i8]* @print.str, i32 0, i32 0), i32 " + temp + ")" );
}
bool Compiler::compileWhile(string s){
    if(this->inIf || this->inWhile)
        return false;
    this->inWhile = true;
    string whcond = "whcond" + to_string( this->whiles);
    string whbody = "whbody" + to_string( this->whiles);
    this->p.print("br label %" + whcond);
    this->p.print("");
    this->p.print(whcond + ":");
    this->p.print("");

    s = s.substr(6,s.length()-8);
    string exp = this->compileExpression(s);
    string temp = this->getTemp();
    this->p.print(temp + " = icmp ne i32 " + exp + " , 0");
    this->p.print("");
    this->p.print("br i1 " + temp +", label %" +whbody + ", label %whend" +to_string( this->whiles));
    this->p.print("");
    this->p.print(whbody + ":");
    return true;

}
bool Compiler::compileCurv(string s){
    if(!(this->inIf || this->inWhile))
    return false;

    if(inIf){
    this->p.print("br label %ifend" + to_string( this->ifs));
    this->p.print( "ifend" + to_string( this->ifs) + ":");
    this->ifs ++;
    }
    else{
    this->p.print("br label %whcond" + to_string( this->whiles));
    this->p.print( "whend" + to_string( this->whiles) + ":");
    this->whiles ++;
    }
    return true;
}
void Compiler::finalize(){
    this->p.finalize(this->variableMap);
}
