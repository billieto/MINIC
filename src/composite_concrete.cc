#include "../lib/composite_concrete.hh"
#include "../lib/composite.hh"
#include "../lib/visitor.hh"
#include <iostream>
#include <list>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector>

// Signals
struct continue_signal
{
};

struct break_signal
{
};

struct void_return_signal
{
};

// Constructors
NUMBER::NUMBER(int value) : STNode(NUMBER_NODE, {}) { m_value = value; }

IDENTIFIER::IDENTIFIER(std::string str) : STNode(IDENTIFIER_NODE, {})
{
    m_label = str;
}

expression::expression(NUMBER *NUMBER) : STNode(EXPRESSION_NODE, {NUMBER}) {}

expression::expression(IDENTIFIER *IDENTIFIER)
    : STNode(EXPRESSION_NODE, {IDENTIFIER})
{
}

multiplication::multiplication(expression *left, expression *right)
    : STNode(MULTIPLICATION_NODE, {left, right})
{
}

division::division(expression *left, expression *right)
    : STNode(DIVISION_NODE, {left, right})
{
}

addition::addition(expression *left, expression *right)
    : STNode(ADDITION_NODE, {left, right})
{
}

subtraction::subtraction(expression *left, expression *right)
    : STNode(SUBTRACTION_NODE, {left, right})
{
}

less_equals::less_equals(expression *left, expression *right)
    : STNode(LESS_EQUALS_NODE, {left, right})
{
}

less::less(expression *left, expression *right)
    : STNode(LESS_NODE, {left, right})
{
}

greater::greater(expression *left, expression *right)
    : STNode(GREATER_EQUALS_NODE, {left, right})
{
}

greater_equals::greater_equals(expression *left, expression *right)
    : STNode(GREATER_EQUALS_NODE, {left, right})
{
}

logic_equals::logic_equals(expression *left, expression *right)
    : STNode(LOGIC_EQUALS_NODE, {left, right})
{
}

logic_not_equals::logic_not_equals(expression *left, expression *right)
    : STNode(LOGIC_NOT_EQUALS_NODE, {left, right})
{
}

logic_and::logic_and(expression *left, expression *right)
    : STNode(LOGIC_AND_NODE, {left, right})
{
}

logic_or::logic_or(expression *left, expression *right)
    : STNode(LOGIC_OR_NODE, {left, right})
{
}

logic_not::logic_not(expression *expression)
    : STNode(LOGIC_NOT_NODE, {expression})
{
}

increment::increment(expression *expression)
    : STNode(INCREMENT_NODE, {expression})
{
}

decrement::decrement(expression *expression)
    : STNode(DECREMENT_NODE, {expression})
{
}

