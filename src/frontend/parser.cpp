#include "frontend/parser.h"
#include <iostream>
#include <third_party/magic_enum.hpp>

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    os << "Token(Type: " << magic_enum::enum_name(token.type) << ", Value: \"" << token.value << "\")";
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
    switch (at().type)
    {
    case TokenType::Sprout:
    case TokenType::Root:
        return parseVarDeclaration();

    case TokenType::Spit:
    {
        eat();
        auto arg = parseExpr();
        expect(TokenType::Tilde, "Expected tilde (~) after spit statement");
        auto stmt = std::make_shared<PrintStatement>();
        stmt->argument = arg;
        return stmt;
    }
    case TokenType::Whisper:
    {
        eat();
        auto cond = parseExpr();
        expect(TokenType::Then, "Expected 'then' after condition in whisper statement");
        expect(TokenType::OpenBracket, "Expected '[' to start then block");
        std::vector<std::shared_ptr<Stmt>> thenBlock;
        while (at().type != TokenType::CloseBracket)
        {
            thenBlock.push_back(parseStmt());
        }
        expect(TokenType::CloseBracket, "Expected ']' after then block");
        std::vector<std::pair<std::shared_ptr<Expr>, std::vector<std::shared_ptr<Stmt>>>> elifs;
        while (at().type == TokenType::Or)
        {
            eat();
            if (at().type == TokenType::OpenBracket)
            {
                std::vector<std::shared_ptr<Stmt>> elseBlock;
                while (at().type != TokenType::CloseBracket)
                {
                    elseBlock.push_back(parseStmt());
                }
                expect(TokenType::CloseBracket, "Expected ']' after else block");
                expect(TokenType::Tilde, "Expected '~' after conditional statement");
                auto stmt = std::make_shared<ConditionalStatement>();
                stmt->condition = cond;
                stmt->thenBlock = thenBlock;
                stmt->elseBlock = elseBlock;
                return stmt;
            }
            else
            {
                auto elifCond = parseExpr();
                expect(TokenType::Then, "Expected 'then' after elif condition");
                expect(TokenType::OpenBracket, "Expected '[' to start elif block");
                std::vector<std::shared_ptr<Stmt>> elifBlock;
                while (at().type != TokenType::CloseBracket)
                {
                    elifBlock.push_back(parseStmt());
                }
                expect(TokenType::CloseBracket, "Expected ']' after elif block");
                elifs.emplace_back(elifCond, elifBlock);
            }
        }
        std::vector<std::shared_ptr<Stmt>> elseBlock;
        if (at().type == TokenType::Or)
        {
            eat();
            expect(TokenType::OpenBracket, "Expected '[' to start else block");
            while (at().type != TokenType::CloseBracket)
            {
                elseBlock.push_back(parseStmt());
            }
            expect(TokenType::CloseBracket, "Expected ']' after else block");
        }
        expect(TokenType::Tilde, "Expected '~' after conditional statement");
        std::shared_ptr<ConditionalStatement> current = std::make_shared<ConditionalStatement>();
        current->condition = cond;
        current->thenBlock = thenBlock;
        for (auto it = elifs.rbegin(); it != elifs.rend(); ++it)
        {
            auto elif = std::make_shared<ConditionalStatement>();
            elif->condition = it->first;
            elif->thenBlock = it->second;
            elif->elseBlock = elseBlock;
            elseBlock = {elif};
        }
        current->elseBlock = elseBlock;
        return current;
    }

    default:
        return parseExprStatement();
    }
}

std::shared_ptr<Stmt> Parser::parseExprStatement()
{
    auto expr = parseExpr();
    expect(TokenType::Tilde, "Expected tilde (~) after statement expression");
    auto stmt = std::make_shared<ExprStatement>();
    stmt->expr = expr;
    return stmt;
}

std::shared_ptr<Stmt> Parser::parseVarDeclaration()
{
    const bool isConstant = (eat().type == TokenType::Root);
    const Token ident = expect(TokenType::Identifier, "Expected identifier name following sprout|root keywords.");

    // If no assignment, must not be a constant
    if (at().type == TokenType::Tilde)
    {
        eat();
        if (isConstant)
            throw std::runtime_error("Must assign value to a constant expression. No value provided.");
        auto varDeclare = std::make_shared<VarDeclaration>();
        varDeclare->ident = ident.value;
        varDeclare->constant = false;
        return varDeclare;
    }

    expect(TokenType::Arrow, "Expected arrow token (->) after identifier in variable declaration");
    auto varDeclare = std::make_shared<VarDeclaration>();
    varDeclare->value = parseExpr();
    varDeclare->constant = isConstant;
    varDeclare->ident = ident.value;
    expect(TokenType::Tilde, "Variable declaration statement must end in a tilde (~).");
    return varDeclare;
}

// Parse an expression (assignment or lower precedence)
std::shared_ptr<Expr> Parser::parseExpr()
{
    return parseAssignmentExpr();
}

