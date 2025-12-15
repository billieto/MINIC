%language "C++"
%require "3.2"

%{
	#include "parser.tab.hh"
	// #include "lexer.hh"

	extern int yylex(yy::parser::semantic_type *yylval);
	extern STNode *g_root;
	
%}

%define parse.error verbose
%debug
%verbose

%code requires
{
	#include "composite.hh"
	#include "composite_concrete.hh"
}

%union
{
	STNode *node;
}

%start compile_unit

%token <node> NUMBER IDENTIFIER
%token SEMICOLON OPEN_PAREN CLOSE_PAREN
%token OPEN_BRACKET CLOSE_BRACKET IF ELSE LOWER_THAN_ELSE
%token PLUS EQUALS MINUS MUL DIV 

%type <node> expression_list expression compile_unit
%type <node> if_statement code_block statements statement condition

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right EQUALS
%left PLUS MINUS
%left MUL DIV

%%

compile_unit: 
	statements { g_root = $$ = new compile_unit((statements *) $1); }
;

code_block:
	OPEN_BRACKET statements CLOSE_BRACKET { $$ = new code_block((statements *) $2); }
|	expression SEMICOLON { $$ = new code_block((expression *) $1); }
;

statements:
	statements statement  { $$ = new statements((statements *) $1, (statement *) $2); }
|	statement  { $$ = new statements((statement *) $1); }
;

statement:
	expression_list SEMICOLON { $$ = new statement((expression_list *) $1); }
|	if_statement { $$ = new statement((if_statement *) $1); }
// | switch_statement
// | while_statement
// | do_while_statement
// | for_statement
;

condition:
	OPEN_PAREN expression_list CLOSE_PAREN {$$ = new condition((expression_list *) $2); }
;

if_statement:
	IF condition code_block %prec LOWER_THAN_ELSE { $$ = new if_statement((condition *) $2, (code_block *) $3); }
|	IF condition code_block ELSE code_block { $$ = new if_statement((condition *) $2, (code_block *) $3, (code_block *) $5); }
|	IF condition code_block ELSE if_statement { $$ = new if_statement((condition *) $2, (code_block *) $3, (if_statement *) $5); }
;

expression_list:
	expression { $$ = new expression_list((expression *) $1); }
//|	expression_list expression SEMICOLON { $$ = new expression_list((expression_list *) $1, (expression *) $2);}
;

expression:
	NUMBER { $$ = (NUMBER *) $1; }
|	IDENTIFIER { $$ = (IDENTIFIER *) $1; }
|	OPEN_PAREN expression CLOSE_PAREN { $$ = (expression *) $2; }
|	expression MUL expression { $$ = new multiplication((expression *) $1, (expression *) $3); }
|	expression DIV expression { $$ = new division((expression *) $1, (expression *) $3); }
|	expression PLUS expression { $$ = new addition((expression *) $1, (expression *) $3); }
|	expression MINUS expression { $$ = new subtraction((expression *) $1, (expression *) $3); }
|	IDENTIFIER EQUALS expression { $$ = new assignment((IDENTIFIER *) $1, (expression *) $3); }
;

%%

void yy::parser::error(const std::string& message)
{
    std::cerr << "error: " << message << std::endl;
	exit(1);
}
