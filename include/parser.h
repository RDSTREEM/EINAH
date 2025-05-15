#ifndef PARSER_H
#define PARSER_H

#include "lexer.h" // Ensure this includes lexer.h correctly.
#include "ast.h"
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
    std::shared_ptr<Expr> parseExpr();
    std::shared_ptr<Expr> parsePrimaryExpr();
    std::shared_ptr<Expr> parseAdditiveExpr();
    std::shared_ptr<Expr> parseMultiplicativeExpr();

public:
    std::shared_ptr<Program> produceAST(const std::string &sourceCode);
};

#endif