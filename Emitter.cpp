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

void
Emitter::genCode(OP operation, int arg1index, VARMODE vm1, int arg2index, VARMODE vm2, int resAddrIndex, VARMODE vm3) {
    if (operation != INTOREAL && operation != REALTOINT) {
        arg1index = promoteIfNeeded(arg1index, arg2index);
        arg2index = promoteIfNeeded(arg2index, arg1index);
    }
    SymbolTable::SymbolEntry &result = symbolTable[resAddrIndex];
    std::string opCode = getOpCode(operation, determineOpType(arg1index, arg2index));
    out += opCode + " " + writeSymbol(arg1index, vm1) + ", " + writeSymbol(arg2index, vm2) + ", " +
           writeSymbol(resAddrIndex, vm3);
    out += "\n";
}

void Emitter::genCode(OP operation, int arg1index, VARMODE vm1, int arg2index, VARMODE vm2) {
    if (operation != INTOREAL && operation != REALTOINT) {
        arg1index = promoteIfNeeded(arg1index, arg2index);
        arg2index = promoteIfNeeded(arg2index, arg1index);
    }
    std::string opCode = getOpCode(operation, determineOpType(arg1index, arg2index));
    out += opCode + " " + writeSymbol(arg1index, vm1) + ", " + writeSymbol(arg2index, vm2);
    out += "\n";
}

void Emitter::genCode(OP operation, int arg1index, VARMODE vm1) {
    if (operation == INCSP) {
        out += "incsp.i " + std::to_string(symbolTable[arg1index].incsp) + "\n";
        return;
    } else {
        std::string opCode = getOpCode(operation, symbolTable[arg1index].varType);
        out += opCode + " " + writeSymbol(arg1index, vm1);
        out += "\n";
    }
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
        genCode(conversionOperation, arg1index, value, tempIndex, value);
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
    std::string postfix = ".i";
    bool hasPostfix = true;
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
            hasPostfix = false;
            break;
        case RETURN:
            prefix = "return";
            hasPostfix = false;
            break;
        case PUSH:
            prefix = "push";
            break;
        case CALL:
            prefix = "call";
            break;
        case ENTER:
            prefix = "enter";
            break;
        case INCSP:
            prefix = "incsp";
            break;
        default:
            throw "Unknown operation";
    }
    if (type == REAL) {
        postfix = ".r";
    }
    return hasPostfix ? prefix + postfix : prefix;
}

std::string Emitter::writeSymbol(int symbolIndex, VARMODE vm) {
    SymbolTable::SymbolEntry &entry = symbolTable[symbolIndex];
    if (vm == label || symbolTable[symbolIndex].isLiteral) {
        return "#" + entry.tokenVal;
    }
    std::string toEmit;
    if (entry.isRef && vm == value) {
        toEmit += "*";
    }
    if (entry.isLocal) {
        toEmit += "BP";
        toEmit += entry.place > 0 ? "+" : "";
    }
    if (vm == address) {
        toEmit += "#";
    }
    toEmit += std::to_string(entry.place);
    return toEmit;
}

int Emitter::emmitFunc(int funcIndex, std::vector<int> expressionListHolder) {
    int funcResAddr = -1;
    for (int symbolIndex : expressionListHolder) {
        emitter.genCode(PUSH, symbolIndex, address);
    }
    if (!symbolTable[funcIndex].isProcedure) {
        funcResAddr = symbolTable.insertTempVar(symbolTable[funcIndex].varType);
        emitter.genCode(PUSH, funcResAddr, address);
    }
    emitter.genCode(CALL, funcIndex, label);
    emitter.genCode(INCSP, funcIndex, address);
    return funcResAddr;
}

void Emitter::exitSubProgramDecl() {
    stringReplace(out, "{allocSize}", std::to_string(symbolTable.getAllocatedMem()));
    emitter.genCode(LEAVE);
    emitter.genCode(RETURN);
}

