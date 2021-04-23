#include "compiler.h"

using namespace std;

int main()
{
    
    //TODO remove comments
    //TODO print syntax error
    
    string reading = "trial.my";

    ifstream read(reading);
    Printer p("output.ll");
    Compiler c(p);
    while(getline(read,reading)){
        
        string type = getType(reading);
        reading.erase(remove(reading.begin(), reading.end(), ' '), reading.end());
        reading.erase(remove(reading.begin(), reading.end(), '\t'), reading.end());
        if(type == "assignment"){
            c.compileAssignment(reading);
        }
        if(type == "if"){
            c.compileIf(reading);
        }
        if(type == "print"){
            c.compilePrint(reading);
        }
        if(type =="while"){
            bool syntax = c.compileWhile(reading);  
            // if(!syntax) { //syntax error }
        }
        if(type =="curv"){
            c.compileCurv(reading);   
        }
    }

    c.finalize();

    return 0;
}
