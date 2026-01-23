#include <cstdlib>
#include <fstream>
#include <iostream>

#include "../lib/declarator_visitor.hh"
#include "../lib/evaluator_visitor.hh"
#include "../lib/ir_emitter_visitor.hh"
#include "../lib/parser.tab.hh"
#include "../lib/type_checker_visitor.hh"
// #include "../lib/lexer.hh"

extern STNode *g_root;
extern FILE *yyin;

/*
 *  Notes for me to try or to change:
 *  1) Do a big test on my compiler, like big input file and tricky things (mid prio)
 *  2) Maybe rearrange the Syntax Tree for better grammar, like binary op, unary op etc. (med prio)
 *  3) Migrate to AST (med prio)
 *  4) Support char and/or string (low prio)
 *  5) Better error messages for Lexer, Parser, Semantic, Runtime errors (high prio)
 *  6) Maybe expand my files? (low prio)
 */

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
    *dot << "digraph ST\n{\n";
    g_root->printSyntaxTree(dot);
    *dot << "}";
    dot->close();
    delete dot;

    // Visitor way
    TypeCheckerVisitor tc;
    g_root->accept(tc);

    IREmitterVisitor ir;
    g_root->accept(ir);

    // DeclaratorVisitor decl;
    // g_root->accept(decl);

    // EvaluatorVisitor eval;
    // g_root->accept(eval);

    delete g_root;

    return 0;
}
