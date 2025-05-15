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

    std::shared_ptr<Stmt> parseStmt();
    std::shared_ptr<Expr> parseExpr();
    std::shared_ptr<Expr> parsePrimaryExpr();

public:
    std::shared_ptr<Program> produceAST(const std::string &sourceCode);
};

#endif