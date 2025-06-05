#ifndef PARSER_H
#define PARSER_H

#include "frontend/lexer.h"
#include "frontend/ast.h"
#include <cstdlib>
#include <memory>
#include <vector>

/**
 * @brief The Parser class is responsible for converting a list of tokens into an AST (Abstract Syntax Tree).
 *
 * The parser processes tokens produced by the lexer and builds a tree of AST nodes representing the program structure.
 */
class Parser
{
private:
    std::vector<Token> tokens; // List of tokens to parse

    /**
     * @brief Check if the parser has not reached the end of the token stream.
     * @return true if not at end of file, false otherwise.
     */
    bool notEOF();

    /**
     * @brief Get the current token without consuming it.
     * @return The current Token.
     */
    Token at();
    /**
     * @brief Consume and return the current token.
     * @return The consumed Token.
     */
    Token eat();
    /**
     * @brief Expect a specific token type, or throw an error if not found.
     * @param type_ The expected token type.
     * @param err The error message if the token does not match.
     * @return The consumed Token.
     */
    Token expect(TokenType type_, const std::string &err);

    /**
     * @brief Parse a statement (variable declaration, print, or expression statement).
     * @return The parsed statement node.
     */
    std::shared_ptr<Stmt> parseStmt();
    /**
     * @brief Parse a variable declaration (sprout/root).
     * @return The parsed variable declaration node.
     */
    std::shared_ptr<Stmt> parseVarDeclaration();
    /**
     * @brief Parse an expression statement (expr ~).
     * @return The parsed expression statement node.
     */
    std::shared_ptr<Stmt> parseExprStatement();
    /**
     * @brief Parse an assignment expression (a -> b).
     * @return The parsed assignment expression node.
     */
    std::shared_ptr<Expr> parseAssignmentExpr();
    /**
     * @brief Parse any expression.
     * @return The parsed expression node.
     */
    std::shared_ptr<Expr> parseExpr();
    /**
     * @brief Parse a primary expression (identifier, literal, parens).
     * @return The parsed primary expression node.
     */
    std::shared_ptr<Expr> parsePrimaryExpr();
    /**
     * @brief Parse additive expressions (+, -).
     * @return The parsed additive expression node.
     */
    std::shared_ptr<Expr> parseAdditiveExpr();
    /**
     * @brief Parse multiplicative expressions (*, /, %).
     * @return The parsed multiplicative expression node.
     */
    std::shared_ptr<Expr> parseMultiplicativeExpr();
    /**
     * @brief Parse logical OR expressions (|=).
     * @return The parsed logical OR expression node.
     */
    std::shared_ptr<Expr> parseLogicalOrExpr();
    /**
     * @brief Parse logical AND expressions (&=).
     * @return The parsed logical AND expression node.
     */
    std::shared_ptr<Expr> parseLogicalAndExpr();
    /**
     * @brief Parse equality expressions (~~, !~).
     * @return The parsed equality expression node.
     */
    std::shared_ptr<Expr> parseEqualityExpr();
    /**
     * @brief Parse NOT expressions (~!).
     * @return The parsed NOT expression node.
     */
    std::shared_ptr<Expr> parseNotExpr();
    /**
     * @brief Parse relational expressions (>, <, >=, <=).
     * @return The parsed relational expression node.
     */
    std::shared_ptr<Expr> parseRelationalExpr();
    /**
     * @brief Parse a print statement (spit ... ~)
     * @return The parsed print statement node.
     */
    std::shared_ptr<Stmt> parsePrintStatement();
    /**
     * @brief Parse a conditional statement (whisper ...)
     * @return The parsed conditional statement node.
     */
    std::shared_ptr<Stmt> parseConditionalStatement();
    /**
     * @brief Parse a while loop (while ... do ... end)
     * @return The parsed while loop node.
     */
    std::shared_ptr<Stmt> parseWhileLoop();
    /**
     * @brief Parse a function declaration (named function).
     * @return The parsed function declaration node.
     */
    std::shared_ptr<Stmt> parseFunctionDeclaration();

public:
    /**
     * @brief Entry point: produce an AST from source code.
     * @param sourceCode The source code to parse.
     * @return std::shared_ptr<Program> The root AST node.
     */
    std::shared_ptr<Program> produceAST(const std::string &sourceCode);
};

#endif