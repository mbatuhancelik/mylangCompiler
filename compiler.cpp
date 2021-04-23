#include "compiler.h"

using namespace std;

extern regex chooseRegex;
Compiler::Compiler (Printer &p):p{p}{
    this->pointers = 0;
    this->variables = 0;
    this->inWhile = false;
    this->inIf = false;
    this->whiles = 0;
    this->ifs = 0;
    this->chooses = 0;
    this->chooseVariables = 0;
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
    replaceChoose(s);

    s = infixtopostfix(s);

    stack<string> var;

    string token= "";
    // a+%c
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
bool Compiler::compileIf(string s){
    if(this->inIf || this->inWhile)
        return false;
    this->inIf = true;
    string ifcond = "ifcond" + to_string( this->ifs);
    string ifbody = "ifbody" + to_string( this->ifs);
    this->p.print("br label %" + ifcond);
    this->p.print("");
    this->p.print(ifcond + ":");
    this->p.print("");

    s = s.substr(3,s.length()-5);
    string exp = this->compileExpression(s);
    string temp = this->getTemp();
    this->p.print(temp + " = icmp ne i32 " + exp + " , 0");
    this->p.print("");
    this->p.print("br i1 " + temp +", label %" +ifbody + ", label %ifend" +to_string( this->ifs));
    this->p.print("");
    this->p.print(ifbody + ":");
    return true;

}
bool Compiler::compileCurv(string s){
    if(!(this->inIf || this->inWhile))
    return false;

    if(inIf){
    this->p.print("br label %ifend" + to_string( this->ifs));
    this->p.print( "ifend" + to_string( this->ifs) + ":");
    this->ifs ++;
    this->inIf = false;
    }
    else{
    this->p.print("br label %whcond" + to_string( this->whiles));
    this->p.print( "whend" + to_string( this->whiles) + ":");
    this->whiles ++;
    this->inWhile = false;
    }
    return true;
}
void Compiler::finalize(int line, bool syntaxError /*false by default*/){
    if(syntaxError || this->inIf || this->inWhile)
    this->p.syntaxError(line);
    else
    this->p.finalize(this->variableMap);
}

void Compiler::replaceChoose(string &s){

    smatch m;
    int index = 0;

    while (regex_search(s, m, chooseRegex)){

        string xx = m[0].str();

        int parantval = 0;
        for(int i = 0; i < xx.size(); i++){

        	if(xx[i] == '(') parantval++;
        	else if(xx[i] == ')') parantval--;

        }
        int index = s.find(xx);
        while(parantval > 0){
            int len = xx.length();
        	string temp = s.substr(index+len);

            int t = temp.find_first_of(")");

            xx += temp.substr(0,t+1);
            parantval -- ;

        }
        // find xx
        string comp = compileChoose(xx);

        findAndReplace(s , xx , comp);

    }
}
//compiles chooseses but does not compile nested chooses
string Compiler::compileChoose(string s){
    s = s.substr(7); // 7 is index of first (
    s=s.substr(0,s.length() -1);
    
    int comma = s.find_first_of(",");
    string expr1 = s.substr(0,comma);
    s= s.substr(comma+1);
    comma = s.find_first_of(",");
    string expr2 = s.substr(0,comma);
    s= s.substr(comma+1);
    comma = s.find_first_of(",");
    string expr3 = s.substr(0,comma);

    string expr4 = s.substr(comma+1);

    expr1 = compileExpression(expr1);
    string g = "%cb" + to_string(this->chooseVariables++);
    string l = "%cb" + to_string(this->chooseVariables++);
    string result = "c" + to_string(this->chooses);
    string resultPointer = compilePoint(result);
    string glabel = "chooseG" + to_string(this->chooses);
    string lelabel = "chooseLE" + to_string(this->chooses);
    string llabel = "chooseL" + to_string(this->chooses);
    string elabel = "chooseE" + to_string(this->chooses);
    string endlabel = "chooseend" + to_string(this->chooses);
    this->p.print(g +" = icmp sgt i32 " + expr1 + " , 0");
    this->p.print(l + " = icmp slt i32 " + expr1 + " , 0");

   this->p.print("br i1 " + g +", label %" + glabel + ", label %" +lelabel);

    this->p.print(glabel + ":");

    expr3 = compileExpression(expr3);

    this->p.print("store i32 " + expr3 + " ,i32* " + resultPointer);
    this->p.print("\n" + glabel + ":" + "\n");

    this->p.print(lelabel + ":");

    this->p.print("br i1 " + l +", label %" + llabel + ", label %" + elabel);

    this->p.print("\n" + lelabel + ":" + "\n");

    expr4 = compileExpression(expr4);
    this->p.print("store i32 " + expr4 + " ,i32* " + resultPointer);
    this->p.print("\n" + llabel + ":" + "\n");
    
    this->p.print(elabel + ":");

    expr2 = compileExpression(expr2);
    this->p.print("store i32 " + expr2 + " ,i32* " + resultPointer);
    this->p.print("\n" + elabel + ":" + "\n");

    this->p.print(endlabel + ":");
    this->chooses ++;
    return result;
}