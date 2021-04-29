#include "printer.h"

using namespace std;

//Creates the output file and prints out the beginning of llvm code
Printer::Printer(string filename){
    //create the output file
    this->filename = filename;
    this->output.open(filename);

    //print out the beginning of the llvm code
    this->output << "; ModuleID = 'mylang2ir'\ndeclare i32 @printf(i8*, ...)\n@print.str = constant [4 x i8] c\"%d\\0A\\00\"";
    this->output << "\n\ndefine i32 @main() {\n\nbr label %alloc \n\ncontinue:\n\n";

}

//print a single statement 
void Printer::print(string s){

    this->output << s << endl;

}
//Finalize the output file via allocating and initializing pointers under alloc label
void Printer::finalize(unordered_map<string, string> &pointers){

    //print return statement and alloc 
    print("\nret i32 0\n\nalloc:\n");

    for (auto pointer : pointers){
        //allocate and store each pointer
        string second = pointer.second;
        print(second + " = alloca i32");
        print("store i32 0, i32*" + second);

    }
    //after allocation is done, go to continue label
    print("br label %continue \n\n}");
    
}

//generates llvm code that prints out syntax error 
void Printer::syntaxError(int line){

    //flush the output file
    this->output.close();
    this->output.open(filename);

    //generate llvm code
    print("; ModuleID = 'mylang2ir'\ndeclare i32 @printf(i8*, ...)\n@print.str = constant [23 x i8] c\"Line %d: syntax error\\0A\\00\"");
    print("\ndefine i32 @main() {\n");

    print("call i32 (i8*, ...)* @printf(i8* getelementptr ([23x i8]* @print.str, i32 0, i32 0), i32 " + to_string(line) + ")");

    print("\nret i32 0\n\n}");
}
//close the files befode destruction of the object
Printer::~Printer(){

    if (this->output.is_open())
        this->output.close();
    
}
