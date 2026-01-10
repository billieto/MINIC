#include "../lib/visitor.hh"
#include "../lib/composite.hh"

Visitor::Visitor() {}

void Visitor::visit(STNode *node) { node->accept(*this); }

void Visitor::visitChildren(STNode *node)
{
    for (auto *child : node->getChildrenList())
    {
        visit(child);
    }
}

// Leaf nodes have no children to traverse, so their implementation is empty.
// (visitChildren(node); is not needed but compiler cries)
void Visitor::visitIDENTIFIER(IDENTIFIER *node) { visitChildren(node); }
void Visitor::visitNUMBER(NUMBER *node) { visitChildren(node); }
void Visitor::visitTypeSpecifier(type_specifier *node) { visitChildren(node); }

// Arithmetic Operations
void Visitor::visitMultiplication(multiplication *node) { visitChildren(node); }
void Visitor::visitDivision(division *node) { visitChildren(node); }
void Visitor::visitAddition(addition *node) { visitChildren(node); }
void Visitor::visitSubtraction(subtraction *node) { visitChildren(node); }
void Visitor::visitMod(mod *node) { visitChildren(node); }

// Logical & Relational Operations
void Visitor::visitLess(less *node) { visitChildren(node); }
void Visitor::visitLessEquals(less_equals *node) { visitChildren(node); }
void Visitor::visitGreater(greater *node) { visitChildren(node); }
void Visitor::visitGreaterEquals(greater_equals *node) { visitChildren(node); }
void Visitor::visitLogicEquals(logic_equals *node) { visitChildren(node); }
void Visitor::visitLogicNotEquals(logic_not_equals *node)
{
    visitChildren(node);
}
void Visitor::visitLogicAnd(logic_and *node) { visitChildren(node); }
void Visitor::visitLogicOr(logic_or *node) { visitChildren(node); }
void Visitor::visitLogicNot(logic_not *node) { visitChildren(node); }

// Unary & Assignment
void Visitor::visitIncrement(increment *node) { visitChildren(node); }
void Visitor::visitDecrement(decrement *node) { visitChildren(node); }
void Visitor::visitAssignment(assignment *node) { visitChildren(node); }

// Bitwise Operations
void Visitor::visitBitWiseOr(bit_wise_or *node) { visitChildren(node); }
void Visitor::visitBitWiseAnd(bit_wise_and *node) { visitChildren(node); }
void Visitor::visitBitWiseXor(bit_wise_xor *node) { visitChildren(node); }
void Visitor::visitBitWiseNot(bit_wise_not *node) { visitChildren(node); }
void Visitor::visitShiftLeft(shift_left *node) { visitChildren(node); }
void Visitor::visitShiftRight(shift_right *node) { visitChildren(node); }

// Compound Assignments
void Visitor::visitPlusAssignment(plus_assignment *node)
{
    visitChildren(node);
}
void Visitor::visitMinusAssignment(minus_assignment *node)
{
    visitChildren(node);
}
void Visitor::visitMulAssignment(mul_assignment *node) { visitChildren(node); }
void Visitor::visitDivAssignment(div_assignment *node) { visitChildren(node); }
void Visitor::visitModAssignment(mod_assignment *node) { visitChildren(node); }

// Declarations
void Visitor::visitVariableDeclaration(variable_declaration *node)
{
    visitChildren(node);
}
void Visitor::visitVariableDeclarationList(variable_declaration_list *node)
{
    visitChildren(node);
}
void Visitor::visitVariableDeclarationStatement(
    variable_declaration_statement *node)
{
    visitChildren(node);
}
void Visitor::visitExternalDeclaration(external_declaration *node)
{
    visitChildren(node);
}
// Statements & Control Flow
void Visitor::visitCompoundStatement(compound_statement *node)
{
    visitChildren(node);
}
void Visitor::visitStatementList(statement_list *node) { visitChildren(node); }
void Visitor::visitStatement(statement *node) { visitChildren(node); }
void Visitor::visitIfStatement(if_statement *node) { visitChildren(node); }
void Visitor::visitWhileStatement(while_statement *node)
{
    visitChildren(node);
}
void Visitor::visitDoWhileStatement(do_while_statement *node)
{
    visitChildren(node);
}
void Visitor::visitForStatement(for_statement *node) { visitChildren(node); }

// Control flow leaves
void Visitor::visitContinue(continue_node *node) { visitChildren(node); }
void Visitor::visitBreak(break_node *node) { visitChildren(node); }
void Visitor::visitReturn(return_node *node) { visitChildren(node); }

// Functions
void Visitor::visitFunctionCall(function_call *node) { visitChildren(node); }
void Visitor::visitFunctionDefinition(function_definition *node)
{
    visitChildren(node);
}
void Visitor::visitFunctionDeclaration(function_declaration *node)
{
    visitChildren(node);
}
void Visitor::visitArgumentList(argument_list *node) { visitChildren(node); }
void Visitor::visitParameterList(parameter_list *node) { visitChildren(node); }

// High Level
void Visitor::visitProgram(program *node) { visitChildren(node); }
void Visitor::visitTranslationUnit(translation_unit *node)
{
    visitChildren(node);
}
void Visitor::visitExpression(expression *node) { visitChildren(node); }
void Visitor::visitCondition(condition *node) { visitChildren(node); }
