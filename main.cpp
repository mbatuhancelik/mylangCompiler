#include "compiler.h"

using namespace std;

int main(int argc, char *argv[]){

    string inputName = argv[1];

	ifstream fileReader(inputName);

    string outputName = inputName.substr(inputName.find_last_of("/"));
    outputName = "./myoutputs" + outputName.substr(0, outputName.length() - 2) + "ll";

	Printer mylang2irPrinter(outputName);
	int lineCounter = 0;
	Compiler mylang2irCompiler(mylang2irPrinter);

	string line;

    while(getline(fileReader,line)){

        int indexofBSr = line.find_first_of("\r");
        if(indexofBSr != -1){
            line = line.substr(0 , indexofBSr) ;
        }

        int indexofComment = line.find_first_of("#");
        if(indexofComment != -1){
            line = line.substr(0 , indexofComment) ;
        }

        if(line.empty()){
        	lineCounter++;
        	continue;
    	}

        string type = getType(line);

        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        line.erase(remove(line.begin(), line.end(), '\t'), line.end());

        if(line.empty()){
        	lineCounter++;
        	continue;
        }

        if(type == "assignment"){

            mylang2irCompiler.compileAssignment(line);

        }else if(type == "if"){

            bool nested = mylang2irCompiler.compileIf(line);
            if(!nested){
                mylang2irCompiler.finalize(lineCounter ,  true);
                return 0;
            }
            
        }else if(type == "print"){

            mylang2irCompiler.compilePrint(line);

        }else if(type == "while"){

            bool nested = mylang2irCompiler.compileWhile(line);  
            if(!nested){
                mylang2irCompiler.finalize(lineCounter, true);
                return 0;
            }

        }else if(type == "curv"){

            bool syntax = mylang2irCompiler.compileCurv(line); 
            if(!syntax){
                mylang2irCompiler.finalize(lineCounter, true);
                return 0;
            }  

        }else if(type == "syntax error"){

            mylang2irCompiler.finalize(lineCounter, true);
            return 0; 
            
        }

        lineCounter ++;
        
    }

    mylang2irCompiler.finalize(lineCounter-1);

    return 0;
}
