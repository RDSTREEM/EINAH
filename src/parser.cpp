#include "parser.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    os << "Token(Type: " << static_cast<int>(token.type) << ", Value: \"" << token.value << "\")";
    return os;
}

bool Parser::notEOF()
{
    return !(at().type == TokenType::EOFTk);
}

std::shared_ptr<Program> Parser::produceAST(const std::string &sourceCode)
{
    tokens = tokenize(sourceCode);
    auto program = std::make_shared<Program>();
    program->kind = NodeType::Program;

    while (notEOF())
    {
        program->body.push_back(parseStmt());
    }

    return program;
}

std::shared_ptr<Stmt> Parser::parseStmt()
{
    return parseExpr();
}

std::shared_ptr<Expr> Parser::parseExpr()
{
    return parsePrimaryExpr();
}

std::shared_ptr<Expr> Parser::parsePrimaryExpr()
{
    TokenType tk = at().type;

    switch (tk)
    {
    case TokenType::Identifier:
    {

        Token token = eat();
        auto ident = std::make_shared<Identifier>();
        ident->kind = NodeType::Identifier;
        ident->symbol = token.value;
        return ident;
    };
    case TokenType::Number:
    {

        Token token = eat();
        auto ident = std::make_shared<NumericLiteral>();
        ident->kind = NodeType::NumericLiteral;
        ident->value = std::stod(token.value);
        return ident;
    }

    default:
        std::cerr << "Unexpected token token found during parsing: " << at();
        exit(-1);
    }
}
Token Parser::at()
{
    if (tokens.empty())
        return Token{TokenType::EOFTk, ""};
    return tokens[0];
}

Token Parser::eat()
{
    if (tokens.empty())
        return Token{TokenType::EOFTk, ""};
    Token prev = tokens[0];
    tokens.erase(tokens.begin());
    return prev;
}