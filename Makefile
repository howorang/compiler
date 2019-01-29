compiler: SymbolTable.o lexer.o parser.o
	g++ -Wall -Wextra -Werror -o compiler lexer.o parser.o SymbolTable.o main.o
main.o: main.cpp
	g++ -c main.cpp

parser.c parser.h: parser.y SymbolTable.cpp SymbolTable.h
	bison -o parser.c -d parser.y

lexer.c: lexer.lex parser.h SymbolTable.cpp SymbolTable.h
	flex -o lexer.c lexer.lex

SymbolTable.o: SymbolTable.cpp SymbolTable.h
	g++ -c SymbolTable.h SymbolTable.cpp

lexer.o: lexer.c parser.h parser.c SymbolTable.h SymbolTable.cpp
	g++ -c lexer.c

parser.o: parser.h parser.c SymbolTable.h SymbolTable.cpp
	g++ -c parser.h parser.c