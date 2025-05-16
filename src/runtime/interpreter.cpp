#include "runtime/interpreter.h"
#include <iostream>
#include <math.h>

std::ostream &operator<<(std::ostream &os, const NodeType node)
{
    switch (node)
    {
    case NodeType::Program:
    {
        os << "Program";
        break;
    }
    case NodeType::BinaryExpr:
    {
        os << "BinaryExpr";
        break;
    }
    case NodeType::Identifier:
    {
        os << "Identifier";
        break;
    }
    case NodeType::NumericLiteral:
    {
        os << "NumericLiteral";
        break;
    }
    case NodeType::NullLiteral:
    {
        os << "NullLiteral";
        break;
    }

    default:
        break;
    }
    return os;
}

std::shared_ptr<RuntimeVal> evaluate(std::shared_ptr<Stmt> astNode)

{
    switch (astNode->kind)
    {
    case NodeType::NumericLiteral:
    {
        std::shared_ptr<NumericLiteral> numNode = std::static_pointer_cast<NumericLiteral>(astNode);
        return std::make_shared<NumberVal>(numNode->value);
    }

    case NodeType::NullLiteral:
    {
        return std::make_shared<NullVal>();
    }

    case NodeType::BinaryExpr:
    {
        auto biNode = std::static_pointer_cast<BinaryExpr>(astNode);
        return evaluateBinaryExpr(biNode);
    }
    case NodeType::Program:
    {
        auto program = std::static_pointer_cast<Program>(astNode);
        return evaluateProgram(program);
    }
    default:
        std::cerr << "This AST Node has not yet been setup for interpretation: " << astNode->kind;
        exit(1);
    }
}

std::shared_ptr<RuntimeVal> evaluateBinaryExpr(std::shared_ptr<BinaryExpr> binop)
{
    std::shared_ptr<RuntimeVal> lhs = evaluate(binop->left);
    std::shared_ptr<RuntimeVal> rhs = evaluate(binop->right);

    if (rhs->_type == ValueType::Number && lhs->_type == ValueType::Number)
    {
        auto lhsNum = std::static_pointer_cast<NumberVal>(lhs);
        auto rhsNum = std::static_pointer_cast<NumberVal>(rhs);
        return evaluateNumericBinaryExpr(lhsNum, rhsNum, binop->op);
    }

    return std::make_shared<NullVal>();
}

std::shared_ptr<RuntimeVal> evaluateProgram(std::shared_ptr<Program> program)
{
    std::shared_ptr<RuntimeVal> lastEvaluated = std::make_shared<NullVal>();

    for (auto &statement : program->body)
    {
        lastEvaluated = evaluate(statement);
    }

    return lastEvaluated;
}

std::shared_ptr<NumberVal> evaluateNumericBinaryExpr(std::shared_ptr<NumberVal> lhs, std::shared_ptr<NumberVal> rhs, std::string &op)
{
    double result = 0;
    if (op == "+")
    {
        result = lhs->val + rhs->val;
    }
    else if (op == "-")
    {
        result = lhs->val - rhs->val;
    }
    else if (op == "*")
    {
        result = lhs->val * rhs->val;
    }
    else if (op == "/")
    {
        result = lhs->val / rhs->val;
    }
    else
    {
        result = std::fmod(lhs->val, rhs->val);
    }

    return std::make_shared<NumberVal>(result);
}