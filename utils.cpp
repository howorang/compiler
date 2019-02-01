//
// Created by howorang on 01.02.19.
//

#include "utils.h"


enum MULOP decodeMulOp(std::string mulOpStr) {
    if (mulOpStr == "*") { return MUL;}
    else if (mulOpStr == "/") { return DIV;}
    else if (mulOpStr == "div") { return DIV;}
    else if (mulOpStr == "mod") { return MOD;}
    else if (mulOpStr == "and") { return AND;}
    throw decode_exception();
}

enum RELOP decodeRelOp(std::string relOpStr) {
    if (relOpStr == "=") { return EQ;}
    else if (relOpStr == "<>") { return NEQ;}
    else if (relOpStr == "<") { return LT;}
    else if (relOpStr == "<=") { return LTE;}
    else if (relOpStr == ">") { return GT;}
    else if (relOpStr == ">=") { return GTE;}
    throw decode_exception();
}

class UnknownTypeException{};

int sizeOfSymbol(int symbolType) {
    switch(symbolType) {
        case REAL:
            return 8;
        case INTEGER :
            return 4;
        default:
            throw UnknownTypeException();
    }
}

