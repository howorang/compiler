//
// Created by howorang on 01.02.19.
//

#ifndef COMPILER_CONSTS_H
#define COMPILER_CONSTS_H
#define SUBPROGRAM_OFFSET 8

enum OP {
    MUL,
    DIV,
    MOD,
    AND,
    EQ,
    NEQ,
    LT,
    LTE,
    GT,
    GTE,
    PLUS,
    MINUS,
    INTOREAL,
    REALTOINT,
    MOV,
    WRITE,
    LEAVE,
    RETURN,
    PUSH,
    CALL,
    ENTER,
    INCSP
};

enum VARMODE {
    value,
    address,
    direct,
    label,
    arrlow,
    arrhigh,
    varsize
};

#endif //COMPILER_CONSTS_H
