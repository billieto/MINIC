#include "../lib/symbol_table.hh"
#include <string>
#include <vector>

SymbolTable *SymbolTable::m_instance = nullptr;

SymbolTable::SymbolTable() { enterScope(0); }

SymbolTable::~SymbolTable()
{
    exitScope();
    delete m_instance;
}

void SymbolTable::enterScope(int id)
{
    scopeStack.push_back(std::make_unique<ScopeFrame>(id));
}

void SymbolTable::exitScope()
{
    if (!scopeStack.empty())
    {
        scopeStack.pop_back();
    }
}

int SymbolTable::getCurrentId() { return scopeStack.back()->getId(); }

bool SymbolTable::insertGlobal(Symbol *sym)
{
    if (scopeStack.empty())
        return false;

    auto &globalScopePtr = scopeStack.front();

    return globalScopePtr->insert(sym);
}

bool SymbolTable::insert(Symbol *sym)
{
    if (scopeStack.empty())
    {
        return false;
    }

    auto &currentScopePtr = scopeStack.back();

    return currentScopePtr->insert(sym);
}

Symbol *SymbolTable::lookupGlobal(std::string name)
{
    if (scopeStack.empty())
    {
        return nullptr;
    }

    return scopeStack.front()->lookup(name);
}

Symbol *SymbolTable::lookup(std::string name)
{
    if (scopeStack.empty())
        return nullptr;

    int current_id = scopeStack.back()->getId();

    for (int i = scopeStack.size() - 1; i >= 0; i--)
    {
        auto &temp_frame_ptr = scopeStack[i];

        if (temp_frame_ptr->getId() != current_id)
        {
            break;
        }

        Symbol *sym = temp_frame_ptr->lookup(name);
        if (sym != nullptr)
        {
            return sym;
        }
    }

    // global loopup
    return scopeStack.front()->lookup(name);
}

SymbolTable *SymbolTable::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new SymbolTable();
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

ScopeFrame::ScopeFrame(int id) { m_function_id = id; }

bool ScopeFrame::insert(Symbol *sym)
{
    // Check if it exists
    if (m_table.count(sym->getName()))
    {
        return false;
    }

    // TRANSFER OWNERSHIP:
    // Wrap the raw pointer 'sym' into a unique_ptr and store it.
    m_table[sym->getName()] = std::unique_ptr<Symbol>(sym);
    return true;
}

Symbol *ScopeFrame::lookup(std::string name)
{
    if (m_table.count(name))
    {
        // .get() returns the raw pointer (observer) without transferring
        // ownership
        return m_table[name].get();
    }
    return nullptr;
}

VarSymbol::VarSymbol(Value value, std::string name, dataType type)
    : Symbol(name, VAR_SYM)
{
    m_value = value;
    m_value_type = type;
    // m_ir_addr = this->getName();
}

Value VarSymbol::getValue() { return m_value; }

dataType VarSymbol::getValueType() { return m_value_type; }

std::string VarSymbol::getAddress() { return m_ir_addr; }

void VarSymbol::setValue(Value value) { m_value = value; }

void VarSymbol::setAddress(std::string addr) { m_ir_addr = addr; }

int ScopeFrame::getId() { return m_function_id; }
