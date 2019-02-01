//
// Created by howorang on 01.02.19.
//

#include "utils.h"


enum MULOP decodeMulOp(std::string mulOpStr) {
    if (mulOpStr.compare("*") == 0) { return MUL;}
    else if (mulOpStr.compare("/") == 0) { return DIV;}
    else if (mulOpStr.compare("div") == 0) { return DIV;}
    else if (mulOpStr.compare("mod") == 0) { return MOD;}
    else if (mulOpStr.compare("and") == 0) { return AND;}
    throw decode_exception();
}

enum RELOP decodeRelOp(std::string relOpStr) {
    if (relOpStr.compare("=") == 0) { return EQ;}
    else if (relOpStr.compare("<>") == 0) { return NEQ;}
    else if (relOpStr.compare("<") == 0) { return LT;}
    else if (relOpStr.compare("<=") == 0) { return LTE;}
    else if (relOpStr.compare(">") == 0) { return GT;}
    else if (relOpStr.compare(">=") == 0) { return GTE;}
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

