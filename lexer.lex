%{
#include <stdlib.h>
#include "global.h"

int tokenval;
int lineno = 0;
%}

%option noyywrap
%option yylineno

digit [0-9]
letter [A-z]
NEWLINE "\n"
WHITE [ \t]+
RELOPS "="|"<>"|"<"|"<="|">="|">"
MULOPS "*"|"/"|"div"|"mod"|"and"
SIGNS "+"|"-"
ASSIGN ":="
OR "or"

%%

{RELOPS}        {
                    yylval = decodeRelOp(yytext);
                    return RELOP;
                }
{MULOPS}        {
                    yylval = decodeMulOp(yytext);
                    return MULOP;
                }
{SIGNS}         {
                    yylval = decodeSignOp(yytext);
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
                    int p = symbolTable.insert(yytext, ID);
                    yylval = p;
                    return ID;
                }
{digit}+.{digit}+ {
                    yylval = symbolTable.insertLiteral(yytext);
                    return NUM;
}

{digit}+ {
                    yylval = symbolTable.insertLiteral(yytext);
                    return NUM;
}
.               {return (int) yytext[0];}
%%