//
// Created by howorang on 01.02.19.
//

#ifndef COMPILER_CONSTS_H
#define COMPILER_CONSTS_H
#define SUBPROGRAM_OFFSET 8

#define LABEL 463643

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
    INCSP,
    JUMP,
    OR_OP
};

enum VARMODE {
    value,
    address,
    directi,
    directr,
    direct,
    label,
    arrlow,
    arrhigh,
    varsize
};

#endif //COMPILER_CONSTS_H
