#include "../lib/evaluator_visitor.hh"
#include <iostream>

EvaluatorVisitor::EvaluatorVisitor() {}

Value EvaluatorVisitor::getResult() { return m_result; }

void EvaluatorVisitor::visitIDENTIFIER(IDENTIFIER *node)
{
    VarSymbol *sym = dynamic_cast<VarSymbol *>(
        SymbolTable::getInstance()->lookup(node->getLabel()));

    m_result = sym->getValue();
}

void EvaluatorVisitor::visitNUMBER(NUMBER *node)
{
    if (node->getResolvedType() == T_INT)
    {
        m_result = node->getIValue();
    }
    else if (node->getResolvedType() == T_FLOAT)
    {
        m_result = (int)node->getFValue();
    }
    // This is temp solution.
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
        std::cerr << "Runtime Error: Cant divide with 0" << std::endl;
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

    IDENTIFIER *id = static_cast<IDENTIFIER *>(*it);

    std::string name = id->getLabel();
    VarSymbol *sym =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    Value old_value = sym->getValue();
    sym->setValue(old_value + 1);
    m_result = old_value;
}

void EvaluatorVisitor::visitPostfixDecrement(postfix_decrement *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = static_cast<IDENTIFIER *>(*it);

    std::string name = id->getLabel();
    VarSymbol *sym =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    Value old_value = sym->getValue();
    sym->setValue(old_value - 1);
    m_result = old_value;
}

void EvaluatorVisitor::visitPrefixIncrement(prefix_increment *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = static_cast<IDENTIFIER *>(*it);

    std::string name = id->getLabel();
    VarSymbol *sym =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    sym->setValue(sym->getValue() + 1);
    m_result = sym->getValue();
}

void EvaluatorVisitor::visitPrefixDecrement(prefix_decrement *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = static_cast<IDENTIFIER *>(*it);

    std::string name = id->getLabel();
    VarSymbol *sym =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    sym->setValue(sym->getValue() - 1);
    m_result = sym->getValue();
}

void EvaluatorVisitor::visitAssignment(assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

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
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

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
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

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
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

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
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    it++;
    (*it)->accept(*this);
    if (!m_result)
    {
        std::cerr << "Runtime Error: Cant divide with 0" << std::endl;
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
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

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

    if (temp.size() > 1)
    {
        it++;
        (*it)->accept(*this);
    }
}

void EvaluatorVisitor::visitVariableDeclarationList(
    variable_declaration_list *node)
{
    auto &temp = node->getChildrenList();
    auto it = temp.begin();

    if (temp.size() == 2)
    {
        (*it)->accept(*this);
        it++;
        m_vars.push_back((*it));
    }
    else
    {
        m_vars.push_back((*it));
    }
}

void EvaluatorVisitor::visitVariableDeclarationStatement(
    variable_declaration_statement *node)
{
    auto it = node->getChildrenList().begin();

    dataType currentType = static_cast<type_specifier *>(*it)->getType();

    it++;
    (*it)->accept(*this);
    for (auto &var : m_vars)
    {
        var->accept(*this);

        // Need a way so i dont store the values into the AST.
        VarSymbol *sym = new VarSymbol(
            m_result,
            static_cast<IDENTIFIER *>(var->getChildrenList().front())
                ->getLabel(),
            currentType);

        SymbolTable::getInstance()->insert(sym);
    }

    m_vars.clear();
}

void EvaluatorVisitor::visitStatement(statement *node)
{
    auto temp = node->getChildrenList().front();

    if (temp)
    {
        temp->accept(*this);
    }
}

void EvaluatorVisitor::visitCondition(condition *node)
{
    node->getChildrenList().front()->accept(*this);
}

void EvaluatorVisitor::visitIfStatement(if_statement *node)
{
    auto it = node->getChildrenList().begin();

    (*it)->accept(*this);
    Value cond = m_result;
    it++;

    if (cond)
    {
        (*it)->accept(*this);
    }
    else if (node->getChildrenList().size() == 3)
    {
        it++;
        (*it)->accept(*this);
    }
}

void EvaluatorVisitor::visitWhileStatement(while_statement *node)
{
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
}

void EvaluatorVisitor::visitDoWhileStatement(do_while_statement *node)
{
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
}

void EvaluatorVisitor::visitForStatement(for_statement *node)
{
    auto it = node->getChildrenList().begin();

    (*it)->accept(*this); // first part or for loop
    it++;

    STNode *cond = (*it); // second part
    it++;

    STNode *inc = nullptr;
    if (node->getChildrenList().size() == 4)
    {
        inc = (*it); // third part
        it++;
    }

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

        if (inc != nullptr)
        {
            inc->accept(*this);
        }

        cond->accept(*this);
    }
}

void EvaluatorVisitor::visitContinue(continue_node *node)
{
    throw continue_signal();
}

void EvaluatorVisitor::visitBreak(break_node *node) { throw break_signal(); }

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

void EvaluatorVisitor::visitArgumentList(argument_list *node)
{
    auto childs = node->getChildrenList();

    if (childs.size() == 2)
    {
        auto it = childs.begin();
        (*it)->accept(*this);

        // add expression to vector.
        it++;
        m_args.push_back(static_cast<expression *>(*it));
    }
    else
    {
        // add expression to vector.
        m_args.push_back(node->getChildrenList().front());
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

    compound_statement *func_body =
        static_cast<compound_statement *>(def->getFunctionBody());

    if (childs.size() == 1) // No arguments
    {
    }
    else // With arguments
    {
        it++;
        (*it)->accept(*this);

        for (auto &expr : m_args)
        {
            expr->accept(*this);
            finalValues.push_back(m_result);
        }

        m_args.clear();
    }

    // Calculating the arguments of the function call
    std::vector<parameter> &func_params = def->getParameters();

    SymbolTable::getInstance()->enterScope(
        SymbolTable::getInstance()->getCurrentId() + 1);

    for (size_t i = 0; i < func_params.size(); i++)
    {
        VarSymbol *param = new VarSymbol(finalValues[i], func_params[i].name,
                                         func_params[i].type);

        SymbolTable::getInstance()->insert(param);
    }

    try
    {
        func_body->accept(*this);
    }
    catch (Value return_value)
    {
        SymbolTable::getInstance()->exitScope();
    }
    catch (void_return_signal)
    {
        SymbolTable::getInstance()->exitScope();

        m_result = 0;
    }

    SymbolTable::getInstance()->exitScope();
}

void EvaluatorVisitor::visitProgram(program *node)
{
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

    // if (!entry->getParameters().empty())
    // {
    //     std::cerr << "Cant support main parameters (maybe for now)"
    //               << std::endl;
    //     exit(1);
    // }

    SymbolTable::getInstance()->enterScope(
        SymbolTable::getInstance()->getCurrentId() + 1);
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

    SymbolTable::getInstance()->exitScope();
}
