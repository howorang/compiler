//
// Created by howorang on 01.02.19.
//

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H

#include "global.h"
#include "consts.h"

class decode_exception {
};

enum OP decodeMulOp(std::string mulOpStr);
enum OP decodeRelOp(std::string relOpStr);
enum OP decodeSignOp(std::string signOpStr);
int sizeOfSymbol(int symbolType);
bool stringReplace(std::string& str, const std::string& from, const std::string& to);

constexpr uint64_t mix(char m, uint64_t s)
{
    return ((s<<7) + ~(s>>3)) + ~m;
}

constexpr uint64_t hash(const char * m)
{
    return (*m) ? mix(*m,hash(m+1)) : 0;
}

#endif //COMPILER_UTILS_H
