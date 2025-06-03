#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "runtime/values.h"
#include "frontend/ast.h"
#include "runtime/environment.h"

/**
 * @brief Evaluate an AST node in the given environment.
 *
 * This function dispatches on the node type and interprets the AST recursively.
 *
 * @param astNode The AST node to evaluate.
 * @param env The environment in which to evaluate the AST node.
 * @return std::shared_ptr<RuntimeVal> The result of the evaluation.
 */
std::shared_ptr<RuntimeVal> evaluate(std::shared_ptr<Stmt> astNode, std::shared_ptr<Environment> env);

#endif