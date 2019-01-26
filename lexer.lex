%{
#include "global.h"
#include <stdlib.h>
int tokenval;
int lineno = 0;
%}

%option noyywrap

digit [0-9]
letter [A-z]
NEWLINE "\n"
WHITE "\t"," "
RELOPS "=","<>","<","<=",">=",">"
MULOPS "*","/","div","mod","and"
SIGNS "+","-"
ASSIGN ":="
OR "or"

%%
{letter}({letter} | {digit})* {
                     int p;
                     p = lookup (yytext);
                     if (p == 0)
                     {
                        p = insert (yytext, ID);
                     }
                     yylval = p;
                    return symtable[p].token;
                }
{RELOPS}        {
                    yylval = yytext;
                    return relop;
                }
{MULOPS}        {
                    yylval = yytext;
                    return mulop;
                }
{SIGNS}         {
                    yylval = yytext;
                    return sign;
                }
{ASSIGN}        {
                    return assignop;
                }
{OR}            {
                    return or;
                }
{NEWLINE}       {
                    lineno++;
                }
{WHITE}         {}
%%