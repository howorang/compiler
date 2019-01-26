//
// Created by howorang on 26.01.19.
//

#include <algorithm>
#include "SymbolTable.h"

int SymbolTable::insert(const std::string symbol, int tokenType) {
    SymbolEntry entry = {.tokenType = tokenType, .tokenVal = symbol};
    table.push_back(entry);
    return table.size();
}

int SymbolTable::lookup(const std::string symbol) {
    auto result = std::find_if(table.begin(), table.end(), [&symbol](const SymbolEntry arg) {
            return arg.tokenVal == symbol;
        });
    if (result == table.end()) {
        return -1;
    } else {
        return static_cast<int>(std::distance(table.begin(), result));
    }
}
