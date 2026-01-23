#include "composite.hh"
#include "composite_concrete.hh"
#include "symbol_table.hh"
#include "visitor.hh"
#include <vector>

class DeclaratorVisitor : public Visitor
{
  private:
    Value m_result;

    std::vector<parameter> m_params;
    std::vector<STNode *> m_vars;

  public:
    void visitVariableDeclaration(variable_declaration *node) override;
    void visitVariableDeclarationList(variable_declaration_list *node) override;
    void visitVariableDeclarationStatement(
        variable_declaration_statement *node) override;
    void visitFunctionDefinition(function_definition *node) override;
    void visitFunctionDeclaration(function_declaration *node) override;
    void visitParameterList(parameter_list *node) override;
};