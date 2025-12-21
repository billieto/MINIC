#pragma once
#ifndef CONCRETE_
#define CONCRETE_

#include "composite.hh"
#include <string>

class if_statement;
class compound_statement;
class statement_list;
class statement;

class NUMBER : public STNode
{
private:
    union value
    {
	    int i;
	    double d;
    } m_value;

    enum valueType { INT, DOUBLE } m_valueType;
public:
    NUMBER(int value);
    NUMBER(double value);
    
    std::string getGraphvizLabel() override;
    int evaluate() override;
};

class IDENTIFIER : public STNode
{
private:
    std::string m_label;
    int m_value;
    // union value
    // {
	//     int i;
	//     double d;
    // } m_value;
    // // Isws tha prepei na exw kai mia trith timh dioti kalo einai na mhn ginetai 2 fores set?? mporei  
    // enum valueType { INT, DOUBLE } m_valueType;
public:
    IDENTIFIER(std::string str);
    
    void setValue(int value);
    int getValue();

    std::string getGraphvizLabel() override;
    int evaluate() override;
};

class expression : public STNode
{
public:
    expression(NUMBER *NUMBER);
    expression(IDENTIFIER *IDENTIFIER);
};

class multiplication : public STNode
{
public:
    multiplication(expression *left, expression *right);

    int evaluate() override;
};

class division : public STNode
{
public:
    division(expression *left, expression *right);

    int evaluate() override;
};

class addition : public STNode
{
public:
    addition(expression *left, expression *right);

    int evaluate() override;
};

class subtraction : public STNode
{
public:
    subtraction(expression *left, expression *right);

    int evaluate() override;
};

class less : public STNode
{
public:
    less(expression *left, expression *right);

    int evaluate() override;
};

class less_equals : public STNode
{
public:
    less_equals(expression *left, expression *right);
    
    int evaluate() override;
};

class greater : public STNode
{
public:
    greater(expression *left, expression *right);
    
    int evaluate() override;
};

class greater_equals : public STNode
{
public:
    greater_equals(expression *left, expression *right);
    
    int evaluate() override;
};

class logic_equals : public STNode
{
public:
    logic_equals(expression *left, expression *right);

    int evaluate() override;
};

class logic_and : public STNode
{
public:
    logic_and(expression *left, expression *right);

    int evaluate() override;
};

class logic_or : public STNode
{
public:
    logic_or(expression *left, expression *right);

    int evaluate() override;
};

class logic_not : public STNode
{
public:
    logic_not(expression *expression);

    int evaluate() override;
};


class assignment : public STNode
{
public:
    assignment(IDENTIFIER *IDENTIFIER, expression *expression);

    int evaluate() override;
};

class compound_statement : public STNode
{
public:
    compound_statement(statement_list *statement_list);
    compound_statement();

    int evaluate() override;
};

class statement_list : public STNode
{
public:
    statement_list(statement_list *statement_list, statement *statement);
    statement_list(statement *statement);

    int evaluate() override;
};

class statement : public STNode
{
public:
    statement(if_statement *if_statement);
    statement(expression *expression);
    statement(compound_statement *compound_statement);

    int evaluate() override;
};

class condition : public STNode
{
public:
    condition(expression *expression);

    int evaluate() override;
};

class if_statement : public STNode
{
public:
    if_statement(condition *condition, statement_list *statement_list);
    if_statement(condition *condition, statement_list *statement_list1, statement_list *statement_list2);
    
    int evaluate() override;
};

class compile_unit : public STNode
{
public:
    compile_unit(statement_list *statement_list);

    int evaluate() override;
};

#endif
