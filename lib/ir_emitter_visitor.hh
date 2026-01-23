#pragma once
#ifndef EMITTER_
#define EMITTER_

#include "composite.hh"
#include "composite_concrete.hh"
#include "symbol_table.hh"
#include "types.hh"
#include "visitor.hh"
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>

class IREmitterVisitor : public Visitor
{
  private:
    unsigned int m_reg_count;
    unsigned int m_label_count;
    unsigned int m_var_count;
    std::ofstream m_file_ll;
    std::stringstream m_global_init_buff;
    std::ostream *m_ll;
    std::string m_last_reg;

    std::vector<parameter> m_params;
    std::vector<STNode *> m_args;
    std::vector<STNode *> m_vars;

    std::stack<std::string> m_break_stack;
    std::stack<std::string> m_continue_stack;

    std::string typeToString(dataType type);
    std::string getNextReg();
    std::string operandType(dataType type);
    std::string compareType(dataType type);
    std::string comparatorType(dataType type);
    std::string boolConvertor(dataType type, std::string reg);
    std::string getOne(dataType type);
    void toInteger(dataType &type, std::string &reg);
    void assignmentTypeTransition(dataType type1, dataType &type2);
    void binaryTypeTransition(dataType &type1, dataType &type2,
                              std::string &reg1, std::string &reg2);

  public:
    IREmitterVisitor();
    ~IREmitterVisitor();

    void visitIDENTIFIER(IDENTIFIER *node) override;
    void visitNUMBER(NUMBER *node) override;
    void visitAddition(addition *node) override;
    void visitSubtraction(subtraction *node) override;
    void visitMultiplication(multiplication *node) override;
    void visitDivision(division *node) override;
    void visitMod(mod *node) override;
    void visitLess(less *node) override;
    void visitLessEquals(less_equals *node) override;
    void visitGreater(greater *node) override;
    void visitGreaterEquals(greater_equals *node) override;
    void visitLogicEquals(logic_equals *node) override;
    void visitLogicNotEquals(logic_not_equals *node) override;
    void visitLogicAnd(logic_and *node) override;
    void visitLogicOr(logic_or *node) override;
    void visitLogicNot(logic_not *node) override;
    void visitBitWiseAnd(bit_wise_and *node) override;
    void visitBitWiseOr(bit_wise_or *node) override;
    void visitBitWiseXor(bit_wise_xor *node) override;
    void visitBitWiseNot(bit_wise_not *node) override;
    void visitShiftLeft(shift_left *node) override;
    void visitShiftRight(shift_right *node) override;
    void visitPostfixIncrement(postfix_increment *node) override;
    void visitPostfixDecrement(postfix_decrement *node) override;
    void visitPrefixIncrement(prefix_increment *node) override;
    void visitPrefixDecrement(prefix_decrement *node) override;
    void visitVariableDeclarationStatement(
        variable_declaration_statement *node) override;
    void visitVariableDeclaration(variable_declaration *node) override;
    void visitAssignment(assignment *node) override;
    void visitPlusAssignment(plus_assignment *node) override;
    void visitMulAssignment(mul_assignment *node) override;
    void visitMinusAssignment(minus_assignment *node) override;
    void visitDivAssignment(div_assignment *node) override;
    void visitModAssignment(mod_assignment *node) override;
    void visitFunctionDefinition(function_definition *node) override;
    void visitReturn(return_node *node) override;
    void visitIfStatement(if_statement *node) override;
    void visitWhileStatement(while_statement *node) override;
    void visitDoWhileStatement(do_while_statement *node) override;
    void visitForStatement(for_statement *node) override;
    void visitContinue(continue_node *node) override;
    void visitBreak(break_node *node) override;
    void visitFunctionCall(function_call *node) override;
    void visitFunctionDeclaration(function_declaration *node) override;
    void visitProgram(program *node) override;
    void visitParameterList(parameter_list *node) override;
    void visitArgumentList(argument_list *node) override;
    void visitVariableDeclarationList(variable_declaration_list *node) override;
};

#endif