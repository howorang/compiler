%{
#include "global.h"
#include <vector>

std::vector<int> idListHolder;
std::vector<std::pair<int, std::vector<int>>> paramListHolder;
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
 	idListHolder.clear();
   	emitter.simpleEmit("jump.i  #lab0");
}

declarations
subprogram_declarations {
	//declListHolder.clear();
     	//int programLabel = symbolTable.genLabel();
     	//symbolTable[$2].label = programLabel;
     	emitter.simpleEmit("lab0:");
}
compound_statement
'.' {
	emitter.simpleEmit("exit");
	emitter.saveToFile("prog");
}

identifier_list:
ID {idListHolder.push_back($1);}
| identifier_list ',' ID {idListHolder.push_back($3);}

declarations:
declarations VAR identifier_list ':' type ';' {
	symbolTable.initDeclarationList(idListHolder, $5);
	idListHolder.clear();
}
| %empty

type:
standard_type {$$ = $1;}
| ARRAY '[' NUM RANGEOP NUM ']' OF standard_type

standard_type:
INTEGER {$$ = INTEGER;}
| REAL {$$ = REAL;}

subprogram_declarations:
subprogram_declarations subprogram_declaration ';' {
	emitter.genCode(LEAVE);
	emitter.genCode(RETURN);
	paramListHolder.clear();
	symbolTable.toggleGlobal();
}
| %empty

subprogram_declaration:
subprogram_head declarations compound_statement

subprogram_head:
FUNCTION ID arguments ':' standard_type ';' {
	symbolTable[$2].isSubProgram = true;
	symbolTable[$2].label = symbolTable[$2].tokenVal;
	symbolTable[$2].varType = $5;
	emitter.simpleEmit(symbolTable[$2].label + ":");
	emitter.simpleEmit("enter #0");
	symbolTable.toggleGlobal();
	$$ = symbolTable.initSubProgram($2);
	symbolTable.initSubProgramParams(paramListHolder);

}
| PROCEDURE ID arguments ';' {
	symbolTable[$2].isSubProgram = true;
	symbolTable[$2].isProcedure = true;
	symbolTable[$2].label = symbolTable[$2].tokenVal;
	emitter.simpleEmit(symbolTable[$2].label + ":");
	emitter.simpleEmit("enter #0");
	symbolTable.toggleGlobal();
	symbolTable.initSubProgram($2);
	symbolTable.initSubProgramParams(paramListHolder);
}

arguments:
'(' parameter_list ')' {

}
| %empty

parameter_list:
identifier_list ':' type {
	std::vector<int> ids;
	ids = idListHolder;
	idListHolder.clear();
	paramListHolder.push_back(std::make_pair($3, ids));
}
| parameter_list ';' identifier_list ':' type {
	std::vector<int> ids;
        ids = idListHolder;
        idListHolder.clear();
        paramListHolder.push_back(std::make_pair($5, ids));
}

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
variable ASSIGNOP expression {
	emitter.genCode(MOV, $3, $1);
}
| procedure_statement
| compound_statement
| IF expression THEN statement ELSE statement
| WHILE expression DO statement

variable:
ID {$$ = $1;}
| ID '[' expression ']'

procedure_statement:
ID
| ID '(' expression_list ')' {emitter.genCode(WRITE, $3);}

expression_list:
expression
| expression_list ',' expression

expression:
simple_expression {$$ = $1;}
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
| term MULOP factor {
	int tempVarIndex = symbolTable.insertTempVar(emitter.determineOpType($1, $3));
	emitter.genCode((OP)$2, $1, $3, tempVarIndex);
	$$=tempVarIndex;
}

factor:
variable {$$ = $1;}
| ID '(' expression_list ')'
| NUM {$$ = $1;}
| '(' expression ')'
| NOT factor

%%
int yyerror(const char *s)
{
 fprintf(stderr,"Error | Line: %d\n%s\n",yylineno,s);
}