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

/**
 * @brief Evaluate a unary expression (e.g., ~!a).
 * @param op The unary operator as a string.
 * @param operand The operand value.
 * @return std::shared_ptr<RuntimeVal> The result of the unary operation.
 */
std::shared_ptr<RuntimeVal> evaluateUnaryExpr(const std::string &op, std::shared_ptr<RuntimeVal> operand);

/**
 * @brief Evaluate a relational binary expression (<, >, <~, >~).
 * @param lhs The left-hand side value.
 * @param rhs The right-hand side value.
 * @param op The operator as a string.
 * @return std::shared_ptr<RuntimeVal> The result of the relational operation.
 */
std::shared_ptr<RuntimeVal> evaluateRelationalBinaryExpr(std::shared_ptr<RuntimeVal> lhs, std::shared_ptr<RuntimeVal> rhs, const std::string &op);

/**
 * @brief Evaluate a unary expression node.
 * @param unaryNode The unary expression node.
 * @param env The environment in which to evaluate.
 * @return std::shared_ptr<RuntimeVal> The result of the unary expression evaluation.
 */
std::shared_ptr<RuntimeVal> evalUnaryExpr(std::shared_ptr<UnaryExpr> unaryNode, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate an array literal expression.
 * @param arrNode The array literal AST node.
 * @param env The environment in which to evaluate.
 * @return std::shared_ptr<RuntimeVal> The resulting array value.
 */
std::shared_ptr<RuntimeVal> evalArrayLiteral(std::shared_ptr<ArrayLiteral> arrNode, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate an index expression (array subscript).
 * @param idxExpr The index expression node.
 * @param env The environment in which to evaluate.
 * @return std::shared_ptr<RuntimeVal> The result of the index expression evaluation.
 */
std::shared_ptr<RuntimeVal> evalIndexExpr(std::shared_ptr<IndexExpr> idxExpr, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate a function call expression.
 * @param call The function call expression node.
 * @param env The environment in which to evaluate.
 * @return std::shared_ptr<RuntimeVal> The result of the function call evaluation.
 */
std::shared_ptr<RuntimeVal> evalCallExpr(std::shared_ptr<CallExpr> call, std::shared_ptr<Environment> env);

#endif
