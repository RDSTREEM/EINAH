#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "runtime/values.h"
#include "frontend/ast.h"
#include "runtime/environment.h"

/**
 * @brief Evaluate a program node (sequence of statements).
 * @param program The program node.
 * @param env The environment in which to evaluate.
 * @return std::shared_ptr<RuntimeVal> The result of the last evaluated statement.
 */
std::shared_ptr<RuntimeVal> evaluateProgram(std::shared_ptr<Program> program, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate a variable declaration (sprout/root).
 * @param declaration The variable declaration node.
 * @param env The environment in which to evaluate.
 * @return std::shared_ptr<RuntimeVal> The value assigned to the variable.
 */
std::shared_ptr<RuntimeVal> evaluateVarDeclaration(std::shared_ptr<VarDeclaration> declaration, std::shared_ptr<Environment> env);

#endif