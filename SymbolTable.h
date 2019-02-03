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
        bool isSubProgram = false;
        bool isLocal = false;
        bool isProcedure = false;
        bool isRef = false;
    };

    int insert(const std::string symbol, int tokenType);

    int lookup(const std::string symbol);

    SymbolEntry &operator[](int i);

    void initDeclarationList(std::vector<int> symbolIndexes, int type);

    int insertLiteral(std::string value);

    int insertTempVar(int type);

    void toggleGlobal();

    bool isGlobal();

    int initSubProgram(int index);

    void initSubProgramParams(std::vector<std::pair<int, std::vector<int>>> paramListHolder);

private:
    std::vector<SymbolEntry> table;
    int lastLabel = -1;
    int lastFreeMemAddr = 0;
    int oldLastFreeMemAddr;
    bool global = true;

    int getPlace(int type);


    int getPlace(SymbolEntry entry);
};


#endif //COMPILER_SYMBOLTABLE_H
