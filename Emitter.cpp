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
    if (operation != INTOREAL && operation != REALTOINT) {
        arg1index = promoteIfNeeded(arg1index, arg2index);
        arg2index = promoteIfNeeded(arg2index, arg1index);
    }
    SymbolTable::SymbolEntry &result = symbolTable[resAddrIndex];
    std::string opCode = getOpCode(operation, determineOpType(arg1index, arg2index));
    out += opCode + " " + writeSymbol(arg1index) + ", " + writeSymbol(arg2index) + ", " + writeSymbol(resAddrIndex);
    out += "\n";
}

void Emitter::genCode(OP operation, int arg1index, int arg2index) {
    if (operation != INTOREAL && operation != REALTOINT) {
        arg1index = promoteIfNeeded(arg1index, arg2index);
        arg2index = promoteIfNeeded(arg2index, arg1index);
    }
    std::string opCode = getOpCode(operation, determineOpType(arg1index, arg2index));
    out += opCode + " " + writeSymbol(arg1index) + ", " + writeSymbol(arg2index);
    out += "\n";
}

void Emitter::genCode(OP operation, int arg1index) {
    std::string opCode = getOpCode(operation, symbolTable[arg1index].varType);
    out += opCode + " " + writeSymbol(arg1index);
    out += "\n";
}

void Emitter::genCode(OP operation) {
    std::string opCode = getOpCode(operation, NULL);
    out += opCode;
    out += "\n";
}

int Emitter::promoteIfNeeded(int arg1Index, int arg2Index) {
    SymbolTable::SymbolEntry &arg1 = symbolTable[arg1Index];
    SymbolTable::SymbolEntry &arg2 = symbolTable[arg2Index];
    if (arg1.varType != REAL && arg2.varType == REAL) {
        return convert(arg1Index, REAL);
    }
    return arg1Index;

}

int Emitter::convert(int arg1index, int toType) {
    SymbolTable::SymbolEntry &arg1 = symbolTable[arg1index];
    if (arg1.varType != toType) {
        OP conversionOperation;
        int tempIndex;
        switch (toType) {
            case REAL:
                conversionOperation = INTOREAL;
                tempIndex = symbolTable.insertTempVar(REAL);
                break;
            case INTEGER:
                conversionOperation = REALTOINT;
                tempIndex = symbolTable.insertTempVar(INTEGER);
                break;
        }
        genCode(conversionOperation, arg1index, tempIndex);
        return tempIndex;
    }
    return arg1index;
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
        case REALTOINT:
            prefix = "realtoint";
            break;
        case INTOREAL:
            prefix = "inttoreal";
            break;
        case MOV:
            prefix = "mov";
            break;
        case WRITE:
            prefix = "write";
            break;
        case LEAVE:
            prefix = "leave";
            break;
        case RETURN:
            prefix = "return";
            break;
        default:
            throw "Unknown operation";
    }
    switch (type) {
        case INTEGER:
            postfix = ".i";
            break;
        case REAL:
            postfix = ".r";
            break;
        default:
            break;
    }
    return prefix + postfix;
}

std::string Emitter::writeSymbol(int symbolIndex) {
    SymbolTable::SymbolEntry &entry = symbolTable[symbolIndex];
    return std::string(entry.isRef ? "*" : "") + (entry.isLocal ? "BP + " : "") + std::to_string(entry.place);
}

