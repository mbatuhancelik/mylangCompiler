#include "compiler.h"

using namespace std;

int main(int argc, char *argv[])
{
    
    //TODO remove comments
    //TODO print syntax error
    

    ifstream read(argv[1]);
    Printer p(argv[2]);
    int lines = 0;
    Compiler c(p);
    string reading;
    while(getline(read,reading)){
        int r = reading.find_first_of("\r");
        if(r != -1){
            reading = reading.substr(0 , r) ;
        }
        int comment = reading.find_first_of("#");
        if(comment != -1){
            reading = reading.substr(0 , comment) ;
        }
        if(reading.empty()){
        lines++;
        continue;}
        string type = getType(reading);
        reading.erase(remove(reading.begin(), reading.end(), ' '), reading.end());
        reading.erase(remove(reading.begin(), reading.end(), '\t'), reading.end());
        if(reading.empty()){
        lines++;
        continue;}
        if(type == "assignment"){
            c.compileAssignment(reading);
        }
        if(type == "if"){
            bool nested = c.compileIf(reading);
            if(!nested){
                c.finalize(lines ,  true);
                return 0;
            }
            
        }
        if(type == "print"){
            c.compilePrint(reading);
        }
        if(type =="while"){
            bool nested = c.compileWhile(reading);  
            if(!nested) {
                c.finalize(lines , true);
                return 0;
            }
        }
        if(type =="curv"){
             bool syntax = c.compileCurv(reading); 
             if(!syntax) {
                c.finalize(lines , true);
                return 0;
            }  
        }
        if(type == "syntax error"){
            c.finalize(lines ,  true);
            return 0; 
        }
        lines ++;
    }

    c.finalize(lines);

    return 0;
}
