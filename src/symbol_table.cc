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

void SymbolTable::enterScope(int id)
{
    ScopeFrame sf{id, {}};
    scopeStack.push_back(sf);
}

void SymbolTable::exitScope()
{
    if (!scopeStack.empty())
    {
        scopeStack.pop_back();
    }
}

int SymbolTable::getCurrentId() { return scopeStack.back().getId(); }

bool SymbolTable::insertGlobal(Symbol sym)
{
    auto &globalMap = scopeStack.front();

    if (globalMap.getTable().count(sym.getName()))
    {
        return false;
    }

    globalMap.getTable()[sym.getName()] = sym;

    return true;
}

bool SymbolTable::insert(Symbol sym)
{
    auto &currentScope = scopeStack.back();

    if (currentScope.getTable().count(sym.getName()))
    {
        return false;
    }

    currentScope.getTable()[sym.getName()] = sym;

    return true;
}

Symbol *SymbolTable::lookupGlobal(std::string name)
{
    auto &globalMap = scopeStack.front();

    if (globalMap.getTable().count(name))
    {
        return &globalMap.getTable()[name];
    }

    return nullptr;
}

Symbol *SymbolTable::lookup(std::string name)
{
    int current_id = scopeStack.back().getId();

    for (int i = scopeStack.size() - 1; i >= 0; i--)
    {
        ScopeFrame &temp_frame = scopeStack[i];

        if (temp_frame.getId() != current_id)
        {
            break;
        }

        if (temp_frame.getTable().count(name))
        {
            return &temp_frame.getTable().at(name);
        }
    }

    if (scopeStack.begin()->getTable().count(name))
    {
        return &scopeStack.begin()->getTable().at(name);
    }

    return nullptr;
}

SymbolTable *SymbolTable::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new SymbolTable();
        m_instance->enterScope(0);
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

ScopeFrame::ScopeFrame(int id, std::unordered_map<std::string, Symbol> table)
{
    m_function_id = id;
    m_table = table;
}

int ScopeFrame::getId() { return m_function_id; }

std::unordered_map<std::string, Symbol> &ScopeFrame::getTable()
{
    return m_table;
}
