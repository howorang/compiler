//
// Created by howorang on 01.02.19.
//

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H

#include "global.h"
#include "consts.h"

class decode_exception {
};

enum MULOP decodeMulOp(std::string mulOpStr);

enum RELOP decodeRelOp(std::string relOpStr);


#endif //COMPILER_UTILS_H
