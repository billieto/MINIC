#include "../lib/declarator_visitor.hh"
#include "../lib/evaluator_visitor.hh"
#include <iostream>
#include <vector>

void DeclaratorVisitor::visitFunctionDefinition(function_definition *node)
{
    auto it = node->getChildrenList().begin();

    dataType return_type = static_cast<type_specifier *>(*it)->getType();
    it++;
    std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();
    it++;
    std::vector<parameter> &params =
        static_cast<parameter_list *>(*it)->getParameters();
    it++;
    compound_statement *body = static_cast<compound_statement *>(*it);

    FuncSymbol *existing = dynamic_cast<FuncSymbol *>(
        SymbolTable::getInstance()->lookupGlobal(id));

    if (existing)
    {
        if (existing->getReturnType() != return_type ||
            existing->getParameters() != params)
        {
            // TODO: Maybe will move this into type checker
            std::cerr << "Conflicting types for function" << std::endl;
            exit(1);
        }

        if (existing->getFunctionBody() != nullptr)
        {
            // TODO: Maybe will move this into type checker
            std::cerr << "Function already defined" << std::endl;
            exit(1);
        }
        existing->setFunctionBody(body);
    }
    else
    {
        // FuncSymbol sym{return_type, body, params, id};
        FuncSymbol *sym = new FuncSymbol(return_type, body, params, id);
        if (!SymbolTable::getInstance()->insertGlobal(sym))
        {
            std::cerr << "Function: " + id + "already defined" << std::endl;
        }
    }
}

void DeclaratorVisitor::visitFunctionDeclaration(function_declaration *node)
{
    auto it = node->getChildrenList().begin();

    dataType return_type = static_cast<type_specifier *>(*it)->getType();
    it++;
    std::string id = static_cast<IDENTIFIER *>(*it)->getLabel();
    it++;
    std::vector<parameter> &params =
        static_cast<parameter_list *>(*it)->getParameters();

    FuncSymbol *sym = new FuncSymbol(return_type, nullptr, params, id);

    if (!SymbolTable::getInstance()->insert(sym))
    {
        // TODO: Maybe will move this into type checker
        std::cerr << "Function already declared" << std::endl;
        exit(1);
    }
}

void DeclaratorVisitor::visitVariableDeclaration(variable_declaration *node)
{
    if (node->getParent()->getNodeType() != EXTERNAL_DECLARATION_NODE)
    {
        return;
    }

    auto &temp = node->getChildrenList();
    auto it = temp.begin();

    node->setName(static_cast<IDENTIFIER *>(*it)->getLabel());
    it++;

    if (temp.size() > 1)
    {
        EvaluatorVisitor eval;
        (*it)->accept(eval);
        m_result = eval.getResult();

        // node->setValue(result);
    }
}

void DeclaratorVisitor::visitVariableDeclarationStatement(
    variable_declaration_statement *node)
{
    auto it = node->getChildrenList().begin();

    dataType currentType = static_cast<type_specifier *>(*it)->getType();

    it++;
    std::vector<variable_declaration *> varList =
        static_cast<variable_declaration_list *>(*it)->getVariables();

    for (auto var : varList)
    {
        var->accept(*this);

        // Need a way so i dont store the values into the AST.
        VarSymbol *sym = new VarSymbol(
            m_result,
            static_cast<IDENTIFIER *>(var->getChildrenList().front())
                ->getLabel(),
            currentType);

        if (!SymbolTable::getInstance()->insertGlobal(sym))
        { // Semantic Error
            std::cerr << "Variable " << sym->getName() << " already exists."
                      << std::endl;
            exit(1);
        }
    }
}