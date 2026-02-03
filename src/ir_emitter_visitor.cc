#include "../lib/ir_emitter_visitor.hh"
#include <fstream>
#include <string>

IREmitterVisitor::IREmitterVisitor()
{
    m_reg_count = 0;
    m_label_count = 0;
    m_var_count = 0;
    m_file_ll.open("ir.ll");
    m_ll = &m_file_ll;
    SymbolTable::getInstance()->enterScope(
        SymbolTable::getInstance()->getCurrentId());
}

IREmitterVisitor::~IREmitterVisitor()
{
    m_file_ll.close();

    SymbolTable::getInstance()->exitScope();
}

std::string IREmitterVisitor::getNextReg()
{
    return "%" + std::to_string(m_reg_count++);
}

// Helper: Handles Binary Operation Promotions (Int <-> Float)
void IREmitterVisitor::binaryTypeTransition(dataType &type1, dataType &type2,
                                            std::string &reg1,
                                            std::string &reg2)
{
    if (type1 == T_FLOAT && type2 == T_INT)
    {
        std::string cast_reg = getNextReg();
        *m_ll << "\t" << cast_reg << " = sitofp i32 " << reg2 << " to float\n";
        reg2 = cast_reg;
        type2 = T_FLOAT;
    }
    else if (type1 == T_INT && type2 == T_FLOAT)
    {
        std::string cast_reg = getNextReg();
        *m_ll << "\t" << cast_reg << " = sitofp i32 " << reg1 << " to float\n";
        reg1 = cast_reg;
        type1 = T_FLOAT;
    }
}

// Helper: Handles Assignment Coercion (LHS type != RHS type)
// Note: This modifies m_last_reg directly to point to the new casted value
void IREmitterVisitor::assignmentTypeTransition(dataType lhsType,
                                                dataType &rhsType)
{
    if (lhsType == T_FLOAT && rhsType == T_INT)
    {
        std::string cast_reg = getNextReg();
        *m_ll << "\t" << cast_reg << " = sitofp i32 " << m_last_reg
              << " to float\n";
        m_last_reg = cast_reg;
        rhsType = T_FLOAT;
    }
    else if (lhsType == T_INT && rhsType == T_FLOAT)
    {
        std::string cast_reg = getNextReg();
        *m_ll << "\t" << cast_reg << " = fptosi float " << m_last_reg
              << " to i32\n";
        m_last_reg = cast_reg;
        rhsType = T_INT;
    }
}

void IREmitterVisitor::toInteger(dataType &type, std::string &reg)
{
    if (type == T_FLOAT)
    {
        std::string cur_reg = getNextReg();
        *m_ll << "\t" << cur_reg << " = fptosi float " << reg << " to i32\n";
        reg = cur_reg;
        type = T_INT;
    }
}

std::string IREmitterVisitor::operandType(dataType type)
{
    if (type == T_INT)
    {
        return ""; // "add"
    }
    else // if (type == T_FLOAT)
    {
        return "f"; // "fadd"
    }
}

std::string IREmitterVisitor::compareType(dataType type)
{
    if (type == T_INT)
    {
        return "icmp"; // "icmp"
    }
    else // if (type == T_FLOAT)
    {
        return "fcmp"; // "fcmp"
    }
}

std::string IREmitterVisitor::comparatorType(dataType type)
{
    if (type == T_INT)
    {
        return ""; // "eq"
    }
    else // if (type == T_FLOAT)
    {
        return "o"; // "oeq"
    }
}

std::string IREmitterVisitor::typeToString(dataType type)
{
    switch (type)
    {
    case T_VOID:
        return "void";
    case T_INT:
        return "i32";
    case T_FLOAT:
        return "float";
    default:
        return "void";
    }
}

std::string IREmitterVisitor::boolConvertor(dataType type, std::string reg)
{
    std::string cur_reg = getNextReg();
    if (type == T_INT)
    {
        *m_ll << "\t" << cur_reg << " = icmp ne i32 " << reg << ", 0\n";
    }
    else if (type == T_FLOAT)
    {
        *m_ll << "\t" << cur_reg << " = fcmp one float " << reg
              << ", 0.0e+00\n";
    }

    return cur_reg;
}

std::string IREmitterVisitor::getOne(dataType type)
{
    if (type == T_INT)
    {
        return "1";
    }
    else if (type == T_FLOAT)
    {
        return "1.0e+00";
    }

    return "";
}

// --- VISITORS ---

