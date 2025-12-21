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
%x PRE_PROCESSOR

ID [a-zA-Z_][a-zA-Z0-9_]*

%%

"+" { return token::PLUS; }
";" { return token::SEMICOLON; }
"=" { return token::EQUALS; }
"-" { return token::MINUS; }
"*" { return token::MUL; }
"/" { return token::DIV; }
"(" { return token::OPEN_PAREN; }
")" { return token::CLOSE_PAREN; }
"{" { return token::OPEN_BRACKET; }
"}" { return token::CLOSE_BRACKET; }
"if"    { return token::IF; }
"else"  { return token::ELSE; }

"<" { return token::LESS; }
"<="  { return token::LESS_EQUALS; }  
">" { return token::GREATER; }
">="    { return token::GREATER_EQUALS; }
"=="    { return token::LOGIC_EQUALS; }
"&&"    { return token::LOGIC_AND; }
"||"    { return token::LOGIC_OR; }
"!" { return token::LOGIC_NOT; }

[0-9]+  {
            yylval -> node = new NUMBER(atoi(yytext));
			return token::NUMBER;
        }

{ID}  {
        if (!SymbolTable::getInstance() -> lookup(yytext))
        {
            yylval -> node = new IDENTIFIER(yytext);
            SymbolTable::getInstance() -> insert(yytext, yylval -> node);
        }
        else
        {
            yylval -> node = SymbolTable::getInstance() -> getNode(yytext);

        return token::IDENTIFIER;
        }
    }

"#" { BEGIN(PRE_PROCESSOR); }
<PRE_PROCESSOR>"include "ID { }
<PRE_PROCESSOR>"define "ID { }
<PRE_PROCESSOR>"\n" { BEGIN(INITIAL); }

"//"    { BEGIN(SINGLE_LINE_COMMENT); }
<SINGLE_LINE_COMMENT>.
<SINGLE_LINE_COMMENT>"\n" { BEGIN(INITIAL); }

"/*"    { BEGIN(MULTI_LINE_COMMENT); }
<MULTI_LINE_COMMENT>.
<MULTI_LINE_COMMENT>"*/" { BEGIN(INITIAL); }

[ \t\n] { /* skip whitespace */ }

%%
