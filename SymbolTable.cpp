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
    for (int i = table.size() - 1; i >= 0; i--) {
        if (table[i].isLocal == !global && table[i].tokenVal == symbol) {
            return i;
        }
    }
    return -1;
}

SymbolTable::SymbolEntry &SymbolTable::operator[](int i) {
    return table[i];
}

void SymbolTable::initDeclarationList(std::vector<int> symbolIndexes, int type,
                                      array_declaration_holder arrayDeclarationHolder) {
    for (int symbolIndex : symbolIndexes) {
        SymbolEntry &entry = operator[](symbolIndex);
        if (type == ARRAY) {
            entry.isArray = true,
            entry.low = arrayDeclarationHolder.low,
            entry.high = arrayDeclarationHolder.high;
            entry.varType = arrayDeclarationHolder.type;
            entry.place = getPlace(entry.varType); // First memory incrementation
            lastFreeMemAddr += (entry.high - entry.low) * sizeOfSymbol(entry.varType); //Second memory incrementation Basically this is high - low + 1
        } else {
            entry.varType = type;
            entry.place = getPlace(type);
        }
    }
}

int SymbolTable::insertLiteral(std::string value, int type) {
    SymbolEntry entry = {
            .tokenVal = std::move(value),
            .varType = type,
            .isLiteral = true,};
    table.push_back(entry);
    return static_cast<int>(table.size() - 1);
}

int SymbolTable::insertTempVar(int type, bool isRef) {
    SymbolEntry entry = {.varType = type, .place = getPlace(type), .isLocal = !global, .isRef = isRef};
    table.push_back(entry);
    return static_cast<int>(table.size() - 1);
}


int SymbolTable::getPlace(int type) {
    int toReturn = lastFreeMemAddr;
    lastFreeMemAddr += global ? sizeOfSymbol(type) : -sizeOfSymbol(type);
    return global ? toReturn : lastFreeMemAddr;
}

void SymbolTable::toggleGlobal() {
    if (global) {
        oldLastFreeMemAddr = lastFreeMemAddr;
        lastFreeMemAddr = 0;
    } else {
        lastFreeMemAddr = oldLastFreeMemAddr;
        removeLocalSymbols();
    }
    global = !global;
}

void SymbolTable::removeLocalSymbols() {
    table.erase(
            std::remove_if(
                    table.begin(), table.end(),
                    [](SymbolEntry &entry) { return entry.isLocal; }),
            table.end());
}


bool SymbolTable::isGlobal() {
    return global;
}

void SymbolTable::initSubProgram(int index, std::vector<std::tuple<int, std::vector<int>, array_declaration_holder>> paramListHolder) {
    int incsp = SUBPROGRAM_OFFSET;
    SymbolEntry &symbolEntry = operator[](index);
    if (symbolEntry.isProcedure) {
    } else {
        //PUTTING IN TEMP VAR FOR FUNCTION RESULT
        SymbolEntry entry = {
                .tokenVal = symbolEntry.tokenVal,
                .varType = symbolEntry.varType,
                .place = incsp,
                .isLocal = true,
                .isRef = true};
        table.push_back(entry);
        incsp += 4;
    }

    for (size_t i = paramListHolder.size(); i--;) {
        const auto &type_indexes = paramListHolder[i];
        for (size_t j = std::get<1>(type_indexes).size(); j--;) {
            const auto &symbolIndex = std::get<1>(type_indexes)[j];
            SymbolEntry &entry = operator[](symbolIndex);
            entry.isRef = true;
            entry.isLocal = true;
            entry.place = incsp;
            entry.varType = std::get<0>(type_indexes);
            incsp += 4;
            symbolEntry.paramListTypeSignature.push_back(std::get<0>(type_indexes));
            if (entry.varType == ARRAY) {
                const auto array_holder = std::get<2>(type_indexes);
                entry.varType = array_holder.type;
                entry.low = array_holder.low;
                entry.high = array_holder.high;
                entry.isArray = true;
            }
        }
    }
    symbolEntry.incsp = incsp - SUBPROGRAM_OFFSET;
    std::reverse(symbolEntry.paramListTypeSignature.begin(), symbolEntry.paramListTypeSignature.end());
}

int SymbolTable::getPlace(SymbolTable::SymbolEntry entry) {
    if (entry.isRef) {
        return 4;
    } else {
        return getPlace(entry.varType);
    }
}

int SymbolTable::getAllocatedMem() {
    return std::abs(lastFreeMemAddr);
}

int SymbolTable::getFunc(int index) {
    symbolTable[index].tokenVal;
    for (int i = table.size() - 1; i >= 0; i--) {
        if (table[i].isLocal == false && table[i].tokenVal == symbolTable[index].tokenVal && table[i].isSubProgram) {
            return i;
        }
    }
    return -1;
}
