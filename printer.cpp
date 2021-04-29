#include "printer.h"

using namespace std;

Printer::Printer(string filename){

    this->filename = filename;
    this->output.open(filename);
    this->output << "; ModuleID = 'mylang2ir'\ndeclare i32 @printf(i8*, ...)\n@print.str = constant [4 x i8] c\"%d\\0A\\00\"";
    this->output << "\n\ndefine i32 @main() {\n\nbr label %alloc \n\ncontinue:\n\n";

}

void Printer::print(string s){

    this->output << s << endl;

}

void Printer::finalize(unordered_map<string, string> &pointers){

    print("\nret i32 0\n\nalloc:\n");

    for (auto pointer : pointers){

        string second = pointer.second;
        print(second + " = alloca i32");
        print("store i32 0, i32*" + second);

    }

    print("br label %continue \n\n}");
    
}

void Printer::syntaxError(int line){

    this->output.close();
    this->output.open(filename);
    print("; ModuleID = 'mylang2ir'\ndeclare i32 @printf(i8*, ...)\n@print.str = constant [23 x i8] c\"Line %d: syntax error\\0A\\00\"");
    print("\ndefine i32 @main() {\n");

    print("call i32 (i8*, ...)* @printf(i8* getelementptr ([23x i8]* @print.str, i32 0, i32 0), i32 " + to_string(line) + ")");

    print("\nret i32 0\n\n}");
}

Printer::~Printer(){

    if (this->output.is_open())
        this->output.close();
    
}
