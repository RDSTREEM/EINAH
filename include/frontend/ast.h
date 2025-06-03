#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <memory>

/**
 * @brief AST node types for the Einah language.
 */
enum class NodeType
{
    Program,        // Top-level program node
    VarDeclaration, // Variable declaration (sprout/root)
    AssignmentExpr, // Assignment (a -> b)
    PrintStatement, // Print statement (spit)
    BinaryExpr,     // Binary operation (e.g., +, -, ~~, !~)
    Identifier,     // Variable or symbol
    NumericLiteral, // Number literal
    BooleanLiteral, // Boolean literal (yup/nope)
    NullLiteral,    // Null literal (zip)
    ExprStatement   // Expression statement (expr ~)
};

/**
 * @brief Base statement node.
 */
struct Stmt
{
    NodeType kind; // The type of the AST node.
};

/**
 * @brief Base expression node (inherits from Stmt).
 */
struct Expr : Stmt
{
};

/**
 * @brief Program node: contains a list of statements.
 */
struct Program : Stmt
{
    std::vector<std::shared_ptr<Stmt>> body; // List of statements in the program.
    Program() { kind = NodeType::Program; }
};

/**
 * @brief Variable declaration node (sprout/root).
 */
struct VarDeclaration : Stmt
{
    bool constant;               // true for root (const), false for sprout (let)
    std::string ident;           // Variable name
    std::shared_ptr<Expr> value; // Initial value (may be null for uninitialized sprout)
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