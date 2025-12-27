#pragma once
#ifndef COMPOSITE_
#define COMPOSITE_

#include <string>
#include <list>
//#include <memory>

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
    VARIABLE_DECLARATION_STATEMENT_NODE

} nodeType;

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
    //std::list<std::unique_ptr<STNode>> m_children;
    std::list<STNode *> m_children;
    STNode *m_parent;
public:
    STNode(nodeType nodeType, std::initializer_list<STNode *> children);
    virtual ~STNode()
    {
        for (STNode* child : m_children)
        {
            delete child;
        }
    
        m_children.clear();
    }

    nodeType getNodeType();
    void setParent(STNode *parent);
    void printSyntaxTree(std::ofstream *dot);
    std::list<STNode *>& getChildrenList();

    virtual std::string getGraphvizLabel();
    virtual int evaluate();
};

#endif