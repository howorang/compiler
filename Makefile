compiler: main.o parser.tab.cpp parser.tab.hpp lexer.cpp SymbolTable.o
	g++ -Wall -Wextra -Werror -c main.cpp -o main.o SymbolTable.o
main.o: main.cpp

parser.tab.cpp parser.tab.hpp: parser.y SymbolTable.cpp SymbolTable.h
	bison -d parser.y

lexer.cpp: lexer.lex parser.tab.hpp SymbolTable.cpp SymbolTable.h
	flex lexer.lex

SymbolTable.o: SymbolTable.cpp SymbolTable.h
