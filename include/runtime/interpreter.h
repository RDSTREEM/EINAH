#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "runtime/values.h"
#include "frontend/ast.h"
#include "runtime/environment.h"

std::shared_ptr<RuntimeVal> evaluate(std::shared_ptr<Stmt> astNode, std::shared_ptr<Environment> env);
std::shared_ptr<RuntimeVal> evaluateBinaryExpr(std::shared_ptr<BinaryExpr> binop, std::shared_ptr<Environment> env);
std::shared_ptr<RuntimeVal> evaluateProgram(std::shared_ptr<Program> program, std::shared_ptr<Environment> env);
std::shared_ptr<NumberVal> evaluateNumericBinaryExpr(std::shared_ptr<NumberVal> lhs, std::shared_ptr<NumberVal> rhs, std::string &op);
std::shared_ptr<RuntimeVal> evalIdentifier(std::shared_ptr<Identifier> ident, std::shared_ptr<Environment> env);

#endif