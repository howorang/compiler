compiler: main.o lexer.o parser.o SymbolTable.o utils.o Emitter.o
	g++ -o compiler main.o lexer.o parser.o SymbolTable.o utils.o Emitter.o
main.o: main.cpp global.h parser.hpp SymbolTable.h utils.h consts.h Emitter.h
	g++ -c main.cpp global.h global.h
parser.o: parser.cpp global.h parser.hpp SymbolTable.h utils.h consts.h Emitter.h
	g++ -c parser.cpp parser.hpp global.h
SymbolTable.o: SymbolTable.cpp SymbolTable.h global.h parser.hpp utils.h consts.h Emitter.h
	g++ -c SymbolTable.cpp SymbolTable.h global.h
parser.hpp parser.c : parser.y global.h
	bison -o parser.cpp parser.y -d -t
lexer.cpp : lexer.lex global.h
	flex -o lexer.cpp lexer.lex
utils.o: utils.cpp utils.h global.h
	g++ -c utils.h utils.cpp global.h
Emitter.o : Emitter.h global.h parser.hpp SymbolTable.h utils.h consts.h Emitter.h
	g++ -c Emitter.h Emitter.cpp global.h
lexer.o :  lexer.cpp global.h parser.hpp SymbolTable.h utils.h consts.h Emitter.h
	g++ -c lexer.cpp global.h