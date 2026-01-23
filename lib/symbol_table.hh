#pragma once
#ifndef SYMBOL_TABLE_
#define SYMBOL_TABLE_

#include "composite.hh"
#include "types.hh"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

typedef int Value;

struct parameter
{
    dataType type;
    std::string name;

    // Gemini told me to add them because a vector cant compare, so i should i
    // make a method that sees if the parameters are the same as arguments.
    bool operator==(const parameter &other) const
    {
        return (this->type == other.type) && (this->name == other.name);
    }

    bool operator!=(const parameter &other) const { return !(*this == other); }
};

class Symbol
{
  private:
    std::string m_name;
    SymbolType m_type;

  public:
    Symbol(std::string name, SymbolType type);
    virtual ~Symbol() = default;

    SymbolType getType();
    std::string getName();

    void setName(std::string name);
    void setType(SymbolType type);
};

class FuncSymbol : public Symbol
{
  private:
    std::vector<parameter> m_params;
    STNode *m_function_body;
    dataType m_return_type;

  public:
    FuncSymbol(dataType return_type, STNode *body,
               std::vector<parameter> params, std::string name);
    // Maybe i should make &params because i want a copy of existing data in the
    // memory
    dataType getReturnType();
    STNode *getFunctionBody();
    std::vector<parameter> &getParameters();

    void setReturnType(dataType return_type);
    void setFunctionBody(STNode *body);
    void setParameters(std::vector<parameter> params);
};

class VarSymbol : public Symbol
{
  private:
    Value m_value;
    dataType m_value_type;
    std::string m_ir_addr;

  public:
    VarSymbol(Value value, std::string name, dataType type);

    Value getValue();
    dataType getValueType();
    std::string getAddress();

    void setValue(Value value);
    void setAddress(std::string addr);
};

class ScopeFrame
{
  private:
    int m_function_id;
    std::unordered_map<std::string, std::unique_ptr<Symbol>> m_table;

  public:
    ScopeFrame(int id);

    int getId();
    bool insert(Symbol *sym);
    Symbol *lookup(std::string name);
};

class SymbolTable
{
  private:
    SymbolTable();

    static SymbolTable *m_instance;
    std::vector<std::unique_ptr<ScopeFrame>> scopeStack;

  public:
    ~SymbolTable();

    static SymbolTable *getInstance();
    int getCurrentId();
    void enterScope(int id);
    void exitScope();
    bool insertGlobal(Symbol *sym);
    bool insert(Symbol *sym);
    Symbol *lookupGlobal(std::string name);
    Symbol *lookup(std::string name);
};

#endif
