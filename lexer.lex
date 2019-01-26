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
"program"       {return program;}
"or"        {return or;}
"not"       {return not;}
"do"        {return do;}
"if"        {return if;}
"else"      {return else;}
"then"      {return then;}
"while"     {return while;}
"var"       {return var;}
".."        {return rangeop;}
"array"     {return array;}
"of"        {return of;}

"integer"   {return integer;}
"real"      {return real;}

"procedure" {return procedure;}
"function"  {return function;}

"begin"     {return begin;}
"end"       {return end;}
"program"   {return program_token}

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
%%