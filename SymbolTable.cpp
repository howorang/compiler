//
// Created by howorang on 26.01.19.
//

#include "SymbolTable.h"

SymbolTable symbolTable = SymbolTable();

int SymbolTable::insert(const std::string symbol, int tokenType) {
    SymbolEntry entry = {.tokenType = tokenType, .tokenVal = symbol, .isLocal = !global};
    table.push_back(entry);
    return static_cast<int>(table.size() - 1);
}

int SymbolTable::lookup(const std::string symbol) {
    for (int i = table.size() -1; i >= 0; i--) {
        if (table[i].isLocal == !global && table[i].tokenVal == symbol) {
            return i;
        } else {
            return -1;
        }
    }
}

SymbolTable::SymbolEntry &SymbolTable::operator[](int i) {
    return table[i];
}

void SymbolTable::initDeclarationList(std::vector<int> symbolIndexes, int type) {
    for (int symbolIndex : symbolIndexes) {
        SymbolEntry &entry = operator[](symbolIndex);
        entry.varType = type;
        entry.place = getPlace(type);
    }
}

int SymbolTable::insertLiteral(std::string value) {
    SymbolEntry entry = {.tokenVal = std::move(value)};
    table.push_back(entry);
    return static_cast<int>(table.size() - 1);
}

int SymbolTable::insertTempVar(int type) {
    SymbolEntry entry = {.varType = type, .place = getPlace(type)};
    table.push_back(entry);
    return static_cast<int>(table.size() - 1);
}


int SymbolTable::getPlace(int type) {
    int toReturn = lastFreeMemAddr;
    lastFreeMemAddr += sizeOfSymbol(type);
    return toReturn;
}

void SymbolTable::toggleGlobal() {
    if (global) {
        oldLastFreeMemAddr = lastFreeMemAddr;
    } else {
        lastFreeMemAddr = oldLastFreeMemAddr;
    }
    global = !global;
}

bool SymbolTable::isGlobal() {
    return global;
}

int SymbolTable::initSubProgram(int index) {
    SymbolEntry &symbolEntry = operator[](index);
    switch (symbolEntry.isProcedure) {
        case true:
            lastFreeMemAddr = SUBPROGRAM_OFFSET;
            return -46574321;
            break;
        case false:
            lastFreeMemAddr = SUBPROGRAM_OFFSET + 4;
            SymbolEntry entry = {.varType = symbolEntry.varType,
                    .place = lastFreeMemAddr,
                    .isLocal = true,
                    .isRef = true};
            table.push_back(entry);
            break;
    }

}

void
SymbolTable::initSubProgramParams(std::vector<std::pair<int, std::vector<int>>> paramListHolder) {
    for (const auto &type_indexes : paramListHolder) {
        for (const auto &index : type_indexes.second) {
            SymbolEntry &entry = operator[](index);
            entry.isRef = true;
            entry.isLocal = true;
            //entry.place = ;
            entry.varType = type_indexes.first;
        }
    }
}

int SymbolTable::getPlace(SymbolTable::SymbolEntry entry) {
    if (entry.isRef) {
        return 4;
    } else {
        return getPlace(entry.varType);
    }
}
