#include "../lib/symbol_table.hh"
#include <string>

SymbolTable* SymbolTable::m_instance = nullptr;

SymbolTable::SymbolTable() { }


void SymbolTable::insert(std::string name, STNode *node)
{
    m_table[name] = node;
}

bool SymbolTable::lookup(std::string name)
{
    if (m_table.find(name) != m_table.end())
    {
        return true;
    }
    return false;
}

SymbolTable* SymbolTable::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new SymbolTable();
    }
    return m_instance;
}

STNode* SymbolTable::getNode(std::string name)
{
    return m_table.at(name);
}
