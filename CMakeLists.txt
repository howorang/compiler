cmake_minimum_required(VERSION 3.13)

project(compiler)

set(CMAKE_CXX_STANDARD 14)

include(FindBISON)
include(FindFLEX)


bison_target(parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp)
flex_target(lexer lexer.lex ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

target_sources(parser PUBLIC SymbolTable.h SymbolTable.cpp)
target_sources(lexer PUBLIC SymbolTable.h SymbolTable.cpp)

add_executable(compiler main.cpp ${BISON_parser_OUTPUTS} ${FLEX_lexer_OUTPUTS} SymbolTable.cpp SymbolTable.h global.h)