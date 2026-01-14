#include "../lib/evaluator_visitor.hh"
#include <iostream>

EvaluatorVisitor::EvaluatorVisitor() {}

Value EvaluatorVisitor::getResult() { return m_result; }

void EvaluatorVisitor::visitIDENTIFIER(IDENTIFIER *node)
{
    VarSymbol *sym = dynamic_cast<VarSymbol *>(
        SymbolTable::getInstance()->lookup(node->getLabel()));

    if (!sym)
    {
        std::cerr << "Identifier not defined in scope" << std::endl;
        exit(1);
    }

    m_result = sym->getValue();
}

void EvaluatorVisitor::visitNUMBER(NUMBER *node)
{
    m_result = node->getValue();
}

void EvaluatorVisitor::visitUnaryPlus(unary_plus *node)
{
    node->getChildrenList().front()->accept(*this);

    m_result = +m_result;
}

void EvaluatorVisitor::visitUnaryMinus(unary_minus *node)
{
    node->getChildrenList().front()->accept(*this);

    m_result = -m_result;
}

void EvaluatorVisitor::visitAddition(addition *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result + right_result;
}

void EvaluatorVisitor::visitMultiplication(multiplication *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result * right_result;
}

void EvaluatorVisitor::visitSubtraction(subtraction *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result - right_result;
}

void EvaluatorVisitor::visitMod(mod *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result % right_result;
}

void EvaluatorVisitor::visitDivision(division *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    if (!right_result)
    {
        std::cerr << "Cant divide with 0" << std::endl;
        exit(1);
    }

    m_result = left_result / right_result;
}

void EvaluatorVisitor::visitLess(less *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result < right_result;
}

void EvaluatorVisitor::visitLessEquals(less_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result <= right_result;
}

void EvaluatorVisitor::visitGreater(greater *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result > right_result;
}

void EvaluatorVisitor::visitGreaterEquals(greater_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result >= right_result;
}

void EvaluatorVisitor::visitLogicEquals(logic_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result == right_result;
}

void EvaluatorVisitor::visitLogicAnd(logic_and *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result && right_result;
}

void EvaluatorVisitor::visitLogicOr(logic_or *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result || right_result;
}

void EvaluatorVisitor::visitLogicNotEquals(logic_not_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result != right_result;
}

void EvaluatorVisitor::visitLogicNot(logic_not *node)
{
    node->getChildrenList().front()->accept(*this);

    m_result = !m_result;
}

void EvaluatorVisitor::visitBitWiseAnd(bit_wise_and *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result & right_result;
}

void EvaluatorVisitor::visitBitWiseOr(bit_wise_or *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result | right_result;
}

void EvaluatorVisitor::visitBitWiseXor(bit_wise_xor *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result ^ right_result;
}

void EvaluatorVisitor::visitBitWiseNot(bit_wise_not *node)
{
    node->getChildrenList().front()->accept(*this);

    m_result = ~m_result;
}

void EvaluatorVisitor::visitShiftLeft(shift_left *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result << right_result;
}

void EvaluatorVisitor::visitShiftRight(shift_right *node)
{
    node->getChildrenList().front()->accept(*this);
    Value left_result = m_result;

    node->getChildrenList().back()->accept(*this);
    Value right_result = m_result;

    m_result = left_result >> right_result;
}

void EvaluatorVisitor::visitPostfixIncrement(postfix_increment *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = dynamic_cast<IDENTIFIER *>(*it);
    // TODO: This is the work of the type checker later
    if (!id)
    {
        std::cerr << "Semantic Error: Increment operator (++) requires a "
                     "variable (l-value)."
                  << std::endl;
        exit(1);
    }

    std::string name = id->getLabel();
    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        std::cerr << "Runtime Error: Variable \"" << name
                  << "\" is not declared" << std::endl;
        exit(1);
    }

    Value old_value = sym->getValue();
    sym->setValue(old_value + 1);
    m_result = old_value;
}

void EvaluatorVisitor::visitPostfixDecrement(postfix_decrement *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = dynamic_cast<IDENTIFIER *>(*it);
    // TODO: This is the work of the type checker later
    if (!id)
    {
        std::cerr << "Semantic Error: Increment operator (++) requires a "
                     "variable (l-value)."
                  << std::endl;
        exit(1);
    }

    std::string name = id->getLabel();
    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        std::cerr << "Runtime Error: Variable \"" << name
                  << "\" is not declared" << std::endl;
        exit(1);
    }

    Value old_value = sym->getValue();
    sym->setValue(old_value - 1);
    m_result = old_value;
}

