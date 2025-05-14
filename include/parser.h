#ifndef PARSER_H
#define PARSER_H

#include "lexer.h" // Ensure this includes lexer.h correctly.
#include "ast.h"

class Parser
{
private:
    std::vector<Token> tokens;
    int current;

    bool notEOF();
    Token at();
    Token eat();
    Token expect(TokenType type, const std::string &errorMessage);

    Expr *parseAdditiveExpr();
    Expr *parseMultiplicativeExpr();
    Expr *parsePrimaryExpr();

public:
    Parser(const std::vector<Token> &toks);
    Expr *parseExpression();
};

#endif