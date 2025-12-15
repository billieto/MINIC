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


[0-9]+  {
            yylval -> node = new NUMBER(atoi(yytext));
			return token::NUMBER;
        }

[a-zA-Z_][a-zA-Z0-9_]*  {
                            if (!SymbolTable::getInstance() -> lookup(yytext))
                            {
                                yylval -> node = new IDENTIFIER(yytext);
                                SymbolTable::getInstance() -> insert(yytext, yylval -> node);
                            }
                            else
                            {
                                yylval -> node = SymbolTable::getInstance() -> getNode(yytext);
                            }

                            return token::IDENTIFIER;
                        }

[ \t\n] { /* skip whitespace */ }

%%
