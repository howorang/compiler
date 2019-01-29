//
// Created by howorang on 29.01.19.
//

#ifndef COMPILER_GLOBAL_H
#define COMPILER_GLOBAL_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parser.hpp"
#include "SymbolTable.h"
class SymbolTable;
#define BSIZE 128
#define EOS '\0'
extern int lineno;
extern int yylex (void);
extern FILE *yyin, *yyout;
extern int yyparse (void);
extern SymbolTable symbolTable;
extern int yyerror(const char*);
#endif //COMPILER_GLOBAL_H