void EvaluatorVisitor::visitPrefixIncrement(prefix_increment *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = dynamic_cast<IDENTIFIER *>(*it);
    // TODO: This is the work of the type checker later
    if (!id)
    {
        std::cerr << "Semantic Error: Increment operator (++) requires a "
                     "variable (l-value)."
                  << std::endl;
        exit(1);
    }

    std::string name = id->getLabel();
    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        std::cerr << "Runtime Error: Variable \"" << name
                  << "\" is not declared" << std::endl;
        exit(1);
    }

    sym->setValue(sym->getValue() + 1);
    m_result = sym->getValue();
}

void EvaluatorVisitor::visitPrefixDecrement(prefix_decrement *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = dynamic_cast<IDENTIFIER *>(*it);
    // TODO: This is the work of the type checker later
    if (!id)
    {
        std::cerr << "Semantic Error: Increment operator (++) requires a "
                     "variable (l-value)."
                  << std::endl;
        exit(1);
    }

    std::string name = id->getLabel();
    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        std::cerr << "Runtime Error: Variable \"" << name
                  << "\" is not declared" << std::endl;
        exit(1);
    }

    sym->setValue(sym->getValue() - 1);
    m_result = sym->getValue();
}

void EvaluatorVisitor::visitAssignment(assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym) // In my evaluation methods with polymorphism i just the global
              // stack as well if any errors happen
    {
        std::cerr << "Semantic Error: identifier not declared" << std::endl;
        exit(1);
    }

    it++;
    (*it)->accept(*this);
    sym->setValue(m_result);

    // Debug print
    std::cout << name << "=" << std::to_string(m_result) << std::endl;
}

void EvaluatorVisitor::visitPlusAssignment(plus_assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym) // In my evaluation methods with polymorphism i just the global
              // stack as well if any errors happen
    {
        std::cerr << "Semantic Error: identifier not declared" << std::endl;
        exit(1);
    }

    it++;
    (*it)->accept(*this);
    sym->setValue(sym->getValue() + m_result);

    // Debug print
    std::cout << name << "=" << sym->getValue() << std::endl;
}

void EvaluatorVisitor::visitMinusAssignment(minus_assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym) // In my evaluation methods with polymorphism i just the global
              // stack as well if any errors happen
    {
        std::cerr << "Semantic Error: identifier not declared" << std::endl;
        exit(1);
    }

    it++;
    (*it)->accept(*this);
    sym->setValue(sym->getValue() - m_result);

    // Debug print
    std::cout << name << "=" << sym->getValue() << std::endl;
}

void EvaluatorVisitor::visitMulAssignment(mul_assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym) // In my evaluation methods with polymorphism i just the global
              // stack as well if any errors happen
    {
        std::cerr << "Semantic Error: identifier not declared" << std::endl;
        exit(1);
    }

    it++;
    (*it)->accept(*this);
    sym->setValue(sym->getValue() * m_result);

    // Debug print
    std::cout << name << "=" << sym->getValue() << std::endl;
}

void EvaluatorVisitor::visitDivAssignment(div_assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym) // In my evaluation methods with polymorphism i just the global
              // stack as well if any errors happen
    {
        std::cerr << "Semantic Error: identifier not declared" << std::endl;
        exit(1);
    }

    it++;
    (*it)->accept(*this);
    if (!m_result)
    {
        std::cerr << "Trying to do a division by 0" << std::endl;
        exit(1);
    }
    sym->setValue(sym->getValue() / m_result);

    // Debug print
    std::cout << name << "=" << sym->getValue() << std::endl;
}

void EvaluatorVisitor::visitModAssignment(mod_assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym) // In my evaluation methods with polymorphism i just the global
              // stack as well if any errors happen
    {
        std::cerr << "Semantic Error: identifier not declared" << std::endl;
        exit(1);
    }

    it++;
    (*it)->accept(*this);
    sym->setValue(sym->getValue() % m_result);

    // Debug print
    std::cout << name << "=" << sym->getValue() << std::endl;
}

