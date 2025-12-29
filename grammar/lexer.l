%option noyywrap nounput noinput
%option yylineno

%{
#include "parser.tab.hh"
#include "composite.hh"
#include "composite_concrete.hh"
#include "symbol_table.hh"
#include <string.h>

#define YY_DECL int yylex(yy::parser::semantic_type *yylval)

typedef yy::parser::token token;
%}

%x SINGLE_LINE_COMMENT
%x MULTI_LINE_COMMENT

ID [a-zA-Z_][a-zA-Z0-9_]*

%%

"++" { return token::INCREMENT; }
"--" { return token::DECREMENT; }

"=" { return token::EQUALS; }
"+" { return token::PLUS; }
";" { return token::SEMICOLON; }
"-" { return token::MINUS; }
"*" { return token::MUL; }
"/" { return token::DIV; }
"%" { return token::MOD; }

"+=" { return token::PLUS_EQUALS; }
"-=" { return token::MINUS_EQUALS; }
"*=" { return token::MUL_EQUALS; }
"/=" { return token::DIV_EQUALS; }
"%=" { return token::MOD_EQUALS; }

"(" { return token::OPEN_PAREN; }
")" { return token::CLOSE_PAREN; }
"{" { return token::OPEN_BRACKET; }
"}" { return token::CLOSE_BRACKET; }
"if"    { return token::IF; }
"else"  { return token::ELSE; }
"," { return token::COMMA; }

"<" { return token::LESS; }
"<="  { return token::LESS_EQUALS; }  
">" { return token::GREATER; }
">="    { return token::GREATER_EQUALS; }

"=="    { return token::LOGIC_EQUALS; }
"&&"    { return token::LOGIC_AND; }
"||"    { return token::LOGIC_OR; }
"!" { return token::LOGIC_NOT; }

"|" { return token::BIT_WISE_OR; }
"&" { return token::BIT_WISE_AND; }
"^" { return token::BIT_WISE_XOR; }
"~" { return token::BIT_WISE_NOT; }
"<<" { return token::SHIFT_LEFT; }
">>" { return token::SHIFT_RIGHT; }

"int" { return token::INT_TYPE; }
"double" { return token::DOUBLE_TYPE; }
"float" { return token::FLOAT_TYPE; }
"char" { return token::CHAR_TYPE; }
"void" { return token::VOID_TYPE; }
"return" { return token::RETURN; }
 /* "break" { return token::BREAK; }
    "continue" { return token::continue; }*/

[0-9]+  { // TODO: If i want type system i need to make float numbers and in the constructor of the NUMBER class i need to sent its type.
            yylval -> node = new NUMBER(atoi(yytext));
			return token::NUMBER;
        }

{ID}    {
            yylval -> node = new IDENTIFIER(yytext);
            return token::IDENTIFIER;
        }   

 /* "#" { BEGIN(PRE_PROCESSOR); }
<PRE_PROCESSOR>"include "ID { }
<PRE_PROCESSOR>"define "ID { }
<PRE_PROCESSOR>"\n" { BEGIN(INITIAL); } */

"//"    { BEGIN(SINGLE_LINE_COMMENT); }
<SINGLE_LINE_COMMENT>.
<SINGLE_LINE_COMMENT>"\n" { BEGIN(INITIAL); }

"/*"    { BEGIN(MULTI_LINE_COMMENT); }
<MULTI_LINE_COMMENT>.
<MULTI_LINE_COMMENT>"*/" { BEGIN(INITIAL); }

[ \t\n] { /* skip whitespace */ }

%%
