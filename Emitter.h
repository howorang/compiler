//
// Created by howorang on 01.02.19.
//

#ifndef COMPILER_EMITTER_H
#define COMPILER_EMITTER_H

#include <string>
#include "global.h"
#include <fstream>
#include <iostream>

class SymbolTable;

class Emitter {
public:
    void simpleEmit(std::string text);

    void saveToFile(std::string fileName);

    void genCode(OP operation, int arg1, int arg2, int resAddr); // arg1 and arg2 should be symbolTable indexes
    void genCode(OP operation, int arg1, int arg2); // arg1 and arg2 should be symbolTable indexes
    int determineOpType(int arg1, int arg2);

private:
    std::string out;

    std::string getOpCode(OP op, int type);

    int convert(int arg1index, int toType);

    int promoteIfNeeded(int argIndex, int resIndex);
};

#endif //COMPILER_EMITTER_H