void EvaluatorVisitor::visitVariableDeclaration(variable_declaration *node)
{
    auto &temp = node->getChildrenList();
    auto it = temp.begin();

    node->setName(static_cast<IDENTIFIER *>(*it)->getLabel());
    it++;

    if (temp.size() > 1)
    {
        (*it)->accept(*this);

        node->setValue(m_result);
    }
}

void EvaluatorVisitor::visitVariableDeclarationStatement(
    variable_declaration_statement *node)
{
    auto it = node->getChildrenList().begin();

    dataType currentType = static_cast<type_specifier *>(*it)->getType();

    it++;
    std::vector<variable_declaration *> varList =
        static_cast<variable_declaration_list *>(*it)->getVariables();

    for (auto var : varList)
    {
        var->accept(*this);

        // Need a way so i dont store the values into the AST.
        VarSymbol *sym = new VarSymbol(
            m_result,
            static_cast<IDENTIFIER *>(var->getChildrenList().front())
                ->getLabel(),
            currentType);

        if (!SymbolTable::getInstance()->insert(sym))
        { // Semantic Error
            std::cerr << "Variable " << sym->getName() << " already exists."
                      << std::endl;
            exit(1);
        }
    }
}

void EvaluatorVisitor::visitStatement(statement *node)
{
    auto temp = node->getChildrenList().front();

    if (temp)
    {
        temp->accept(*this);
    }
}

void EvaluatorVisitor::visitCompoundStatement(compound_statement *node)
{
    node->getChildrenList().front()->accept(*this);
}

void EvaluatorVisitor::visitCondition(condition *node)
{
    node->getChildrenList().front()->accept(*this);
}

void EvaluatorVisitor::visitIfStatement(if_statement *node)
{
    SymbolTable::getInstance()->enterScope(
        SymbolTable::getInstance()->getCurrentId());

    auto it = node->getChildrenList().begin();

    (*it)->accept(*this);
    Value cond = m_result;
    it++;

    if (cond)
    {
        (*it)->accept(*this);
    }
    else if (node->getChildrenList().size() == 3)
    { // Maybe it dose not work that well :(
        it++;
        (*it)->accept(*this);
    }

    SymbolTable::getInstance()->exitScope();
}

void EvaluatorVisitor::visitWhileStatement(while_statement *node)
{
    SymbolTable::getInstance()->enterScope(
        SymbolTable::getInstance()->getCurrentId());

    auto it = node->getChildrenList().begin();

    condition *cond = static_cast<condition *>(*it);
    it++;

    cond->accept(*this);
    while (m_result)
    {
        try
        {
            (*it)->accept(*this);
        }
        catch (continue_signal)
        {
            // continue;
        }
        catch (break_signal)
        {
            break;
        }

        cond->accept(*this);
    }

    SymbolTable::getInstance()->exitScope();
}

void EvaluatorVisitor::visitDoWhileStatement(do_while_statement *node)
{
    SymbolTable::getInstance()->enterScope(
        SymbolTable::getInstance()->getCurrentId());

    auto it = node->getChildrenList().begin();

    compound_statement *body = static_cast<compound_statement *>(*it);
    it++;

    do
    {
        try
        {
            body->accept(*this);
        }
        catch (continue_signal)
        {
            continue;
        }
        catch (break_signal)
        {
            break;
        }
        (*it)->accept(*this);
    } while (m_result);

    SymbolTable::getInstance()->exitScope();
}

void EvaluatorVisitor::visitForStatement(for_statement *node)
{
    SymbolTable::getInstance()->enterScope(
        SymbolTable::getInstance()->getCurrentId());

    auto it = node->getChildrenList().begin();

    (*it)->accept(*this); // first part or for loop
    it++;

    STNode *cond = (*it); // second part
    it++;

    STNode *inc = (*it); // third part
    it++;

    cond->accept(*this);
    while (m_result)
    {
        try
        {
            (*it)->accept(*this); // the body
        }
        catch (continue_signal)
        {
            // continue;
        }
        catch (break_signal)
        {
            break;
        }

        inc->accept(*this);
        cond->accept(*this);
    }

    SymbolTable::getInstance()->exitScope();
}

void EvaluatorVisitor::visitContinue(continue_node *node)
{
    throw continue_signal();

    // Compiler cries if not used
    node->getChildrenList().front()->accept(*this);
}

void EvaluatorVisitor::visitBreak(break_node *node)
{
    throw break_signal();

    // Compiler cries if not used
    node->getChildrenList().front()->accept(*this);
}

