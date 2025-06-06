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
    FunctionDeclaration,  // Named function definition (conjure name <<params>> [ ... ]~)
    CallExpr,             // Function call (name | args~)
    ReturnStatement,      // Return statement (zipback value~)
    BlockStatement,       // Add this line for block node
    ObjectLiteral,        // Object literal node
    ObjectAccess,         // Object property access node
    ForLoop,              // For loop node (cartwheel)
    ForEachLoop           // For-each loop node (drift)
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

struct FunctionDeclaration : Stmt
{
    std::string name;
    std::vector<std::string> params;
    std::vector<std::shared_ptr<Stmt>> body;
    FunctionDeclaration() { kind = NodeType::FunctionDeclaration; }
};

struct CallExpr : Expr
{
    std::shared_ptr<Expr> callee;
    std::vector<std::shared_ptr<Expr>> arguments;
    CallExpr() { kind = NodeType::CallExpr; }
};

struct ReturnStatement : Stmt
{
    std::shared_ptr<Expr> argument;
    ReturnStatement() { kind = NodeType::ReturnStatement; }
};

struct BlockStatement : Stmt
{
    std::vector<std::shared_ptr<Stmt>> body;
    BlockStatement(const std::vector<std::shared_ptr<Stmt>> &b) : body(b) { kind = NodeType::BlockStatement; }
};

struct ObjectLiteral : Expr
{
    std::vector<std::pair<std::string, std::shared_ptr<Expr>>> properties;
    ObjectLiteral(const std::vector<std::pair<std::string, std::shared_ptr<Expr>>> &props) : properties(props) { kind = NodeType::ObjectLiteral; }
};
struct ObjectAccess : Expr
{
    std::shared_ptr<Expr> object;
    std::shared_ptr<Expr> key;
    ObjectAccess() { kind = NodeType::ObjectAccess; }
};
struct ForLoop : Stmt
{
    std::string iterator;
    std::shared_ptr<Expr> start;
    std::shared_ptr<Expr> end;
    std::shared_ptr<Expr> step;
    std::vector<std::shared_ptr<Stmt>> body;
    ForLoop() { kind = NodeType::ForLoop; }
};

struct ForEachLoop : Stmt
{
    std::string iterator;
    std::shared_ptr<Expr> iterable;
    std::vector<std::shared_ptr<Stmt>> body;
    ForEachLoop() { kind = NodeType::ForEachLoop; }
};

#endif