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

/**
 * @brief Evaluate a print statement node.
 * @param printStmt The print statement node to evaluate.
 * @param env The environment in which to evaluate the statement.
 * @return std::shared_ptr<RuntimeVal> The result of the print statement evaluation (usually null or void).
 */
std::shared_ptr<RuntimeVal> evalPrintStatement(std::shared_ptr<PrintStatement> printStmt, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate a conditional statement node (if/else if/else chain).
 * @param condStmt The conditional statement node to evaluate.
 * @param env The environment in which to evaluate the statement.
 * @return std::shared_ptr<RuntimeVal> The result of the evaluated branch, or null if no branch is executed.
 */
std::shared_ptr<RuntimeVal> evalConditionalStatement(std::shared_ptr<ConditionalStatement> condStmt, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate a while loop node.
 * @param loop The while loop node to evaluate.
 * @param env The environment in which to evaluate the loop.
 * @return std::shared_ptr<RuntimeVal> The result of the loop evaluation (usually null or void).
 */
std::shared_ptr<RuntimeVal> evalWhileLoop(std::shared_ptr<WhileLoop> loop, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate a function declaration node.
 * @param funcDecl The function declaration node to evaluate.
 * @param env The environment in which to evaluate the function declaration.
 * @return std::shared_ptr<RuntimeVal> The result of the function declaration evaluation (usually null or void).
 */
std::shared_ptr<RuntimeVal> evalFunctionDeclaration(std::shared_ptr<FunctionDeclaration> funcDecl, std::shared_ptr<Environment> env);

/**
 * @brief Recursively print a RuntimeVal to std::cout in Einah syntax, including arrays and nested arrays.
 * @param val The runtime value to print.
 */
void printValue(const std::shared_ptr<RuntimeVal> &val);

/**
 * @brief Evaluate a block statement node (enclosed in braces).
 * @param block The block statement node to evaluate.
 * @param env The environment in which to evaluate the block.
 * @return std::shared_ptr<RuntimeVal> The result of the block statement evaluation (usually null or void).
 */
std::shared_ptr<RuntimeVal> evaluateBlockStatement(std::shared_ptr<BlockStatement> block, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate a for loop node.
 * @param loop The for loop node to evaluate.
 * @param env The environment in which to evaluate the loop.
 * @return std::shared_ptr<RuntimeVal> The result of the for loop evaluation (usually null or void).
 */
std::shared_ptr<RuntimeVal> evalForLoop(std::shared_ptr<ForLoop> loop, std::shared_ptr<Environment> env);

/**
 * @brief Evaluate a for-each loop node.
 * @param loop The for-each loop node to evaluate.
 * @param env The environment in which to evaluate the loop.
 * @return std::shared_ptr<RuntimeVal> The result of the for-each loop evaluation (usually null or void).
 */
std::shared_ptr<RuntimeVal> evalForEachLoop(std::shared_ptr<ForEachLoop> loop, std::shared_ptr<Environment> env);

#endif