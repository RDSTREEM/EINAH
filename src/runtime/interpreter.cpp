#include "runtime/interpreter.h"
#include "runtime/eval/expressions.h"
#include "runtime/eval/statements.h"
#include <iostream>
#include <third_party/magic_enum.hpp>

std::shared_ptr<RuntimeVal> evaluate(std::shared_ptr<Stmt> astNode, std::shared_ptr<Environment> env)

{
    switch (astNode->kind)
    {
    case NodeType::NumericLiteral:
    {
        std::shared_ptr<NumericLiteral> numNode = std::static_pointer_cast<NumericLiteral>(astNode);
        return std::make_shared<NumberVal>(numNode->value);
    }

    case NodeType::BooleanLiteral:
    {
        auto boolNode = std::static_pointer_cast<BooleanLiteral>(astNode);
        return std::make_shared<BooleanVal>(boolNode->value);
    }
    case NodeType::NullLiteral:
    {
        return mkNull();
    }
    case NodeType::Identifier:
    {
        return evaluateIdentifier(std::static_pointer_cast<Identifier>(astNode), env);
    }
    case NodeType::BinaryExpr:
    {
        auto biNode = std::static_pointer_cast<BinaryExpr>(astNode);
        return evaluateBinaryExpr(biNode, env);
    }
    case NodeType::AssignmentExpr:
    {
        auto assign = std::static_pointer_cast<AssignmentExpr>(astNode);
        return evaluateAssignmentExpr(assign, env);
    }
    case NodeType::Program:
    {
        auto program = std::static_pointer_cast<Program>(astNode);
        return evaluateProgram(program, env);
    }
    case NodeType::VarDeclaration:
    {
        auto varDeclare = std::static_pointer_cast<VarDeclaration>(astNode);
        return evaluateVarDeclaration(varDeclare, env);
    }
    case NodeType::ExprStatement:
    {
        auto exprStmt = std::static_pointer_cast<ExprStatement>(astNode);
        return evaluate(exprStmt->expr, env);
    }
    default:
        std::cerr << "This AST Node has not yet been setup for interpretation: " << magic_enum::enum_name(astNode->kind) << std::endl;
        exit(1);
    }
}