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
	#include "symbol_table.hh"
}

%union
{
	STNode *node;
}

%start program

%token <node> NUMBER IDENTIFIER
%token SEMICOLON OPEN_PAREN CLOSE_PAREN SHIFT_LEFT SHIFT_RIGHT
%token OPEN_BRACKET CLOSE_BRACKET IF ELSE LOWER_THAN_ELSE
%token PLUS EQUALS MINUS MUL DIV INCREMENT DECREMENT
%token LESS LESS_EQUALS GREATER GREATER_EQUALS 
%token LOGIC_EQUALS LOGIC_AND LOGIC_OR LOGIC_NOT
%token COMMA RETURN VAR_PRIORITY MOD MOD_EQUALS
%token INT_TYPE FLOAT_TYPE VOID_TYPE DOUBLE_TYPE CHAR_TYPE
%token PLUS_EQUALS MINUS_EQUALS MUL_EQUALS DIV_EQUALS
%token BIT_WISE_OR BIT_WISE_AND BIT_WISE_XOR BIT_WISE_NOT
%token CONTINUE BREAK WHILE DO FOR

%type <node> expression condition program 
%type <node> if_statement compound_statement statement_list statement
%type <node> type_specifier argument_list parameter_list
%type <node> function_definition function_declaration variable_declaration_statement
%type <node> translation_unit external_declaration variable_declaration_list variable_declaration
%type <node> while_statement do_while_statement for_statement

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%nonassoc VAR_PRIORITY

%right EQUALS PLUS_EQUALS MINUS_EQUALS MUL_EQUALS DIV_EQUALS MOD_EQUALS

%left LOGIC_OR
%left LOGIC_AND

%left BIT_WISE_OR
%left BIT_WISE_XOR
%left BIT_WISE_AND

%left LOGIC_EQUALS LOGIC_NOT_EQUALS
%left LESS LESS_EQUALS GREATER GREATER_EQUALS

%left SHIFT_LEFT SHIFT_RIGHT

%left PLUS MINUS
%left MUL DIV MOD

%right LOGIC_NOT BIT_WISE_NOT

%left INCREMENT DECREMENT

%left OPEN_PAREN

%%

// Root
program: 
	translation_unit { g_root = $$ = new program((translation_unit *) $1); }
;

// Recursive rule for full program
translation_unit:
	translation_unit external_declaration { $$ = new translation_unit((translation_unit *) $1, (external_declaration *) $2); }
|   external_declaration { $$ = new translation_unit((external_declaration *) $1); }
;

// Global declarations: Functions and global variables
external_declaration:
	function_declaration { $$ = new external_declaration((function_declaration *) $1); }
|	function_definition { $$ = new external_declaration((function_definition *) $1); }
|	variable_declaration_statement { $$ = new external_declaration((variable_declaration_statement *) $1); }
;	// ^ Global declaration of variables

// Variable declaration recursion
variable_declaration_statement:
	type_specifier variable_declaration_list SEMICOLON { $$ = new variable_declaration_statement((type_specifier *) $1, (variable_declaration_list *) $2); }
;

variable_declaration_list:
	variable_declaration { $$ = new variable_declaration_list((variable_declaration *) $1); }
|	variable_declaration_list COMMA variable_declaration
	{
		$$ = (variable_declaration_list *) $1;

		((variable_declaration_list *) $$) -> add((variable_declaration *) $3);
	}
;

variable_declaration:
	IDENTIFIER EQUALS expression { $$ = new variable_declaration((IDENTIFIER *) $1, (expression *) $3); }
|	IDENTIFIER { $$ = new variable_declaration((IDENTIFIER *) $1); }
;

// Functions
function_definition:
	type_specifier IDENTIFIER OPEN_PAREN parameter_list CLOSE_PAREN compound_statement
	{
		$$ = new function_definition((type_specifier *) $1, (IDENTIFIER *) $2, (parameter_list *) $4, (compound_statement *) $6);
	}
	// type_specifier IDENTIFIER OPEN_PAREN argument_list CLOSE_PAREN compound_statement
;

function_declaration:
	type_specifier IDENTIFIER OPEN_PAREN parameter_list CLOSE_PAREN SEMICOLON
	{
		$$ = new function_declaration((type_specifier *) $1, (IDENTIFIER *) $2, (parameter_list *) $4);
	}
;

parameter_list:
	VOID_TYPE { $$ = new parameter_list(); }
|	parameter_list COMMA type_specifier IDENTIFIER
	{
		$$ = (parameter_list *) $1;

		((parameter_list *) $$) -> add(((type_specifier *) $3) -> getType(), ((IDENTIFIER *) $4) -> getLabel());
	}
|	type_specifier IDENTIFIER { $$ = new parameter_list((type_specifier *) $1, (IDENTIFIER *) $2); }
|	/* Empty */ { $$ = new parameter_list(); }
;

// Arguments for function calls
argument_list:
	argument_list COMMA expression
	{
		$$ = (argument_list *) $1;

		((argument_list *) $$) -> add($3);
	}
|	expression { $$ = new argument_list((expression *) $1); }
;

// Compound statement for functions, loops, etc.
compound_statement:
	OPEN_BRACKET statement_list CLOSE_BRACKET { $$ = new compound_statement((statement_list *) $2); }
|	OPEN_BRACKET CLOSE_BRACKET { $$ = new compound_statement(); }
;

statement_list:
	statement_list statement  { $$ = new statement_list((statement_list *) $1, (statement *) $2); }
|	statement  { $$ = new statement_list((statement *) $1); }
;

statement:
	expression SEMICOLON { $$ = $1; }
