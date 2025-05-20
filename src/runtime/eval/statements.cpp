#include "runtime/eval/statements.h"
#include "runtime/interpreter.h"

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
    auto value = evaluate(declaration->value, env);
    value = value ? value : mkNull();
    return env->declareVar(declaration->ident, value, declaration->constant);
}