void IREmitterVisitor::visitIDENTIFIER(IDENTIFIER *node)
{
    std::string name = node->getLabel();
    VarSymbol *sym =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    std::string cur_reg = getNextReg();
    std::string type_str = typeToString(sym->getValueType());

    *m_ll << "\t" << cur_reg << " = load " << type_str << ", " << type_str
          << "* " << sym->getAddress() << "\n";

    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitNUMBER(NUMBER *node)
{
    if (node->getResolvedType() == T_INT)
    {
        m_last_reg = std::to_string(node->getIValue());
    }
    else if (node->getResolvedType() == T_FLOAT)
    {
        std::string temp = getNextReg();
        *m_ll << "\t" << temp + " = fptrunc double "
              << std::to_string(node->getFValue()) << " to float\n";
        m_last_reg = temp;
    }
}

void IREmitterVisitor::visitAddition(addition *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().back()->getResolvedType();

    binaryTypeTransition(left_type, right_type, left_reg, right_reg);

    dataType cur_type = node->getResolvedType();
    std::string op = operandType(cur_type) + "add";

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = " << op << " " << typeToString(cur_type)
          << " " << left_reg << ", " << right_reg << "\n";

    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitSubtraction(subtraction *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().back()->getResolvedType();

    binaryTypeTransition(left_type, right_type, left_reg, right_reg);

    dataType cur_type = node->getResolvedType();
    std::string op = operandType(cur_type) + "sub";

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = " << op << " " << typeToString(cur_type)
          << " " << left_reg << ", " << right_reg << "\n";

    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitMultiplication(multiplication *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().back()->getResolvedType();

    binaryTypeTransition(left_type, right_type, left_reg, right_reg);

    dataType cur_type = node->getResolvedType();
    std::string op = operandType(cur_type) + "mul";

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = " << op << " " << typeToString(cur_type)
          << " " << left_reg << ", " << right_reg << "\n";

    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitDivision(division *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().back()->getResolvedType();

    binaryTypeTransition(left_type, right_type, left_reg, right_reg);

    dataType cur_type = node->getResolvedType();
    std::string op = operandType(cur_type) + "div";

    if (cur_type == T_INT)
    {
        op = "s" + op;
    }

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = " << op << " " << typeToString(cur_type)
          << " " << left_reg << ", " << right_reg << "\n";

    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitMod(mod *node)
{
    // Modulo usually only works for integers (srem)
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;

    std::string cur_reg = getNextReg();
    // LLVM instruction for signed remainder is 'srem'
    *m_ll << "\t" << cur_reg << " = srem i32 " << left_reg << ", " << right_reg
          << "\n";

    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitLess(less *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().back()->getResolvedType();

    // Matching their type
    binaryTypeTransition(left_type, right_type, left_reg, right_reg);

    std::string comp = compareType(left_type);
    std::string op = comparatorType(left_type) + "lt";
    if (left_type == T_INT)
    {
        op = "s" + op;
    }

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = " << comp << " " << op << " "
          << typeToString(left_type) << " " << left_reg << ", " << right_reg
          << "\n";

    // Converting bool (i1) to int (i32) like C does!
    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = zext i1 " << m_last_reg << " to i32\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitLessEquals(less_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().back()->getResolvedType();

    binaryTypeTransition(left_type, right_type, left_reg, right_reg);

    std::string comp = compareType(left_type);
    std::string op = comparatorType(left_type) + "le";
    if (left_type == T_INT)
    {
        op = "s" + op;
    }

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = " << comp << " " << op << " "
          << typeToString(left_type) << " " << left_reg << ", " << right_reg
          << "\n";

    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = zext i1 " << m_last_reg << " to i32\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitGreater(greater *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().back()->getResolvedType();

    binaryTypeTransition(left_type, right_type, left_reg, right_reg);

    std::string comp = compareType(left_type);
    std::string op = comparatorType(left_type) + "gt";
    if (left_type == T_INT)
    {
        op = "s" + op;
    }

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = " << comp << " " << op << " "
          << typeToString(left_type) << " " << left_reg << ", " << right_reg
          << "\n";

    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = zext i1 " << m_last_reg << " to i32\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitGreaterEquals(greater_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().back()->getResolvedType();

    binaryTypeTransition(left_type, right_type, left_reg, right_reg);

    std::string comp = compareType(left_type);
    std::string op = comparatorType(left_type) + "ge";
    if (left_type == T_INT)
    {
        op = "s" + op;
    }

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = " << comp << " " << op << " "
          << typeToString(left_type) << " " << left_reg << ", " << right_reg
          << "\n";

    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = zext i1 " << m_last_reg << " to i32\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitLogicEquals(logic_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().back()->getResolvedType();

    binaryTypeTransition(left_type, right_type, left_reg, right_reg);

    std::string comp = compareType(left_type);
    std::string op = comparatorType(left_type) + "eq";

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = " << comp << " " << op << " "
          << typeToString(left_type) << " " << left_reg << ", " << right_reg
          << "\n";

    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = zext i1 " << m_last_reg << " to i32\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitLogicNotEquals(logic_not_equals *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().back()->getResolvedType();

    binaryTypeTransition(left_type, right_type, left_reg, right_reg);

    std::string comp = compareType(left_type);
    std::string op = comparatorType(left_type) + "ne";

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = " << comp << " " << op << " "
          << typeToString(left_type) << " " << left_reg << ", " << right_reg
          << "\n";

    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = zext i1 " << m_last_reg << " to i32\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitLogicAnd(logic_and *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().front()->getResolvedType();

    left_reg = boolConvertor(left_type, left_reg);
    right_reg = boolConvertor(right_type, right_reg);

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = and i1 " << left_reg << ", " << right_reg
          << "\n";

    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = zext i1 " << m_last_reg << " to i32\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitLogicOr(logic_or *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().front()->getResolvedType();

    left_reg = boolConvertor(left_type, left_reg);
    right_reg = boolConvertor(right_type, right_reg);

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = or i1 " << left_reg << ", " << right_reg
          << "\n";

    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = zext i1 " << m_last_reg << " to i32\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitLogicNot(logic_not *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().front()->getResolvedType();

    left_reg = boolConvertor(left_type, left_reg);
    right_reg = boolConvertor(right_type, right_reg);

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = xor i1 " << left_reg << ", " << right_reg
          << "\n";

    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = zext i1 " << m_last_reg << " to i32\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitBitWiseAnd(bit_wise_and *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().front()->getResolvedType();

    toInteger(left_type, left_reg);
    toInteger(right_type, right_reg);

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = and i32" << left_reg << ", " << right_reg
          << "\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitBitWiseOr(bit_wise_or *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().front()->getResolvedType();

    toInteger(left_type, left_reg);
    toInteger(right_type, right_reg);

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = or i32" << left_reg << ", " << right_reg
          << "\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitBitWiseXor(bit_wise_xor *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().front()->getResolvedType();

    toInteger(left_type, left_reg);
    toInteger(right_type, right_reg);

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = xor i32" << left_reg << ", " << right_reg
          << "\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitBitWiseNot(bit_wise_not *node)
{
    node->getChildrenList().front()->accept(*this);
    dataType type = node->getChildrenList().front()->getResolvedType();
    toInteger(type, m_last_reg);
    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = xor i32 " << m_last_reg << ", -1\n";
}

void IREmitterVisitor::visitShiftLeft(shift_left *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().front()->getResolvedType();

    toInteger(left_type, left_reg);
    toInteger(right_type, right_reg);

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = shl i32 " << left_reg << ", " << right_reg
          << "\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitShiftRight(shift_right *node)
{
    node->getChildrenList().front()->accept(*this);
    std::string left_reg = m_last_reg;
    dataType left_type = node->getChildrenList().front()->getResolvedType();

    node->getChildrenList().back()->accept(*this);
    std::string right_reg = m_last_reg;
    dataType right_type = node->getChildrenList().front()->getResolvedType();

    toInteger(left_type, left_reg);
    toInteger(right_type, right_reg);

    std::string cur_reg = getNextReg();
    *m_ll << "\t" << cur_reg << " = arhl i32 " << left_reg << ", " << right_reg
          << "\n";
    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitPostfixIncrement(postfix_increment *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = static_cast<IDENTIFIER *>(*it);
    std::string name = id->getLabel();
    VarSymbol *sym =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    std::string cur_reg = getNextReg();
    std::string type_str = typeToString(sym->getValueType());
    std::string mem_loc = sym->getAddress();

    *m_ll << "\t" << cur_reg << " = load " << type_str << ", " << type_str
          << "* " << mem_loc << "\n";

    std::string old_reg = cur_reg;
    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    std::string op = operandType(sym->getValueType()) + "add";
    std::string num_one = getOne(sym->getValueType());

    *m_ll << "\t" << cur_reg << " = " << op << " " << type_str << " "
          << m_last_reg << ", " << num_one << "\n";

    *m_ll << "\t" << "store " << type_str << " " << cur_reg << ", " << type_str
          << "* " << mem_loc << "\n";

    m_last_reg = old_reg;
}

void IREmitterVisitor::visitPostfixDecrement(postfix_decrement *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = static_cast<IDENTIFIER *>(*it);
    std::string name = id->getLabel();
    VarSymbol *sym =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    std::string cur_reg = getNextReg();
    std::string type_str = typeToString(sym->getValueType());
    std::string mem_loc = sym->getAddress();

    *m_ll << "\t" << cur_reg << " = load " << type_str << ", " << type_str
          << "* " << mem_loc << "\n";

    std::string old_reg = cur_reg;
    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    std::string op = operandType(sym->getValueType()) + "sub";
    std::string num_one = getOne(sym->getValueType());

    *m_ll << "\t" << cur_reg << " = " << op << " " << type_str << " "
          << m_last_reg << ", " << num_one << "\n";

    *m_ll << "\t" << "store " << type_str << " " << cur_reg << ", " << type_str
          << "* " << mem_loc << "\n";

    m_last_reg = old_reg;
}

void IREmitterVisitor::visitPrefixIncrement(prefix_increment *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = static_cast<IDENTIFIER *>(*it);
    std::string name = id->getLabel();
    VarSymbol *sym =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    std::string cur_reg = getNextReg();
    std::string type_str = typeToString(sym->getValueType());
    std::string num_one = getOne(sym->getValueType());
    std::string mem_loc = sym->getAddress();

    *m_ll << "\t" << cur_reg << " = load " << type_str << ", " << type_str
          << "* " << mem_loc << "\n";

    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    std::string op = operandType(sym->getValueType()) + "add";

    *m_ll << "\t" << cur_reg << " = " << op << " " << type_str << " "
          << m_last_reg << ", " << num_one << "\n";

    *m_ll << "\t" << "store " << type_str << " " << cur_reg << ", " << type_str
          << "* " << mem_loc << "\n";

    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitPrefixDecrement(prefix_decrement *node)
{
    auto it = node->getChildrenList().begin();

    IDENTIFIER *id = static_cast<IDENTIFIER *>(*it);
    std::string name = id->getLabel();
    VarSymbol *sym =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));

    std::string cur_reg = getNextReg();
    std::string type_str = typeToString(sym->getValueType());
    std::string mem_loc = sym->getAddress();

    *m_ll << "\t" << cur_reg << " = load " << type_str << ", " << type_str
          << "* " << mem_loc << "\n";

    m_last_reg = cur_reg;
    cur_reg = getNextReg();
    std::string op = operandType(sym->getValueType()) + "sub";
    std::string num_one = getOne(sym->getValueType());

    *m_ll << "\t" << cur_reg << " = " << op << " " << type_str << " "
          << m_last_reg << ", " << num_one << "\n";

    *m_ll << "\t" << "store " << type_str << " " << cur_reg << ", " << type_str
          << "* " << mem_loc << "\n";

    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitAssignment(assignment *node)
{
    auto it = node->getChildrenList().begin();
    std::string name = static_cast<IDENTIFIER *>((*it))->getLabel();
    VarSymbol *var =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));
    std::string cur_reg = var->getAddress();

    it++;
    (*it)->accept(*this);

    dataType lhs_type = node->getResolvedType();
    dataType rhs_type = (*it)->getResolvedType();
    assignmentTypeTransition(lhs_type, rhs_type);

    *m_ll << "\tstore " << typeToString(lhs_type) << " " << m_last_reg << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    m_last_reg = cur_reg;
}

void IREmitterVisitor::visitPlusAssignment(plus_assignment *node)
{
    auto it = node->getChildrenList().begin();
    std::string name = static_cast<IDENTIFIER *>((*it))->getLabel();
    VarSymbol *var =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));
    std::string cur_reg = var->getAddress();
    dataType lhs_type = var->getValueType();

    it++;
    (*it)->accept(*this);
    dataType rhs_type = (*it)->getResolvedType();

    assignmentTypeTransition(lhs_type, rhs_type);
    std::string val_to_add = m_last_reg;

    std::string loaded_reg = getNextReg();
    *m_ll << "\t" << loaded_reg << " = load " << typeToString(lhs_type) << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    std::string temp_reg = getNextReg();
    std::string op = operandType(lhs_type) + "add";

    *m_ll << "\t" << temp_reg << " = " << op << " " << typeToString(lhs_type)
          << " " << loaded_reg << ", " << val_to_add << "\n";

    *m_ll << "\tstore " << typeToString(lhs_type) << " " << temp_reg << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    m_last_reg = temp_reg;
}

void IREmitterVisitor::visitMulAssignment(mul_assignment *node)
{
    auto it = node->getChildrenList().begin();
    std::string name = static_cast<IDENTIFIER *>((*it))->getLabel();
    VarSymbol *var =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));
    std::string cur_reg = var->getAddress();
    dataType lhs_type = var->getValueType();

    it++;
    (*it)->accept(*this);
    dataType rhs_type = (*it)->getResolvedType();

    assignmentTypeTransition(lhs_type, rhs_type);
    std::string val_to_add = m_last_reg;

    std::string loaded_reg = getNextReg();
    *m_ll << "\t" << loaded_reg << " = load " << typeToString(lhs_type) << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    std::string temp_reg = getNextReg();
    std::string op = operandType(lhs_type) + "mul";

    *m_ll << "\t" << temp_reg << " = " << op << " " << typeToString(lhs_type)
          << " " << loaded_reg << ", " << val_to_add << "\n";

    *m_ll << "\tstore " << typeToString(lhs_type) << " " << temp_reg << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    m_last_reg = temp_reg;
}

void IREmitterVisitor::visitMinusAssignment(minus_assignment *node)
{
    auto it = node->getChildrenList().begin();
    std::string name = static_cast<IDENTIFIER *>((*it))->getLabel();
    VarSymbol *var =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));
    std::string cur_reg = var->getAddress();
    dataType lhs_type = var->getValueType();

    it++;
    (*it)->accept(*this);
    dataType rhs_type = (*it)->getResolvedType();

    assignmentTypeTransition(lhs_type, rhs_type);
    std::string val_to_add = m_last_reg;

    std::string loaded_reg = getNextReg();
    *m_ll << "\t" << loaded_reg << " = load " << typeToString(lhs_type) << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    std::string temp_reg = getNextReg();
    std::string op = operandType(lhs_type) + "sub";

    *m_ll << "\t" << temp_reg << " = " << op << " " << typeToString(lhs_type)
          << " " << loaded_reg << ", " << val_to_add << "\n";

    *m_ll << "\tstore " << typeToString(lhs_type) << " " << temp_reg << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    m_last_reg = temp_reg;
}

void IREmitterVisitor::visitDivAssignment(div_assignment *node)
{
    auto it = node->getChildrenList().begin();
    std::string name = static_cast<IDENTIFIER *>((*it))->getLabel();
    VarSymbol *var =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));
    std::string cur_reg = var->getAddress();
    dataType lhs_type = var->getValueType();

    it++;
    (*it)->accept(*this);
    dataType rhs_type = (*it)->getResolvedType();

    assignmentTypeTransition(lhs_type, rhs_type);
    std::string val_to_add = m_last_reg;

    std::string loaded_reg = getNextReg();
    *m_ll << "\t" << loaded_reg << " = load " << typeToString(lhs_type) << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    std::string temp_reg = getNextReg();
    std::string op = operandType(lhs_type) + "div";

    if (lhs_type == T_INT)
    {
        op = "s" + op;
    }

    *m_ll << "\t" << temp_reg << " = " << op << " " << typeToString(lhs_type)
          << " " << loaded_reg << ", " << val_to_add << "\n";

    *m_ll << "\tstore " << typeToString(lhs_type) << " " << temp_reg << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    m_last_reg = temp_reg;
}

void IREmitterVisitor::visitModAssignment(mod_assignment *node)
{
    auto it = node->getChildrenList().begin();
    std::string name = static_cast<IDENTIFIER *>((*it))->getLabel();
    VarSymbol *var =
        static_cast<VarSymbol *>(SymbolTable::getInstance()->lookup(name));
    std::string cur_reg = var->getAddress();
    dataType lhs_type = var->getValueType();

    it++;
    (*it)->accept(*this);
    dataType rhs_type = (*it)->getResolvedType();

    assignmentTypeTransition(lhs_type, rhs_type);
    std::string val_to_add = m_last_reg;

    std::string loaded_reg = getNextReg();
    *m_ll << "\t" << loaded_reg << " = load " << typeToString(lhs_type) << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    std::string temp_reg = getNextReg();
    std::string op = "srem";

    *m_ll << "\t" << temp_reg << " = " << op << " " << typeToString(lhs_type)
          << " " << loaded_reg << ", " << val_to_add << "\n";

    *m_ll << "\tstore " << typeToString(lhs_type) << " " << temp_reg << ", "
          << typeToString(lhs_type) << "* " << cur_reg << "\n";

    m_last_reg = temp_reg;
}

void IREmitterVisitor::visitVariableDeclaration(variable_declaration *node)
{
    auto &temp = node->getChildrenList();
    auto it = temp.begin();

    it++;

    if (temp.size() > 1) // var decl with expression
    {
        (*it)->accept(*this);
    }
    else // var decl with no expression
    {
        m_last_reg.clear();
    }
}

void IREmitterVisitor::visitVariableDeclarationList(
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

void IREmitterVisitor::visitVariableDeclarationStatement(
    variable_declaration_statement *node)
{
    auto it = node->getChildrenList().begin();

    dataType current_type = static_cast<type_specifier *>(*it)->getType();

    it++;
    (*it)->accept(*this);

    if (node->getParent()->getNodeType() == EXTERNAL_DECLARATION_NODE)
    {
        // Global Variables

        for (auto &var : m_vars)
        {
            std::string name =
                static_cast<IDENTIFIER *>(var->getChildrenList().front())
                    ->getLabel();

            std::string mem_loc = "@" + name;
            std::string zero = (current_type == T_INT) ? "0" : "0.0e+00";

            *m_ll << mem_loc << " = global " << typeToString(current_type)
                  << " " << zero << "\n";

            VarSymbol *sym = new VarSymbol(0, name, current_type);
            sym->setAddress(mem_loc);
            SymbolTable::getInstance()->insert(sym);

            m_ll = &m_global_init_buff;
            // Number will not initiate
            var->accept(*this);

            if (!m_last_reg.empty())
            {
                dataType lhs_type = current_type;
                dataType rhs_type = var->getResolvedType();

                assignmentTypeTransition(lhs_type, rhs_type);

                *m_ll << "\tstore " << typeToString(lhs_type) << " "
                      << m_last_reg << ", " << typeToString(lhs_type) << "* "
                      << mem_loc << "\n";
            }
            m_ll = &m_file_ll;
        }
    }
    else
    {
        // Local Variables

        for (auto &var : m_vars)
        {
            var->accept(*this);
            std::string name =
                static_cast<IDENTIFIER *>(var->getChildrenList().front())
                    ->getLabel();

            std::string mem_loc =
                "%" + name + ".addr." + std::to_string(m_var_count++);

            VarSymbol *sym = new VarSymbol(0, name, current_type);
            sym->setAddress(mem_loc);
            SymbolTable::getInstance()->insert(sym);

            *m_ll << "\t" << mem_loc << " = alloca "
                  << typeToString(current_type) << ", align 4\n";

            if (!m_last_reg.empty())
            {
                dataType lhs_type = current_type;
                dataType rhs_type = var->getResolvedType();

                assignmentTypeTransition(lhs_type, rhs_type);

                *m_ll << "\tstore " << typeToString(lhs_type) << " "
                      << m_last_reg << ", " << typeToString(lhs_type) << "* "
                      << mem_loc << "\n";
            }
        }
    }

    m_vars.clear();
}

void IREmitterVisitor::visitParameterList(parameter_list *node)
{
    auto childs = node->getChildrenList();

    if (childs.size() == 3)
    {
        auto it = childs.begin();
        (*it)->accept(*this);

        // add type specifier and id to a vector.
        it++;
        dataType type = static_cast<type_specifier *>(*it)->getType();
        it++;
        std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();

        parameter param = {type, id};
        m_params.push_back(param);
    }
    else if (childs.size() == 2)
    {
        // add type specifier and id to a vector.
        auto it = childs.begin();
        dataType type = static_cast<type_specifier *>(*it)->getType();
        it++;
        std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();

        parameter param = {type, id};
        m_params.push_back(param);
    }
    else
    {
        // Empty list
    }
}

void IREmitterVisitor::visitArgumentList(argument_list *node)
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

void IREmitterVisitor::visitFunctionDefinition(function_definition *node)
{
    auto it = node->getChildrenList().begin();
    m_reg_count = 0;

    dataType return_type = static_cast<type_specifier *>(*it)->getType();
    it++;
    std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();
    it++;
    (*it)->accept(*this);
    it++;
    compound_statement *body = static_cast<compound_statement *>(*it);

    std::string str_params = "";
    for (auto &param : m_params)
    {
        if (!str_params.empty())
        {
            str_params += ", ";
        }

        str_params += typeToString(param.type) + " %" + param.name;
    }

    *m_ll << "define " << typeToString(return_type) << " @" << id << "("
          << str_params << ") {\n";
    *m_ll << "entry:\n";

    if (id == "main")
    {
        *m_ll << "\tcall void @_init_globals()\n";
    }

    SymbolTable::getInstance()->enterScope(
        SymbolTable::getInstance()->getCurrentId() + 1);

    for (auto &param : m_params)
    {
        std::string raw_arg = "%" + param.name;
        std::string mem_loc = raw_arg + ".addr";

        *m_ll << "\t" << mem_loc << " = alloca " << typeToString(param.type)
              << ", align 4" << "\n";
        *m_ll << "\t" << "store " << typeToString(param.type) << " " << raw_arg
              << ", " << typeToString(param.type) << "* " << mem_loc << "\n";

        VarSymbol *sym = new VarSymbol(0, param.name, param.type);
        sym->setAddress(mem_loc);
        SymbolTable::getInstance()->insert(sym);
    }

    m_params.clear();
    
    body->accept(*this);

    if (return_type == T_VOID)
    {
        *m_ll << "\tret void\n";
    }

    SymbolTable::getInstance()->exitScope();

    *m_ll << "}\n\n";

    // I make reg count 0 because i need to handle global variable init
    m_reg_count = 0;
}

void IREmitterVisitor::visitReturn(return_node *node)
{
    auto childs = node->getChildrenList();

    if (!childs.empty())
    {
        childs.front()->accept(*this);

        dataType expect_ret = node->getResolvedType();
        dataType expr_type = childs.front()->getResolvedType();

        // Using assignmentTypeTransition because return expression must match
        // function return type
        assignmentTypeTransition(expect_ret, expr_type);

        *m_ll << "\tret " << typeToString(expect_ret) << " " << m_last_reg
              << "\n";
    }
    // I prefer that void functions should eb handled on function definition,
    // because its acceptable to not have a return statement.
}

void IREmitterVisitor::visitIfStatement(if_statement *node)
{
    unsigned int id = m_label_count++;
    std::string label_true = "if_then_" + std::to_string(id);
    std::string label_false = "if_else_" + std::to_string(id);
    std::string label_end = "if_end_" + std::to_string(id);

    auto it = node->getChildrenList().begin();
    (*it)->accept(*this);

    std::string cond_reg = m_last_reg;
    dataType cond_type = (*it)->getResolvedType();
    it++;

    boolConvertor(cond_type, cond_reg);

    bool has_else = node->getChildrenList().size() == 3;
    std::string real_end = has_else ? label_false : label_end;

    // Initial check
    *m_ll << "\tbr i1 " << cond_reg << ", label %" << label_true << ", label %"
          << real_end << "\n";

    *m_ll << label_true << ":\n";
    (*it)->accept(*this);
    *m_ll << "\tbr label %" << label_end << "\n";

    if (has_else)
    {
        *m_ll << label_false << ":\n";
        it++;
        (*it)->accept(*this);
        *m_ll << "\tbr label %" << label_end << "\n";
    }

    *m_ll << label_end << ":\n";
}

void IREmitterVisitor::visitWhileStatement(while_statement *node)
{
    unsigned int id = m_label_count++;
    std::string label_cond = "while_cond_" + std::to_string(id);
    std::string label_body = "while_body_" + std::to_string(id);
    std::string label_exit = "while_end_" + std::to_string(id);

    auto it = node->getChildrenList().begin();
    STNode *cond_node = *it;
    it++;
    STNode *body_node = *it;

    *m_ll << "\tbr label %" << label_cond << "\n";

    *m_ll << "\n" << label_cond << ":\n";
    cond_node->accept(*this);

    std::string cond_reg = m_last_reg;
    dataType cond_type = cond_node->getResolvedType();
    cond_reg = boolConvertor(cond_type, cond_reg);

    *m_ll << "\tbr i1 " << cond_reg << ", label %" << label_body << ", label %"
          << label_exit << "\n";

    *m_ll << "\n" << label_body << ":\n";

    m_continue_stack.push(label_cond);
    m_break_stack.push(label_exit);

    body_node->accept(*this);

    m_continue_stack.pop();
    m_break_stack.pop();

    *m_ll << "\tbr label %" << label_cond << "\n";

    *m_ll << "\n" << label_exit << ":\n";
}

void IREmitterVisitor::visitDoWhileStatement(do_while_statement *node)
{
    unsigned int id = m_label_count++;
    std::string label_cond = "do_while_cond_" + std::to_string(id);
    std::string label_true = "do_while_true_" + std::to_string(id);
    std::string label_exit = "do_while_end_" + std::to_string(id);

    auto it = node->getChildrenList().begin();
    compound_statement *comp_state = static_cast<compound_statement *>(*it);
    it++;
    condition *cond = static_cast<condition *>(*it);

    *m_ll << "\tbr label %" << label_true << "\n";
    *m_ll << label_true << ":\n";

    m_continue_stack.push(label_cond);
    m_break_stack.push(label_exit);

    comp_state->accept(*this);

    m_continue_stack.pop();
    m_break_stack.pop();

    *m_ll << "\tbr label %" << label_cond << "\n";
    *m_ll << label_cond << ":\n";
    cond->accept(*this);

    std::string cond_reg = m_last_reg;
    dataType cond_type = cond->getResolvedType();
    cond_reg = boolConvertor(cond_type, cond_reg);

    *m_ll << "\tbr i1 " << cond_reg << ", label %" << label_true << ", label %"
          << label_exit << "\n";

    *m_ll << label_exit << ":\n";
}

void IREmitterVisitor::visitForStatement(for_statement *node)
{
    unsigned int id = m_label_count++;
    std::string label_cond = "for_cond_" + std::to_string(id);
    std::string label_true = "for_true_" + std::to_string(id);
    std::string label_exit = "for_end_" + std::to_string(id);
    std::string label_inc = "for_inc_" + std::to_string(id);

    auto &children = node->getChildrenList();
    auto it = children.begin();

    // Init and Cond are always the first two.
    STNode *init_node = (*it++);
    STNode *cond_node = (*it++);

    STNode *step_node = nullptr;
    STNode *body_node = nullptr;

    if (children.size() == 4)
    {
        // Full loop: for(init; cond; step) body
        step_node = (*it++);
        body_node = (*it);
        ;
    }
    else
    {
        // Missing step: for(init; cond; ) body
        body_node = (*it);
    }

    m_continue_stack.push(label_inc);
    m_break_stack.push(label_exit);

    // A. INITIALIZATION
    init_node->accept(*this);

    *m_ll << "\tbr label %" << label_cond << "\n";
    *m_ll << "\n" << label_cond << ":\n";

    if (cond_node->getNodeType() != STATEMENT_NODE)
    {
        cond_node->accept(*this);

        std::string cond_reg = m_last_reg;
        dataType cond_type = cond_node->getResolvedType();
        cond_reg = boolConvertor(cond_type, cond_reg);

        *m_ll << "\tbr i1 " << cond_reg << ", label %" << label_true
              << ", label %" << label_exit << "\n";
    }
    else // If its not statement it will be expression based on the grammar
    {
        // Infinite loop for (;;)
        *m_ll << "\tbr label %" << label_true << "\n";
    }

    *m_ll << "\n" << label_true << ":\n";

    // C. BODY
    body_node->accept(*this);

    *m_ll << "\tbr label %" << label_inc << "\n";

    m_continue_stack.pop();
    m_break_stack.pop();

    *m_ll << "\n" << label_inc << ":\n";

    // D. STEP (Only accept if it exists)
    if (step_node != nullptr)
    {
        step_node->accept(*this);
    }
    *m_ll << "\tbr label %" << label_cond << "\n";

    *m_ll << "\n" << label_exit << ":\n";
}

void IREmitterVisitor::visitContinue(continue_node *node)
{
    if (m_continue_stack.empty())
    {
        exit(1);
    }

    std::string target = m_break_stack.top();
    *m_ll << "\tbr label %" << target << "\n";
}

void IREmitterVisitor::visitBreak(break_node *node)
{
    if (m_break_stack.empty())
    {
        exit(1);
    }

    std::string target = m_continue_stack.top();
    *m_ll << "\tbr label %" << target << "\n";
}

void IREmitterVisitor::visitFunctionCall(function_call *node)
{
    auto childs = node->getChildrenList();

    auto it = childs.begin();
    std::string func_name = (static_cast<IDENTIFIER *>(*it))->getLabel();

    FuncSymbol *def = static_cast<FuncSymbol *>(
        SymbolTable::getInstance()->lookupGlobal(func_name));

    std::string str_args = "";
    if (childs.size() == 1) // No arguments
    {
    }
    else
    {
        it++;
        (*it)->accept(*this);

        for (auto &arg : m_args)
        {
            arg->accept(*this);
            if (!str_args.empty())
            {
                str_args += ", ";
            }
            str_args += typeToString(arg->getResolvedType()) + " " + m_last_reg;
        }
    }

    dataType return_type = def->getReturnType();
    if (return_type == T_VOID)
    {
        *m_ll << "\tcall " << typeToString(return_type) << " @" << func_name
              << "(" << str_args << ")\n";
    }
    else
    {
        std::string cur_reg = getNextReg();
        *m_ll << "\t" << cur_reg << " = " << "call "
              << typeToString(return_type) << " @" << func_name << "("
              << str_args << ")\n";
        m_last_reg = cur_reg;
    }
}

void IREmitterVisitor::visitFunctionDeclaration(function_declaration *node)
{
    auto it = node->getChildrenList().begin();

    dataType return_type = static_cast<type_specifier *>(*it)->getType();
    it++;
    std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();
    it++;
    (*it)->accept(*this);

    FuncSymbol *sym = new FuncSymbol(return_type, nullptr, m_params, id);
    SymbolTable::getInstance()->insert(sym);

    std::string str_params = "";
    for (auto &param : m_params)
    {
        if (!str_params.empty())
        {
            str_params += ", ";
        }

        str_params += typeToString(param.type) + " %" + param.name;
    }

    // For some reason declare for function dose not work like function
    // declaration *m_ll << "declare " << typeToString(return_type) << " @" <<
    // id << "("
    //       << str_params << ")\n";
}

void IREmitterVisitor::visitProgram(program *node)
{
    (*node->getChildrenList().begin())->accept(*this);

    *m_ll << "\ndefine void @_init_globals() {\n";
    *m_ll << "entry:\n";

    *m_ll << m_global_init_buff.str();

    *m_ll << "\tret void\n";
    *m_ll << "}\n";
}
