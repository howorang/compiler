//
// Created by howorang on 26.01.19.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H


#include <string>
#include <vector>
#include "global.h"
#include <algorithm>

class SymbolTable {
public:
    struct SymbolEntry {
        int tokenType;
        std::string tokenVal;
        std::string label;
        int varType;
        int place;
        double realVal;
        int intVal;
        bool isSubProgram = false;
    };
    int insert(const std::string symbol, int tokenType);
    int lookup(const std::string symbol);
    SymbolEntry &operator[](int i);
    void initDeclarationList(std::vector<int> symbolIndexes, int type);
    int insertLiteral(double value);
    int insertLiteral(int value);
    int insertTempVar(int type);

private:
    std::vector<SymbolEntry> table;
    int lastLabel = -1;
    int lastFreeMemAddr = 0;
    int getPlace(int type);
};


#endif //COMPILER_SYMBOLTABLE_H
