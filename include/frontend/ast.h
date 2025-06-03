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
    PrintStatement, // Add this for 'spit'

    // ? Expressions
    BinaryExpr,
    Identifier,
    NumericLiteral,
    BooleanLiteral, // for true/false
    NullLiteral,    // for null

    // ? Compiler Only
    ExprStatement
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

struct PrintStatement : Stmt
{
    std::shared_ptr<Expr> argument;
    PrintStatement() { kind = NodeType::PrintStatement; }
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
    NumericLiteral(double v) : value(v) { kind = NodeType::NumericLiteral; }
};

struct BooleanLiteral : Expr
{
    bool value;
    BooleanLiteral(bool v) : value(v) { kind = NodeType::BooleanLiteral; }
};

struct NullLiteral : Expr
{
    NullLiteral() { kind = NodeType::NullLiteral; }
};

#endif