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
    AssignmentExpr,

    // ? Expressions
    BinaryExpr,
    Identifier,
    NumericLiteral,

    // ? Compiler Only
    ExprStatement
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

struct AssignmentExpr : Expr
{
    std::shared_ptr<Expr> asignee;
    std::shared_ptr<Expr> value;
    AssignmentExpr() { kind = NodeType::AssignmentExpr; }
};

struct ExprStatement : Stmt
{
    std::shared_ptr<Expr> expr;
    ExprStatement() { kind = NodeType::ExprStatement; }
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