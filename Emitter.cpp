//
// Created by howorang on 01.02.19.
//

#include "Emitter.h"

Emitter emitter = Emitter();

void Emitter::simpleEmit(std::string text) {
    out += text + "\n";
}

void Emitter::saveToFile(std::string fileName) {
    std::ofstream outfile;
    outfile.open(fileName);
    outfile << out;
    outfile.close();
}

void Emitter::genCode(OP operation, int arg1index, int arg2index, int resAddrIndex) {
    SymbolTable::SymbolEntry &arg1 = symbolTable[arg1index];
    SymbolTable::SymbolEntry &arg2 = symbolTable[arg2index];
    SymbolTable::SymbolEntry &result = symbolTable[resAddrIndex];
    std::string opCode = getOpCode(operation, result.varType);

    out += opCode + " " + std::to_string(arg1.place) + " " + std::to_string(arg2.place) + " " +
           std::to_string(result.place);
    out += "\n";
}

int Emitter::determineOpType(int arg1index, int arg2index) {
    SymbolTable::SymbolEntry &arg1 = symbolTable[arg1index];
    SymbolTable::SymbolEntry &arg2 = symbolTable[arg2index];
    if (arg1.varType == INTEGER && arg2.varType == INTEGER) {
        return INTEGER;
    } else {
        return REAL;
    }
}

std::string Emitter::getOpCode(OP op, int type) {
    std::string prefix;
    std::string postfix;
    switch (op) {
        case MUL:
            prefix = "mul";
            break;
        case DIV:
            prefix = "div";
            break;
        case MOD:
            prefix = "mod";
            break;
        case AND:
            break;
        case EQ:
            break;
        case NEQ:
            break;
        case LT:
            break;
        case LTE:
            break;
        case GT:
            break;
        case GTE:
            break;
        case PLUS:
            prefix = "add";
            break;
        case MINUS:
            prefix = "sub";
            break;
    }

    switch (type) {
        case INTEGER:
            postfix = ".i";
            break;
        case REAL:
            postfix = ".r";
            break;
    }
    return prefix + postfix;
}