assignment::assignment(IDENTIFIER *IDENTIFIER, expression *expression)
    : STNode(ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

bit_wise_or::bit_wise_or(expression *left, expression *right)
    : STNode(BIT_WISE_OR_NODE, {left, right})
{
}

bit_wise_and::bit_wise_and(expression *left, expression *right)
    : STNode(BIT_WISE_AND_NODE, {left, right})
{
}

bit_wise_xor::bit_wise_xor(expression *left, expression *right)
    : STNode(BIT_WISE_XOR_NODE, {left, right})
{
}

bit_wise_not::bit_wise_not(expression *expression)
    : STNode(BIT_WISE_NOT_NODE, {expression})
{
}

shift_left::shift_left(expression *left, expression *right)
    : STNode(SHIFT_LEFT_NODE, {left, right})
{
}

shift_right::shift_right(expression *left, expression *right)
    : STNode(SHIFT_RIGHT_NODE, {left, right})
{
}

plus_assignment::plus_assignment(IDENTIFIER *IDENTIFIER, expression *expression)
    : STNode(PLUS_ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

minus_assignment::minus_assignment(IDENTIFIER *IDENTIFIER,
                                   expression *expression)
    : STNode(MINUS_ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

mul_assignment::mul_assignment(IDENTIFIER *IDENTIFIER, expression *expression)
    : STNode(MUL_ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

div_assignment::div_assignment(IDENTIFIER *IDENTIFIER, expression *expression)
    : STNode(DIV_ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

mod_assignment::mod_assignment(IDENTIFIER *IDENTIFIER, expression *expression)
    : STNode(MOD_ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

mod::mod(expression *left, expression *right) : STNode(MOD_NODE, {left, right})
{
}

variable_declaration_list::variable_declaration_list(
    variable_declaration *variable_declaration)
    : STNode(VARIABLE_DECLARATION_LIST_NODE, {variable_declaration})
{
    m_vars.push_back(variable_declaration);
}

variable_declaration_statement::variable_declaration_statement(
    type_specifier *type_specifier,
    variable_declaration_list *variable_declaration_list)
    : STNode(VARIABLE_DECLARATION_STATEMENT_NODE,
             {type_specifier, variable_declaration_list})
{
}

variable_declaration::variable_declaration(IDENTIFIER *IDENTIFIER)
    : STNode(VARIABLE_DECLARATION_NODE, {IDENTIFIER})
{
}

variable_declaration::variable_declaration(IDENTIFIER *IDENTIFIER,
                                           expression *expression)
    : STNode(VARIABLE_DECLARATION_NODE, {IDENTIFIER, expression})
{
}

statement::statement(if_statement *if_statement)
    : STNode(STATEMENT_NODE, {if_statement})
{
}

statement::statement(expression *expression)
    : STNode(STATEMENT_NODE, {expression})
{
}

statement::statement(compound_statement *compound_statement)
    : STNode(COMPMOUNT_STATEMENT_NODE, {compound_statement})
{
}

statement_list::statement_list(statement_list *statement_list,
                               statement *statement)
    : STNode(STATEMENT_LIST_NODE, {statement_list, statement})
{
}

statement_list::statement_list(statement *statement)
    : STNode(STATEMENT_LIST_NODE, {statement})
{
}

compound_statement::compound_statement(statement_list *statement_list)
    : STNode(COMPMOUNT_STATEMENT_NODE, {statement_list})
{
}

compound_statement::compound_statement() : STNode(COMPMOUNT_STATEMENT_NODE, {})
{
}

condition::condition(expression *expression)
    : STNode(CONDITION_NODE, {expression})
{
}

if_statement::if_statement(condition *condition, statement *statement)
    : STNode(IF_STATEMENT_NODE, {condition, statement})
{
}

if_statement::if_statement(condition *condition, statement *statement1,
                           statement *statement2)
    : STNode(IF_STATEMENT_NODE, {condition, statement1, statement2})
{
}

while_statement::while_statement(condition *condition, statement *statement)
    : STNode(WHILE_STATEMENT_NODE, {condition, statement})
{
}

do_while_statement::do_while_statement(compound_statement *compound_statement,
                                       condition *condition)
    : STNode(DO_WHILE_STATEMENT_NODE, {compound_statement, condition})
{
}

for_statement::for_statement(expression *expression1, expression *expression2,
                             expression *expression3,
                             compound_statement *compound_statement)
    : STNode(FOR_STATEMENT_NODE,
             {expression1, expression2, expression3, compound_statement})
{
}

continue_node::continue_node() : STNode(CONTINUE_NODE, {}) {}

break_node::break_node() : STNode(BREAK_NODE, {}) {}

type_specifier::type_specifier(dataType dataType)
    : STNode(TYPE_SPECIFIER_NODE, {})
{
    m_type = dataType;
}

function_call::function_call(IDENTIFIER *IDENTIFIER)
    : STNode(FUNCTION_CALL_NODE, {IDENTIFIER})
{
}

function_call::function_call(IDENTIFIER *IDENTIFIER,
                             argument_list *argument_list)
    : STNode(FUNCTION_CALL_NODE, {IDENTIFIER, argument_list})
{
}

function_definition::function_definition(type_specifier *type_specifier,
                                         IDENTIFIER *IDENTIFIER,
                                         parameter_list *parameter_list,
                                         compound_statement *compound_statement)
    : STNode(FUNCTION_DEFINITION_NODE,
             {type_specifier, IDENTIFIER, parameter_list, compound_statement})
{
}

function_declaration::function_declaration(type_specifier *type_specifier,
                                           IDENTIFIER *IDENTIFIER,
                                           parameter_list *parameter_list)
    : STNode(FUNCTION_DECLARATION_NODE,
             {type_specifier, IDENTIFIER, parameter_list})
{
}

argument_list::argument_list(expression *expression)
    : STNode(ARGUMENT_LIST_NODE, {expression})
{
    add(expression);
}

parameter_list::parameter_list(type_specifier *type_specifier,
                               IDENTIFIER *IDENTIFIER)
    : STNode(PARAMETER_LIST_NODE, {type_specifier, IDENTIFIER})
{
    this->add(type_specifier->getType(), IDENTIFIER->getLabel());
}

parameter_list::parameter_list() : STNode(PARAMETER_LIST_NODE, {}) {}

external_declaration::external_declaration(
    function_declaration *function_declaration)
    : STNode(EXTERNAL_DECLARATION_NODE, {function_declaration})
{
}

external_declaration::external_declaration(
    function_definition *function_definition)
    : STNode(EXTERNAL_DECLARATION_NODE, {function_definition})
{
}

external_declaration::external_declaration(
    variable_declaration_statement *variable_declaration_statement)
    : STNode(EXTERNAL_DECLARATION_NODE, {variable_declaration_statement})
{
}

translation_unit::translation_unit(translation_unit *translation_unit,
                                   external_declaration *external_declaration)
    : STNode(TRANSLATION_UNIT_NODE, {translation_unit, external_declaration})
{
}

translation_unit::translation_unit(external_declaration *external_declaration)
    : STNode(TRANSLATION_UNIT_NODE, {external_declaration})
{
}

return_node::return_node(expression *expression)
    : STNode(RETURN_NODE, {expression})
{
}

return_node::return_node() : STNode(RETURN_NODE, {}) {}

program::program(translation_unit *translation_unit)
    : STNode(PROGRAM_NODE, {translation_unit})
{
}

// Graph Viz Labels
std::string NUMBER::getGraphvizLabel()
{
    return STNode::getGraphvizLabel() + "=" + std::to_string(m_value);
}

std::string IDENTIFIER::getGraphvizLabel()
{
    return STNode::getGraphvizLabel() + "=" + m_label;
}

// Getters
std::string IDENTIFIER::getLabel() { return m_label; }
Value NUMBER::getValue() { return m_value; }
std::vector<variable_declaration *> &variable_declaration_list::getVariables()
{
    return m_vars;
}
std::string variable_declaration::getName() { return m_name; }
int variable_declaration::getValue() { return m_value; }
dataType type_specifier::getType() { return m_type; }
std::vector<parameter> &parameter_list::getParameters() { return parameters; }
std::vector<STNode *> argument_list::getArguments() { return arguments; }

// Setters
void variable_declaration::setName(std::string name) { m_name = name; }
void variable_declaration::setValue(Value value) { m_value = value; }

// Helper Methods
void variable_declaration_list::add(variable_declaration *variable_declaration)
{
    m_vars.push_back(variable_declaration);
}

void argument_list::add(STNode *expression) { arguments.push_back(expression); }

void parameter_list::add(dataType type, std::string name)
{
    parameters.push_back({type, name});
}

// // Evaluate Methods
// int NUMBER::evaluate() { return m_value; }

// int IDENTIFIER::evaluate()
// {
//     Symbol *sym = SymbolTable::getInstance()->lookup(this->m_label);

//     if (!sym)
//     {
//         std::cout << "Identifier not defined in scope" << std::endl;
//         exit(1);
//     }

//     return sym->getValue();
// }

// int multiplication::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() * rightNode->evaluate();
// }

// int division::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     int result = rightNode->evaluate();
//     if (!result)
//     {
//         std::cerr << "Trying to do a division by 0" << std::endl;
//         exit(1);
//     }

//     return leftNode->evaluate() / result;
// }

// int addition::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() + rightNode->evaluate();
// }

// int subtraction::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() - rightNode->evaluate();
// }

// int less::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() < rightNode->evaluate();
// }

// int less_equals::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() <= rightNode->evaluate();
// }

// int greater::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() > rightNode->evaluate();
// }

// int greater_equals::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() >= rightNode->evaluate();
// }

// int logic_equals::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() == rightNode->evaluate();
// }

// int logic_not_equals::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() != rightNode->evaluate();
// }

// int logic_and::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() && rightNode->evaluate();
// }

// int logic_or::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() || rightNode->evaluate();
// }

// int logic_not::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     return !((*it)->evaluate());
// }

// int increment::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     IDENTIFIER *id = (IDENTIFIER *)(*it);

//     if (!id)
//     {
//         std::cerr << "cant be none other that lvalue to increment" << std::endl;
//         exit(1);
//     }

//     std::string name = id->getLabel();
//     Symbol *sym = SymbolTable::getInstance()->lookup(name);

//     if (!sym)
//     {
//         std::cerr << "Variable: \"" << name << "\" is not declared"
//                   << std::endl;
//         exit(1);
//     }

//     int old_value = (*it)->evaluate();

//     sym->setValue(old_value + 1);

//     return old_value;
// }

// int decrement::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     IDENTIFIER *id = (IDENTIFIER *)(*it);

//     if (!id)
//     {
//         std::cerr << "cant be none other that lvalue to increment" << std::endl;
//     }

//     std::string name = id->getLabel();
//     Symbol *sym = SymbolTable::getInstance()->lookup(name);

//     if (!sym)
//     {
//         std::cerr << "Variable: \"" << name << "\" is not declared"
//                   << std::endl;
//         exit(1);
//     }

//     int old_value = (*it)->evaluate();

//     sym->setValue(old_value - 1);

//     return old_value;
// }

// int assignment::evaluate()
// {
//     auto it = this->STNode::getChildrenList().begin();

//     std::string name = ((IDENTIFIER *)(*it))->getLabel();
//     Symbol *sym = SymbolTable::getInstance()->lookup(name);

//     if (!sym)
//     {
//         sym = SymbolTable::getInstance()->lookupGlobal(name);
//         if (!sym || sym->getIsFunction())
//         {
//             std::cerr << "Variable: \"" << name << "\" is not declared"
//                       << std::endl;
//             exit(1);
//         }
//     }

//     it++;
//     int result = (*it)->evaluate();
//     sym->setValue(result);

//     // Debug print
//     std::cout << name << "=" << std::to_string(result) << std::endl;

//     return sym->getValue();
// }

// int bit_wise_or::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() | rightNode->evaluate();
// }

// int bit_wise_and::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() & rightNode->evaluate();
// }

// int bit_wise_xor::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() ^ rightNode->evaluate();
// }

// int bit_wise_not::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     return ~((*it)->evaluate());
// }

// int shift_left::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() << rightNode->evaluate();
// }

// int shift_right::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() >> rightNode->evaluate();
// }

// int plus_assignment::evaluate()
// {
//     auto it = this->STNode::getChildrenList().begin();

//     std::string name = ((IDENTIFIER *)(*it))->getLabel();
//     Symbol *sym = SymbolTable::getInstance()->lookup(name);

//     if (!sym)
//     {
//         sym = SymbolTable::getInstance()->lookupGlobal(name);
//         if (!sym || sym->getIsFunction())
//         {
//             std::cerr << "Variable: \"" << name << "\" is not declared"
//                       << std::endl;
//             exit(1);
//         }
//     }

//     it++;
//     int result = (*it)->evaluate();
//     sym->setValue((sym->getValue() + result));

//     // Debug print
//     std::cout << name << "=" << std::to_string(sym->getValue()) << std::endl;

//     return sym->getValue();
// }

// int minus_assignment::evaluate()
// {
//     auto it = this->STNode::getChildrenList().begin();

//     std::string name = ((IDENTIFIER *)(*it))->getLabel();
//     Symbol *sym = SymbolTable::getInstance()->lookup(name);

//     if (!sym)
//     {
//         sym = SymbolTable::getInstance()->lookupGlobal(name);
//         if (!sym || sym->getIsFunction())
//         {
//             std::cerr << "Variable: \"" << name << "\" is not declared"
//                       << std::endl;
//             exit(1);
//         }
//     }

//     it++;
//     int result = (*it)->evaluate();
//     sym->setValue((sym->getValue() - result));

//     // Debug print
//     std::cout << name << "=" << std::to_string(sym->getValue()) << std::endl;

//     return sym->getValue();
// }

// int mul_assignment::evaluate()
// {
//     auto it = this->STNode::getChildrenList().begin();

//     std::string name = ((IDENTIFIER *)(*it))->getLabel();
//     Symbol *sym = SymbolTable::getInstance()->lookup(name);

//     if (!sym)
//     {
//         sym = SymbolTable::getInstance()->lookupGlobal(name);
//         if (!sym || sym->getIsFunction())
//         {
//             std::cerr << "Variable: \"" << name << "\" is not declared"
//                       << std::endl;
//             exit(1);
//         }
//     }

//     it++;
//     int result = (*it)->evaluate();
//     sym->setValue((sym->getValue() * result));

//     // Debug print
//     std::cout << name << "=" << std::to_string(sym->getValue()) << std::endl;

//     return sym->getValue();
// }

// int div_assignment::evaluate()
// {
//     auto it = this->STNode::getChildrenList().begin();

//     std::string name = ((IDENTIFIER *)(*it))->getLabel();
//     Symbol *sym = SymbolTable::getInstance()->lookup(name);

//     if (!sym)
//     {
//         sym = SymbolTable::getInstance()->lookupGlobal(name);
//         if (!sym || sym->getIsFunction())
//         {
//             std::cerr << "Variable: \"" << name << "\" is not declared"
//                       << std::endl;
//             exit(1);
//         }
//     }

//     it++;
//     int result = (*it)->evaluate();
//     if (!result)
//     {
//         std::cerr << "Trying to do a division by 0" << std::endl;
//         exit(1);
//     }

//     sym->setValue((sym->getValue() / result));

//     // Debug print
//     std::cout << name << "=" << std::to_string(sym->getValue()) << std::endl;

//     return sym->getValue();
// }

// int mod_assignment::evaluate()
// {
//     auto it = this->STNode::getChildrenList().begin();

//     std::string name = ((IDENTIFIER *)(*it))->getLabel();
//     Symbol *sym = SymbolTable::getInstance()->lookup(name);

//     if (!sym)
//     {
//         sym = SymbolTable::getInstance()->lookupGlobal(name);
//         if (!sym || sym->getIsFunction())
//         {
//             std::cerr << "Variable: \"" << name << "\" is not declared"
//                       << std::endl;
//             exit(1);
//         }
//     }

//     it++;
//     int result = (*it)->evaluate();
//     sym->setValue((sym->getValue() % result));

//     // Debug print
//     std::cout << name << "=" << std::to_string(sym->getValue()) << std::endl;

//     return sym->getValue();
// }

// int mod::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     STNode *leftNode = *it;
//     it++;
//     STNode *rightNode = *it;

//     return leftNode->evaluate() % rightNode->evaluate();
// }

// int variable_declaration_statement::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     dataType currentType = ((type_specifier *)(*it))->getType();

//     it++;
//     std::vector<variable_declaration *> varList =
//         ((variable_declaration_list *)(*it))->getVariables();

//     for (auto var : varList)
//     {
//         var->evaluate();

//         Symbol sym;
//         sym.setType(currentType);
//         sym.setName(var->getName());
//         sym.setValue(var->getValue());
//         sym.setIsFunction(false);

//         if (!SymbolTable::getInstance()->insert(sym))
//         { // Semantic Error
//             std::cerr << "Variable " << sym.getName() << " already exists."
//                       << std::endl;
//         }
//     }

//     return 0;
// }

// int variable_declaration::evaluate()
// {
//     auto &temp = this->getChildrenList();

//     auto it = temp.begin();
//     m_name = ((IDENTIFIER *)(*it))->getLabel();

//     if (temp.size() > 1)
//     {
//         it++;
//         m_value = (*it)->evaluate();
//     }

//     return m_value;
// }

// int statement::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     return (*it)->evaluate();
// }

// int statement_list::evaluate()
// {
//     int result = 0;

//     for (const auto &child : this->getChildrenList())
//     {
//         result += child->evaluate();
//     }

//     return result;
// }

// int compound_statement::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     return (*it)->evaluate();
// }

// int condition::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     return (*it)->evaluate();
// }

// int if_statement::evaluate()
// {
//     auto it = this->getChildrenList().begin();
//     int cond = (*it)->evaluate();
//     int result = 0;

//     it++;

//     if (cond)
//     {
//         result = (*it)->evaluate();
//     }
//     else if (this->getChildrenList().size() ==
//              3) // Maybe it dose not work that well :(
//     {
//         it++;
//         result = (*it)->evaluate();
//     }

//     std::cout << result << std::endl;
//     return result;
// }

// int while_statement::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     condition *cond = (condition *)(*it);
//     it++;

//     while (cond->evaluate())
//     {
//         try
//         {
//             (*it)->evaluate();
//         }
//         catch (continue_signal)
//         {
//             continue;
//         }
//         catch (break_signal)
//         {
//             break;
//         }
//     }
//     // return statement auto-thrown into the function call node.
//     return 0;
// }

// int do_while_statement::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     compound_statement *body = (compound_statement *)(*it);
//     it++;

//     do
//     {
//         try
//         {
//             body->evaluate();
//         }
//         catch (continue_signal)
//         {
//             continue;
//         }
//         catch (break_signal)
//         {
//             break;
//         }
//     } while ((*it)->evaluate());

//     return 0;
// }

// int for_statement::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     (*it)->evaluate(); // first part or for loop
//     it++;

//     STNode *cond = (*it); // second part
//     it++;

//     STNode *inc = (*it); // third part
//     it++;

//     while (cond->evaluate())
//     {
//         try
//         {
//             (*it)->evaluate(); // the body
//         }
//         catch (continue_signal)
//         {
//             continue;
//         }
//         catch (break_signal)
//         {
//             break;
//         }

//         inc->evaluate();
//     }

//     return 0;
// }

// int continue_node::evaluate() { throw continue_signal(); }

// int break_node::evaluate() { throw break_signal(); }

// int function_call::evaluate()
// {
//     std::vector<int> finalValues;
//     auto childs = this->getChildrenList();

//     auto it = childs.begin();
//     std::string func_name = ((IDENTIFIER *)*it)->getLabel();

//     // Debugging print
//     // std::cout << func_name << std::endl;

//     Symbol *def = SymbolTable::getInstance()->lookupGlobal(func_name);

//     if (!def)
//     { // Need to make it so the error is emitted from the parser.
//         std::cerr << "The function you try to call isn't defined" << std::endl;
//         exit(1);
//     }

//     compound_statement *func_body =
//         (compound_statement *)(def->getFunctionBody());

//     if (!func_body)
//     { // Need to make it so the error is emitted from the parser.
//         std::cerr << "The function you try to call isn't implemented"
//                   << std::endl;
//         exit(1);
//     }

//     if (childs.size() == 1) // No arguments
//     {
//     }
//     else // With arguments
//     {
//         it++;
//         auto expressions = ((argument_list *)(*it))->getArguments();

//         for (auto expr : expressions)
//         {
//             finalValues.push_back(expr->evaluate());
//         }
//     }

//     // Here it should do the type check as well!
//     std::vector<parameter> &func_params = def->getParameters();
//     if (func_params.size() != finalValues.size())
//     {
//         std::cerr
//             << "The function you try to call dose not have the same arguments "
//                "as parameters that are defined"
//             << std::endl;
//         exit(1);
//     }

//     SymbolTable::getInstance()->enterScope(
//         SymbolTable::getInstance()->getCurrentId() + 1);

//     for (size_t i = 0; i < func_params.size(); i++)
//     {
//         Symbol param;
//         param.setName(func_params[i].name);
//         param.setType(func_params[i].type);
//         param.setValue(finalValues[i]);

//         SymbolTable::getInstance()->insert(param);
//     }

//     try
//     {
//         func_body->evaluate();
//     }
//     catch (int returnValue)
//     {
//         SymbolTable::getInstance()->exitScope();

//         if (def->getType() == T_VOID)
//         {
//             std::cerr << "Void functions should not return a value"
//                       << std::endl;
//             exit(1); // Should i exit or not?
//         }

//         return returnValue;
//     }
//     catch (void_return_signal)
//     {
//         SymbolTable::getInstance()->exitScope();

//         if (def->getType() != T_VOID)
//         {
//             std::cerr << "Non-void functions should return a value"
//                       << std::endl;
//             exit(1); // Should i exit or not?
//         }
//     }

//     SymbolTable::getInstance()->exitScope();

//     if (def->getType() != T_VOID)
//     { // Semantic-error
//         std::cerr << "Non-void functions should return a value" << std::endl;
//         exit(1); // Should i exit or not?
//     }

//     return 0;
// }

// int return_node::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     if (!(*it)) // return for void functions
//     {
//         throw void_return_signal();
//     }
//     else
//     {
//         throw((*it)->evaluate());
//     }
// }

// int program::evaluate()
// {
//     auto it = this->getChildrenList().begin();

//     (*it)->evaluate();

//     Symbol *entry = SymbolTable::getInstance()->lookupGlobal("main");
//     if (entry == nullptr || entry->getFunctionBody())
//     {
//         std::cerr << "Error: Linker error - undefined reference to 'main'"
//                   << std::endl;
//         exit(1);
//     }

//     SymbolTable::getInstance()->enterScope(0); // Main Scope
//     // When i will make scopes for every compound statement this will leave
//     // Because function_body is always a compound statement
//     int result = entry->getFunctionBody()->evaluate();
//     SymbolTable::getInstance()->exitScope();

//     return result;
// }

// Accepts for visitor
// --- Leaf Nodes ---
void IDENTIFIER::accept(Visitor &v) { v.visitIDENTIFIER(this); }
void NUMBER::accept(Visitor &v) { v.visitNUMBER(this); }
void type_specifier::accept(Visitor &v) { v.visitTypeSpecifier(this); }

// --- Arithmetic Operations ---
void multiplication::accept(Visitor &v) { v.visitMultiplication(this); }
void division::accept(Visitor &v) { v.visitDivision(this); }
void addition::accept(Visitor &v) { v.visitAddition(this); }
void subtraction::accept(Visitor &v) { v.visitSubtraction(this); }
void mod::accept(Visitor &v) { v.visitMod(this); }

// --- Logical & Relational Operations ---
void less::accept(Visitor &v) { v.visitLess(this); }
void less_equals::accept(Visitor &v) { v.visitLessEquals(this); }
void greater::accept(Visitor &v) { v.visitGreater(this); }
void greater_equals::accept(Visitor &v) { v.visitGreaterEquals(this); }
void logic_equals::accept(Visitor &v) { v.visitLogicEquals(this); }
void logic_not_equals::accept(Visitor &v) { v.visitLogicNotEquals(this); }
void logic_and::accept(Visitor &v) { v.visitLogicAnd(this); }
void logic_or::accept(Visitor &v) { v.visitLogicOr(this); }
void logic_not::accept(Visitor &v) { v.visitLogicNot(this); }

// --- Unary & Assignment ---
void increment::accept(Visitor &v) { v.visitIncrement(this); }
void decrement::accept(Visitor &v) { v.visitDecrement(this); }
void assignment::accept(Visitor &v) { v.visitAssignment(this); }

// --- Bitwise ---
void bit_wise_or::accept(Visitor &v) { v.visitBitWiseOr(this); }
void bit_wise_and::accept(Visitor &v) { v.visitBitWiseAnd(this); }
void bit_wise_xor::accept(Visitor &v) { v.visitBitWiseXor(this); }
void bit_wise_not::accept(Visitor &v) { v.visitBitWiseNot(this); }
void shift_left::accept(Visitor &v) { v.visitShiftLeft(this); }
void shift_right::accept(Visitor &v) { v.visitShiftRight(this); }

// --- Compound Assignments ---
void plus_assignment::accept(Visitor &v) { v.visitPlusAssignment(this); }
void minus_assignment::accept(Visitor &v) { v.visitMinusAssignment(this); }
void mul_assignment::accept(Visitor &v) { v.visitMulAssignment(this); }
void div_assignment::accept(Visitor &v) { v.visitDivAssignment(this); }
void mod_assignment::accept(Visitor &v) { v.visitModAssignment(this); }

// --- Declarations ---
void variable_declaration::accept(Visitor &v)
{
    v.visitVariableDeclaration(this);
}
void variable_declaration_list::accept(Visitor &v)
{
    v.visitVariableDeclarationList(this);
}
void variable_declaration_statement::accept(Visitor &v)
{
    v.visitVariableDeclarationStatement(this);
}
void external_declaration::accept(Visitor &v)
{
    v.visitExternalDeclaration(this);
}

// --- Statements & Control Flow ---
void compound_statement::accept(Visitor &v) { v.visitCompoundStatement(this); }
void statement_list::accept(Visitor &v) { v.visitStatementList(this); }
void statement::accept(Visitor &v) { v.visitStatement(this); }
void if_statement::accept(Visitor &v) { v.visitIfStatement(this); }
void while_statement::accept(Visitor &v) { v.visitWhileStatement(this); }
void do_while_statement::accept(Visitor &v) { v.visitDoWhileStatement(this); }
void for_statement::accept(Visitor &v) { v.visitForStatement(this); }
void continue_node::accept(Visitor &v) { v.visitContinue(this); }
void break_node::accept(Visitor &v) { v.visitBreak(this); }
void return_node::accept(Visitor &v) { v.visitReturn(this); }

// --- Functions ---
void function_call::accept(Visitor &v) { v.visitFunctionCall(this); }
void function_definition::accept(Visitor &v)
{
    v.visitFunctionDefinition(this);
}
void function_declaration::accept(Visitor &v)
{
    v.visitFunctionDeclaration(this);
}
void argument_list::accept(Visitor &v) { v.visitArgumentList(this); }
void parameter_list::accept(Visitor &v) { v.visitParameterList(this); }

// --- High Level ---
void program::accept(Visitor &v) { v.visitProgram(this); }
void expression::accept(Visitor &v) { v.visitExpression(this); }
void condition::accept(Visitor &v) { v.visitCondition(this); }
void translation_unit::accept(Visitor &v) { v.visitTranslationUnit(this); }
