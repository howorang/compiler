%{
#include "global.h"
#include <vector>

std::vector<int> idListHolder;
std::vector<std::tuple<int, std::vector<int>, array_declaration_holder>> paramListHolder;
std::vector<int> expressionListHolder;
array_declaration_holder arrayDeclarationHolder;

int falseLabel = -1;
int afterLabel = -1;
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
	symbolTable.initDeclarationList(idListHolder, $5, arrayDeclarationHolder);
	idListHolder.clear();
}
| %empty

type:
standard_type {$$ = $1;}
| ARRAY '[' NUM RANGEOP NUM ']' OF standard_type {
	arrayDeclarationHolder.low = atoi(symbolTable[$3].tokenVal.c_str());
	arrayDeclarationHolder.high = atoi(symbolTable[$5].tokenVal.c_str());
	arrayDeclarationHolder.type = $8;
	$$ = ARRAY;
}

standard_type:
INTEGER {$$ = INTEGER;}
| REAL {$$ = REAL;}

subprogram_declarations:
subprogram_declarations subprogram_declaration ';' {
	emitter.exitSubProgramDecl();
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
	emitter.simpleEmit("enter.i #{allocSize}");
	symbolTable.toggleGlobal();
	symbolTable.initSubProgram($2, paramListHolder);

}
| PROCEDURE ID arguments ';' {
	symbolTable[$2].isSubProgram = true;
	symbolTable[$2].isProcedure = true;
	symbolTable[$2].label = symbolTable[$2].tokenVal;
	emitter.simpleEmit(symbolTable[$2].label + ":");
	emitter.simpleEmit("enter.i #{allocSize}");
	symbolTable.toggleGlobal();
	symbolTable.initSubProgram($2, paramListHolder);
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
	paramListHolder.push_back(std::make_tuple($3, ids, arrayDeclarationHolder));
}
| parameter_list ';' identifier_list ':' type {
	std::vector<int> ids;
        ids = idListHolder;
        idListHolder.clear();
        paramListHolder.push_back(std::make_tuple($5, ids, arrayDeclarationHolder));
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
	emitter.genCode(MOV, $3, value, $1, value);
}
| procedure_statement {

}
| compound_statement
| IF expression {
	falseLabel = symbolTable.insertLabel();
	afterLabel = symbolTable.insertLabel();
	emitter.genCode(EQ, $2, value, 0, directi, falseLabel, label);
} THEN statement {
	emitter.genCode(JUMP, afterLabel, label);
} ELSE {
	emitter.emmitLabel(falseLabel);
} statement {
	emitter.emmitLabel(afterLabel);
}
| WHILE expression DO statement

variable:
ID {$$ = $1;}
| ID '[' expression ']' {
	$$ = emitter.emmitArray($1, $3);
}

procedure_statement:
ID {
	emitter.emmitFunc($1, expressionListHolder);
}
| ID '(' expression_list ')' {
	if(symbolTable[$1].tokenVal == "write") {
		emitter.genCode(WRITE, expressionListHolder[0], value);
	} else {
		emitter.emmitFunc($1, expressionListHolder);
        }
        expressionListHolder.clear();
}

expression_list:
expression {
	expressionListHolder.push_back($1);
}
| expression_list ',' expression {
	expressionListHolder.push_back($3);
}

expression:
simple_expression {$$ = $1;}
| simple_expression RELOP simple_expression {
	int trueLabel = symbolTable.insertLabel();
	int _afterLabel = symbolTable.insertLabel();
	int expValue = symbolTable.insertTempVar(INTEGER);
	emitter.genCode((OP)$2, $1, value, $3, value, trueLabel, label);
	emitter.genCode(MOV, 0, directi, expValue, value);
	emitter.genCode(JUMP, _afterLabel, label);
	emitter.emmitLabel(trueLabel);
	emitter.genCode(MOV, 1, directi, expValue, value);
	emitter.emmitLabel(_afterLabel);
	$$ = expValue;
}

simple_expression:
term {$$ = $1;}
| SIGN term
| simple_expression SIGN term {
	int tempVarIndex = symbolTable.insertTempVar(emitter.determineOpType($1, value, $3, value));
	emitter.genCode((OP)$2, $1, value, $3, value, tempVarIndex, value);
	$$=tempVarIndex;
}
| simple_expression OR term

term:
factor {$$ = $1;}
| term MULOP factor {
	int tempVarIndex = symbolTable.insertTempVar(emitter.determineOpType($1, value, $3, value));
	emitter.genCode((OP)$2, $1, value, $3, value, tempVarIndex, value);
	$$=tempVarIndex;
}

factor:
variable { // FUN CALL WITHOUT ARGS
	//PASCAL IS FUCKING STUPID
	symbolTable.lookup(symbolTable[$1].tokenVal);
	if(symbolTable[$1].isSubProgram && !symbolTable[$1].isProcedure) {
		$$ = emitter.emmitFunc($1, expressionListHolder);
	} else {
		$$=$1;
	}

}
| ID '(' expression_list ')' { // FUN CALL WITH ARGS
	$$ = emitter.emmitFunc($1, expressionListHolder);
	expressionListHolder.clear();

}
| NUM {$$ = $1;}
| '(' expression ')' {$$ = $2;}
| NOT factor {
	int tempVarIndex = symbolTable.insertTempVar(symbolTable[$2].varType);
        emitter.genCode(MINUS, 0, direct, $2, value, tempVarIndex, value);
        $$=tempVarIndex;
}
%%
int yyerror(const char *s)
{
 fprintf(stderr,"Error | Line: %d\n%s\n",yylineno,s);
}