#pragma once
#ifndef TYPECHECKER_
#define TYPECHECKER_

#include "composite.hh"
#include "composite_concrete.hh"
#include "symbol_table.hh"
#include "visitor.hh"
#include <vector>

class TypeCheckerVisitor : public Visitor
{
  private:
    unsigned int m_loop_depth;

    // The basic field for visitor pattern to work
    dataType m_last_type;

    // Helper fields to identify function correctness
    dataType m_expected_return_type;
    bool m_found_return;

    // Helper lists/vectors for parameters, arguments and variables
    std::vector<parameter> m_params;
    std::vector<STNode *> m_args;
    std::vector<STNode *> m_vars;

    // Helper methods
    void semanticError(std::string s);
    std::string typeToString(dataType type);
    dataType checkMathTypes(dataType left, dataType right, std::string op);
    bool isCompatible(dataType target, dataType source);

  public:
    TypeCheckerVisitor();
    virtual ~TypeCheckerVisitor() = default;

    // 1. Terminals & Identifiers
    void visitIDENTIFIER(IDENTIFIER *node) override;
    void visitNUMBER(NUMBER *node) override;

    // 2. Variables & Functions
    void visitVariableDeclaration(variable_declaration *node) override;
    void visitVariableDeclarationStatement(
        variable_declaration_statement *node) override;
    void visitFunctionDeclaration(function_declaration *node) override;
    void visitFunctionDefinition(function_definition *node) override;
    void visitFunctionCall(function_call *node) override;
    void visitParameterList(parameter_list *node) override;
    void visitArgumentList(argument_list *node) override;
    void visitVariableDeclarationList(variable_declaration_list *node) override;

    // 3. Statements & Control Flow
    void visitReturn(return_node *node) override;
    void visitIfStatement(if_statement *node) override;
    void visitWhileStatement(while_statement *node) override;
    void visitDoWhileStatement(do_while_statement *node) override;
    void visitForStatement(for_statement *node) override;
    void visitCondition(condition *node) override;
    void visitContinue(continue_node *node) override;
    void visitBreak(break_node *node) override;

    // 4. Assignments (LHS = RHS)
    void visitAssignment(assignment *node) override;
    void visitPlusAssignment(plus_assignment *node) override;
    void visitMinusAssignment(minus_assignment *node) override;
    void visitMulAssignment(mul_assignment *node) override;
    void visitDivAssignment(div_assignment *node) override;
    void visitModAssignment(mod_assignment *node) override;

    // 5. Increment / Decrement
    void visitPrefixIncrement(prefix_increment *node) override;
    void visitPrefixDecrement(prefix_decrement *node) override;
    void visitPostfixIncrement(postfix_increment *node) override;
    void visitPostfixDecrement(postfix_decrement *node) override;

    // 6. Binary Operations (Math)
    void visitAddition(addition *node) override;
    void visitSubtraction(subtraction *node) override;
    void visitMultiplication(multiplication *node) override;
    void visitDivision(division *node) override;
    void visitMod(mod *node) override;

    // 7. Relational Operations (Result is T_INT/Bool)
    void visitLess(less *node) override;
    void visitLessEquals(less_equals *node) override;
    void visitGreater(greater *node) override;
    void visitGreaterEquals(greater_equals *node) override;
    void visitLogicEquals(logic_equals *node) override;
    void visitLogicNotEquals(logic_not_equals *node) override;

    // 8. Logical Operations (&&, ||, !)
    void visitLogicAnd(logic_and *node) override;
    void visitLogicOr(logic_or *node) override;
    void visitLogicNot(logic_not *node) override;

    // 9. Bitwise Operations
    void visitBitWiseAnd(bit_wise_and *node) override;
    void visitBitWiseOr(bit_wise_or *node) override;
    void visitBitWiseXor(bit_wise_xor *node) override;
    void visitBitWiseNot(bit_wise_not *node) override;
    void visitShiftLeft(shift_left *node) override;
    void visitShiftRight(shift_right *node) override;
};

#endif
