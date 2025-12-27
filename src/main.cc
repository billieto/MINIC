#include <cstdlib>
#include <fstream>
#include <iostream>

#include "../lib/parser.tab.hh"
//#include "../lib/lexer.hh"


extern STNode *g_root;
extern FILE *yyin;

int main(int argc, char *argv[])
{
    yy::parser parser;
    std::ofstream *dot;

    if (argc != 2)
    {
        std::cerr << "Wrong number of arguments" << std::endl;
        exit(1);
    }

    yyin = fopen(argv[1], "r");

    parser.parse();

    // Syntax Tree
    dot = new std::ofstream("ST.dot", std::ofstream::out);
    (*dot) << "digraph ST\n{\n";
    g_root -> printSyntaxTree(dot);
    (*dot) << "}";
    dot -> close();
    
    Symbol *entry = SymbolTable::getInstance() -> lookupGlobal("main");
    
    if (entry == nullptr)
    {   // Wanna be linker error XD
        std::cout << "Error: Linker error - undefined reference to 'main'" << std::endl;
    }
    else
    {
        try
        {
            SymbolTable::getInstance() -> enterScope(); // Main scope
            entry -> getFunctionBody() -> evaluate();
        }
        catch (int mainReturn)
        {
            if (!mainReturn)
            {
                std::cout << "Super!" << std::endl;
            }
            else
            {
                std::cout << "Not Super" << std::endl;
            }

            SymbolTable::getInstance() -> exitScope();
        }
    }

    return 0;
}