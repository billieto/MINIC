#include "../lib/composite_concrete.hh"
#include "../lib/composite.hh"
#include <cstdint>
#include <iostream>
#include <list>
#include <ostream>
#include <string>

NUMBER::NUMBER(int value) : STNode(NUMBER_NODE, {})
{
    m_value.i = value;
    m_valueType = INT;
}

NUMBER::NUMBER(double value) : STNode(NUMBER_NODE, {})
{
    m_value.d = value;
    m_valueType = DOUBLE;
}

std::string NUMBER::getGraphvizLabel()
{
    std:: string temp = STNode::getGraphvizLabel() + "=";
    if (m_valueType == INT)
    {
        temp += std::to_string(m_value.i);
    }
    else if (m_valueType == DOUBLE)
    {
        temp += std::to_string(m_value.d);
    }
    return temp;
}

int NUMBER::evaluate()
{
    return m_value.i;
}

IDENTIFIER::IDENTIFIER(std::string str) : STNode(IDENTIFIER_NODE, {})
{
    m_label = str;
    m_value =  INT16_MAX;
}

void IDENTIFIER::setValue(int value)
{
    m_value = value;
}

int IDENTIFIER::getValue()
{
    if (m_value == INT16_MAX)
    {
        // Error handling, not assigned a value
    }
    return m_value;
}

std::string IDENTIFIER::getGraphvizLabel()
{
    return STNode::getGraphvizLabel() + "=" + m_label;
}

int IDENTIFIER::evaluate()
{
    return getValue();
}

expression::expression(NUMBER *NUMBER) : STNode(EXPRESSION_NODE, {NUMBER}) { }

expression::expression(IDENTIFIER *IDENTIFIER) : STNode(EXPRESSION_NODE, {IDENTIFIER}) { }

multiplication::multiplication(expression *left, expression *right) : STNode(MULTIPLICATION_NODE, {left, right}) { }

int multiplication::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() * rightNode -> evaluate();
}

division::division(expression *left, expression *right) : STNode(DIVISION_NODE, {left, right}) { }

int division::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() / rightNode -> evaluate();
}

addition::addition(expression *left, expression *right) : STNode(ADDITION_NODE, {left, right}) { }

int addition::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() + rightNode -> evaluate();
}

subtraction::subtraction(expression *left, expression *right) : STNode(SUBTRACTION_NODE, {left, right}) { }

int subtraction::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() - rightNode -> evaluate();
}

assignment::assignment(IDENTIFIER *IDENTIFIER, expression *expression) : STNode(ASSIGNMENT_NODE, {IDENTIFIER, expression}) { }

int assignment::evaluate()
{
    const std::list<STNode *> &temp = this -> STNode::getChildrenList();

    auto it = temp.begin();
    IDENTIFIER *id = (IDENTIFIER *) *it;
    it++;
    id -> setValue((*it) -> evaluate());

    return id -> getValue();
}

expression_list::expression_list(expression *expression) : STNode(EXPRESSION_LIST_NODE, {expression}) { }

// expression_list::expression_list(expression_list *expression_list, expression *expression) : STNode(EXPRESSION_LIST_NODE, {expression_list, expression}) { }

int expression_list::evaluate()
{
    const std::list<STNode *> &temp = this -> STNode::getChildrenList();
    int result = 0;

    for (const auto &it : temp)
    {
        result = it->evaluate();
        std::cout << result << std::endl;
    }

    return result;
}

statement::statement(if_statement *if_statement) : STNode(STATEMENT_NODE, {if_statement}) { }

statement::statement(expression_list *expression_list) : STNode(STATEMENT_NODE, {expression_list}) { }

statements::statements(statements *statements, statement *statement) : STNode(STATEMENTS_NODE, {statements, statement}) { }

statements::statements(statement *statement) : STNode(STATEMENTS_NODE, {statement}) { }

code_block::code_block(statements *statements) : STNode(CODE_BLOCK_NODE, {statements}) { }

code_block::code_block(expression *expression) : STNode(CODE_BLOCK_NODE, {expression}) { }

int code_block::evaluate()
{
    auto it = this -> getChildrenList().begin();

    return (*it) -> evaluate();
}

condition::condition(expression_list *expression_list) : STNode(CONDITION_NODE, {expression_list}) { }

int condition::evaluate()
{
    auto it = this -> getChildrenList().begin();

    return (*it) -> evaluate();
}

if_statement::if_statement(condition *condition, code_block *code_block)
: STNode(IF_STATEMENT_NODE, {condition, code_block}) { }

if_statement::if_statement(condition *condition, code_block *code_block1, code_block *code_block2)
: STNode(IF_STATEMENT_NODE, {condition, code_block1, code_block2}) { }

if_statement::if_statement(condition *condition, code_block *code_block, if_statement *if_statement)
: STNode(IF_STATEMENT_NODE, {condition, code_block, if_statement}) { }

int if_statement::evaluate()
{
    auto it = this -> getChildrenList().begin();
    int cond = (*it) -> evaluate();
    int result = 0;
    if (cond)
    {
        it++;
        result = (*it) -> evaluate();
    }
    else if (it != this -> getChildrenList().end())
    {
        ++++it;
        result = (*it) -> evaluate();
    }

    std::cout << result << std::endl;
    return result;
}

compile_unit::compile_unit(statements *statements) : STNode(COMPILE_UNITE_NODE, {statements}) { }

int compile_unit::evaluate()
{
    auto it = this -> getChildrenList().begin();

    return (*it) -> evaluate();
}
