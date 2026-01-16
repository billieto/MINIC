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

bool SymbolTable::insertGlobal(Symbol *sym)
{
    auto &globalMap = scopeStack.front();

    if (globalMap.getTable().count(sym->getName()))
    {
        return false;
    }

    globalMap.getTable()[sym->getName()] = sym;

    return true;
}

bool SymbolTable::insert(Symbol *sym)
{
    auto &currentScope = scopeStack.back();

    if (currentScope.getTable().count(sym->getName()))
    {
        return false;
    }

    currentScope.getTable()[sym->getName()] = sym;

    return true;
}

Symbol *SymbolTable::lookupGlobal(std::string name)
{
    auto &globalMap = scopeStack.front();

    if (globalMap.getTable().count(name))
    {
        return globalMap.getTable()[name];
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
            return temp_frame.getTable().at(name);
        }
    }

    if (scopeStack.begin()->getTable().count(name))
    {
        return scopeStack.begin()->getTable().at(name);
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

Symbol::Symbol(std::string name, SymbolType type)
{
    m_name = name;
    m_type = type;
}

SymbolType Symbol::getType() { return m_type; }

std::string Symbol::getName() { return m_name; }

void Symbol::setType(SymbolType type) { m_type = type; }

void Symbol::setName(std::string name) { m_name = name; }

FuncSymbol::FuncSymbol(dataType return_type, STNode *body,
                       std::vector<parameter> params, std::string name)
    : Symbol(name, FUNC_SYM)
{
    m_return_type = return_type;
    m_function_body = body;
    m_params = params;
}

dataType FuncSymbol::getReturnType() { return m_return_type; }
STNode *FuncSymbol::getFunctionBody() { return m_function_body; }
std::vector<parameter> &FuncSymbol::getParameters() { return m_params; }

void FuncSymbol::setReturnType(dataType return_type)
{
    m_return_type = return_type;
}

void FuncSymbol::setFunctionBody(STNode *body) { m_function_body = body; }

void FuncSymbol::setParameters(std::vector<parameter> params)
{
    m_params = params;
}

ScopeFrame::ScopeFrame(int id, std::unordered_map<std::string, Symbol *> table)
{
    m_function_id = id;
    m_table = table;
}

VarSymbol::VarSymbol(Value value, std::string name, dataType type)
    : Symbol(name, VAR_SYM)
{
    m_value = value;
    m_value_type = type;
}

Value VarSymbol::getValue() { return m_value; }

dataType VarSymbol::getValueType() { return m_value_type;}

void VarSymbol::setValue(Value value) { m_value = value; }

int ScopeFrame::getId() { return m_function_id; }

std::unordered_map<std::string, Symbol *> &ScopeFrame::getTable()
{
    return m_table;
}
