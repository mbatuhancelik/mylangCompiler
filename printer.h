#include <fstream>
#include <string>
#include <unordered_map>

#ifndef PRINTER_H
#define PRINTER_H

using namespace std;

//Printing utility to engage with output file
class Printer{

    private:
		//output file
    	ofstream output;
		//output file name
    	string filename;

    public:
		//Creates the output file and prints out the beginning of llvm code
    	Printer(string filename);
		//print a single statement 
    	void print(string s);
		//generates llvm code that prints out syntax error 
    	void syntaxError(int line);
		//Finalize the output file via allocating and initializing pointers under alloc label
    	void finalize(unordered_map<string , string>& pointers);
		//close the files befode destruction of the object
    	~Printer();

};

#endif // PRINTER_H