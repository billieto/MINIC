#include "composite_concrete.hh"
#include "visitor.hh"

class DeclaratorVisitor : public Visitor
{
  private:
    Value m_result;

  public:
    void visitVariableDeclaration(variable_declaration *node) override;
    void visitVariableDeclarationStatement(
        variable_declaration_statement *node) override;
    void visitFunctionDefinition(function_definition *node) override;
    void visitFunctionDeclaration(function_declaration *node) override;
};