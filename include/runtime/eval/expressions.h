#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "frontend/ast.h"
#include "runtime/values.h"
#include "runtime/environment.h"

std::shared_ptr<RuntimeVal> evaluateBinaryExpr(std::shared_ptr<BinaryExpr> binop, std::shared_ptr<Environment> env);
std::shared_ptr<NumberVal> evaluateNumericBinaryExpr(std::shared_ptr<NumberVal> lhs, std::shared_ptr<NumberVal> rhs, const std::string &op);
std::shared_ptr<RuntimeVal> evaluateIdentifier(std::shared_ptr<Identifier> ident, std::shared_ptr<Environment> env);
std::shared_ptr<RuntimeVal> evaluateAssignmentExpr(std::shared_ptr<AssignmentExpr> node, std::shared_ptr<Environment> env);

#endif