void EvaluatorVisitor::visitReturn(return_node *node)
{
    if (!node->getChildrenList().size())
    {
        throw void_return_signal();
    }
    else
    {
        node->getChildrenList().front()->accept(*this);
        throw m_result;
    }
}

void EvaluatorVisitor::visitFunctionCall(function_call *node)
{
    std::vector<Value> finalValues;
    auto childs = node->getChildrenList();

    auto it = childs.begin();
    std::string func_name = (static_cast<IDENTIFIER *>(*it))->getLabel();

    // Debugging print
    // std::cout << func_name << std::endl;

    FuncSymbol *def = dynamic_cast<FuncSymbol *>(
        SymbolTable::getInstance()->lookupGlobal(func_name));

    if (!def)
    { // Need to make it so the error is emitted from the parser.
        std::cerr << "The function you try to call isn't defined" << std::endl;
        exit(1);
    }

    compound_statement *func_body =
        static_cast<compound_statement *>(def->getFunctionBody());

    if (!func_body)
    { // Need to make it so the error is emitted from the parser.
        std::cerr << "The function you try to call isn't implemented"
                  << std::endl;
        exit(1);
    }

    if (childs.size() == 1) // No arguments
    {
    }
    else // With arguments
    {
        it++;
        auto expressions = static_cast<argument_list *>(*it)->getArguments();

        for (auto expr : expressions)
        {
            expr->accept(*this);
            finalValues.push_back(m_result);
        }
    }

    // Here it should do the type check as well!
    std::vector<parameter> &func_params = def->getParameters();
    if (func_params.size() != finalValues.size()) // TODO: type checker work
    {
        std::cerr
            << "The function you try to call dose not have the same arguments "
               "as parameters that are defined"
            << std::endl;
        exit(1);
    }

    SymbolTable::getInstance()->enterScope(
        SymbolTable::getInstance()->getCurrentId() + 1);

    for (size_t i = 0; i < func_params.size(); i++)
    {
        VarSymbol *param = new VarSymbol(finalValues[i], func_params[i].name,
                                         func_params[i].type);

        SymbolTable::getInstance()->insert(param);
    }

    // I need this bool to know if non void function have return statement
    bool return_happened = false;
    try
    {
        func_body->accept(*this);
    }
    catch (Value return_value)
    {
        SymbolTable::getInstance()->exitScope();

        if (def->getReturnType() == T_VOID) // TODO: Type checker work
        {
            std::cerr << "Void functions should not return a value"
                      << std::endl;
            exit(1); // Should i exit or not?
        }

        return_happened = true;
    }
    catch (void_return_signal)
    {
        SymbolTable::getInstance()->exitScope();

        if (def->getReturnType() != T_VOID)
        {
            std::cerr << "Non-void functions should return a value"
                      << std::endl;
            exit(1); // Should i exit or not?
        }
        // Idk if this is necessary
        m_result = 0;

        return_happened = true;
    }

    if (def->getReturnType() != T_VOID &&
        return_happened == false) // TODO: type checker
    {
        SymbolTable::getInstance()->exitScope();

        std::cerr << "Non-void functions should return a value" << std::endl;
        exit(1); // Should i exit or not?
    }
}

void EvaluatorVisitor::visitProgram(program *node)
{
    // i do this because i dont want the compiler to cry
    // auto it = node->getChildrenList().begin();

    FuncSymbol *entry = dynamic_cast<FuncSymbol *>(
        SymbolTable::getInstance()->lookupGlobal("main"));
    if (entry == nullptr || !(entry->getFunctionBody()))
    {
        std::cerr << "Linker Error: Undefined reference to \"main\""
                  << std::endl;
        exit(1);
    }
    // When i will make scopes for every compound statement this will leave
    // Because function_body is always a compound statement

    if (!entry->getParameters().empty())
    {
        std::cerr << "Cant support main parameters (maybe for now)"
                  << std::endl;
        exit(1);
    }

    SymbolTable::getInstance()->enterScope(1);
    try
    {
        entry->getFunctionBody()->accept(*this);
    }
    catch (Value main_return)
    {
        SymbolTable::getInstance()->exitScope();
        if (!main_return)
        {
            std::cout << "Super!" << std::endl;
        }
        else
        {
            std::cout << "Not Super" << std::endl;
        }
    }

    // TODO: This should be on type checker
    // Handle main nor having return statement.
    SymbolTable::getInstance()->exitScope();
}