|	compound_statement { $$ = $1; }
|	if_statement { $$ = $1; }
|	variable_declaration_statement { $$ = $1; }
|	while_statement { $$ = $1; }
| 	do_while_statement SEMICOLON { $$ = $1; }
| 	for_statement { $$ = $1; }
|	RETURN expression SEMICOLON { $$ = new return_node((expression *) $2); }
|	RETURN SEMICOLON { $$ = new return_node(); }
|	CONTINUE SEMICOLON { $$ = new continue_node(); }
|	BREAK SEMICOLON { $$ = new break_node(); }
// | switch_statement
;

condition:
	OPEN_PAREN expression CLOSE_PAREN {$$ = new condition((expression *) $2); }
;

if_statement:
	IF condition statement %prec LOWER_THAN_ELSE { $$ = new if_statement((condition *) $2, (statement *) $3); }
|	IF condition statement ELSE statement { $$ = new if_statement((condition *) $2, (statement *) $3, (statement *) $5); }
;

while_statement:
	WHILE condition statement { $$ = new while_statement((condition *) $2, (statement *) $3); }
;

do_while_statement:
	DO compound_statement WHILE condition { $$ = new do_while_statement((compound_statement *) $2, (condition *) $4); }
;

for_statement: // Maybe i should have a statement instead of expression for first part of for
	FOR OPEN_PAREN expression SEMICOLON expression SEMICOLON expression CLOSE_PAREN compound_statement
	{ $$ = new for_statement((expression *) $3, (expression *) $5, (expression *) $7, (compound_statement *) $9); }
;

expression:
	NUMBER { $$ = (NUMBER *) $1; }
|	IDENTIFIER %prec VAR_PRIORITY { $$ = (IDENTIFIER *) $1; }
|	OPEN_PAREN expression CLOSE_PAREN { $$ = (expression *) $2; }
|	IDENTIFIER OPEN_PAREN CLOSE_PAREN { $$ = new function_call((IDENTIFIER *) $1); }
|	IDENTIFIER OPEN_PAREN argument_list CLOSE_PAREN { $$ = new function_call((IDENTIFIER *) $1, (argument_list *) $3); }
|	expression MUL expression { $$ = new multiplication((expression *) $1, (expression *) $3); }
|	expression DIV expression { $$ = new division((expression *) $1, (expression *) $3); }
|	expression PLUS expression { $$ = new addition((expression *) $1, (expression *) $3); }
|	expression MINUS expression { $$ = new subtraction((expression *) $1, (expression *) $3); }
|	expression MOD expression { $$ = new mod((expression *) $1, (expression *) $3); }
|	expression LESS expression { $$ = new less((expression *) $1, (expression *) $3);}
|	expression LESS_EQUALS expression {	$$ = new less_equals((expression *) $1, (expression *) $3); }
|	expression GREATER expression { $$ = new greater((expression *) $1, (expression *) $3); }
|	expression GREATER_EQUALS expression { $$ = new greater_equals((expression *) $1, (expression *) $3); }
|	expression LOGIC_EQUALS expression { $$ = new logic_equals((expression *) $1, (expression *) $3); }
|	expression LOGIC_NOT_EQUALS expression { $$ = new logic_not_equals((expression *) $1, (expression *) $3); }
|	expression LOGIC_AND expression { $$ = new logic_and((expression *) $1, (expression *) $3); }
|	expression LOGIC_OR expression { $$ = new logic_or((expression *) $1, (expression *) $3); }
|	expression BIT_WISE_OR expression { $$ = new bit_wise_or((expression *) $1, (expression *) $3); }
|	expression BIT_WISE_AND expression { $$ = new bit_wise_and((expression *) $1, (expression *) $3); }
|	expression BIT_WISE_XOR expression { $$ = new bit_wise_xor((expression *) $1, (expression *) $3); }
|	BIT_WISE_NOT expression { $$ = new bit_wise_not((expression *) $2); }
|	expression SHIFT_LEFT expression { $$ = new shift_left((expression *) $1, (expression *) $3); }
|	expression SHIFT_RIGHT expression { $$ = new shift_left((expression *) $1, (expression *) $3); }
|	LOGIC_NOT expression { $$ = new logic_not((expression *) $2); }
|	expression INCREMENT { $$ = new increment((expression *) $1); }
|	expression DECREMENT { $$ = new decrement((expression *) $1); }
|	IDENTIFIER EQUALS expression { $$ = new assignment((IDENTIFIER *) $1, (expression *) $3); }
|	IDENTIFIER PLUS_EQUALS expression { $$ = new plus_assignment((IDENTIFIER *) $1, (expression *) $3); }
|	IDENTIFIER MINUS_EQUALS expression { $$ = new minus_assignment((IDENTIFIER *) $1, (expression *) $3); }
|	IDENTIFIER MUL_EQUALS expression { $$ = new mul_assignment((IDENTIFIER *) $1, (expression *) $3); }
|	IDENTIFIER DIV_EQUALS expression { $$ = new div_assignment((IDENTIFIER *) $1, (expression *) $3); }
|	IDENTIFIER MOD_EQUALS expression { $$ = new mod_assignment((IDENTIFIER *) $1, (expression *) $3); }
;

type_specifier:
	INT_TYPE { $$ = new type_specifier(T_INT); }
|	FLOAT_TYPE { $$ = new type_specifier(T_FLOAT); }
|	CHAR_TYPE { $$ = new type_specifier(T_CHAR); }
|	DOUBLE_TYPE { $$ = new type_specifier(T_DOUBLE); }
|	VOID_TYPE { $$ = new type_specifier(T_VOID); }
;

%%

void yy::parser::error(const std::string& message)
{
    std::cerr << "error: " << message << std::endl;
	exit(1);
}
