#pragma once
#ifndef COMPOSITE_
#define COMPOSITE_

#include "types.hh"
#include <list>
#include <string>

class Visitor;

class STNode
{
  private:
    dataType m_resolved_type;
    nodeType m_nodeType;
    std::string m_graphvizLabel;
    unsigned int m_serial;
    static int m_serialCounter;
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
