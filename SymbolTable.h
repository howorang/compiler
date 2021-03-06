//
// Created by howorang on 26.01.19.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H


#include <string>
#include <vector>
#include "global.h"
#include <algorithm>

struct array_declaration_holder;

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
        bool isLiteral = false;
        int incsp = 0;
        std::vector<int> paramListTypeSignature;
        bool isArray;
        int low;
        int high;
        int ifFalseLabel;
        int ifAfterLabel;
        int loopExitLabel;
        int loopReturnLabel;
        bool isArrayRef = true;
    };

    int insert(const std::string symbol, int tokenType);

    int insertHolder(int holderType);

    int lookup(const std::string symbol);

    SymbolEntry &operator[](int i);

    void initDeclarationList(std::vector<int> symbolIndexes, int type, array_declaration_holder arrayDeclarationHolder);

    int insertLiteral(std::string value, int type);

    int insertTempVar(int type, bool isRef = false, bool isArrayRef = false);

    int insertLabel();

    void toggleGlobal();

    bool isGlobal();

    void initSubProgram(int index, std::vector<std::tuple<int, std::vector<int>, array_declaration_holder>> paramListHolder);

    int getAllocatedMem();

    int getFunc(int i);

private:
    std::vector<SymbolEntry> table;
    int lastLabel = -1;
    int lastFreeMemAddr = 0;
    int oldLastFreeMemAddr;
    bool global = true;
    int nextLabel = 1;

    int getPlace(int type);

    int getPlace(SymbolEntry entry);

    void removeLocalSymbols();
};


#endif //COMPILER_SYMBOLTABLE_H
