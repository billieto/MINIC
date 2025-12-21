#pragma once
#ifndef COMPOSITE_
#define COMPOSITE_

#include <string>
#include <list>
//#include <memory>

typedef enum nodeType
{
    COMPILE_UNITE_NODE,
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
    SUBTRACTION_NODE
} nodeType;

class STNode 
{
private:
    nodeType m_nodeType;
    std::string m_graphvizLabel;
    int m_serial;
    static int m_serialCounter;
    //std::list<std::unique_ptr<STNode>> m_children; // NA TO KOITAKSW OTAN PAW TRIPOLI APO TO STRUSTAP
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
    } // EKANA ALLAGH EDW! to ekana virtual default kai ekana to list xwris dikti gia na diaxirizete mono tou to delete

    nodeType getNodeType();
    void setParent(STNode *parent);
    void printSyntaxTree(std::ofstream *dot);
    std::list<STNode *>& getChildrenList();

    virtual std::string getGraphvizLabel();
    virtual int evaluate();
};

#endif