#include "../lib/composite.hh"
#include <cstdarg>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <list>

STNode *g_root = NULL;
int STNode::m_serialCounter = 0;
std::string g_nodeTypeLabels[] = {"PROGRAM",
                                  "COMPOUND_STATEMENT",
                                  "STATEMENT_LIST",
                                  "STATEMENT",
                                  "CONDITION",
                                  "IF_STATEMENT",
                                  "NUMBER",
                                  "EXPRESSION",
                                  "IDENTIFIER",
                                  "ASSIGNMENT",
                                  "MULTIPLICATION",
                                  "DIVISION",
                                  "ADDITION",
                                  "SUBTRACTION",
                                  "LESS",
                                  "LESS_EQUALS",
                                  "GREATER",
                                  "GREATER_EQUALS",
                                  "LOGIC_EQUALS",
                                  "LOGIC_AND",
                                  "LOGIC_OR",
                                  "LOGIC_NOT",
                                  "LOGIC_NOT_EQUALS",
                                  "INCREMENT",
                                  "DECREMENT",
                                  "TYPE_SPECIFIER",
                                  "VARIABLE_DECLARATION",
                                  "FUNCTION_CALL",
                                  "FUNCTION_DEFINITION",
                                  "PARAMETER_LIST",
                                  "ARGUMENT_LIST",
                                  "FUNCTION_DECLARATION",
                                  "EXTERNAL_DECLARATION",
                                  "TRANSLATION_UNIT",
                                  "RETURN_NODE",
                                  "VARIABLE_DECLARATION_LIST",
                                  "VARIABLE_DECLARATION_STATEMENT"};

STNode::STNode(nodeType nodeType, std::initializer_list<STNode *> children)
{
    m_nodeType = nodeType;
    m_serial = m_serialCounter++;
    m_graphvizLabel = g_nodeTypeLabels[m_nodeType];

    for (const auto &child : children)
    {
        m_children.push_back(child);
        child->setParent(this);
    }
}

nodeType STNode::getNodeType() { return m_nodeType; }

std::string STNode::getGraphvizLabel()
{
    return (m_graphvizLabel + "_" + std::to_string(m_serial));
}

void STNode::setParent(STNode *parent) { m_parent = parent; }

void STNode::printSyntaxTree(std::ofstream *dot)
{
    std::cout << "Visiting node " << m_graphvizLabel << std::endl;

    // Parent is responsible for printing edges to its children
    for (const auto &child : m_children)
    {
        (*dot) << "\"" << getGraphvizLabel() << "\"->\""
               << child->getGraphvizLabel() << "\";\n";
        child->printSyntaxTree(dot);
    }
}

std::list<STNode *> &STNode::getChildrenList() { return m_children; }

int STNode::evaluate()
{
    for (const auto &child : m_children)
    {
        child->evaluate();
    }

    return 0;
}