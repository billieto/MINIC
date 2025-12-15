#pragma once
#ifndef SYMBOL_TABLE_
#define SYMBOL_TABLE_

#include <unordered_map>
#include "composite.hh"

class SymbolTable
{
private:
    SymbolTable();

    static SymbolTable *m_instance;
    std::unordered_map<std::string, STNode *> m_table;

public:
    static SymbolTable* getInstance();
    void insert(std::string name, STNode* node);
	bool lookup(std::string name);
    STNode* getNode(std::string name);
};

#endif