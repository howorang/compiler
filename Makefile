compiler: main.o lexer.o parser.o SymbolTable.o utils.o Emitter.o
	g++ -o compiler main.o lexer.o parser.o SymbolTable.o utils.o Emitter.o
main.o: main.cpp parser.hpp global.h
	g++ -c main.cpp global.h global.h
parser.o: parser.c lexer.cpp parser.hpp SymbolTable.h SymbolTable.cpp global.h
	g++ -c parser.cpp lexer.cpp parser.hpp global.h
SymbolTable.o: SymbolTable.cpp SymbolTable.h global.h
	g++ -c SymbolTable.cpp SymbolTable.h global.h
parser.hpp parser.c : parser.y global.h
	bison -o parser.cpp parser.y -d -t
lexer.cpp : lexer.lex global.h
	flex -o lexer.cpp lexer.lex
utils.o: utils.cpp utils.h global.h
	g++ -c utils.h utils.cpp global.h
Emitter.o : Emitter.h Emitter.cpp global.h
	g++ -c Emitter.h Emitter.cpp global.h