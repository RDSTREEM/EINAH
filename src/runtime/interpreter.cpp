#include "frontend/ast.h"
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
        return mkNumber(std::static_pointer_cast<NumericLiteral>(astNode)->value);
    case NodeType::BooleanLiteral:
        return mkBool(std::static_pointer_cast<BooleanLiteral>(astNode)->value);
    case NodeType::StringLiteral:
        return mkString(std::static_pointer_cast<StringLiteral>(astNode)->value);
    case NodeType::NullLiteral:
        return mkNull();
    case NodeType::Identifier:
        return evaluateIdentifier(std::static_pointer_cast<Identifier>(astNode), env);
    case NodeType::BinaryExpr:
        return evaluateBinaryExpr(std::static_pointer_cast<BinaryExpr>(astNode), env);
    case NodeType::AssignmentExpr:
        return evaluateAssignmentExpr(std::static_pointer_cast<AssignmentExpr>(astNode), env);
    case NodeType::Program:
        return evaluateProgram(std::static_pointer_cast<Program>(astNode), env);
    case NodeType::VarDeclaration:
        return evaluateVarDeclaration(std::static_pointer_cast<VarDeclaration>(astNode), env);
    case NodeType::ExprStatement:
        return evaluate(std::static_pointer_cast<ExprStatement>(astNode)->expr, env);
    case NodeType::PrintStatement:
        return evalPrintStatement(std::static_pointer_cast<PrintStatement>(astNode), env);
    case NodeType::UnaryExpr:
        return evalUnaryExpr(std::static_pointer_cast<UnaryExpr>(astNode), env);
    case NodeType::ConditionalStatement:
        return evalConditionalStatement(std::static_pointer_cast<ConditionalStatement>(astNode), env);
    case NodeType::WhileLoop:
        return evalWhileLoop(std::static_pointer_cast<WhileLoop>(astNode), env);
    case NodeType::Skip:
        return (evalSkipStatement() == SKIP_SIGNAL) ? mkNull() : evalSkipStatement();
    case NodeType::Shatter:
        return (evalShatterStatement() == SHATTER_SIGNAL) ? mkNull() : evalShatterStatement();
    default:
        std::cerr << "This AST Node has not yet been setup for interpretation: " << magic_enum::enum_name(astNode->kind) << std::endl;
        exit(1);
    }
}