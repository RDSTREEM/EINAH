#include "runtime/eval/expressions.h"
#include "runtime/interpreter.h"
#include <math.h>
#include <iostream>

std::shared_ptr<RuntimeVal> evaluateBinaryExpr(std::shared_ptr<BinaryExpr> binop, std::shared_ptr<Environment> env)
{
    std::shared_ptr<RuntimeVal> lhs = evaluate(binop->left, env);
    std::shared_ptr<RuntimeVal> rhs = evaluate(binop->right, env);

    if (binop->op == "==")
    {
        if (lhs->_type == ValueType::Number && rhs->_type == ValueType::Number)
        {
            auto lhsNum = std::static_pointer_cast<NumberVal>(lhs);
            auto rhsNum = std::static_pointer_cast<NumberVal>(rhs);
            return mkBool(lhsNum->val == rhsNum->val);
        }
        return mkBool(false);
    }

    if (rhs->_type == ValueType::Number && lhs->_type == ValueType::Number)
    {
        auto lhsNum = std::static_pointer_cast<NumberVal>(lhs);
        auto rhsNum = std::static_pointer_cast<NumberVal>(rhs);
        return evaluateNumericBinaryExpr(lhsNum, rhsNum, binop->op);
    }

    return mkNull();
}

std::shared_ptr<NumberVal> evaluateNumericBinaryExpr(std::shared_ptr<NumberVal> lhs, std::shared_ptr<NumberVal> rhs, const std::string &op)
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

std::shared_ptr<RuntimeVal> evaluateIdentifier(std::shared_ptr<Identifier> ident, std::shared_ptr<Environment> env)
{
    auto val = env->lookUp(ident->symbol);
    return val;
}

std::shared_ptr<RuntimeVal> evaluateAssignmentExpr(std::shared_ptr<AssignmentExpr> node, std::shared_ptr<Environment> env)
{
    if (node->asignee->kind != NodeType::Identifier)
        throw std::runtime_error("Invalid LHS inside the expression");

    const std::string varName = std::static_pointer_cast<Identifier>(node->asignee)->symbol;
    return env->assignVar(varName, evaluate(node->value, env));
}