#include "../lib/symbol_table.hh"
#include <string>
#include <vector>

SymbolTable *SymbolTable::m_instance = nullptr;

SymbolTable::SymbolTable() {}

SymbolTable::~SymbolTable()
{
    exitScope();
    delete m_instance;
}

void SymbolTable::enterScope() { scopeStack.push_back({}); }

void SymbolTable::exitScope()
{
    if (!scopeStack.empty())
    {
        scopeStack.pop_back();
    }
}

bool SymbolTable::insertGlobal(Symbol sym)
{
    auto &globalMap = scopeStack.front();

    if (globalMap.count(sym.getName()))
    {
        return false;
    }

    globalMap[sym.getName()] = sym;

    return true;
}

bool SymbolTable::insert(Symbol sym)
{
    auto &currentScope = scopeStack.back();

    if (currentScope.count(sym.getName()))
    {
        return false;
    }

    currentScope[sym.getName()] = sym;

    return true;
}

Symbol *SymbolTable::lookupGlobal(std::string name)
{
    auto &globalMap = scopeStack.front();

    if (globalMap.count(name))
    {
        return &globalMap[name];
    }

    return nullptr;
}

Symbol *SymbolTable::lookup(std::string name)
{
    for (int i = scopeStack.size() - 1; i >= 0; i--)
    {
        if (scopeStack[i].count(name))
        {
            return &scopeStack[i][name];
        }
    }
    return nullptr;
}

SymbolTable *SymbolTable::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new SymbolTable();
        SymbolTable::getInstance()->enterScope(); // Initialize Global Scope
    }
    return m_instance;
}

Symbol::Symbol()
{
    m_is_function = false;
    m_function_body = nullptr;
    m_params.clear();
}

Symbol::~Symbol() {}

void Symbol::setParameters(std::vector<parameter> params)
{
    for (const auto &param : params)
    {
        m_params.push_back(param);
    }
}

void Symbol::setIsFunction(bool is_function) { m_is_function = is_function; }

void Symbol::setName(std::string name) { m_name = name; }

void Symbol::setType(dataType type) { m_type = type; }

void Symbol::setValue(int value) { m_value = value; }

void Symbol::setFunctionBody(STNode *function_body)
{
    m_function_body = function_body;
}

std::string Symbol::getName() { return m_name; }

bool Symbol::getIsFunction() { return m_is_function; }

dataType Symbol::getType() { return m_type; }

std::vector<parameter> &Symbol::getParameters() { return m_params; }

int Symbol::getValue() { return m_value; }

STNode *Symbol::getFunctionBody() { return m_function_body; }
