/*RPN Lexer*/
%{
#include "global.h"
#include <stdlib.h>
int tokenval;
int lineno = 0;
%}

%option noyywrap

NUM   [0-9]
ID    [A-z]+
NEWLINE "\n"
WHITE "\t"," "

%%
{NUM}+          {
                    yylval = atoi(yytext);
                    return NUM;
                }
{ID}            {
                    int p;
                    p = lookup (yytext);
                    if (p == 0)
                    {
                        p = insert (yytext, ID);
                    }
                    yylval = p;
                    return symtable[p].token;
                }
.               {
                    yylval = yytext[0];
                    return yytext[0];
                }
{NEWLINE}       {
                    lineno++;
                }
{WHITE}         {}
%%