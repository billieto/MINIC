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

less::less(expression *left, expression *right) : STNode(LESS_NODE ,{left, right}) {}

int less::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() < rightNode -> evaluate();
}


less_equals::less_equals(expression *left, expression *right) : STNode(LESS_EQUALS_NODE ,{left, right}) {}

int less_equals::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() <= rightNode -> evaluate();
}

greater::greater(expression *left, expression *right) : STNode(GREATER_EQUALS_NODE ,{left, right}) {}

int greater::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() > rightNode -> evaluate();
}

greater_equals::greater_equals(expression *left, expression *right) : STNode(GREATER_EQUALS_NODE ,{left, right}) {}

int greater_equals::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() >= rightNode -> evaluate();
}

logic_equals::logic_equals(expression *left, expression *right) : STNode(LOGIC_EQUALS_NODE ,{left, right}) {}

int logic_equals::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() == rightNode -> evaluate();
}

logic_not_equals::logic_not_equals(expression *left, expression *right) : STNode(LOGIC_NOT_EQUALS_NODE ,{left, right}) {}

int logic_not_equals::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() != rightNode -> evaluate();
}

logic_and::logic_and(expression *left, expression *right) : STNode(LOGIC_AND_NODE ,{left, right}) {}

int logic_and::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() && rightNode -> evaluate();
}

logic_or::logic_or(expression *left, expression *right) : STNode(LOGIC_OR_NODE ,{left, right}) {}

int logic_or::evaluate()
{
    auto it = this -> getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode -> evaluate() || rightNode -> evaluate();
}

logic_not::logic_not(expression *expression) : STNode(LOGIC_NOT_NODE ,{expression}) {}

int logic_not::evaluate()
{
    auto it = this -> getChildrenList().begin();
    return !((*it) -> evaluate());
}

increment::increment(expression *expression) : STNode(INCREMENT_NODE ,{expression}) { }

int increment::evaluate()
{
    auto it = this -> getChildrenList().begin();
    int temp = (*it) -> evaluate();
    return (temp + 1);
}

decrement::decrement(expression *expression) : STNode(DECREMENT_NODE ,{expression}) { }

int decrement::evaluate()
{
    auto it = this -> getChildrenList().begin();
    int temp = (*it) -> evaluate();
    return (temp - 1);
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

statement::statement(if_statement *if_statement) : STNode(STATEMENT_NODE, {if_statement}) { }

statement::statement(expression *expression) : STNode(STATEMENT_NODE, {expression}) { }

statement::statement(compound_statement *compound_statement) : STNode(COMPMOUNT_STATEMENT_NODE, {compound_statement}) { }

int statement::evaluate()
{
    auto it = this -> getChildrenList().begin(); // NA KOITA3W NA DW DIAFORA ME BEGIN KAI SKETO GET METHOD

    return (*it) -> evaluate(); 
}

statement_list::statement_list(statement_list *statement_list, statement *statement) : STNode(STATEMENT_LIST_NODE, {statement_list, statement}) { }

statement_list::statement_list(statement *statement) : STNode(STATEMENT_LIST_NODE, {statement}) { }

int statement_list::evaluate()
{
    int result = 0;

    for (const auto &child : this -> getChildrenList())
    {
        result += child -> evaluate();
    }

    return result;
}

compound_statement::compound_statement(statement_list *statement_list) : STNode(COMPMOUNT_STATEMENT_NODE, {statement_list}) { }

compound_statement::compound_statement() : STNode(COMPMOUNT_STATEMENT_NODE, {}) { }

int compound_statement::evaluate()
{
    auto it = this -> getChildrenList().begin();

    return (*it) -> evaluate();
}

condition::condition(expression *expression) : STNode(CONDITION_NODE, {expression}) { }

int condition::evaluate()
{
    auto it = this -> getChildrenList().begin();

    return (*it) -> evaluate();
}

if_statement::if_statement(condition *condition, statement_list *statement_list)
: STNode(IF_STATEMENT_NODE, {condition, statement_list}) { }

if_statement::if_statement(condition *condition, statement_list *statement_list1, statement_list *statement_list2)
: STNode(IF_STATEMENT_NODE, {condition, statement_list1, statement_list2}) { }

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

compile_unit::compile_unit(statement_list *statement_list) : STNode(COMPILE_UNITE_NODE, {statement_list}) { }

int compile_unit::evaluate()
{
    auto it = this -> getChildrenList().begin();

    return (*it) -> evaluate();
}
