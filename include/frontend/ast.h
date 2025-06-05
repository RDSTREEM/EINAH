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
    Program,              // Top-level program node
    VarDeclaration,       // Variable declaration (sprout/root)
    AssignmentExpr,       // Assignment (a -> b)
    PrintStatement,       // Print statement (spit)
    BinaryExpr,           // Binary operation (e.g., +, -, ~~, !~)
    Identifier,           // Variable or symbol
    NumericLiteral,       // Number literal
    BooleanLiteral,       // Boolean literal (yup/nope)
    NullLiteral,          // Null literal (zip)
    ExprStatement,        // Expression statement (expr ~)
    UnaryExpr,            // Unary operation (e.g., ~!)
    ConditionalStatement, // Conditional (whisper ... then ... or ...~)
    StringLiteral,        // String literal
    WhileLoop,            // While loop node
    Skip,                 // Continue statement
    Shatter,              // Break statement
    ArrayLiteral,         // Array literal node
    IndexExpr,            // Array index expression node
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

struct UnaryExpr : Expr
{
    std::string op;                 // The unary operator (e.g., ~!)
    std::shared_ptr<Expr> argument; // The operand expression
    UnaryExpr() { kind = NodeType::UnaryExpr; }
};

struct ConditionalStatement : Stmt
{
    std::shared_ptr<Expr> condition;
    std::vector<std::shared_ptr<Stmt>> thenBlock;
    std::vector<std::shared_ptr<Stmt>> elseBlock;
    ConditionalStatement() { kind = NodeType::ConditionalStatement; }
};

struct StringLiteral : Expr
{
    std::string value;
    StringLiteral(const std::string &v) : value(v) { kind = NodeType::StringLiteral; }
};

struct WhileLoop : Stmt
{
    std::shared_ptr<Expr> condition;
    std::vector<std::shared_ptr<Stmt>> body;
    WhileLoop() { kind = NodeType::WhileLoop; }
};

struct SkipStatement : Stmt
{
    SkipStatement() { kind = NodeType::Skip; }
};

struct ShatterStatement : Stmt
{
    ShatterStatement() { kind = NodeType::Shatter; }
};

struct ArrayLiteral : Expr
{
    std::vector<std::shared_ptr<Expr>> elements;
    ArrayLiteral(const std::vector<std::shared_ptr<Expr>> &elems) : elements(elems) { kind = NodeType::ArrayLiteral; }
};

struct IndexExpr : Expr
{
    std::shared_ptr<Expr> array;
    std::shared_ptr<Expr> index;
    IndexExpr() { kind = NodeType::IndexExpr; }
};

#endif