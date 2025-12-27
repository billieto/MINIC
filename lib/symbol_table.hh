#pragma once
#ifndef SYMBOL_TABLE_
#define SYMBOL_TABLE_

#include "composite.hh"
#include <string>
#include <vector>
#include <unordered_map>

typedef enum
{
    T_INT,
    T_CHAR,
    T_FLOAT,
    T_DOUBLE,
    T_VOID
} dataType;

typedef struct param
{
    dataType type;
    std::string name;

    // Gemini told me to add them because a vector cant compare, so i should i make a method that sees if the parameters are the same as arguments.
    bool operator==(const param& other) const
    {
        return (this->type == other.type) && (this->name == other.name);
    }
    
    bool operator!=(const param& other) const
    {
        return !(*this == other);
    }
} parameter;

class Symbol
{
private:
    std::string m_name;
    dataType m_type;// if its a variable, this shows its type. If its a function, this shows its return value
    int m_value;

    bool m_is_function;
    std::vector<parameter> m_params;
    STNode *m_function_body;

public:
    Symbol();
    ~Symbol();

    void setIsFunction(bool is_function);
    void setParameters(std::vector<parameter> params);
    void setName(std::string name);
    void setType(dataType type);
    void setValue(int value);
    void setFunctionBody(STNode *function_body);

    bool getIsFunction();
    std::string getName();
    dataType getType();
    std::vector<parameter>& getParameters();
    int getValue();
    STNode* getFunctionBody();
};

class SymbolTable
{
private:
    SymbolTable();

    static SymbolTable *m_instance;
    std::vector<std::unordered_map<std::string, Symbol>> scopeStack;

public:
    ~SymbolTable();

    static SymbolTable* getInstance();
    void enterScope();
    void exitScope();
    bool insertGlobal(Symbol sym);
    bool insert(Symbol sym);
    Symbol* lookupGlobal(std::string name);
	Symbol* lookup(std::string name);
};

#endif