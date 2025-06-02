#ifndef PARSER_H
#define PARSER_H

#include "frontend/lexer.h"
#include "frontend/ast.h"
#include <cstdlib>
#include <memory>

class Parser
{
private:
    std::vector<Token> tokens;
    bool notEOF();

    Token at();
    Token eat();
    Token expect(TokenType type_, const std::string &err);

    std::shared_ptr<Stmt> parseStmt();
    std::shared_ptr<Stmt> parseVarDeclaration();
    std::shared_ptr<Stmt> parseExprStatement();
    std::shared_ptr<Expr> parseAssignmentExpr();
    std::shared_ptr<Expr> parseExpr();
    std::shared_ptr<Expr> parsePrimaryExpr();
    std::shared_ptr<Expr> parseAdditiveExpr();
    std::shared_ptr<Expr> parseMultiplicativeExpr();
    std::shared_ptr<Expr> parseLogicalOrExpr();
    std::shared_ptr<Expr> parseLogicalAndExpr();
    std::shared_ptr<Expr> parseEqualityExpr();
    std::shared_ptr<Expr> parseNotExpr();

public:
    std::shared_ptr<Program> produceAST(const std::string &sourceCode);
};

#endif