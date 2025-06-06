#include "runtime/eval/statements.h"
#include "runtime/interpreter.h"
#include "frontend/ast.h"
#include <iostream>
#include <stdexcept>

std::shared_ptr<RuntimeVal> evaluateProgram(std::shared_ptr<Program> program, std::shared_ptr<Environment> env)
{
    std::shared_ptr<RuntimeVal> lastEvaluated = mkNull();

    for (auto &statement : program->body)
    {
        lastEvaluated = evaluate(statement, env);
    }

    return lastEvaluated;
}

std::shared_ptr<RuntimeVal> evaluateVarDeclaration(std::shared_ptr<VarDeclaration> declaration, std::shared_ptr<Environment> env)
{
    auto value = declaration->value ? evaluate(declaration->value, env) : mkNull();
    return env->declareVar(declaration->ident, value, declaration->constant);
}

std::shared_ptr<RuntimeVal> evalPrintStatement(std::shared_ptr<PrintStatement> printStmt, std::shared_ptr<Environment> env)
{
    auto val = evaluate(printStmt->argument, env);
    printValue(val);
    std::cout << std::endl;
    return mkNull();
}

std::shared_ptr<RuntimeVal> evalConditionalStatement(std::shared_ptr<ConditionalStatement> condStmt, std::shared_ptr<Environment> env)
{
    auto condVal = evaluate(condStmt->condition, env);
    if (condVal->_type != ValueType::Boolean)
        throw std::runtime_error("Condition in conditional must evaluate to a boolean (yup/nope)");
    auto b = std::static_pointer_cast<BooleanVal>(condVal);
    if (b->val)
    {
        auto blockEnv = std::make_shared<Environment>(env);
        std::shared_ptr<RuntimeVal> last = mkNull();
        for (const auto &stmt : condStmt->thenBlock)
        {
            last = evaluate(stmt, blockEnv);
        }
        return last;
    }
    std::shared_ptr<RuntimeVal> last = mkNull();
    for (const auto &stmt : condStmt->elseBlock)
    {
        if (stmt->kind == NodeType::ConditionalStatement)
        {
            last = evalConditionalStatement(std::static_pointer_cast<ConditionalStatement>(stmt), env);
            if (last != nullptr && last->_type != ValueType::Null)
            {
                return last;
            }
        }
        else
        {
            auto blockEnv = std::make_shared<Environment>(env);
            last = evaluate(stmt, blockEnv);
        }
    }
    return last;
}

std::shared_ptr<RuntimeVal> evalWhileLoop(std::shared_ptr<WhileLoop> loop, std::shared_ptr<Environment> env)
{
    std::shared_ptr<RuntimeVal> last = mkNull();
    while (true)
    {
        auto condVal = evaluate(loop->condition, env);
        if (condVal->_type != ValueType::Boolean)
            throw std::runtime_error("Condition in while loop must evaluate to a boolean (yup/nope)");
        auto b = std::static_pointer_cast<BooleanVal>(condVal);
        if (!b->val)
            break;
        auto blockEnv = std::make_shared<Environment>(env);
        try
        {
            for (const auto &stmt : loop->body)
            {
                try
                {
                    auto result = evaluate(stmt, env);
                    last = result;
                }
                catch (const std::runtime_error &e)
                {
                    if (std::string(e.what()) == "__EINAH_SKIP__")
                        break;
                    else if (std::string(e.what()) == "__EINAH_SHATTER__")
                        return last;
                    else
                        throw;
                }
            }
        }
        catch (const std::runtime_error &e)
        {
            if (std::string(e.what()) == "__EINAH_SHATTER__")
                return last;
            else
                throw;
        }
    }
    return last;
}

std::shared_ptr<RuntimeVal> evalFunctionDeclaration(std::shared_ptr<FunctionDeclaration> funcDecl, std::shared_ptr<Environment> env)
{
    auto funcVal = mkFunction(funcDecl->params, funcDecl->body, env);
    env->declareVar(funcDecl->name, funcVal, true);
    return funcVal;
}

std::shared_ptr<RuntimeVal> evaluateBlockStatement(std::shared_ptr<BlockStatement> block, std::shared_ptr<Environment> env)
{
    auto blockEnv = std::make_shared<Environment>(env);
    std::shared_ptr<RuntimeVal> last = mkNull();
    for (const auto &stmt : block->body)
    {
        last = evaluate(stmt, blockEnv);
    }
    return last;
}

