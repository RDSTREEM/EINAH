#include "runtime/eval/expressions.h"
#include "runtime/interpreter.h"
#include <math.h>
#include <iostream>

std::shared_ptr<RuntimeVal> evaluateBinaryExpr(std::shared_ptr<BinaryExpr> binop, std::shared_ptr<Environment> env)
{
    // Unary NOT
    if (binop->op == "~!")
    {
        std::shared_ptr<RuntimeVal> rhs = evaluate(binop->right, env);
        if (rhs->_type == ValueType::Boolean)
        {
            auto b = std::static_pointer_cast<BooleanVal>(rhs);
            return mkBool(!b->val);
        }
        if (rhs->_type == ValueType::Number)
        {
            auto n = std::static_pointer_cast<NumberVal>(rhs);
            return mkBool(n->val == 0);
        }
        return mkBool(false);
    }

    std::shared_ptr<RuntimeVal> lhs = binop->left ? evaluate(binop->left, env) : nullptr;
    std::shared_ptr<RuntimeVal> rhs = evaluate(binop->right, env);

    if (binop->op == "~~")
    {
        if (lhs && rhs && lhs->_type == rhs->_type)
        {
            if (lhs->_type == ValueType::Number)
            {
                auto l = std::static_pointer_cast<NumberVal>(lhs);
                auto r = std::static_pointer_cast<NumberVal>(rhs);
                return mkBool(l->val == r->val);
            }
            if (lhs->_type == ValueType::Boolean)
            {
                auto l = std::static_pointer_cast<BooleanVal>(lhs);
                auto r = std::static_pointer_cast<BooleanVal>(rhs);
                return mkBool(l->val == r->val);
            }
        }
        return mkBool(false);
    }
    if (binop->op == "!~")
    {
        if (lhs && rhs && lhs->_type == rhs->_type)
        {
            if (lhs->_type == ValueType::Number)
            {
                auto l = std::static_pointer_cast<NumberVal>(lhs);
                auto r = std::static_pointer_cast<NumberVal>(rhs);
                return mkBool(l->val != r->val);
            }
            if (lhs->_type == ValueType::Boolean)
            {
                auto l = std::static_pointer_cast<BooleanVal>(lhs);
                auto r = std::static_pointer_cast<BooleanVal>(rhs);
                return mkBool(l->val != r->val);
            }
        }
        return mkBool(true);
    }
    if (binop->op == "&=")
    {
        if (lhs && rhs && lhs->_type == ValueType::Boolean && rhs->_type == ValueType::Boolean)
        {
            auto l = std::static_pointer_cast<BooleanVal>(lhs);
            auto r = std::static_pointer_cast<BooleanVal>(rhs);
            return mkBool(l->val && r->val);
        }
        return mkBool(false);
    }
    if (binop->op == "|=")
    {
        if (lhs && rhs && lhs->_type == ValueType::Boolean && rhs->_type == ValueType::Boolean)
        {
            auto l = std::static_pointer_cast<BooleanVal>(lhs);
            auto r = std::static_pointer_cast<BooleanVal>(rhs);
            return mkBool(l->val || r->val);
        }
        return mkBool(false);
    }

    if (rhs && lhs && rhs->_type == ValueType::Number && lhs->_type == ValueType::Number)
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
    else if (op == "%")
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