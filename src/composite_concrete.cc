#include "../lib/composite_concrete.hh"
#include "../lib/composite.hh"
#include <iostream>
#include <list>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector>

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
    std::string temp = STNode::getGraphvizLabel() + "=";
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

int NUMBER::evaluate() { return m_value.i; }

IDENTIFIER::IDENTIFIER(std::string str) : STNode(IDENTIFIER_NODE, {})
{
    m_label = str;
}

std::string IDENTIFIER::getLabel() { return m_label; }

std::string IDENTIFIER::getGraphvizLabel()
{
    return STNode::getGraphvizLabel() + "=" + m_label;
}

int IDENTIFIER::evaluate()
{
    Symbol *sym = SymbolTable::getInstance()->lookup(this->m_label);

    if (!sym)
    {
        std::cout << "Identifier not defined in scope" << std::endl;
        exit(1);
    }

    return sym->getValue();
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

int multiplication::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() * rightNode->evaluate();
}

division::division(expression *left, expression *right)
    : STNode(DIVISION_NODE, {left, right})
{
}

int division::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    int result = rightNode->evaluate();
    if (!result)
    {
        std::cerr << "Trying to do a division by 0" << std::endl;
        exit(1);
    }

    return leftNode->evaluate() / result;
}

addition::addition(expression *left, expression *right)
    : STNode(ADDITION_NODE, {left, right})
{
}

int addition::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() + rightNode->evaluate();
}

subtraction::subtraction(expression *left, expression *right)
    : STNode(SUBTRACTION_NODE, {left, right})
{
}

int subtraction::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() - rightNode->evaluate();
}

less::less(expression *left, expression *right)
    : STNode(LESS_NODE, {left, right})
{
}

int less::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() < rightNode->evaluate();
}

less_equals::less_equals(expression *left, expression *right)
    : STNode(LESS_EQUALS_NODE, {left, right})
{
}

int less_equals::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() <= rightNode->evaluate();
}

greater::greater(expression *left, expression *right)
    : STNode(GREATER_EQUALS_NODE, {left, right})
{
}

int greater::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() > rightNode->evaluate();
}

greater_equals::greater_equals(expression *left, expression *right)
    : STNode(GREATER_EQUALS_NODE, {left, right})
{
}

int greater_equals::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() >= rightNode->evaluate();
}

logic_equals::logic_equals(expression *left, expression *right)
    : STNode(LOGIC_EQUALS_NODE, {left, right})
{
}

int logic_equals::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() == rightNode->evaluate();
}

logic_not_equals::logic_not_equals(expression *left, expression *right)
    : STNode(LOGIC_NOT_EQUALS_NODE, {left, right})
{
}

int logic_not_equals::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() != rightNode->evaluate();
}

logic_and::logic_and(expression *left, expression *right)
    : STNode(LOGIC_AND_NODE, {left, right})
{
}

int logic_and::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() && rightNode->evaluate();
}

logic_or::logic_or(expression *left, expression *right)
    : STNode(LOGIC_OR_NODE, {left, right})
{
}

int logic_or::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() || rightNode->evaluate();
}

logic_not::logic_not(expression *expression)
    : STNode(LOGIC_NOT_NODE, {expression})
{
}

int logic_not::evaluate()
{
    auto it = this->getChildrenList().begin();
    return !((*it)->evaluate());
}

increment::increment(expression *expression)
    : STNode(INCREMENT_NODE, {expression})
{
}

int increment::evaluate()
{
    auto it = this->getChildrenList().begin();
    int temp = (*it)->evaluate();
    return (temp + 1);
}

decrement::decrement(expression *expression)
    : STNode(DECREMENT_NODE, {expression})
{
}

int decrement::evaluate()
{
    auto it = this->getChildrenList().begin();
    int temp = (*it)->evaluate();
    return (temp - 1);
}

assignment::assignment(IDENTIFIER *IDENTIFIER, expression *expression)
    : STNode(ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

int assignment::evaluate()
{
    auto it = this->STNode::getChildrenList().begin();

    std::string name = ((IDENTIFIER *)(*it))->getLabel();
    Symbol *sym = SymbolTable::getInstance()->lookup(name);

    if (!sym)
    {
        sym = SymbolTable::getInstance()->lookupGlobal(name);
        if (!sym || sym->getIsFunction())
        {
            std::cerr << "Variable: \"" << name << "\" is not declared"
                      << std::endl;
            exit(1);
        }
    }

    it++;
    int result = (*it)->evaluate();
    sym->setValue(result);

    // Debug print
    std::cout << name << "=" << std::to_string(result) << std::endl;

    return sym->getValue();
}

bit_wise_or::bit_wise_or(expression *left, expression *right)
    : STNode(BIT_WISE_OR_NODE, {left, right})
{
}

int bit_wise_or::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() | rightNode->evaluate();
}