std::shared_ptr<RuntimeVal> evalForLoop(std::shared_ptr<ForLoop> loop, std::shared_ptr<Environment> env)
{
    auto startVal = evaluate(loop->start, env);
    auto endVal = evaluate(loop->end, env);
    auto stepVal = evaluate(loop->step, env);
    if (startVal->_type != ValueType::Number || endVal->_type != ValueType::Number || stepVal->_type != ValueType::Number)
        throw std::runtime_error("For loop bounds and step must be numbers");
    int start = static_cast<int>(std::static_pointer_cast<NumberVal>(startVal)->val);
    int end = static_cast<int>(std::static_pointer_cast<NumberVal>(endVal)->val);
    int step = static_cast<int>(std::static_pointer_cast<NumberVal>(stepVal)->val);
    if (step == 0)
        throw std::runtime_error("For loop step cannot be zero");
    std::shared_ptr<RuntimeVal> last = mkNull();
    for (int i = start; (step > 0) ? (i <= end) : (i >= end); i += step)
    {
        auto loopEnv = std::make_shared<Environment>(env);
        loopEnv->declareVar(loop->iterator, mkNumber(i), false);
        try
        {
            for (const auto &stmt : loop->body)
            {
                try
                {
                    last = evaluate(stmt, loopEnv);
                }
                catch (const std::runtime_error &e)
                {
                    if (std::string(e.what()) == "__EINAH_SKIP__")
                        break;
                    else if (std::string(e.what()) == "__EINAH_SHATTER__")
                        return last;
                    else
                        throw;
                }
            }
        }
        catch (const std::runtime_error &e)
        {
            if (std::string(e.what()) == "__EINAH_SHATTER__")
                return last;
            else
                throw;
        }
    }
    return last;
}

std::shared_ptr<RuntimeVal> evalForEachLoop(std::shared_ptr<ForEachLoop> loop, std::shared_ptr<Environment> env)
{
    auto arrVal = evaluate(loop->iterable, env);
    if (arrVal->_type != ValueType::Array)
        throw std::runtime_error("For-each loop expects an array as the iterable");
    auto arr = std::static_pointer_cast<ArrayVal>(arrVal);
    std::shared_ptr<RuntimeVal> last = mkNull();
    for (const auto &item : arr->val)
    {
        auto loopEnv = std::make_shared<Environment>(env);
        loopEnv->declareVar(loop->iterator, item, false);
        try
        {
            for (const auto &stmt : loop->body)
            {
                try
                {
                    last = evaluate(stmt, loopEnv);
                }
                catch (const std::runtime_error &e)
                {
                    if (std::string(e.what()) == "__EINAH_SKIP__")
                        break;
                    else if (std::string(e.what()) == "__EINAH_SHATTER__")
                        return last;
                    else
                        throw;
                }
            }
        }
        catch (const std::runtime_error &e)
        {
            if (std::string(e.what()) == "__EINAH_SHATTER__")
                return last;
            else
                throw;
        }
    }
    return last;
}

void printValue(const std::shared_ptr<RuntimeVal> &val)
{
    switch (val->_type)
    {
    case ValueType::Number:
    {
        auto num = std::static_pointer_cast<NumberVal>(val);
        std::cout << num->val;
        break;
    }
    case ValueType::Boolean:
    {
        auto b = std::static_pointer_cast<BooleanVal>(val);
        std::cout << (b->val ? "yup" : "nope");
        break;
    }
    case ValueType::Null:
    {
        std::cout << "zip";
        break;
    }
    case ValueType::String:
    {
        auto s = std::static_pointer_cast<StringVal>(val);
        std::cout << s->val;
        break;
    }
    case ValueType::Array:
    {
        auto arr = std::static_pointer_cast<ArrayVal>(val);
        std::cout << "$[";
        for (size_t i = 0; i < arr->val.size(); ++i)
        {
            printValue(arr->val[i]);
            if (i + 1 < arr->val.size())
                std::cout << ", ";
        }
        std::cout << "]$";
        break;
    }
    case ValueType::Object:
    {
        auto obj = std::static_pointer_cast<ObjectVal>(val);
        std::cout << "@";
        bool first = true;
        for (const auto &pair : obj->val)
        {
            if (!first)
                std::cout << ";";
            std::cout << pair.first << " => ";
            printValue(pair.second);
            first = false;
        }
        std::cout << "@";
        break;
    }
    default:
        std::cout << "[unknown]";
    }
}
