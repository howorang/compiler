//
// Created by howorang on 01.02.19.
//

#include "utils.h"

enum OP decodeMulOp(std::string opStr) {
    switch (hash(opStr.c_str())) {
        case hash("*"):
            return MUL;
        case hash("/"):
            return DIV;
        case hash("div"):
            return DIV;
        case hash("mod"):
            return MOD;
        case hash("and"):
            return AND;
        default:
            throw decode_exception();
    }
}
enum OP decodeRelOp(std::string opStr) {
    switch (hash(opStr.c_str())) {
        case hash("="):
            return EQ;
        case hash("<>"):
            return NEQ;
        case hash("<"):
            return LT;
        case hash("<="):
            return LTE;
        case hash(">"):
            return GT;
        case hash(">="):
            return GTE;
        default:
            throw decode_exception();
    }
}
enum OP decodeSignOp(std::string opStr) {
    switch (hash(opStr.c_str())) {
        case hash("+"):
            return PLUS;
        case hash("-"):
            return MINUS;
        default:
            throw decode_exception();
    }
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

bool stringReplace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

