#include "compiler.h"

using namespace std;
regex choosePointerRegex("choose_pointer[0-9]+");
regex chooseRegexforCompiler("choose\\s{0,}\\((((?!choose\\()[a-zA-Z0-9+\\*\\-\\/\\s()_])*,){3,3}((?!choose\\()[_a-zA-Z0-9+\\*\\-\\/\\s()])*?\\)");; //regex that specifies choose functions
//initializes variableCount
Compiler::Compiler (Printer &p):p{p}{

    this->pointerCount = 0;
    this->variableCount = 0;
    this->inWhile = false;
    this->inIf = false;
    this->whileCount = 0;
    this->ifCount = 0;
    this->chooseCount = 0;
    this->choosePointerCount = 0;

}
//returns a unique temporary variable
string Compiler::getTemp(){

    string temp = "%t" + to_string(this->variableCount);
    this->variableCount++ ;
    return temp;
    
}
//takes a mylang variable and returns the corresponding llvm pointer
string Compiler::compilePoint(string mylangvar){
    // if mylangvar is not a key in pointerMap
    // creates a pointer and matches them in variable map
    if(this->pointerMap.find(mylangvar) == this->pointerMap.end()){
        string pointer = "%p" + to_string(pointerCount);
        this->pointerMap[mylangvar] = pointer;
        this->pointerCount += 1;
    }
    // returns the llvm pointer corresponding to the mylangvar
    return pointerMap[mylangvar];
}
// takes a mylang var and loads its pointer in llvm code
string Compiler::loadPoint(string mylangvar){

    mylangvar = compilePoint(mylangvar);
    string temp = this->getTemp();
    this->p.print(temp + " = load i32* " + mylangvar);

    return temp;
}
//compiles assignment statements
// <assignstm>-> <var> = <expression>
void Compiler::compileAssignment(string assignstm){

    string var = assignstm.substr(0,assignstm.find_first_of("=")); // variable part in BNF
    string exp = assignstm.substr(assignstm.find_first_of("=") + 1); // expression part in BNF
    
    exp = compileExpression(exp); // temporary variable which holds result of the expression
    var = compilePoint(var); // llvm pointer corresponds to mylang var

    this->p.print("store i32 "+ exp+", i32* " + var); // store the rsult of the expression in the pointer
}
//compiles expressions
string Compiler::compileExpression(string exp){
    // compiles choose functions within the expression
    // replaces them with temp variableCount that hold results of choose functions
    replaceChoose(exp); 

    exp = infixtopostfix(exp);

    stack<string> var;

    string token;
    // a+%c
    stringstream tokenizer(exp);
    // compiles postfix expressions
    // after each step stores the result in a llvm variable and pushes it to var stack
    while(tokenizer >> token){

        if(token[0] == '+' || token[0] == '-'|| token[0] == '*' || token[0] == '/'){

            string firstoperand;
            string secondoperand;

            secondoperand = var.top();
            var.pop();

            firstoperand = var.top();
            var.pop();

            if(token[0] == '+' ){
                // get a temp variable to store the result
                string temp = this->getTemp(); 
                // perform addition in llvm
                this->p.print(temp +" = add i32 " + firstoperand + "," + secondoperand);
                //push the temp var to stack for further operations
                var.push(temp);
            }else if(token[0] == '-' ){
                // get a temp variable to store the result
                string temp = this->getTemp();
                // perform subtraction in llvm
                this->p.print(temp +" = sub i32 " + firstoperand + "," + secondoperand);
                //push the temp var to stack for further operations
                var.push(temp);
            }else if(token[0] == '*' ){
                // get a temp variable to store the result
                string temp = this->getTemp();
                // perform multiplication in llvm
                this->p.print(temp + " = mul i32 " + firstoperand + "," + secondoperand);
                //push the temp var to stack for further operations
                var.push(temp);
            }else if(token[0] == '/' ){
                // get a temp variable to store the result
                string temp = this->getTemp();
                // perform division in llvm
                this->p.print(temp + " = sdiv i32 " +firstoperand+ "," +secondoperand);
                //push the temp var to stack for further operations
                var.push(temp);
            }

        }else{
            // if token is a valid variable, its a mylang variable
            //loads its pointer to a temp var and stores it in stack
            // otherwise token is an temporary llvm variable
            if(isValidVariable(token)|| regex_match(token , choosePointerRegex))
                var.push(loadPoint(token));
            else
                var.push(token);

        }
        
    }
    // result of the expression is stored in the last temp llvm variable in the stack
    string result = var.top();
    return result;
}
//compiles print statements
// <printstm>-> print(<expr>)
void Compiler::compilePrint(string printstm){
    // expression in the BNF
    string exp = printstm.substr(6, printstm.length() - 7);
    //compiles the expression
    string val = this->compileExpression(exp);
    //prints out the result of the expression
    this->p.print("call i32 (i8*, ...)* @printf(i8* getelementptr ([4x i8]* @print.str, i32 0, i32 0), i32 " + val + ")" );
}
// <whilestm>-> while(<expr>) {
//compiles the statement above
//begins the while body in llvm code
bool Compiler::compileWhile(string whstm){
    // if compiler currently compiles a while or body
    //thus there is nested while or if
    //returns false
    if(this->inIf || this->inWhile)
        return false;

    this->inWhile = true;

    string whcond = "whcond" + to_string( this->whileCount); // while condition label
    string whbody = "whbody" + to_string( this->whileCount); // while body label

    this->p.print("br label %" + whcond + "\n"); // llvm goes to whcond label
    this->p.print(whcond + ":" + "\n"); 

    string exp = whstm.substr(6, whstm.length() - 8); // expression in BNF
    string val = this->compileExpression(exp); // tempvar which stores result of the expression
    string temp = this->getTemp(); // tempvar to store boolean
    this->p.print(temp + " = icmp ne i32 " + val + " , 0" + "\n"); // check if exp equals 0 in llvm
    // if exp is not 0 goes to whbody
    this->p.print("br i1 " + temp + ", label %" + whbody + ", label %whend" + to_string(this->whileCount) + "\n"); 
    this->p.print(whbody + ":" + "\n");
    //if while is compiled successfully returns true 
    return true;
}
// compiles if statemens
bool Compiler::compileIf(string ifstm){
    //if already in a if or while body returns false
    if(this->inIf || this->inWhile)
        return false;

    this->inIf = true;

    string ifcond = "ifcond" + to_string(this->ifCount); // ifcond label
    string ifbody = "ifbody" + to_string(this->ifCount); //ifbody label

    this->p.print("br label %" + ifcond + "\n"); // llvm goes to ifcond label
    this->p.print(ifcond + ":" + "\n");
    string exp = ifstm.substr(3, ifstm.length() - 5); // expression in if statement
    string val = this->compileExpression(exp); // temp var stores result of exp
    string temp = this->getTemp(); // temp var to store boolean
    this->p.print(temp + " = icmp ne i32 " + val + " , 0" + "\n"); // compare exp to 0 in llvm
    // if exp is not 0 goes to ifbody
    this->p.print("br i1 " + temp +", label %" + ifbody + ", label %ifend" +to_string(this->ifCount) + "\n");
    //begins the ifbody
    this->p.print(ifbody + ":" + "\n");

    return true;

}
//compiles } token
bool Compiler::compileCurv(string s){
    // if currently not in while of if body
    //returns false
    if(!(this->inIf || this->inWhile))
        return false;
    //if } ends a ifbody
    if(inIf){

        this->p.print("br label %ifend" + to_string(this->ifCount) + "\n");
        this->p.print( "ifend" + to_string(this->ifCount) + ":" + "\n");
        this->ifCount ++;
        this->inIf = false;

    }
    //if } ends a while body
    else{

        this->p.print("br label %whcond" + to_string(this->whileCount) + "\n");
        this->p.print( "whend" + to_string(this->whileCount) + ":" + "\n");
        this->whileCount ++;
        this->inWhile = false;

    }

    return true;
}
//finalizes compilation by allocating pointerCount 
//and setting their value to 0
void Compiler::finalize(int line, bool syntaxError /*false by default*/){
    //if there is a syntax error or there is a nonfinished if or while body 
    // prints out a syntax error
    if(syntaxError || this->inIf || this->inWhile)
        this->p.syntaxError(line);
    // if everyting is ok finalizes compilation
    else
        this->p.finalize(this->pointerMap);
}
//replaces choose functions in an expression with llvm pointers which 
//stores result of the function
void Compiler::replaceChoose(string &exp){

    smatch matches;

    while (regex_search(exp, matches, chooseRegexforCompiler)){ // while there are choose functions in expression

        string chooseMatch = matches[0].str(); //choose function substring

        int parantval = 0; // difference between number of closed parenthesis and open parenthesis
        
        // calculate the parentval
        for(int i = 0; i < chooseMatch.size(); i++){ 

        	if(chooseMatch[i] == '(')
                parantval++;
        	else if(chooseMatch[i] == ')')
                parantval--;

        }
        
        int indexofMatch = exp.find(chooseMatch);// beginning index of choose function
        
        // if parenthesis are not balanced, balance them by extending chooseMatch
        while(parantval > 0){

            int len = chooseMatch.length();
        	string temp = exp.substr(indexofMatch + len);
            int t = temp.find_first_of(")");
            chooseMatch += temp.substr(0, t + 1);
            parantval -- ;

        }
        // find chooseMatch
        string choosePointer = compileChoose(chooseMatch); // llvm pointer which stores result of the choose function

        findAndReplace(exp , chooseMatch , choosePointer); //replaces the removed choose functions with llvm pointer

    }
}
//compiles a choose function
string Compiler::compileChoose(string s){

    //get the inside of parenthesis
    s = s.substr(7); // 7 is indexofMatch of first (
    s = s.substr(0, s.length() - 1);
    
    //extract expression 1
    int comma = s.find_first_of(",");
    string expr1 = s.substr(0,comma);
    s = s.substr(comma + 1);

    //extract expression 2
    comma = s.find_first_of(",");
    string expr2 = s.substr(0,comma);
    s = s.substr(comma + 1);

    //extract expression 3 and 4
    comma = s.find_first_of(",");
    string expr3 = s.substr(0, comma);
    string expr4 = s.substr(comma + 1);

    string greaterCmp = "%cb" + to_string(this->choosePointerCount++); //llvm boolean to store if expr1 is bigger than 0
    string lessCmp = "%cb" + to_string(this->choosePointerCount++);//llvm boolean to store if expr1 is less than 0
    string result = "choose_pointer" + to_string(this->chooseCount); //pseudo mylang variable that represents this choose function
    string resultPointer = compilePoint(result);// llvm pointer that corresponds to mylang variable above
    string glabel = "chooseG" + to_string(this->chooseCount); //llvm label to go if expr 1 is greater than 0
    string lelabel = "chooseLE" + to_string(this->chooseCount);//llvm label to go if expr 1 is less than or equal to 0
    string llabel = "chooseL" + to_string(this->chooseCount);//llvm label to go if expr 1 is less than to 0
    string elabel = "chooseE" + to_string(this->chooseCount);//llvm label to go if expr 1 equals to 0
    string endlabel = "chooseend" + to_string(this->chooseCount);//llvm label to go when execution ends

    //from now on llvm code is generated 

    expr1 = compileExpression(expr1); // llvm temp variable which stores value of expr 1
    this->p.print(greaterCmp +" = icmp sgt i32 " + expr1 + " , 0"); // if expression is greater than 0
    this->p.print(lessCmp + " = icmp slt i32 " + expr1 + " , 0");// if expression is less than 0
    // if greaterCmp is true goto glabel, else go to lelabel
    this->p.print("br i1 " + greaterCmp +", label %" + glabel + ", label %" +lelabel);
    //beginning of glabel
    this->p.print("\n" + glabel + ":" + "\n");
    // calculate expr3 under glabel
    expr3 = compileExpression(expr3);
    //store value of expr3 in result pointer
    this->p.print("store i32 " + expr3 + " ,i32* " + resultPointer);
    //goto of endlabel
    this->p.print("br label %" + endlabel);
    
    //beginning of lelabel
    this->p.print("\n" + lelabel + ":" + "\n");
    // if expr1 is less than 0 go to label, else go to elabel
    this->p.print("br i1 " + lessCmp +", label %" + llabel + ", label %" + elabel);

    //beginning of llabel
    this->p.print("\n" + llabel + ":" + "\n");
    // calculate expr4 in llvm
    expr4 = compileExpression(expr4);
    //store result of expr4 in result pointer
    this->p.print("store i32 " + expr4 + " ,i32* " + resultPointer);
    //goto endlabel
    this->p.print("br label %" + endlabel);

    //beginning of elabel 
    this->p.print("\n" + elabel + ":" + "\n");
    //calculate expr2 
    expr2 = compileExpression(expr2);
    //store result of expr2 to result pointer
    this->p.print("store i32 " + expr2 + " ,i32* " + resultPointer);
    //goto endlabel
    this->p.print("br label %" + endlabel);
    //endlabel and end of choose function
    this->p.print("\n" + endlabel + ":" + "\n");


    //increment the number of chooses
    this->chooseCount++;
    // return the pseudo mylang variable
    return result;
}
