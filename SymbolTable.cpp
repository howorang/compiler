//
// Created by howorang on 26.01.19.
//

#include <algorithm>
#include "SymbolTable.h"

SymbolTable symbolTable = SymbolTable();

int SymbolTable::insert(const std::string symbol, int tokenType) {
    SymbolEntry entry = {.tokenType = tokenType, .tokenVal = symbol};
    table.push_back(entry);
    return table.size() - 1;
}

int SymbolTable::lookup(const std::string symbol) {
    auto result = std::find_if(table.begin(), table.end(), [&symbol](const SymbolEntry arg) {
            return arg.tokenVal == symbol;
        });
    if (result == table.end()) {
        return 0;
    } else {
        return static_cast<int>(std::distance(table.begin(), result));
    }
}

SymbolTable::SymbolEntry &SymbolTable::operator[](int i) {
    return table[i];
}

int SymbolTable::genLabel() {
    insert(std::to_string(++lastLabel), LABEL);
}

