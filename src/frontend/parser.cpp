#include "frontend/parser.h"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    os << "Token(Type: " << static_cast<int>(token.type) << ", Value: \"" << token.value << "\")";
    return os;
}

std::ostream &operator<<(std::ostream &os, TokenType type)
{
    switch (type)
    {
    case TokenType::OpenParen:
        return os << "OpenParen";
    case TokenType::CloseParen:
        return os << "CloseParen";
    case TokenType::BinaryOperator:
        return os << "BinaryOperator";
    case TokenType::Equals:
        return os << "Equals";
    case TokenType::Number:
        return os << "Number";
    case TokenType::Identifier:
        return os << "Identifier";
    case TokenType::EOFTk:
        return os << "EOF";
    default:
        return os << "UnknownTokenType";
    }
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
    switch (at().type)
    {
    case TokenType::Let:
    case TokenType::Const:
        return parseVarDeclaration();

    default:
        return parseExpr();
    }
}

std::shared_ptr<Expr> Parser::parseExpr()
{
    return parseAdditiveExpr();
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
        auto num = std::make_shared<NumericLiteral>();
        num->kind = NodeType::NumericLiteral;
        num->value = std::stod(token.value);
        return num;
    }
    // case TokenType::Null:
    // {
    //     eat();
    //     auto null = std::make_shared<NullLiteral>();
    //     null->kind = NodeType::NullLiteral;
    //     return null;
    // }
    case TokenType::OpenParen:
    {
        eat();
        auto value = parseExpr();
        expect(TokenType::CloseParen, "Expected Closing Parenthesis");
        return value;
    }

    default:
        std::cerr << "Unexpected token token found during parsing: " << at();
        exit(-1);
    }
}

std::shared_ptr<Stmt> Parser::parseVarDeclaration()
{
    const bool isConstant = (eat().type == TokenType::Const);
    const Token ident = expect(TokenType::Identifier, "Expected identifier name following let|c;onst keywords.");

    if (at().type == TokenType::Semicolon)
    {
        eat();
        if (isConstant)
            throw std::runtime_error("Must assign value to a constant expression. No value provided.");
        std::shared_ptr<VarDeclaration> varDeclare = std::make_shared<VarDeclaration>();
        varDeclare->ident = ident.value;
        varDeclare->constant = false;
    }

    expect(TokenType::Equals, "Expected equals token identifier in variable declaration");
    std::shared_ptr<VarDeclaration> varDeclare = std::make_shared<VarDeclaration>();
    varDeclare->value = parseExpr();
    varDeclare->constant = isConstant;
    varDeclare->ident = ident.value;

    expect(TokenType::Semicolon, "Variable declaration statment must end in a semicolon.");
    return varDeclare;
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

std::shared_ptr<Expr> Parser::parseAdditiveExpr()
{
    auto left = parseMultiplicativeExpr();

    while (at().value == "+" || at().value == "-")
    {
        std::string op = eat().value;
        auto right = parseMultiplicativeExpr();
        auto binop = std::make_shared<BinaryExpr>();

        binop->kind = NodeType::BinaryExpr;
        binop->right = right;
        binop->left = left;
        binop->op = op;

        left = binop;
    }

    return left;
}

std::shared_ptr<Expr> Parser::parseMultiplicativeExpr()
{
    auto left = parsePrimaryExpr();

    while (at().value == "/" || at().value == "*" || at().value == "%")
    {
        std::string op = eat().value;
        auto right = parsePrimaryExpr();
        auto binop = std::make_shared<BinaryExpr>();

        binop->kind = NodeType::BinaryExpr;
        binop->right = right;
        binop->left = left;
        binop->op = op;

        left = binop;
    }

    return left;
}

Token Parser::expect(TokenType type_, const std::string &err)
{
    if (tokens.empty())
        return Token{TokenType::EOFTk, ""};
    Token prev = tokens[0];
    tokens.erase(tokens.begin());
    if (prev.type != type_)
    {
        std::cerr << "Parser Error:" << std::endl
                  << err << "" << prev << "- Expecting: " << type_;
        exit(1);
    }

    return prev;
}