bit_wise_and::bit_wise_and(expression *left, expression *right)
    : STNode(BIT_WISE_AND_NODE, {left, right})
{
}

int bit_wise_and::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() & rightNode->evaluate();
}

bit_wise_xor::bit_wise_xor(expression *left, expression *right)
    : STNode(BIT_WISE_XOR_NODE, {left, right})
{
}

int bit_wise_xor::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() ^ rightNode->evaluate();
}

bit_wise_not::bit_wise_not(expression *expression)
    : STNode(BIT_WISE_NOT_NODE, {expression})
{
}
int bit_wise_not::evaluate()
{
    auto it = this->getChildrenList().begin();

    return ~((*it)->evaluate());
}

shift_left::shift_left(expression *left, expression *right)
    : STNode(SHIFT_LEFT_NODE, {left, right})
{
}

int shift_left::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() << rightNode->evaluate();
}

shift_right::shift_right(expression *left, expression *right)
    : STNode(SHIFT_RIGHT_NODE, {left, right})
{
}

int shift_right::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() >> rightNode->evaluate();
}

plus_assignment::plus_assignment(IDENTIFIER *IDENTIFIER, expression *expression)
    : STNode(PLUS_ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

int plus_assignment::evaluate()
{
    auto it = this->STNode::getChildrenList().begin();

    std::string name = ((IDENTIFIER *)(*it))->getLabel();
    Symbol *sym = SymbolTable::getInstance()->lookup(name);

    if (!sym)
    {
        sym = SymbolTable::getInstance()->lookupGlobal(name);
        if (!sym || sym->getIsFunction())
        {
            std::cerr << "Variable: \"" << name << "\" is not declared"
                      << std::endl;
            exit(1);
        }
    }

    it++;
    int result = (*it)->evaluate();
    sym->setValue((sym->getValue() + result));

    // Debug print
    std::cout << name << "=" << std::to_string(sym->getValue()) << std::endl;

    return sym->getValue();
}

minus_assignment::minus_assignment(IDENTIFIER *IDENTIFIER,
                                   expression *expression)
    : STNode(MINUS_ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

int minus_assignment::evaluate()
{
    auto it = this->STNode::getChildrenList().begin();

    std::string name = ((IDENTIFIER *)(*it))->getLabel();
    Symbol *sym = SymbolTable::getInstance()->lookup(name);

    if (!sym)
    {
        sym = SymbolTable::getInstance()->lookupGlobal(name);
        if (!sym || sym->getIsFunction())
        {
            std::cerr << "Variable: \"" << name << "\" is not declared"
                      << std::endl;
            exit(1);
        }
    }

    it++;
    int result = (*it)->evaluate();
    sym->setValue((sym->getValue() - result));

    // Debug print
    std::cout << name << "=" << std::to_string(sym->getValue()) << std::endl;

    return sym->getValue();
}

mul_assignment::mul_assignment(IDENTIFIER *IDENTIFIER, expression *expression)
    : STNode(MUL_ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

int mul_assignment::evaluate()
{
    auto it = this->STNode::getChildrenList().begin();

    std::string name = ((IDENTIFIER *)(*it))->getLabel();
    Symbol *sym = SymbolTable::getInstance()->lookup(name);

    if (!sym)
    {
        sym = SymbolTable::getInstance()->lookupGlobal(name);
        if (!sym || sym->getIsFunction())
        {
            std::cerr << "Variable: \"" << name << "\" is not declared"
                      << std::endl;
            exit(1);
        }
    }

    it++;
    int result = (*it)->evaluate();
    sym->setValue((sym->getValue() * result));

    // Debug print
    std::cout << name << "=" << std::to_string(sym->getValue()) << std::endl;

    return sym->getValue();
}

div_assignment::div_assignment(IDENTIFIER *IDENTIFIER, expression *expression)
    : STNode(DIV_ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

int div_assignment::evaluate()
{
    auto it = this->STNode::getChildrenList().begin();

    std::string name = ((IDENTIFIER *)(*it))->getLabel();
    Symbol *sym = SymbolTable::getInstance()->lookup(name);

    if (!sym)
    {
        sym = SymbolTable::getInstance()->lookupGlobal(name);
        if (!sym || sym->getIsFunction())
        {
            std::cerr << "Variable: \"" << name << "\" is not declared"
                      << std::endl;
            exit(1);
        }
    }

    it++;
    int result = (*it)->evaluate();
    if (!result)
    {
        std::cerr << "Trying to do a division by 0" << std::endl;
        exit(1);
    }

    sym->setValue((sym->getValue() / result));

    // Debug print
    std::cout << name << "=" << std::to_string(sym->getValue()) << std::endl;

    return sym->getValue();
}

mod_assignment::mod_assignment(IDENTIFIER *IDENTIFIER, expression *expression)
    : STNode(MOD_ASSIGNMENT_NODE, {IDENTIFIER, expression})
{
}

int mod_assignment::evaluate()
{
    auto it = this->STNode::getChildrenList().begin();

    std::string name = ((IDENTIFIER *)(*it))->getLabel();
    Symbol *sym = SymbolTable::getInstance()->lookup(name);

    if (!sym)
    {
        sym = SymbolTable::getInstance()->lookupGlobal(name);
        if (!sym || sym->getIsFunction())
        {
            std::cerr << "Variable: \"" << name << "\" is not declared"
                      << std::endl;
            exit(1);
        }
    }

    it++;
    int result = (*it)->evaluate();
    sym->setValue((sym->getValue() % result));

    // Debug print
    std::cout << name << "=" << std::to_string(sym->getValue()) << std::endl;

    return sym->getValue();
}

mod::mod(expression *left, expression *right) : STNode(MOD_NODE, {left, right})
{
}

int mod::evaluate()
{
    auto it = this->getChildrenList().begin();
    STNode *leftNode = *it;
    it++;
    STNode *rightNode = *it;

    return leftNode->evaluate() % rightNode->evaluate();
}

variable_declaration_list::variable_declaration_list(
    variable_declaration *variable_declaration)
    : STNode(VARIABLE_DECLARATION_LIST_NODE, {variable_declaration})
{
    m_vars.push_back(variable_declaration);
}

std::vector<variable_declaration *> &variable_declaration_list::getVariables()
{
    return m_vars;
}

void variable_declaration_list::add(variable_declaration *variable_declaration)
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

int variable_declaration_statement::evaluate()
{
    auto it = this->getChildrenList().begin();

    dataType currentType = ((type_specifier *)(*it))->getType();

    it++;
    std::vector<variable_declaration *> varList =
        ((variable_declaration_list *)(*it))->getVariables();

    for (auto var : varList)
    {
        var->evaluate();

        Symbol sym;
        sym.setType(currentType);
        sym.setName(var->getName());
        sym.setValue(var->getValue());
        sym.setIsFunction(false);

        if (!SymbolTable::getInstance()->insert(sym))
        { // Semantic Error
            std::cerr << "Variable " << sym.getName() << " already exists."
                      << std::endl;
        }
    }

    return 0;
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

int variable_declaration::evaluate()
{
    auto &temp = this->getChildrenList();

    auto it = temp.begin();
    m_name = ((IDENTIFIER *)(*it))->getLabel();

    if (temp.size() > 1)
    {
        it++;
        m_value = (*it)->evaluate();
    }

    return m_value;
}

std::string variable_declaration::getName() { return m_name; }

int variable_declaration::getValue() { return m_value; }

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

int statement::evaluate()
{
    auto it = this->getChildrenList().begin();

    return (*it)->evaluate();
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

int statement_list::evaluate()
{
    int result = 0;

    for (const auto &child : this->getChildrenList())
    {
        result += child->evaluate();
    }

    return result;
}

compound_statement::compound_statement(statement_list *statement_list)
    : STNode(COMPMOUNT_STATEMENT_NODE, {statement_list})
{
}

compound_statement::compound_statement() : STNode(COMPMOUNT_STATEMENT_NODE, {})
{
}

int compound_statement::evaluate()
{
    auto it = this->getChildrenList().begin();

    return (*it)->evaluate();
}

condition::condition(expression *expression)
    : STNode(CONDITION_NODE, {expression})
{
}

int condition::evaluate()
{
    auto it = this->getChildrenList().begin();

    return (*it)->evaluate();
}

if_statement::if_statement(condition *condition, statement_list *statement_list)
    : STNode(IF_STATEMENT_NODE, {condition, statement_list})
{
}

if_statement::if_statement(condition *condition,
                           statement_list *statement_list1,
                           statement_list *statement_list2)
    : STNode(IF_STATEMENT_NODE, {condition, statement_list1, statement_list2})
{
}

int if_statement::evaluate()
{
    auto it = this->getChildrenList().begin();
    int cond = (*it)->evaluate();
    int result = 0;

    if (cond)
    {
        it++;
        result = (*it)->evaluate();
    }
    else if (it != this->getChildrenList().end())
    {
        ++ ++it;
        result = (*it)->evaluate();
    }

    std::cout << result << std::endl;
    return result;
}

type_specifier::type_specifier(dataType dataType)
    : STNode(TYPE_SPECIFIER_NODE, {})
{
    m_type = dataType;
}

dataType type_specifier::getType() { return m_type; }

function_call::function_call(IDENTIFIER *IDENTIFIER)
    : STNode(FUNCTION_CALL_NODE, {IDENTIFIER})
{
}

function_call::function_call(IDENTIFIER *IDENTIFIER,
                             argument_list *argument_list)
    : STNode(FUNCTION_CALL_NODE, {IDENTIFIER, argument_list})
{
}

int function_call::evaluate()
{
    std::vector<int> finalValues;
    auto childs = this->getChildrenList();

    auto it = childs.begin();
    std::string func_name = ((IDENTIFIER *)*it)->getLabel();
    std::cout << func_name << std::endl;
    Symbol *def = SymbolTable::getInstance()->lookupGlobal(func_name);

    if (!def)
    { // Need to make it so the error is emitted from the parser.
        perror("The function you try to call isn't defined");
    }

    compound_statement *func_body =
        (compound_statement *)(def->getFunctionBody());

    if (!func_body)
    { // Need to make it so the error is emitted from the parser.
        perror("The function you try to call isn't implemented");
    }

    if (childs.size() == 1) // No arguments
    {
    }
    else // With arguments
    {
        it++;
        auto expressions = ((argument_list *)(*it))->getArguments();

        for (auto expr : expressions)
        {
            finalValues.push_back(expr->evaluate());
        }
    }

    // Here it should do the type check as well!
    std::vector<parameter> &func_params = def->getParameters();
    if (func_params.size() != finalValues.size())
    { // Need to make it so the error is emitted from the parser.
        perror("The function you try to call dose not have the same arguments "
               "as parameters that are defined");
    }

    SymbolTable::getInstance()->enterScope();

    for (size_t i = 0; i < func_params.size(); i++)
    {
        Symbol param;
        param.setName(func_params[i].name);
        param.setType(func_params[i].type);
        param.setValue(finalValues[i]);

        SymbolTable::getInstance()->insert(param);
    }

    try
    {
        func_body->evaluate();
    }
    catch (int returnValue)
    {
        SymbolTable::getInstance()->exitScope();
        return returnValue;
    }

    SymbolTable::getInstance()->exitScope();
    if (def->getType() != T_VOID)
    { // Semantic-error
        std::cout << "Non-void functions should return a value" << std::endl;
    }

    return 0;
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

void argument_list::add(STNode *expression) { arguments.push_back(expression); }

std::vector<STNode *> argument_list::getArguments() { return arguments; }

parameter_list::parameter_list(type_specifier *type_specifier,
                               IDENTIFIER *IDENTIFIER)
    : STNode(PARAMETER_LIST_NODE, {type_specifier, IDENTIFIER})
{
    this->add(type_specifier->getType(), IDENTIFIER->getLabel());
}

parameter_list::parameter_list() : STNode(PARAMETER_LIST_NODE, {}) {}

void parameter_list::add(dataType type, std::string name)
{
    parameters.push_back({type, name});
}

std::vector<parameter> parameter_list::getParameters() { return parameters; }

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

int return_node::evaluate()
{
    auto it = this->getChildrenList().begin();

    throw((*it)->evaluate());
}

program::program(translation_unit *translation_unit)
    : STNode(PROGRAM_NODE, {translation_unit})
{
}

int program::evaluate()
{
    auto it = this->getChildrenList().begin();

    (*it)->evaluate();

    Symbol *entry = SymbolTable::getInstance()->lookupGlobal("main");
    if (entry == nullptr || entry->getFunctionBody())
    {
        std::cerr << "Error: Linker error - undefined reference to 'main'"
                  << std::endl;
        exit(1);
    }

    SymbolTable::getInstance()->enterScope(); // Main Scope
    // When i will make scopes for every compound statement this will leave
    // Because function_body is always a compound statement
    int result = entry->getFunctionBody()->evaluate();
    SymbolTable::getInstance()->exitScope();

    return result;
}
