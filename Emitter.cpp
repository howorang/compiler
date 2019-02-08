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
    std::string opCode = getOpCode(operation, determineOpType(arg1index, vm1, arg2index, vm2));
    out += opCode + " " + writeSymbol(arg1index, vm1) + ", " + writeSymbol(arg2index, vm2) + ", " +
           writeSymbol(resAddrIndex, vm3);
    out += "\n";
}

void Emitter::genCode(OP operation, int arg1index, VARMODE vm1, int arg2index, VARMODE vm2) {
    if (operation != INTOREAL && operation != REALTOINT) {
        arg1index = promoteIfNeeded(arg1index, arg2index);
        arg2index = promoteIfNeeded(arg2index, arg1index);
    }
    std::string opCode = getOpCode(operation, determineOpType(arg1index, vm1, arg2index, vm2));
    out += opCode + " " + writeSymbol(arg1index, vm1) + ", " + writeSymbol(arg2index, vm2);
    out += "\n";
}

void Emitter::genCode(OP operation, int arg1index, VARMODE vm1) {
    if (operation == INCSP) {
        out += "incsp.i #" + std::to_string(symbolTable[arg1index].incsp) + "\n";
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

int Emitter::determineOpType(int arg1index, VARMODE vm1, int arg2index, VARMODE vm2) {
    SymbolTable::SymbolEntry &arg1 = symbolTable[arg1index];
    SymbolTable::SymbolEntry &arg2 = symbolTable[arg2index];
    if ((arg1.varType == INTEGER || vm1 == directi) && (arg2.varType == INTEGER || vm2 == directi)) {
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
            prefix = "and";
            break;
        case EQ:
            prefix = "je";
            break;
        case NEQ:
            break;
        case LT:
            prefix = "jl";
            break;
        case LTE:
            prefix = "jle";
            break;
        case GT:
            prefix = "jg";
            break;
        case GTE:
            prefix = "jge";
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
            prefix = "push.i";
            hasPostfix = false;
            break;
        case CALL:
            prefix = "call.i";
            hasPostfix = false;
            break;
        case ENTER:
            prefix = "enter.i";
            hasPostfix = false;
            break;
        case INCSP:
            prefix = "incsp.i";
            hasPostfix = false;
            break;
        case JUMP:
            prefix = "jump.i";
            hasPostfix = false;
            break;
        case OR_OP:
            prefix = "or";
            break;
        case READ:
            prefix = "read";
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
    if (vm == label) {
        return "#" + entry.label;
    }

    if(entry.isLiteral) {
        return "#" + entry.tokenVal;
    }

    if (vm == directi || vm == directr || vm == direct) {
        return "#" + std::to_string(symbolIndex);
    }

    if (!entry.isLocal) {
        switch (vm) {
            case value:
                return std::to_string(entry.place);
            case address:
                return "#" + std::to_string(entry.place);
        }
    } else {
        std::string buffer = "BP";
        buffer += (entry.place > 0 ? "+" : "") + std::to_string(entry.place);
        switch (vm) {
            case value:
                if (entry.isRef) {
                    return "*" + buffer;
                } else {
                    return buffer;
                }
            case address:
                if (entry.isRef) {
                    return buffer;
                } else {
                    return "#" + buffer;
                }
        }
    }
}

int Emitter::emmitFunc(int funcIndex, std::vector<int> expressionListHolder) {
    funcIndex = symbolTable.getFunc(funcIndex); // funcs and procs are global this can be local
    int funcResAddr = -1;
    std::vector<int> &paramListTypeSignature = symbolTable[funcIndex].paramListTypeSignature;
    for (int i = 0; i < expressionListHolder.size(); i++) {
        int symbolIndex = expressionListHolder[i];
        int symbolToEmit = symbolIndex;
        if (symbolTable[symbolIndex].isLiteral) {
            int tempVarIndex = symbolTable.insertTempVar(symbolTable[symbolIndex].varType);
            emitter.genCode(MOV, symbolIndex, value, tempVarIndex, value);
            symbolToEmit = tempVarIndex;
        }
        if (!symbolTable[symbolIndex].isArray && paramListTypeSignature[i] != symbolTable[symbolToEmit].varType) {
            symbolToEmit = convert(symbolToEmit, paramListTypeSignature[i]);
        }
        emitter.genCode(PUSH, symbolToEmit, address);
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

int Emitter::emmitArray(int arrayIndex, int subscriptVarIndex) {
    SymbolTable::SymbolEntry &arrayEntry = symbolTable[arrayIndex];
    SymbolTable::SymbolEntry &subscriptEntry = symbolTable[subscriptVarIndex];

    int rangeTemp = symbolTable.insertTempVar(INTEGER);
    out += "sub.i " + writeSymbol(subscriptVarIndex, value) + ", " + writeSymbol(arrayEntry.low, directi) + ", " +
           writeSymbol(rangeTemp, value) + "\n";
    out += "mul.i " + writeSymbol(rangeTemp, value) + ", " +
           writeSymbol(sizeOfSymbol(arrayEntry.varType), directi) +
           ", " +
           writeSymbol(rangeTemp, value) + "\n";
    int arrayRef = symbolTable.insertTempVar(INTEGER);
    genCode(PLUS, arrayIndex, address, rangeTemp, value, arrayRef, value);
    return arrayRef;


}

void Emitter::emmitLabel(int index) {
    simpleEmit(symbolTable[index].label + ":");
}

