//
// Created by howorang on 26.01.19.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H


#include <string>
#include <vector>
#include "global.h"

class SymbolTable {
public:
    struct SymbolEntry {
        int tokenType;
        std::string tokenVal;
        std::string label;
    };
    int insert(const std::string symbol, int tokenType);
    int lookup(const std::string symbol);
    SymbolEntry &operator[](int i);
    int genLabel();

private:
    std::vector<SymbolEntry> table;
    int lastLabel = -1;
};


#endif //COMPILER_SYMBOLTABLE_H
