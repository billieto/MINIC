#pragma once
#include "composite.hh"
#include "composite_concrete.hh"
#include "symbol_table.hh"
#include <vector>
#ifndef _EVALUATOR
#define _EVALUATOR

#include "visitor.hh"

typedef int Value;

class EvaluatorVisitor : public Visitor
{
  private:
    Value m_result = 0;

    std::vector<STNode *> m_args;
    std::vector<STNode *> m_vars;

    struct continue_signal
    {
    };

    struct break_signal
    {
    };

    struct void_return_signal
    {
    };

  public:
    EvaluatorVisitor();
    ~EvaluatorVisitor() = default;

    // Helper to get the calculation result
    Value getResult();

    // Leaf Nodes
    void visitIDENTIFIER(IDENTIFIER *node) override;
    void visitNUMBER(NUMBER *node) override;

    // Arithmetic
    void visitMultiplication(multiplication *node) override;
    void visitDivision(division *node) override;
    void visitAddition(addition *node) override;
    void visitSubtraction(subtraction *node) override;
    void visitMod(mod *node) override;

    // Logic/Relational
    void visitLess(less *node) override;
    void visitLessEquals(less_equals *node) override;
    void visitGreater(greater *node) override;
    void visitGreaterEquals(greater_equals *node) override;
    void visitLogicEquals(logic_equals *node) override;
    void visitLogicNotEquals(logic_not_equals *node) override;
    void visitLogicAnd(logic_and *node) override;
    void visitLogicOr(logic_or *node) override;
    void visitLogicNot(logic_not *node) override;

    // Unary/Bitwise
    void visitUnaryPlus(unary_plus *node) override;
    void visitUnaryMinus(unary_minus *node) override;
    void visitPostfixIncrement(postfix_increment *node) override;
    void visitPrefixIncrement(prefix_increment *node) override;
    void visitPostfixDecrement(postfix_decrement *node) override;
    void visitPrefixDecrement(prefix_decrement *node) override;
    void visitBitWiseOr(bit_wise_or *node) override;
    void visitBitWiseAnd(bit_wise_and *node) override;
    void visitBitWiseXor(bit_wise_xor *node) override;
    void visitBitWiseNot(bit_wise_not *node) override;
    void visitShiftLeft(shift_left *node) override;
    void visitShiftRight(shift_right *node) override;

    // Assignment
    void visitAssignment(assignment *node) override;
    void visitPlusAssignment(plus_assignment *node) override;
    void visitMinusAssignment(minus_assignment *node) override;
    void visitMulAssignment(mul_assignment *node) override;
    void visitDivAssignment(div_assignment *node) override;
    void visitModAssignment(mod_assignment *node) override;

    // Statements & Control Flow
    void visitStatement(statement *node) override;
    void visitIfStatement(if_statement *node) override;
    void visitWhileStatement(while_statement *node) override;
    void visitDoWhileStatement(do_while_statement *node) override;
    void visitForStatement(for_statement *node) override;
    void visitCondition(condition *node) override;
    void visitArgumentList(argument_list *node) override;

    // Declarations & Functions
    void visitVariableDeclaration(variable_declaration *node) override;
    void visitVariableDeclarationList(variable_declaration_list *node) override;
    void visitVariableDeclarationStatement(
        variable_declaration_statement *node) override;
    void visitFunctionCall(function_call *node) override;
    void visitReturn(return_node *node) override;
    void visitProgram(program *node) override;

    // Ignored or handled internally
    void visitContinue(continue_node *node) override;
    void visitBreak(break_node *node) override;
};

#endif
