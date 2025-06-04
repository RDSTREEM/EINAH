#include "runtime/eval/statements.h"
#include "runtime/interpreter.h"
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
