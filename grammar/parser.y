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
%token LESS LESS_EQUALS GREATER GREATER_EQUALS 
%token LOGIC_EQUALS LOGIC_AND LOGIC_OR LOGIC_NOT

%type <node> expression condition compile_unit
%type <node> if_statement compound_statement statement_list statement 

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right EQUALS
%left PLUS MINUS
%left MUL DIV

%%

compile_unit: 
	statement_list { g_root = $$ = new compile_unit((statement_list *) $1); }
;

compound_statement:
	OPEN_BRACKET statement_list CLOSE_BRACKET { $$ = new compound_statement((statement_list *) $2); }
|	OPEN_BRACKET CLOSE_BRACKET { $$ = new compound_statement(); }
;

statement_list:
	statement_list statement  { $$ = new statement_list((statement_list *) $1, (statement *) $2); }
|	statement  { $$ = new statement_list((statement *) $1); }
;

statement:
	expression SEMICOLON { $$ = new statement((expression *) $1); }
|	compound_statement { $$ = new statement((compound_statement * ) $1); }
|	if_statement { $$ = new statement((if_statement *) $1); }
// | switch_statement
// | while_statement
// | do_while_statement SEMICOLON
// | for_statement
;

condition:
	OPEN_PAREN expression CLOSE_PAREN {$$ = new condition((expression *) $2); }
;

if_statement:
	IF condition statement_list %prec LOWER_THAN_ELSE { $$ = new if_statement((condition *) $2, (statement_list *) $3); }
|	IF condition statement_list ELSE statement_list { $$ = new if_statement((condition *) $2, (statement_list *) $3, (statement_list *) $5); }
;

expression:
	NUMBER { $$ = (NUMBER *) $1; }
|	IDENTIFIER { $$ = (IDENTIFIER *) $1; }
|	OPEN_PAREN expression CLOSE_PAREN { $$ = (expression *) $2; }
|	expression MUL expression { $$ = new multiplication((expression *) $1, (expression *) $3); }
|	expression DIV expression { $$ = new division((expression *) $1, (expression *) $3); }
|	expression PLUS expression { $$ = new addition((expression *) $1, (expression *) $3); }
|	expression MINUS expression { $$ = new subtraction((expression *) $1, (expression *) $3); }
|	expression LESS expression {}
|	expression LESS_EQUALS expression {}
|	expression GREATER expression {}
|	expression GREATER_EQUALS expression {}
|	expression LOGIC_EQUALS expression {}
|	expression LOGIC_AND expression {}
|	expression LOGIC_OR expression {}
|	LOGIC_NOT expression {}
|	IDENTIFIER EQUALS expression { $$ = new assignment((IDENTIFIER *) $1, (expression *) $3); }
;

%%

void yy::parser::error(const std::string& message)
{
    std::cerr << "error: " << message << std::endl;
	exit(1);
}
