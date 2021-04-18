#include <fstream>
#include <string>
#include "infixtopostfix.h"
#include "tokens.h"
#include <stack>
#include <sstream>
#include <unordered_map>
#include <fstream>
using namespace std;

int pointers = 0;
int variables = 0;


unordered_map<string, string> variableMap;

string compileVariable(string s){
    if(variableMap.find(s) == variableMap.end()){
        string pointer = "%p" + to_string(pointers);
        variableMap[s] = pointer;
        pointers += 1;
    }

    return variableMap[s];
    
}
bool compileAssignment(string s){

}

string compileExpression(string s){
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

            if(isValidVariable(firstoperand)){
                firstoperand = compileVariable(firstoperand);
            }
            if(isValidVariable(secondoperand)){
                firstoperand = compileVariable(secondoperand);
            }

        }else{


        }
    }



}
