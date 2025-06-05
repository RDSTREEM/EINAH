#include "runtime/eval/expressions.h"
#include "runtime/interpreter.h"
#include <math.h>
#include <iostream>

std::shared_ptr<RuntimeVal> evaluateBinaryExpr(std::shared_ptr<BinaryExpr> binop, std::shared_ptr<Environment> env)
{
    std::shared_ptr<RuntimeVal> lhs = binop->left ? evaluate(binop->left, env) : nullptr;
    std::shared_ptr<RuntimeVal> rhs = evaluate(binop->right, env);

    if (binop->op == "<" || binop->op == ">" || binop->op == "<~" || binop->op == ">~")
        return evaluateRelationalBinaryExpr(lhs, rhs, binop->op);

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

std::shared_ptr<RuntimeVal> evaluateUnaryExpr(const std::string &op, std::shared_ptr<RuntimeVal> operand)
{
    if (op == "~!")
    {
        if (operand->_type == ValueType::Boolean)
        {
            auto b = std::static_pointer_cast<BooleanVal>(operand);
            return mkBool(!b->val);
        }
        if (operand->_type == ValueType::Number)
        {
            auto n = std::static_pointer_cast<NumberVal>(operand);
            return mkBool(n->val == 0);
        }
        return mkBool(false);
    }
    return mkNull();
}

std::shared_ptr<RuntimeVal> evaluateRelationalBinaryExpr(std::shared_ptr<RuntimeVal> lhs, std::shared_ptr<RuntimeVal> rhs, const std::string &op)
{
    if (!lhs || !rhs || lhs->_type != ValueType::Number || rhs->_type != ValueType::Number)
        return mkBool(false);
    auto l = std::static_pointer_cast<NumberVal>(lhs);
    auto r = std::static_pointer_cast<NumberVal>(rhs);
    if (op == "<")
        return mkBool(l->val < r->val);
    if (op == ">")
        return mkBool(l->val > r->val);
    if (op == "<~")
        return mkBool(l->val <= r->val);
    if (op == ">~")
        return mkBool(l->val >= r->val);
    return mkBool(false);
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

std::shared_ptr<RuntimeVal> evalUnaryExpr(std::shared_ptr<UnaryExpr> unaryNode, std::shared_ptr<Environment> env)
{
    auto argVal = evaluate(unaryNode->argument, env);
    return evaluateUnaryExpr(unaryNode->op, argVal);
}

std::shared_ptr<RuntimeVal> evalArrayLiteral(std::shared_ptr<ArrayLiteral> arrNode, std::shared_ptr<Environment> env)
{
    std::vector<std::shared_ptr<RuntimeVal>> elements;
    for (const auto &expr : arrNode->elements)
    {
        elements.push_back(evaluate(expr, env));
    }
    return mkArray(elements);
}

std::shared_ptr<RuntimeVal> evalIndexExpr(std::shared_ptr<IndexExpr> idxExpr, std::shared_ptr<Environment> env)
{
    auto arrVal = evaluate(idxExpr->array, env);
    auto idxVal = evaluate(idxExpr->index, env);
    if (arrVal->_type != ValueType::Array)
        throw std::runtime_error("Tried to index a non-array value");
    if (idxVal->_type != ValueType::Number)
        throw std::runtime_error("Array index must be a number");
    auto arr = std::static_pointer_cast<ArrayVal>(arrVal);
    auto idx = static_cast<size_t>(std::static_pointer_cast<NumberVal>(idxVal)->val);
    if (idx >= arr->val.size())
        throw std::runtime_error("Array index out of bounds");
    return arr->val[idx];
}

std::shared_ptr<RuntimeVal> evalCallExpr(std::shared_ptr<CallExpr> call, std::shared_ptr<Environment> env)
{
    auto calleeVal = evaluate(call->callee, env);
    if (calleeVal->_type != ValueType::Function)
    {
        throw std::runtime_error("Attempted to call a non-function value");
    }
    auto func = std::static_pointer_cast<FunctionVal>(calleeVal);
    std::vector<std::shared_ptr<RuntimeVal>> argVals;
    for (auto &arg : call->arguments)
    {
        argVals.push_back(evaluate(arg, env));
    }
    auto funcEnv = std::make_shared<Environment>(func->closure);
    if (argVals.size() != func->params.size())
    {
        throw std::runtime_error("Function argument count mismatch");
    }
    for (size_t i = 0; i < func->params.size(); ++i)
    {
        funcEnv->declareVar(func->params[i], argVals[i], true);
    }
    std::shared_ptr<RuntimeVal> last = mkNull();
    try
    {
        for (auto &stmt : func->body)
        {
            last = evaluate(stmt, funcEnv);
        }
    }
    catch (const std::pair<std::string, std::shared_ptr<RuntimeVal>> &ret)
    {
        if (ret.first == "__EINAH_RETURN__")
            return ret.second;
        throw;
    }
    return last;
}