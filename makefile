mylang : main.o  tokens.o printer.o util.o infixtopostfix.o compiler.o
		g++ main.o tokens.o printer.o util.o infixtopostfix.o  compiler.o -o mylang -std=c++14


main.o: compiler.h 
		g++ -c main.cpp -o main.o -std=c++14


compiler.o: compiler.cpp compiler.h tokens.h printer.h utilities.h 
		g++ -c compiler.cpp -o compiler.o -std=c++14


printer.o: printer.cpp printer.h  
		g++ -c printer.cpp -o printer.o -std=c++14


tokens.o: tokens.cpp tokens.h utilities.h
		g++ -c tokens.cpp -o tokens.o -std=c++14


infixtopostfix.o: infixtopostfix.cpp infixtopostfix.h
	g++ -c infixtopostfix.cpp -o infixtopostfix.o -std=c++14


util.o: utilities.h utilities.cpp 
	g++ -c utilities.cpp -o util.o -std=c++14