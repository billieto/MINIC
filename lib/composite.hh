#pragma once
#ifndef COMPOSITE_
#define COMPOSITE_

// #include "visitor.hh"
class Visitor;
#include <list>
#include <string>
#include "types.hh"
// #include <memory>

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
    dataType m_resolved_type;
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
    STNode *getParent();
    dataType getResolvedType();

    void setParent(STNode *parent);
    void setResolvedType(dataType type);

    void printSyntaxTree(std::ofstream *dot);
    std::list<STNode *> &getChildrenList();

    virtual std::string getGraphvizLabel();
    virtual void accept(Visitor &v);
};

#endif
