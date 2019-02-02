%{
#include "global.h"
#include <vector>

std::vector<int> declListHolder;
%}

%debug
%define parse.error verbose
%locations

%token ID
%token NUM
%token SIGN
%token RELOP
%token MULOP
%token ASSIGNOP
%token OR

%token NOT

%token DO
%token IF
%token ELSE
%token THEN
%token WHILE
%token VAR
%token RANGEOP
%token ARRAY
%token OF

%token INTEGER
%token REAL

%token PROCEDURE
%token FUNCTION

%token BEGIN_TOKEN
%token END_TOKEN
%token PROGRAM_TOKEN


%%
program:
PROGRAM_TOKEN ID '(' identifier_list ')'
 ';' {
 	declListHolder.clear();
     	int programLabel = symbolTable.genLabel();
     	symbolTable[$2].label = programLabel;
     	emitter.simpleEmit("jump.i  #" + symbolTable[programLabel].tokenVal);
     	emitter.simpleEmit(symbolTable[programLabel].tokenVal + ":\n");
}

declarations
subprogram_declarations
compound_statement
'.' {
	emitter.saveToFile("prog");
}

identifier_list:
ID {declListHolder.push_back($1);}
| identifier_list ',' ID {declListHolder.push_back($3);}

declarations:
declarations VAR identifier_list ':' type ';' {
	symbolTable.initDeclarationList(declListHolder, $5);
	declListHolder.clear();
}
| %empty

type:
standard_type {$$ = $1;}
| ARRAY '[' NUM RANGEOP NUM ']' OF standard_type

standard_type:
INTEGER {$$ = INTEGER;}
| REAL {$$ = REAL;}

subprogram_declarations:
subprogram_declarations subprogram_declaration ';'
| %empty

subprogram_declaration:
subprogram_head declarations compound_statement

subprogram_head:
FUNCTION ID arguments ':' standard_type ';'
| PROCEDURE ID arguments ';'

arguments:
'(' parameter_list ')'
| %empty

parameter_list:
identifier_list ':' type
| parameter_list ';' identifier_list ':' type

compound_statement:
BEGIN_TOKEN
optional_statements
END_TOKEN

optional_statements:
statement_list
| %empty

statement_list:
statement
| statement_list ';' statement

statement:
variable ASSIGNOP expression
| procedure_statement
| compound_statement
| IF expression THEN statement ELSE statement
| WHILE expression DO statement

variable:
ID {$$ = $1;}
| ID '[' expression ']'

procedure_statement:
ID
| ID '(' expression_list ')'

expression_list:
expression
| expression_list ',' expression

expression:
simple_expression {}
| simple_expression RELOP simple_expression

simple_expression:
term {$$ = $1;}
| SIGN term
| simple_expression SIGN term {
	int tempVarIndex = symbolTable.insertTempVar(emitter.determineOpType($1, $3));
	emitter.genCode((OP)$2, $1, $3, tempVarIndex);
	$$=tempVarIndex;
}
| simple_expression OR term

term:
factor {$$ = $1;}
| term MULOP factor

factor:
variable {$$ = $1;}
| ID '(' expression_list ')'
| NUM
| '(' expression ')'
| NOT factor

%%
int yyerror(const char *s)
{
 fprintf(stderr,"Error | Line: %d\n%s\n",yylineno,s);
}