#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <memory>

enum class NodeType
{
    // ? Statements
    Program,
    VarDeclaration,

    // ? Expressions
    BinaryExpr,
    Identifier,
    NumericLiteral,
    // NullLiteral
};

struct Stmt
{
    NodeType kind;
};

struct Expr : Stmt
{
};
struct Program : Stmt
{
    std::vector<std::shared_ptr<Stmt>> body;
    Program() { kind = NodeType::Program; }
};

struct VarDeclaration : Stmt
{
    bool constant;
    std::string ident;
    std::shared_ptr<Expr> value;
    VarDeclaration() { kind = NodeType::VarDeclaration; }
};

struct BinaryExpr : Expr
{
    std::shared_ptr<Expr> left;
    std::shared_ptr<Expr> right;
    std::string op;
    BinaryExpr() { kind = NodeType::BinaryExpr; }
};

struct Identifier : Expr
{
    std::string symbol;
    Identifier() { kind = NodeType::Identifier; };
};

struct NumericLiteral : Expr
{
    double value;
    NumericLiteral() { kind = NodeType::NumericLiteral; };
};

// struct NullLiteral : Expr
// {
//     std::string value;
//     NullLiteral()
//     {
//         kind = NodeType::NullLiteral;
//         value = "null";
//     };
// };

#endif