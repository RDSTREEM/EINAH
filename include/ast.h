#ifndef AST_H
#define AST_H

#include <vector>
#include <string>

enum NodeType
{
    PROGRAM,
    BINARYEXPR,
    IDENTIFIER,
    NUMERIC_LIT
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
    std::vector<Stmt *> body;
    Program() { kind = PROGRAM; }
};

struct BinaryExpr : Expr
{
    Expr *left;
    Expr *right;
    std::string op;
    BinaryExpr() { kind = BINARYEXPR; }
};

struct Identifier : Expr
{
    std::string symbol;
    Identifier() { kind = IDENTIFIER; };
};

struct NumericLiteral : Expr
{
    double value;
    NumericLiteral() { kind = NUMERIC_LIT; };
};

#endif