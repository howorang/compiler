%{
#include <stdlib.h>
#include "global.h"

int tokenval;
int lineno = 0;
%}

%option noyywrap

digit [0-9]
letter [A-z]
NEWLINE "\n"
WHITE [ \t]+
RELOPS "=","<>","<","<=",">=",">"
MULOPS "*","/","div","mod","and"
SIGNS "+","-"
ASSIGN ":="
OR "or"

%%

{RELOPS}        {
                    yylval.strVal = yytext;
                    return RELOP;
                }
{MULOPS}        {
                    yylval.strVal = yytext;
                    return MULOP;
                }
{SIGNS}         {
                    yylval.strVal = yytext;
                    return SIGN;
                }
{ASSIGN}        {
                    return ASSIGNOP;
                }
{NEWLINE}       {
                    lineno++;
                }
{WHITE}         {}
"or"            {return OR;}
"not"           {return NOT;}
"do"            {return DO;}
"if"            {return IF;}
"else"          {return ELSE;}
"then"          {return THEN;}
"while"         {return WHILE;}
"var"           {return VAR;}
".."            {return RANGEOP;}
"array"         {return ARRAY;}
"of"            {return OF;}

"integer"       {return INTEGER;}
"real"          {return REAL;}

"procedure"     {return PROCEDURE;}
"function"      {return FUNCTION;}

"begin"         {return BEGIN_TOKEN;}
"end"           {return END_TOKEN;}
"program"       {return PROGRAM_TOKEN;}

{letter}({letter}|{digit})* {
                     int p;
                     p = symbolTable.lookup(yytext);
                     if (p == 0)
                     {
                        p = symbolTable.insert(yytext, ID);
                     }
                     yylval.intVal = p;
                    return symbolTable[p].tokenType;
                }
{digit}+.{digit}+ {
                    yylval.strVal = yytext;
                    return NUM;
}

{digit}+ {
                    yylval.strVal = yytext;
                    return NUM;
}
.               {return (int) yytext[0];}
%%