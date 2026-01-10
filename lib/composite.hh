#pragma once
#ifndef COMPOSITE_
#define COMPOSITE_

// #include "visitor.hh"
class Visitor;
#include <list>
#include <string>
// #include <memory>

typedef enum nodeType
{
    PROGRAM_NODE,
    COMPMOUNT_STATEMENT_NODE,
    STATEMENT_LIST_NODE,
    STATEMENT_NODE,
    CONDITION_NODE,
    IF_STATEMENT_NODE,
    NUMBER_NODE,
    EXPRESSION_NODE,
    IDENTIFIER_NODE,
    ASSIGNMENT_NODE,
    MULTIPLICATION_NODE,
    DIVISION_NODE,
    ADDITION_NODE,
    SUBTRACTION_NODE,
    LESS_NODE,
    LESS_EQUALS_NODE,
    GREATER_NODE,
    GREATER_EQUALS_NODE,
    LOGIC_EQUALS_NODE,
    LOGIC_AND_NODE,
    LOGIC_OR_NODE,
    LOGIC_NOT_NODE,
    LOGIC_NOT_EQUALS_NODE,
    INCREMENT_NODE,
    DECREMENT_NODE,
    TYPE_SPECIFIER_NODE,
    VARIABLE_DECLARATION_NODE,
    FUNCTION_CALL_NODE,
    FUNCTION_DEFINITION_NODE,
    PARAMETER_LIST_NODE,
    ARGUMENT_LIST_NODE,
    FUNCTION_DECLARATION_NODE,
    EXTERNAL_DECLARATION_NODE,
    TRANSLATION_UNIT_NODE,
    RETURN_NODE,
    VARIABLE_DECLARATION_LIST_NODE,
    VARIABLE_DECLARATION_STATEMENT_NODE,
    BIT_WISE_OR_NODE,
    BIT_WISE_AND_NODE,
    BIT_WISE_XOR_NODE,
    BIT_WISE_NOT_NODE,
    SHIFT_LEFT_NODE,
    SHIFT_RIGHT_NODE,
    PLUS_ASSIGNMENT_NODE,
    MINUS_ASSIGNMENT_NODE,
    MUL_ASSIGNMENT_NODE,
    DIV_ASSIGNMENT_NODE,
    MOD_ASSIGNMENT_NODE,
    MOD_NODE,
    WHILE_STATEMENT_NODE,
    CONTINUE_NODE,
    BREAK_NODE,
    DO_WHILE_STATEMENT_NODE,
    FOR_STATEMENT_NODE
} nodeType;

typedef int Value;
// class Value
// {
// private:
//     dataType m_type;
//     union
//     {
//         int i;
//         float f;
//         double d;
//         char c;
//     } m_value
//     ;
// public:
//     Value();
// };

class STNode
{
  private:
    nodeType m_nodeType;
    std::string m_graphvizLabel;
    int m_serial;
    static int m_serialCounter;
    // std::list<std::unique_ptr<STNode>> m_children;
    std::list<STNode *> m_children;
    STNode *m_parent;

  public:
    STNode(nodeType nodeType, std::initializer_list<STNode *> children);
    virtual ~STNode()
    {
        for (STNode *child : m_children)
        {
            delete child;
        }

        m_children.clear();
    }

    nodeType getNodeType();
    void setParent(STNode *parent);
    void printSyntaxTree(std::ofstream *dot);
    std::list<STNode *> &getChildrenList();

    virtual std::string getGraphvizLabel();
    virtual int evaluate();
    virtual void accept(Visitor &v);
};

#endif
