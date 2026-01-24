#include "../lib/type_checker_visitor.hh"
#include <cstddef>
#include <iostream>
#include <string>

TypeCheckerVisitor::TypeCheckerVisitor()
{
    m_last_type = T_VOID;
    m_expected_return_type = T_VOID;
    m_found_return = false;
    m_loop_depth = 0;
}

// --- Helper methods ---
void TypeCheckerVisitor::semanticError(std::string s)
{
    std::cerr << "Semantic Error: " + s << std::endl;
    exit(1);
}

std::string TypeCheckerVisitor::typeToString(dataType type)
{
    switch (type)
    {
    case T_VOID:
        return "void";

    case T_INT:
        return "int";

    case T_FLOAT:
        return "float";
    }

    return "";
}

dataType TypeCheckerVisitor::checkMathTypes(dataType left, dataType right,
                                            std::string op)
{

    if (left == T_FLOAT || right == T_FLOAT)
    {
        return T_FLOAT;
    }
    else if (left == T_INT && right == T_INT)
    {
        return T_INT;
    }

    semanticError("Type Mismatch: Cannot perform '" + op + "' on " +
                  typeToString(left) + " and " + typeToString(right));
    return T_VOID;
}

dataType TypeCheckerVisitor::checkLogicalTypes(dataType left, dataType right,
                                               std::string op)
{
    if (left == T_VOID || right == T_VOID)
    {
        semanticError("Type Error: Logical operator '" + op +
                      "' cannot operate on void operands.");
    }

    // 2. Warning for Floats (Optional but good practice)
    // While C allows (5.5 && 1.0), it is often a bug.
    // if (left == T_FLOAT || right == T_FLOAT) { ... warn ... }
    return T_INT;
}

dataType TypeCheckerVisitor::checkBitwiseTypes(dataType left, dataType right,
                                               std::string op)
{
    if (left != T_INT || right != T_INT)
    {
        semanticError("Type Error: Bitwise operator '" + op +
                      "' requires integer operands. Got " + typeToString(left) +
                      " and " + typeToString(right));
    }

    return T_INT;
}

bool TypeCheckerVisitor::isCompatible(dataType target, dataType source)
{

    if (target == source)
    {
        return true;
    }
    if (target == T_FLOAT && source == T_INT)
    {
        return true; // Upgrade
    }
    if (target == T_INT && source == T_FLOAT)
    {
        return true; // Truncation (C allows this)
    }

    return false;
}

