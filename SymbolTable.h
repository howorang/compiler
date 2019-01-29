//
// Created by howorang on 26.01.19.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H


#include <string>
#include <vector>

class SymbolTable {
public:
    struct SymbolEntry {
        int tokenType;
        std::string tokenVal;
    };
    int insert(const std::string symbol, int tokenType);
    int lookup(const std::string symbol);
    SymbolEntry &operator[](int i);

private:
    std::vector<SymbolEntry> table;
};


#endif //COMPILER_SYMBOLTABLE_H
