#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "runtime/values.h"
#include "frontend/ast.h"
#include "runtime/environment.h"

std::shared_ptr<RuntimeVal> evaluateProgram(std::shared_ptr<Program> program, std::shared_ptr<Environment> env);
std::shared_ptr<RuntimeVal> evaluateVarDeclaration(std::shared_ptr<VarDeclaration> declaration, std::shared_ptr<Environment> env);

#endif