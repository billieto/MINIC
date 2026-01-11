#pragma once
#ifndef CONCRETE_
#define CONCRETE_

#include "composite.hh"
#include "symbol_table.hh"
#include <string>
#include <vector>

class if_statement;
class compound_statement;
class statement_list;
class statement;

class NUMBER : public STNode
{
  private:
    int m_value;

  public:
    NUMBER(int value);

    std::string getGraphvizLabel() override;
    Value getValue();
    // int evaluate() override;

    void accept(Visitor &v) override;
};

class IDENTIFIER : public STNode
{
  private:
    std::string m_label;

  public:
    IDENTIFIER(std::string str);

    std::string getLabel();

    std::string getGraphvizLabel() override;
    // int evaluate() override;
    void accept(Visitor &v) override;
};

class type_specifier : public STNode
{
  private:
    dataType m_type;

  public:
    type_specifier(dataType type);

    dataType getType();
    void accept(Visitor &v) override;
};

class expression : public STNode
{
  public:
    expression(NUMBER *NUMBER);
    expression(IDENTIFIER *IDENTIFIER);
    void accept(Visitor &v) override;
};

class multiplication : public STNode
{
  public:
    multiplication(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class division : public STNode
{
  public:
    division(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class addition : public STNode
{
  public:
    addition(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class subtraction : public STNode
{
  public:
    subtraction(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class less : public STNode
{
  public:
    less(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class less_equals : public STNode
{
  public:
    less_equals(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class greater : public STNode
{
  public:
    greater(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class greater_equals : public STNode
{
  public:
    greater_equals(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class logic_equals : public STNode
{
  public:
    logic_equals(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class logic_not_equals : public STNode
{
  public:
    logic_not_equals(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class logic_and : public STNode
{
  public:
    logic_and(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class logic_or : public STNode
{
  public:
    logic_or(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class logic_not : public STNode
{
  public:
    logic_not(expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class increment : public STNode
{
  public:
    increment(expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class decrement : public STNode
{
  public:
    decrement(expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class assignment : public STNode
{
  public:
    assignment(IDENTIFIER *IDENTIFIER, expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class bit_wise_or : public STNode
{
  public:
    bit_wise_or(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class bit_wise_and : public STNode
{
  public:
    bit_wise_and(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class bit_wise_xor : public STNode
{
  public:
    bit_wise_xor(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class bit_wise_not : public STNode
{
  public:
    bit_wise_not(expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class shift_left : public STNode
{
  public:
    shift_left(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class shift_right : public STNode
{
  public:
    shift_right(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class plus_assignment : public STNode
{
  public:
    plus_assignment(IDENTIFIER *IDENTIFIER, expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class minus_assignment : public STNode
{
  public:
    minus_assignment(IDENTIFIER *IDENTIFIER, expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class mul_assignment : public STNode
{
  public:
    mul_assignment(IDENTIFIER *IDENTIFIER, expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class div_assignment : public STNode
{
  public:
    div_assignment(IDENTIFIER *IDENTIFIER, expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class mod_assignment : public STNode
{
  public:
    mod_assignment(IDENTIFIER *IDENTIFIER, expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class mod : public STNode
{
  public:
    mod(expression *left, expression *right);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class variable_declaration : public STNode
{
  private:
    std::string m_name;
    int m_value;

  public:
    variable_declaration(IDENTIFIER *IDENTIFIER);
    variable_declaration(IDENTIFIER *IDENTIFIER, expression *expression);

    // int evaluate() override;
    std::string getName();
    int getValue();
    void setName(std::string name);
    void setValue(Value value);
    void accept(Visitor &v) override;
};

class variable_declaration_list : public STNode
{
  private:
    std::vector<variable_declaration *> m_vars;

  public:
    variable_declaration_list(variable_declaration *variable_declaration);

    std::vector<variable_declaration *> &getVariables();
    void add(variable_declaration *variable_declaration);
    void accept(Visitor &v) override;
};

class variable_declaration_statement : public STNode
{
  public:
    variable_declaration_statement(
        type_specifier *type_specifier,
        variable_declaration_list *variable_declaration_list);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class compound_statement : public STNode
{
  public:
    compound_statement(statement_list *statement_list);
    compound_statement();

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class statement_list : public STNode
{
  public:
    statement_list(statement_list *statement_list, statement *statement);
    statement_list(statement *statement);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class statement : public STNode
{
  public:
    statement(if_statement *if_statement);
    statement(expression *expression);
    statement(compound_statement *compound_statement);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class condition : public STNode
{
  public:
    condition(expression *expression);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class if_statement : public STNode
{
  public:
    if_statement(condition *condition, statement *statement);
    if_statement(condition *condition, statement *statement1,
                 statement *statement2);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class while_statement : public STNode
{
  public:
    while_statement(condition *condition, statement *statement);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class do_while_statement : public STNode
{
  public:
    do_while_statement(compound_statement *compound_statement,
                       condition *condition);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class for_statement : public STNode
{
  public:
    for_statement(expression *expression1, expression *expression2,
                  expression *expression3,
                  compound_statement *compound_statement);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class continue_node : public STNode
{
  public:
    continue_node();

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class break_node : public STNode
{
  public:
    break_node();

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class argument_list : public STNode
{
  private:
    std::vector<STNode *> arguments;

  public:
    argument_list(expression *expression);

    void add(STNode *expression);
    std::vector<STNode *> getArguments();
    void accept(Visitor &v) override;
};

class parameter_list : public STNode
{
  private:
    std::vector<parameter> parameters;

  public:
    parameter_list(type_specifier *type_specifier, IDENTIFIER *IDENTIFIER);
    parameter_list();

    void add(dataType type, std::string name);
    std::vector<parameter> &getParameters();
    void accept(Visitor &v) override;
};

class function_call : public STNode
{
  public:
    function_call(IDENTIFIER *IDENTIFIER);
    function_call(IDENTIFIER *IDENTIFIER, argument_list *argument_list);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class function_definition : public STNode
{
  public:
    function_definition(type_specifier *type_specifier, IDENTIFIER *IDENTIFIER,
                        parameter_list *parameter_list,
                        compound_statement *compound_statement);

    void accept(Visitor &v) override;
};

class function_declaration : public STNode
{
  public:
    function_declaration(type_specifier *type_specifier, IDENTIFIER *IDENTIFIER,
                         parameter_list *parameter_list);

    void accept(Visitor &v) override;
};

class external_declaration : public STNode
{
  public:
    external_declaration(function_declaration *function_declaration);
    external_declaration(function_definition *function_definition);
    external_declaration(
        variable_declaration_statement *variable_declaration_statement);

    void accept(Visitor &v) override;
};

class translation_unit : public STNode
{
  public:
    translation_unit(translation_unit *translation_unit,
                     external_declaration *external_declaration);
    translation_unit(external_declaration *external_declaration);

    void accept(Visitor &v) override;
};

class return_node : public STNode
{
  public:
    return_node(expression *expression);
    return_node();

    // int evaluate() override;
    void accept(Visitor &v) override;
};

class program : public STNode
{
  public:
    program(translation_unit *translation_unit);

    // int evaluate() override;
    void accept(Visitor &v) override;
};

#endif
