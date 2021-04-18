#include <fstream>
#include <string>
#include <unordered_map>
#if !defined(printer)
#define printer
using namespace std;
class Printer{
    private:
    ofstream output;
    string filename;
    public:
    
    Printer(string filename);
    void print(string s);
    void syntaxError(int line);
    void finalize(unordered_map<string , string>& pointers);
    ~Printer();
};



#endif // printer
