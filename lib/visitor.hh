#pragma once
#ifndef _VISITOR
#define _VISITOR

#include "composite.hh"
#include "composite_concrete.hh"

class Visitor
{
  public:
    Visitor();
    virtual ~Visitor() = default;

    virtual void visit(STNode *node);
    virtual void visitChildren(STNode *node);

    // Leaf Nodes
    virtual void visitIDENTIFIER(IDENTIFIER *node);
    virtual void visitNUMBER(NUMBER *node);
    virtual void visitTypeSpecifier(type_specifier *node);

    // Arithmetic Operations
    virtual void visitMultiplication(multiplication *node);
    virtual void visitDivision(division *node);
    virtual void visitAddition(addition *node);
    virtual void visitSubtraction(subtraction *node);
    virtual void visitMod(mod *node);

    // Logical & Relational Operations
    virtual void visitLess(less *node);
    virtual void visitLessEquals(less_equals *node);
    virtual void visitGreater(greater *node);
    virtual void visitGreaterEquals(greater_equals *node);
    virtual void visitLogicEquals(logic_equals *node);
    virtual void visitLogicNotEquals(logic_not_equals *node);
    virtual void visitLogicAnd(logic_and *node);
    virtual void visitLogicOr(logic_or *node);
    virtual void visitLogicNot(logic_not *node);

    // Unary & Assignment
    virtual void visitIncrement(increment *node);
    virtual void visitDecrement(decrement *node);
    virtual void visitAssignment(assignment *node);

    // Bitwise
    virtual void visitBitWiseOr(bit_wise_or *node);
    virtual void visitBitWiseAnd(bit_wise_and *node);
    virtual void visitBitWiseXor(bit_wise_xor *node);
    virtual void visitBitWiseNot(bit_wise_not *node);
    virtual void visitShiftLeft(shift_left *node);
    virtual void visitShiftRight(shift_right *node);

    // Compound Assignments
    virtual void visitPlusAssignment(plus_assignment *node);
    virtual void visitMinusAssignment(minus_assignment *node);
    virtual void visitMulAssignment(mul_assignment *node);
    virtual void visitDivAssignment(div_assignment *node);
    virtual void visitModAssignment(mod_assignment *node);

    // Declarations
    virtual void visitVariableDeclaration(variable_declaration *node);
    virtual void visitVariableDeclarationList(variable_declaration_list *node);
    virtual void
    visitVariableDeclarationStatement(variable_declaration_statement *node);
    virtual void visitExternalDeclaration(external_declaration *node);

    // Statements & Control Flow
    virtual void visitCompoundStatement(compound_statement *node);
    virtual void visitStatementList(statement_list *node);
    virtual void visitStatement(statement *node);
    virtual void visitIfStatement(if_statement *node);
    virtual void visitWhileStatement(while_statement *node);
    virtual void visitDoWhileStatement(do_while_statement *node);
    virtual void visitForStatement(for_statement *node);
    virtual void visitContinue(continue_node *node);
    virtual void visitBreak(break_node *node);
    virtual void visitReturn(return_node *node);

    // Functions
    virtual void visitFunctionCall(function_call *node);
    virtual void visitFunctionDefinition(function_definition *node);
    virtual void visitFunctionDeclaration(function_declaration *node);
    virtual void visitArgumentList(argument_list *node);
    virtual void visitParameterList(parameter_list *node);

    // High Level
    virtual void visitProgram(program *node);
    virtual void visitTranslationUnit(translation_unit *node);
    virtual void visitExpression(expression *node);
    virtual void visitCondition(condition *node);
};

#endif
