compiler: main.o lexer.o parser.o SymbolTable.o utils.o Emitter.o
	g++ -o compiler main.o lexer.o parser.o SymbolTable.o utils.o Emitter.o
main.o: main.cpp parser.hpp
	g++ -c main.cpp global.h
parser.o: parser.c lexer.cpp parser.hpp SymbolTable.h
	g++ -c parser.cpp lexer.cpp parser.hpp SymbolTable.h
SymbolTable.o: SymbolTable.cpp SymbolTable.h global.h
	g++ -c SymbolTable.cpp SymbolTable.h
parser.hpp parser.c : parser.y
	bison -o parser.cpp parser.y -d -t
lexer.cpp : lexer.lex
	flex -o lexer.cpp lexer.lex
utils.o: utils.cpp utils.h
	g++ -c utils.h utils.cpp
Emitter.o : Emitter.h Emitter.cpp
	g++ -c Emitter.h Emitter.cpp