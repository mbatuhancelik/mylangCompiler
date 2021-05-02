#include "infixtopostfix.h"

// returns 1 for + or -, returns 2 for * or /, returns 0 for neither
int operatorprecedence(char op){

	if(op == '+' || op == '-'){

		return 1;

	} else if(op == '*' || op == '/'){

		return 2;

	}

	return 0;

}

//Well known infix to postfix algorithm
string infixtopostfix(string expression){
	
	expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());

	stack<int> myStack;
	
	string newexpression = "";
	
	int lastopindex = -1;

	for(int i = 0; i < expression.size(); i++){

		if(expression[i] == '(' || expression[i] == ')' || expression[i] == '+'
			|| expression[i] == '-' || expression[i] == '*' || expression[i] == '/'){

			if(expression[i] != '(' && expression[i] != ')' && newexpression[newexpression.size()-1] != ' '){

				newexpression += " ";

			}

			if(expression[i] == '('){

				myStack.push('(');

			} else if(expression[i] == ')'){

				while(myStack.top() != '('){

					if(newexpression[newexpression.size()-1] != ' '){

						newexpression += " ";

					}

					newexpression.push_back((char)myStack.top());

					myStack.pop();

					newexpression += " ";

				}

				myStack.pop();

			} else {

				if(myStack.empty() || myStack.top() == '('){

					myStack.push(expression[i]);

				}else{

					if(operatorprecedence((char)expression[i]) <= operatorprecedence((char)myStack.top())){

						while(!myStack.empty() && myStack.top() != '('){

							if(newexpression[newexpression.size()-1] != ' '){

								newexpression += " ";

							}

							newexpression.push_back((char)myStack.top());

							myStack.pop();

							newexpression += " ";

						}

						myStack.push(expression[i]);

					}else {

						myStack.push(expression[i]);

					}

				}

			}

		}else{

			newexpression = newexpression + expression[i];

		}

	}

	while(!myStack.empty()){

		if(newexpression[newexpression.size()-1] != ' '){

			newexpression += " ";

		}

		newexpression.push_back((char)myStack.top());

		myStack.pop();

	}

	return newexpression;

}