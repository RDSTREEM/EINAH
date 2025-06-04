#include "runtime/eval/statements.h"
#include "runtime/interpreter.h"
#include <iostream>
#include <stdexcept>

// Provide the single definitions for the singleton signals (no static!)
std::shared_ptr<RuntimeVal> SKIP_SIGNAL = std::make_shared<RuntimeVal>();    // Singleton for skip statement
std::shared_ptr<RuntimeVal> SHATTER_SIGNAL = std::make_shared<RuntimeVal>(); // Singleton for shatter statement

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
    if (val == SKIP_SIGNAL || val == SHATTER_SIGNAL)
    {
        return val;
    }
    if (val->_type == ValueType::Number)
    {
        auto num = std::static_pointer_cast<NumberVal>(val);
        std::cout << num->val << std::endl;
    }
    else if (val->_type == ValueType::Boolean)
    {
        auto b = std::static_pointer_cast<BooleanVal>(val);
        std::cout << (b->val ? "yup" : "nope") << std::endl;
    }
    else if (val->_type == ValueType::Null)
    {
        std::cout << "zip" << std::endl;
    }
    else if (val->_type == ValueType::String)
    {
        auto s = std::static_pointer_cast<StringVal>(val);
        std::cout << s->val << std::endl;
    }
    else
    {
        std::cout << "[object]" << std::endl;
    }
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

// Todo: Pls for gods sake fix me
static std::shared_ptr<RuntimeVal> evalLoopBody(const std::vector<std::shared_ptr<Stmt>> &body, std::shared_ptr<Environment> env, std::shared_ptr<RuntimeVal> &last)
{
    for (const auto &stmt : body)
    {
        auto result = evaluate(stmt, env);
        if (result == SKIP_SIGNAL)
            break;
        if (result == SHATTER_SIGNAL)
            return SHATTER_SIGNAL;
        last = result;
    }
    return nullptr;
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
        auto signal = evalLoopBody(loop->body, blockEnv, last);
        if (signal == SHATTER_SIGNAL)
            break;
        if (signal == SKIP_SIGNAL)
            continue;
    }
    return last;
}

std::shared_ptr<RuntimeVal> evalSkipStatement()
{
    return SKIP_SIGNAL;
}

std::shared_ptr<RuntimeVal> evalShatterStatement()
{
    return SHATTER_SIGNAL;
}