std::shared_ptr<Expr> Parser::parseLogicalOrExpr()
{
    auto left = parseLogicalAndExpr();
    while (at().type == TokenType::Or)
    {
        eat();
        auto right = parseLogicalAndExpr();
        auto binop = std::make_shared<BinaryExpr>();
        binop->left = left;
        binop->right = right;
        binop->op = "|=";
        left = binop;
    }
    return left;
}

std::shared_ptr<Expr> Parser::parseLogicalAndExpr()
{
    auto left = parseEqualityExpr();
    while (at().type == TokenType::And)
    {
        eat();
        auto right = parseEqualityExpr();
        auto binop = std::make_shared<BinaryExpr>();
        binop->left = left;
        binop->right = right;
        binop->op = "&=";
        left = binop;
    }
    return left;
}

std::shared_ptr<Expr> Parser::parseRelationalExpr()
{
    auto left = parseAdditiveExpr();
    while (
        at().type == TokenType::Less ||
        at().type == TokenType::Greater ||
        at().type == TokenType::LessEq ||
        at().type == TokenType::GreaterEq)
    {
        std::string op;
        if (at().type == TokenType::Less)
            op = "<";
        else if (at().type == TokenType::Greater)
            op = ">";
        else if (at().type == TokenType::LessEq)
            op = "<~";
        else
            op = ">~";
        eat();
        auto right = parseAdditiveExpr();
        auto binop = std::make_shared<BinaryExpr>();
        binop->left = left;
        binop->right = right;
        binop->op = op;
        left = binop;
    }
    return left;
}

std::shared_ptr<Expr> Parser::parseEqualityExpr()
{
    auto left = parseRelationalExpr();
    while (at().type == TokenType::Eq || at().type == TokenType::Neq)
    {
        std::string op = (at().type == TokenType::Eq) ? "~~" : "!~";
        eat();
        auto right = parseRelationalExpr();
        auto binop = std::make_shared<BinaryExpr>();
        binop->left = left;
        binop->right = right;
        binop->op = op;
        left = binop;
    }
    return left;
}

std::shared_ptr<Expr> Parser::parseNotExpr()
{
    if (at().type == TokenType::Not)
    {
        eat();
        auto expr = parseNotExpr();
        auto binop = std::make_shared<BinaryExpr>();
        binop->left = nullptr;
        binop->right = expr;
        binop->op = "~!";
        return binop;
    }
    return parsePrimaryExpr();
}

std::shared_ptr<Expr> Parser::parseMultiplicativeExpr()
{
    auto left = parseNotExpr();
    while (
        at().type == TokenType::Slash ||
        at().type == TokenType::Star ||
        at().type == TokenType::Percent)
    {
        std::string op;
        if (at().type == TokenType::Slash)
            op = "/";
        else if (at().type == TokenType::Star)
            op = "*";
        else
            op = "%";
        eat();
        auto right = parseNotExpr();
        auto binop = std::make_shared<BinaryExpr>();
        binop->right = right;
        binop->left = left;
        binop->op = op;
        left = binop;
    }
    return left;
}

std::shared_ptr<Expr> Parser::parseAdditiveExpr()
{
    auto left = parseMultiplicativeExpr();
    while (
        at().type == TokenType::Plus ||
        at().type == TokenType::Minus)
    {
        std::string op = (at().type == TokenType::Plus) ? "+" : "-";
        eat();
        auto right = parseMultiplicativeExpr();
        auto binop = std::make_shared<BinaryExpr>();
        binop->right = right;
        binop->left = left;
        binop->op = op;
        left = binop;
    }
    return left;
}

std::shared_ptr<Expr> Parser::parseAssignmentExpr()
{
    auto left = parseLogicalOrExpr();
    if (at().type == TokenType::Arrow)
    {
        eat();
        auto right = parseAssignmentExpr();
        auto assign = std::make_shared<AssignmentExpr>();
        assign->asignee = left;
        assign->value = right;
        return assign;
    }
    return left;
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
        ident->symbol = token.value;
        return ident;
    }
    case TokenType::Number:
    {
        Token token = eat();
        auto num = std::make_shared<NumericLiteral>(std::stod(token.value));
        return num;
    }
    case TokenType::Boolean:
    {
        Token token = eat();
        auto boolean = std::make_shared<BooleanLiteral>(token.value == "yup");
        return boolean;
    }
    case TokenType::Null:
    {
        eat();
        auto null = std::make_shared<NullLiteral>();
        return null;
    }
    case TokenType::OpenParen:
    {
        eat();
        auto value = parseExpr();
        expect(TokenType::CloseParen, "Expected Closing Parenthesis");
        return value;
    }
    default:
        std::cerr << "Unexpected token found during parsing: " << at();
        exit(-1);
    }
    return nullptr;
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

Token Parser::expect(TokenType type_, const std::string &err)
{
    if (tokens.empty())
        return Token{TokenType::EOFTk, ""};
    Token prev = tokens[0];
    tokens.erase(tokens.begin());
    if (prev.type != type_)
    {
        std::cerr << "Parser Error:" << std::endl
                  << err << std::endl
                  << prev << "- Expecting: " << magic_enum::enum_name(type_) << std::endl;
        exit(1);
    }
    return prev;
}