// --- Visitors ---
void TypeCheckerVisitor::visitIDENTIFIER(IDENTIFIER *node)
{
    VarSymbol *sym = dynamic_cast<VarSymbol *>(
        SymbolTable::getInstance()->lookup(node->getLabel()));

    if (!sym)
    {
        semanticError("Identifier" + node->getLabel() + "not defined in scope");
    }

    m_last_type = sym->getValueType();
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitNUMBER(NUMBER *node)
{
    m_last_type = node->getResolvedType();
}

void TypeCheckerVisitor::visitAddition(addition *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkMathTypes(leftType, rightType, "Addition (+)");
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitSubtraction(subtraction *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkMathTypes(leftType, rightType, "Subtraction (-)");
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitMultiplication(multiplication *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkMathTypes(leftType, rightType, "Multiplication (*)");
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitDivision(division *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkMathTypes(leftType, rightType, "Division (/)");
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitMod(mod *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    // Modulo usually ONLY works on Integers in C-like languages
    if (leftType != T_INT || rightType != T_INT)
    {
        semanticError("Modulo operator (%) requires Integer operands.");
    }

    m_last_type = T_INT;
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitLess(less *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    checkMathTypes(leftType, rightType, "Less Than (<)");

    // RESULT IS ALWAYS INT (True/False)
    m_last_type = T_INT;
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitLessEquals(less_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkLogicalTypes(leftType, rightType, "Less Equals (<=)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitGreater(greater *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkLogicalTypes(leftType, rightType, "Greater Than (>)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitGreaterEquals(greater_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkLogicalTypes(leftType, rightType, "Greater Equals (>=)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitLogicEquals(logic_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkLogicalTypes(leftType, rightType, "Equals (==)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitLogicNotEquals(logic_not_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkLogicalTypes(leftType, rightType, "Not Equals (!=)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitLogicAnd(logic_and *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkLogicalTypes(leftType, rightType, "Logical And (&&)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitLogicOr(logic_or *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkLogicalTypes(leftType, rightType, "Logical Or (||)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitLogicNot(logic_not *node)
{
    node->getChildrenList().front()->accept(*this);

    if (m_last_type == T_VOID)
    {
        semanticError("Logical NOT (!) invalid operand.");
    }

    m_last_type = T_INT;
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitBitWiseAnd(bit_wise_and *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkBitwiseTypes(leftType, rightType, "Bitwise And (&)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitBitWiseOr(bit_wise_or *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkBitwiseTypes(leftType, rightType, "Bitwise Or (|)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitBitWiseXor(bit_wise_xor *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type = checkBitwiseTypes(leftType, rightType, "Bitwise XOR (^)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitBitWiseNot(bit_wise_not *node)
{
    node->getChildrenList().front()->accept(*this);

    if (m_last_type != T_INT)
    {
        semanticError("Bitwise NOT (~) requires Integer operand.");
    }

    m_last_type = T_INT;
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitShiftLeft(shift_left *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type =
        checkBitwiseTypes(leftType, rightType, "Bitwise Shift Left (<<)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitShiftRight(shift_right *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType leftType = m_last_type;

    node->getChildrenList().back()->accept(*this);
    dataType rightType = m_last_type;

    m_last_type =
        checkBitwiseTypes(leftType, rightType, "Bitwise Shift Right (>>)");
    node->setResolvedType(T_INT);
}

void TypeCheckerVisitor::visitPostfixIncrement(postfix_increment *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = dynamic_cast<IDENTIFIER *>(*it);
    if (!id)
    {
        semanticError("Increment operator (++) requires a "
                      "variable (l-value).");
    }

    std::string name = id->getLabel();
    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        semanticError("Variable \"" + name + "\" is not declared");
    }

    m_last_type = sym->getValueType();
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitPostfixDecrement(postfix_decrement *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = dynamic_cast<IDENTIFIER *>(*it);
    if (!id)
    {
        semanticError("Increment operator (++) requires a "
                      "variable (l-value).");
    }

    std::string name = id->getLabel();
    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        semanticError("Variable \"" + name + "\" is not declared");
    }

    m_last_type = sym->getValueType();
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitPrefixIncrement(prefix_increment *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = dynamic_cast<IDENTIFIER *>(*it);
    if (!id)
    {
        semanticError("Increment operator (++) requires a "
                      "variable (l-value).");
    }

    std::string name = id->getLabel();
    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        semanticError("Variable \"" + name + "\" is not declared");
    }

    m_last_type = sym->getValueType();
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitPrefixDecrement(prefix_decrement *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = dynamic_cast<IDENTIFIER *>(*it);
    if (!id)
    {
        semanticError("Increment operator (++) requires a "
                      "variable (l-value).");
    }

    std::string name = id->getLabel();
    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        semanticError("Variable \"" + name + "\" is not declared");
    }

    m_last_type = sym->getValueType();
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitPlusAssignment(plus_assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        semanticError("Identifier \"" + name + "\" not defined in scope");
    }

    it++;
    (*it)->accept(*this);
    dataType rhsType = m_last_type;
    dataType lhsType = sym->getValueType();

    if (!isCompatible(lhsType, rhsType))
    {
        semanticError("Cannot assign " + typeToString(rhsType) + " to " +
                      typeToString(lhsType));
    }

    m_last_type = lhsType;
    node->setResolvedType(lhsType);
}

void TypeCheckerVisitor::visitMinusAssignment(minus_assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        semanticError("Identifier \"" + name + "\" not defined in scope");
    }

    it++;
    (*it)->accept(*this);
    dataType rhsType = m_last_type;
    dataType lhsType = sym->getValueType();

    if (!isCompatible(lhsType, rhsType))
    {
        semanticError("Cannot assign " + typeToString(rhsType) + " to " +
                      typeToString(lhsType));
    }

    m_last_type = lhsType;
    node->setResolvedType(lhsType);
}

void TypeCheckerVisitor::visitMulAssignment(mul_assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        semanticError("Identifier \"" + name + "\" not defined in scope");
    }

    it++;
    (*it)->accept(*this);
    dataType rhsType = m_last_type;
    dataType lhsType = sym->getValueType();

    if (!isCompatible(lhsType, rhsType))
    {
        semanticError("Cannot assign " + typeToString(rhsType) + " to " +
                      typeToString(lhsType));
    }

    m_last_type = lhsType;
    node->setResolvedType(lhsType);
}

void TypeCheckerVisitor::visitDivAssignment(div_assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        semanticError("Identifier \"" + name + "\" not defined in scope");
    }

    it++;
    (*it)->accept(*this);
    dataType rhsType = m_last_type;
    dataType lhsType = sym->getValueType();

    if (!isCompatible(lhsType, rhsType))
    {
        semanticError("Cannot assign " + typeToString(rhsType) + " to " +
                      typeToString(lhsType));
    }

    m_last_type = lhsType;
    node->setResolvedType(lhsType);
}

void TypeCheckerVisitor::visitModAssignment(mod_assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        semanticError("Identifier \"" + name + "\" not defined in scope");
    }

    it++;
    (*it)->accept(*this);
    dataType rhsType = m_last_type;
    dataType lhsType = sym->getValueType();

    if (lhsType != T_INT || rhsType != T_INT)
    {
        semanticError("Modulo assignment (%=) requires Integer operands.");
    }

    m_last_type = lhsType;
    node->setResolvedType(lhsType);
}

void TypeCheckerVisitor::visitAssignment(assignment *node)
{
    auto it = node->getChildrenList().begin();

    std::string name = static_cast<IDENTIFIER *>(*it)->getLabel();

    VarSymbol *sym =
        dynamic_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    if (!sym)
    {
        semanticError("Identifier \"" + name + "\" not defined in scope");
    }

    it++;
    (*it)->accept(*this);
    dataType rhsType = m_last_type;
    dataType lhsType = sym->getValueType();

    if (!isCompatible(lhsType, rhsType))
    {
        semanticError("Cannot assign " + typeToString(rhsType) + " to " +
                      typeToString(lhsType));
    }

    m_last_type = lhsType;
    node->setResolvedType(lhsType);
}

void TypeCheckerVisitor::visitParameterList(parameter_list *node)
{
    auto childs = node->getChildrenList();
    dataType type = T_VOID;

    if (childs.size() == 3)
    {
        auto it = childs.begin();
        (*it)->accept(*this);

        // add type specifier and id to a vector.
        it++;
        type = static_cast<type_specifier *>(*it)->getType();
        if (type == T_VOID)
        {
            semanticError("Void type parameters are not allowed");
        }
        it++;
        std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();

        parameter param = {type, id};
        m_params.push_back(param);
    }
    else if (childs.size() == 2)
    {
        // add type specifier and id to a vector.
        auto it = childs.begin();
        type = static_cast<type_specifier *>(*it)->getType();
        if (type == T_VOID)
        {
            semanticError("Void type parameters are not allowed");
        }
        it++;
        std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();

        parameter param = {type, id};
        m_params.push_back(param);
    }
    else
    {
        // Empty list
    }

    node->setResolvedType(type);
    m_last_type = type;
}

void TypeCheckerVisitor::visitArgumentList(argument_list *node)
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

    // node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitFunctionCall(function_call *node)
{
    std::vector<dataType> final_types;
    auto childs = node->getChildrenList();

    auto it = childs.begin();
    std::string func_name = (static_cast<IDENTIFIER *>(*it))->getLabel();

    FuncSymbol *def = dynamic_cast<FuncSymbol *>(
        SymbolTable::getInstance()->lookupGlobal(func_name));

    if (!def)
    {
        semanticError("Function \"" + func_name +
                      "\" isn't defined, thus you can't call it");
    }

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
            final_types.push_back(m_last_type);
        }

        m_args.clear();
    }

    std::vector<parameter> &func_params = def->getParameters();
    if (func_params.size() != final_types.size())
    {
        semanticError("Function \"" + func_name +
                      "\" has different number or parameters declared that "
                      "arguments given on call");
    }

    for (size_t i = 0; i < func_params.size(); i++)
    {
        if (!isCompatible(func_params[i].type, final_types[i]))
        {
            semanticError("Argument number " + std::to_string(i) +
                          " has a different type than the parameter declared "
                          "at the same position");
        }
    }

    m_last_type = def->getReturnType();
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitFunctionDeclaration(function_declaration *node)
{
    auto it = node->getChildrenList().begin();

    dataType return_type = static_cast<type_specifier *>(*it)->getType();
    it++;
    std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();
    it++;
    (*it)->accept(*this);

    FuncSymbol *sym = new FuncSymbol(return_type, nullptr, m_params, id);

    if (!SymbolTable::getInstance()->insert(sym))
    {
        semanticError("Function \"" + id + "\" already declared");
    }

    m_params.clear();
}

void TypeCheckerVisitor::visitFunctionDefinition(function_definition *node)
{
    auto it = node->getChildrenList().begin();

    dataType return_type = static_cast<type_specifier *>(*it)->getType();
    it++;
    std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();
    it++;
    // std::vector<parameter> &params =
    // static_cast<parameter_list *>(*it)->getParameters();
    (*it)->accept(*this);
    it++;
    compound_statement *body = static_cast<compound_statement *>(*it);

    FuncSymbol *existing = dynamic_cast<FuncSymbol *>(
        SymbolTable::getInstance()->lookupGlobal(id));

    if (existing)
    {
        if (existing->getReturnType() != return_type)
        {
            semanticError("Conflicting return type for function " + id);
        }
        else if (existing->getParameters() != m_params)
        {
            semanticError(
                "Different parameters declared than defined for function " +
                id);
        }
        else if (existing->getFunctionBody() != nullptr)
        {
            semanticError("Function \"" + id + "\" already defined");
        }
        existing->setFunctionBody(body);
    }
    else
    {
        FuncSymbol *sym = new FuncSymbol(return_type, body, m_params, id);
        if (!SymbolTable::getInstance()->insertGlobal(sym))
        {
            semanticError("Function \"" + id + "\" already defined");
        }
    }

    m_expected_return_type = return_type;
    m_found_return = false;

    SymbolTable::getInstance()->enterScope(
        SymbolTable::getInstance()->getCurrentId() + 1);

    for (auto &param : m_params)
    {
        VarSymbol *sym = new VarSymbol(0, param.name, param.type);
        SymbolTable::getInstance()->insert(sym);
    }

    m_params.clear();
    body->accept(*this);

    SymbolTable::getInstance()->exitScope();

    if (return_type != T_VOID && !m_found_return)
    {
        semanticError("Non-void function \"" + id + "\" should return a value");
    }

    m_expected_return_type = T_VOID;
}

void TypeCheckerVisitor::visitVariableDeclaration(variable_declaration *node)
{
    auto &temp = node->getChildrenList();
    auto it = temp.begin();

    it++;

    if (temp.size() > 1)
    {
        (*it)->accept(*this);
    }
    else
    {
        m_last_type = T_VOID;
    }

    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitVariableDeclarationList(
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

void TypeCheckerVisitor::visitVariableDeclarationStatement(
    variable_declaration_statement *node)
{
    auto it = node->getChildrenList().begin();

    dataType current_type = static_cast<type_specifier *>(*it)->getType();

    it++;
    (*it)->accept(*this);

    for (auto &var : m_vars)
    {
        var->accept(*this);

        std::string name =
            static_cast<IDENTIFIER *>(var->getChildrenList().front())
                ->getLabel();

        if (var->getChildrenList().size() > 1)
        {
            if (!isCompatible(current_type, m_last_type))
            {
                semanticError("Type Mismatch, cannot initialize variable \"" +
                              name +
                              "\" with "
                              "conflicting types of " +
                              typeToString(current_type) + " and " +
                              typeToString(m_last_type));
            }
        }

        VarSymbol *sym = new VarSymbol(
            0,
            static_cast<IDENTIFIER *>(var->getChildrenList().front())
                ->getLabel(),
            current_type);

        if (!SymbolTable::getInstance()->insert(sym))
        {
            semanticError("Variable \"" + sym->getName() +
                          "\" already exists.");
        }
    }

    m_vars.clear();
    node->setResolvedType(current_type);
}

void TypeCheckerVisitor::visitReturn(return_node *node)
{
    m_found_return = true;
    auto childs = node->getChildrenList();

    if (childs.empty())
    {
        if (m_expected_return_type != T_VOID)
        {
            semanticError(
                "Return-statement with no value, in function returning.");
        }
        node->setResolvedType(T_VOID);
        return;
    }

    childs.front()->accept(*this);

    if (!isCompatible(m_expected_return_type, m_last_type))
    {
        // Check if i want to make promotions
        semanticError("Expression in return statement must not conflict with "
                      "expected return type");
    }
    node->setResolvedType(m_last_type);
}

void TypeCheckerVisitor::visitIfStatement(if_statement *node)
{
    auto it = node->getChildrenList().begin();

    // Condition
    (*it)->accept(*this);
    if (m_last_type == T_VOID)
    {
        semanticError("If statement condition should not be void type");
    }

    it++;
    // First body
    (*it)->accept(*this);

    if (node->getChildrenList().size() == 3)
    {
        it++;
        // Second body
        (*it)->accept(*this);
    }

    m_last_type = T_VOID;
    node->setResolvedType(T_VOID);
}

void TypeCheckerVisitor::visitWhileStatement(while_statement *node)
{
    auto it = node->getChildrenList().begin();

    // Condition
    (*it)->accept(*this);
    if (m_last_type == T_VOID)
    {
        semanticError("While statement condition should not be void type");
    }

    // Loop body
    it++;
    m_loop_depth++;
    (*it)->accept(*this);
    m_loop_depth--;

    m_last_type = T_VOID;
    node->setResolvedType(T_VOID);
}

void TypeCheckerVisitor::visitDoWhileStatement(do_while_statement *node)
{
    auto it = node->getChildrenList().begin();

    // Condition
    (*it)->accept(*this);
    if (m_last_type == T_VOID)
    {
        semanticError("While statement condition should not be void type");
    }

    // Loop body
    it++;
    m_loop_depth++;
    (*it)->accept(*this);
    m_loop_depth--;

    m_last_type = T_VOID;
    node->setResolvedType(T_VOID);
}

void TypeCheckerVisitor::visitForStatement(for_statement *node)
{
    auto it = node->getChildrenList().begin();

    // Initialization
    (*it)->accept(*this);

    // Condition
    it++;
    (*it)->accept(*this);
    if (m_last_type == T_VOID && (*it)->getNodeType() != STATEMENT_NODE)
    {
        semanticError("For statement condition should not be void type");
    }

    if (node->getChildrenList().size() == 3)
    {
        // Loop body
        it++;
        m_loop_depth++;
        (*it)->accept(*this);
        m_loop_depth--;
    }
    else
    {
        // Increment
        it++;
        (*it)->accept(*this);

        // Loop body
        it++;
        m_loop_depth++;
        (*it)->accept(*this);
        m_loop_depth--;
    }

    m_last_type = T_VOID;
    node->setResolvedType(T_VOID);
}

void TypeCheckerVisitor::visitContinue(continue_node *node)
{
    if (m_loop_depth == 0)
    {
        semanticError("Continue statement used outside of a loop.");
    }
}

void TypeCheckerVisitor::visitBreak(break_node *node)
{
    if (m_loop_depth == 0)
    {
        semanticError("Break statement used outside of a loop.");
    }
}

void TypeCheckerVisitor::visitCondition(condition *node)
{
    (*node->getChildrenList().begin())->accept(*this);
    node->setResolvedType(m_last_type);
}
