#include "runtime/interpreter.h"
#include "runtime/eval/expressions.h"
#include "runtime/eval/statements.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const NodeType node)
{
    switch (node)
    {
    case NodeType::Program:
    {
        os << "Program";
        break;
    }
    case NodeType::BinaryExpr:
    {
        os << "BinaryExpr";
        break;
    }
    case NodeType::Identifier:
    {
        os << "Identifier";
        break;
    }
    case NodeType::NumericLiteral:
    {
        os << "NumericLiteral";
        break;
    }
        // case NodeType::NullLiteral:
        // {
        //     os << "NullLiteral";
        //     break;
        // }

    default:
        os << "UnkownNodeType";
        break;
    }
    return os;
}

std::shared_ptr<RuntimeVal> evaluate(std::shared_ptr<Stmt> astNode, std::shared_ptr<Environment> env)

{
    switch (astNode->kind)
    {
    case NodeType::NumericLiteral:
    {
        std::shared_ptr<NumericLiteral> numNode = std::static_pointer_cast<NumericLiteral>(astNode);
        return std::make_shared<NumberVal>(numNode->value);
    }

    // case NodeType::NullLiteral:
    // {
    //     return mkNull();
    // }
    case NodeType::Identifier:
    {
        return evaluateIdentifier(std::static_pointer_cast<Identifier>(astNode), env);
    }
    case NodeType::BinaryExpr:
    {
        auto biNode = std::static_pointer_cast<BinaryExpr>(astNode);
        return evaluateBinaryExpr(biNode, env);
    }
    case NodeType::Program:
    {
        auto program = std::static_pointer_cast<Program>(astNode);
        return evaluateProgram(program, env);
    }
    case NodeType::VarDeclaration:
    {
        auto varDeclare = std::static_pointer_cast<VarDeclaration>(astNode);
        return evaluateVarDeclaration(varDeclare, env);
    }
    default:
        std::cerr << "This AST Node has not yet been setup for interpretation: " << astNode->kind;
        exit(1);
    }
}

// std::shared_ptr<RuntimeVal> evaluateBinaryExpr(std::shared_ptr<BinaryExpr> binop, std::shared_ptr<Environment> env)
// {
//     std::shared_ptr<RuntimeVal> lhs = evaluate(binop->left, env);
//     std::shared_ptr<RuntimeVal> rhs = evaluate(binop->right, env);

//     if (rhs->_type == ValueType::Number && lhs->_type == ValueType::Number)
//     {
//         auto lhsNum = std::static_pointer_cast<NumberVal>(lhs);
//         auto rhsNum = std::static_pointer_cast<NumberVal>(rhs);
//         return evaluateNumericBinaryExpr(lhsNum, rhsNum, binop->op);
//     }

//     return mkNull();
// }

// std::shared_ptr<NumberVal> evaluateNumericBinaryExpr(std::shared_ptr<NumberVal> lhs, std::shared_ptr<NumberVal> rhs, const std::string &op)
// {
//     double result = 0;
//     if (op == "+")
//     {
//         result = lhs->val + rhs->val;
//     }
//     else if (op == "-")
//     {
//         result = lhs->val - rhs->val;
//     }
//     else if (op == "*")
//     {
//         result = lhs->val * rhs->val;
//     }
//     else if (op == "/")
//     {
//         result = lhs->val / rhs->val;
//     }
//     else
//     {
//         result = std::fmod(lhs->val, rhs->val);
//     }

//     return std::make_shared<NumberVal>(result);
// }

// std::shared_ptr<RuntimeVal> evaluateIdentifier(std::shared_ptr<Identifier> ident, std::shared_ptr<Environment> env)
// {
//     auto val = env->lookUp(ident->symbol);
//     return val;
// }

// std::shared_ptr<RuntimeVal> evaluateVarDeclaration(std::shared_ptr<Identifier> ident, std::shared_ptr<Environment> env)
// {
// }
