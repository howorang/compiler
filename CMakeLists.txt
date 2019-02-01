cmake_minimum_required(VERSION 3.13)

project(compiler)

set(CMAKE_CXX_STANDARD 14)

FIND_PACKAGE(BISON REQUIRED)
SET(BisonOutput ${CMAKE_SOURCE_DIR}/parser.cpp)
IF(BISON_FOUND)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${BisonOutput}
            COMMAND ${BISON_EXECUTABLE}
            -d
            -t
            --output=${BisonOutput}
            ${CMAKE_SOURCE_DIR}/parser.y
            COMMENT "Generating parser.cpp"
    )
ENDIF()

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput ${CMAKE_SOURCE_DIR}/lexer.cpp)
IF(FLEX_FOUND)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${FlexOutput}
            COMMAND ${FLEX_EXECUTABLE}
            --outfile=${FlexOutput}
            ${CMAKE_SOURCE_DIR}/lexer.lex
            COMMENT "Generating scanner.cpp"
    )
ENDIF()

add_executable(compiler main.cpp SymbolTable.cpp SymbolTable.h global.h ${BisonOutput} ${FlexOutput} utils.h utils.cpp Emitter.h Emitter.cpp)