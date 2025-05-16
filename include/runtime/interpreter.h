#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "runtime/values.h"
#include "frontend/ast.h"

std::shared_ptr<RuntimeVal> evalue(std::shared_ptr<Stmt> astNode)
{
    switch (astNode->kind)
    {
    case NodeType::NumericLiteral:
    {
        std::shared_ptr<NumericLiteral> numNode = std::static_pointer_cast<NumericLiteral>(astNode);
        return std::make_shared<NumberVal>(numNode->value);
    }

    default:
        return std::make_shared<NullVal>();
    }
}

#endif