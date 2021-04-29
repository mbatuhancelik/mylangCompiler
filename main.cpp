#include "compiler.h"

using namespace std;


//beginning of the program
int main(int argc, char *argv[]){

    string inputName = argv[1]; //name of the input file

	ifstream fileReader(inputName); // reading the input file

    //name of the output file
    string outputName = inputName.substr(0, inputName.length() - 2) + "ll";

    //printer object, an interface to output file
	Printer mylang2irPrinter(outputName);
	//counter for line numbers
    int lineCounter = 0;

    //compiler to generate llvm code
	Compiler mylang2irCompiler(mylang2irPrinter);
    //string to store mylang statements
	string line;

    while(getline(fileReader,line)){ //while there are statements to compile

        //if exists, remove the \r at the end of the line
        int indexofBSr = line.find_first_of("\r"); 
        if(indexofBSr != -1){
            line = line.substr(0 , indexofBSr) ;
        }
        //if exists, remove comments from the line
        int indexofComment = line.find_first_of("#");
        if(indexofComment != -1){
            line = line.substr(0 , indexofComment) ;
        }
        //TODO: make sure this is useful
        // //if line is empty now, continue,
        // if(line.empty()){
        // 	lineCounter++;
        // 	continue;
    	// }
        //using BNF notation, get the type of the line
        //type = "syntax error" in case of an syntax error
        string type = getType(line);

        //remove empty spaces from line before compilation
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        line.erase(remove(line.begin(), line.end(), '\t'), line.end());

        //if line is empty now, continue,
        if(line.empty()){
        	lineCounter++;
        	continue;
        }

        //if line is an assignment statement
        if(type == "assignment"){
            //use compiler to compile the line
            mylang2irCompiler.compileAssignment(line);

        }
        //if line is an if statement
        else if(type == "if"){
            
            //boolean that stores if there are nested ifs or whiles
            bool nested = mylang2irCompiler.compileIf(line);
            //if there are nested ifs or whiles, print out a syntax error and terminate
            if(!nested){
                mylang2irCompiler.finalize(lineCounter ,  true);
                return 0;
            }

        }
        //if line is an print statement
        else if(type == "print"){
            //use compiler to compile the statement
            mylang2irCompiler.compilePrint(line);

        }
        //if line is an while statement
        else if(type == "while"){

            //boolean that stores if there are nested ifs or whiles
            bool nested = mylang2irCompiler.compileWhile(line);
            //if there are nested ifs or whiles, print out a syntax error and terminate
            if(!nested){
                mylang2irCompiler.finalize(lineCounter, true);
                return 0;
            }

        }
        //if line is an curvy braces close
        else if(type == "curv"){
            //boolean that stores if curvy braces are matched
            bool syntax = mylang2irCompiler.compileCurv(line); 
            //if they are not matched, print a syntax error and terminate
            if(!syntax){
                mylang2irCompiler.finalize(lineCounter, true);
                return 0;
            }  

        }
        //if line is a syntax error, print if out and terminate
        else if(type == "syntax error"){

            mylang2irCompiler.finalize(lineCounter, true);
            return 0; 
            
        }
        //increment line counter after each line
        lineCounter ++;
        
    }

    // finalize LLVM code without a syntax error
    mylang2irCompiler.finalize(lineCounter-1);

    return 0;
}
