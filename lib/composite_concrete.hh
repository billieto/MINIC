#pragma once
#ifndef CONCRETE_
#define CONCRETE_

#include "composite.hh"
#include <string>

class if_statement;

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

class assignment : public STNode
{
public:
    assignment(IDENTIFIER *IDENTIFIER, expression *expression);

    int evaluate() override;
};

class expression_list : public STNode
{
public:
    expression_list(expression *expression);
    //expression_list(expression_list *expression_list, expression *expression);

    int evaluate() override;
};

class statement : public STNode
{
public:
    statement(if_statement *if_statement);
    statement(expression_list *expression_list);
};

class statements : public STNode
{
public:
    statements(statements *statements, statement *statement);
    statements(statement *statement);
};

class code_block : public STNode
{
public:
    code_block(statements *statements);
    code_block(expression *expression);

    int evaluate() override;
};

class condition : public STNode
{
public:
    condition(expression_list *expression_list);

    int evaluate() override;
};

class if_statement : public STNode
{
public:
    if_statement(condition *condition, code_block *code_block);
    if_statement(condition *condition, code_block *code_block1, code_block *code_block2);
    if_statement(condition *condition, code_block *code_block1, if_statement *if_statement);
    
    int evaluate() override;
};

class compile_unit : public STNode
{
public:
    compile_unit(statements *statements);

    int evaluate() override;
};

#endif
