#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "frontend/ast.h"
#include "runtime/values.h"
#include "runtime/environment.h"

/**
 * @brief Evaluate a binary expression (arithmetic, logical, equality, etc.).
 * @param binop The binary expression node.
 * @param env The environment in which to evaluate.
 * @return std::shared_ptr<RuntimeVal> The result of the evaluation.
 */
std::shared_ptr<RuntimeVal> evaluateBinaryExpr(std::shared_ptr<BinaryExpr> binop, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate a numeric binary expression (+, -, *, /, %).
 * @param lhs The left-hand side number.
 * @param rhs The right-hand side number.
 * @param op The operator as a string.
 * @return std::shared_ptr<NumberVal> The result of the operation.
 */
std::shared_ptr<NumberVal> evaluateNumericBinaryExpr(std::shared_ptr<NumberVal> lhs, std::shared_ptr<NumberVal> rhs, const std::string &op);

/**
 * @brief Evaluate an identifier (variable lookup).
 * @param ident The identifier node.
 * @param env The environment in which to evaluate.
 * @return std::shared_ptr<RuntimeVal> The value of the identifier.
 */
std::shared_ptr<RuntimeVal> evaluateIdentifier(std::shared_ptr<Identifier> ident, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate an assignment expression.
 * @param node The assignment expression node.
 * @param env The environment in which to evaluate.
 * @return std::shared_ptr<RuntimeVal> The result of the assignment.
 */
std::shared_ptr<RuntimeVal> evaluateAssignmentExpr(std::shared_ptr<AssignmentExpr> node, std::shared_ptr<Environment> env);

#endif
