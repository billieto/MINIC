#include <cstdlib>
#include <fstream>
#include <iostream>

#include "../lib/evaluator_visitor.hh"
#include "../lib/declarator_visitor.hh"
#include "../lib/parser.tab.hh"
// #include "../lib/lexer.hh"

extern STNode *g_root;
extern FILE *yyin;

int main(int argc, char *argv[])
{
    yy::parser parser;
    std::ofstream *dot;

    // Maybe i could support multiple files
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
    g_root->printSyntaxTree(dot);
    (*dot) << "}";
    dot->close();
    delete dot;

    // Polymorphic way
    // try
    // {
    //     g_root->evaluate();
    // }
    // catch (int mainReturn)
    // {
    //     if (!mainReturn)
    //     {
    //         std::cout << "Super!" << std::endl;
    //     }
    //     else
    //     {
    //         std::cout << "Not Super" << std::endl;
    //     }
    // }

    // Visitor way
    DeclaratorVisitor decl;
    g_root->accept(decl);

    EvaluatorVisitor eval;
    g_root->accept(eval);

    return 0;
}
