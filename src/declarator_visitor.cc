#include "../lib/declarator_visitor.hh"
#include "../lib/evaluator_visitor.hh"
#include <vector>

void DeclaratorVisitor::visitFunctionDefinition(function_definition *node)
{
    auto it = node->getChildrenList().begin();

    dataType return_type = static_cast<type_specifier *>(*it)->getType();
    it++;
    std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();
    it++;
    (*it)->accept(*this);
    it++;
    compound_statement *body = static_cast<compound_statement *>(*it);

    FuncSymbol *existing = dynamic_cast<FuncSymbol *>(
        SymbolTable::getInstance()->lookupGlobal(id));

    if (existing)
    {
        existing->setFunctionBody(body);
    }
    else
    {
        FuncSymbol *sym = new FuncSymbol(return_type, body, m_params, id);
        SymbolTable::getInstance()->insertGlobal(sym);
    }

    m_params.clear();
}

void DeclaratorVisitor::visitFunctionDeclaration(function_declaration *node)
{
    auto it = node->getChildrenList().begin();

    dataType return_type = static_cast<type_specifier *>(*it)->getType();
    it++;
    std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();
    it++;
    (*it)->accept(*this);

    FuncSymbol *sym = new FuncSymbol(return_type, nullptr, m_params, id);

    SymbolTable::getInstance()->insert(sym);
    m_params.clear();
}

void DeclaratorVisitor::visitVariableDeclaration(variable_declaration *node)
{
    if (node->getParent()->getNodeType() != EXTERNAL_DECLARATION_NODE)
    {
        return;
    }

    auto &temp = node->getChildrenList();
    auto it = temp.begin();

    if (temp.size() > 1)
    {
        it++;
        EvaluatorVisitor eval;
        (*it)->accept(eval);
        m_result = eval.getResult();
    }
}

void DeclaratorVisitor::visitVariableDeclarationList(
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

void DeclaratorVisitor::visitVariableDeclarationStatement(
    variable_declaration_statement *node)
{
    auto it = node->getChildrenList().begin();

    dataType currentType = static_cast<type_specifier *>(*it)->getType();

    it++;
    (*it)->accept(*this);
    for (auto &var : m_vars)
    {
        var->accept(*this);

        VarSymbol *sym = new VarSymbol(
            m_result,
            static_cast<IDENTIFIER *>(var->getChildrenList().front())
                ->getLabel(),
            currentType);

        SymbolTable::getInstance()->insertGlobal(sym);
    }

    m_vars.clear();
}

void DeclaratorVisitor::visitParameterList(parameter_list *node)
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
