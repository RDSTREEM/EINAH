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
        return parsePrintStatement();

    case TokenType::Whisper:
        return parseConditionalStatement();

    case TokenType::Twirl:
        return parseWhileLoop();

    case TokenType::Skip:
        eat();
        expect(TokenType::Tilde, "Expected '~' after skip statement");
        return std::make_shared<SkipStatement>();

    case TokenType::Shatter:
        eat();
        expect(TokenType::Tilde, "Expected '~' after shatter statement");
        return std::make_shared<ShatterStatement>();

    case TokenType::Conjure:
        return parseFunctionDeclaration();

    default:
        return parseExprStatement();
    }
}

std::shared_ptr<Stmt> Parser::parsePrintStatement()
{
    eat();
    auto arg = parseExpr();
    expect(TokenType::Tilde, "Expected tilde (~) after spit statement");
    auto stmt = std::make_shared<PrintStatement>();
    stmt->argument = arg;
    return stmt;
}

std::shared_ptr<Stmt> Parser::parseConditionalStatement()
{
    eat();
    expect(TokenType::AngleOpen, "Expected '<<' before condition in whisper statement");
    auto cond = parseExpr();
    expect(TokenType::AngleClose, "Expected '>>' after condition in whisper statement");
    expect(TokenType::Then, "Expected 'then' after condition in whisper statement");
    expect(TokenType::OpenBracket, "Expected '[' to start then block");
    std::vector<std::shared_ptr<Stmt>> thenBlock;
    while (at().type != TokenType::CloseBracket)
    {
        thenBlock.push_back(parseStmt());
    }
    expect(TokenType::CloseBracket, "Expected ']' after then block");
    std::vector<std::pair<std::shared_ptr<Expr>, std::vector<std::shared_ptr<Stmt>>>> elifs;
    std::vector<std::shared_ptr<Stmt>> elseBlock;
    while (at().type == TokenType::OrKeyword)
    {
        eat();
        if (at().type == TokenType::OpenBracket)
        {
            eat();
            while (at().type != TokenType::CloseBracket)
            {
                elseBlock.push_back(parseStmt());
            }
            expect(TokenType::CloseBracket, "Expected ']' after else block");
            break;
        }
        else
        {
            expect(TokenType::AngleOpen, "Expected '<<' before elif condition");
            auto elifCond = parseExpr();
            expect(TokenType::AngleClose, "Expected '>>' after elif condition");
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
    while (at().type == TokenType::OrKeyword)
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
    std::shared_ptr<Expr> expr;
    switch (tk)
    {
    case TokenType::Identifier:
    {
        Token token = eat();
        auto ident = std::make_shared<Identifier>();
        ident->symbol = token.value;
        expr = ident;
        break;
    }
    case TokenType::Number:
    {
        Token token = eat();
        auto num = std::make_shared<NumericLiteral>(std::stod(token.value));
        expr = num;
        break;
    }
    case TokenType::Boolean:
    {
        Token token = eat();
        auto boolean = std::make_shared<BooleanLiteral>(token.value == "yup");
        expr = boolean;
        break;
    }
    case TokenType::Null:
    {
        eat();
        auto null = std::make_shared<NullLiteral>();
        expr = null;
        break;
    }
    case TokenType::OpenParen:
    {
        eat();
        expr = parseExpr();
        expect(TokenType::CloseParen, "Expected Closing Parenthesis");
        break;
    }
    case TokenType::String:
    {
        Token token = eat();
        auto str = std::make_shared<StringLiteral>(token.value);
        expr = str;
        break;
    }
    case TokenType::Dollar:
    {
        eat();
        std::vector<std::shared_ptr<Expr>> elements;
        if (at().type != TokenType::Dollar)
        {
            while (true)
            {
                elements.push_back(parseExpr());
                if (at().type == TokenType::Comma)
                {
                    eat();
                }
                else
                {
                    break;
                }
            }
        }
        expect(TokenType::Dollar, "Expected closing '$' for array literal");
        auto arr = std::make_shared<ArrayLiteral>(elements);
        expr = arr;
        break;
    }
    default:
        std::cerr << "Unexpected token found during parsing: " << at();
        exit(-1);
    }
    while (at().type == TokenType::Dot)
    {
        eat();
        if (at().type == TokenType::Number)
        {
            auto idx = parsePrimaryExpr();
            auto indexExpr = std::make_shared<IndexExpr>();
            indexExpr->array = expr;
            indexExpr->index = idx;
            expr = indexExpr;
        }
        else
        {
            std::cerr << "Expected number after dot for array indexing" << std::endl;
            exit(1);
        }
    }
    if (at().type == TokenType::Pipe)
    {
        expr = parseCallExpr(expr);
    }
    return expr;
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

std::shared_ptr<Stmt> Parser::parseWhileLoop()
{
    eat();
    std::shared_ptr<Expr> condition;
    if (at().type == TokenType::Identifier && at().value == "forever")
    {
        eat();
        condition = std::make_shared<BooleanLiteral>(true);
    }
    else
    {
        expect(TokenType::AngleOpen, "Expected '<<' before while loop condition");
        condition = parseExpr();
        expect(TokenType::AngleClose, "Expected '>>' after while loop condition");
    }
    expect(TokenType::Spin, "Expected 'spin' after while condition");
    expect(TokenType::OpenBracket, "Expected '[' to start while loop body");
    std::vector<std::shared_ptr<Stmt>> body;
    while (at().type != TokenType::CloseBracket)
    {
        body.push_back(parseStmt());
    }
    expect(TokenType::CloseBracket, "Expected ']' after while loop body");
    expect(TokenType::Tilde, "Expected '~' after while loop");
    auto loop = std::make_shared<WhileLoop>();
    loop->condition = condition;
    loop->body = body;
    return loop;
}

std::shared_ptr<Stmt> Parser::parseFunctionDeclaration()
{
    eat();
    const Token name = expect(TokenType::Identifier, "Expected function name after 'conjure'.");
    expect(TokenType::AngleOpen, "Expected '<<' before parameter list in function definition.");
    std::vector<std::string> params;
    while (at().type != TokenType::AngleClose)
    {
        if (at().type == TokenType::Identifier)
        {
            params.push_back(eat().value);
            if (at().type == TokenType::Comma)
                eat();
        }
        else
        {
            throw std::runtime_error("Expected parameter name in function definition.");
        }
    }
    expect(TokenType::AngleClose, "Expected '>>' after parameter list in function definition.");
    expect(TokenType::OpenBracket, "Expected '[' to start function body.");
    std::vector<std::shared_ptr<Stmt>> body;
    while (at().type != TokenType::CloseBracket)
    {
        body.push_back(parseStmt());
    }
    expect(TokenType::CloseBracket, "Expected ']' to end function body.");
    expect(TokenType::Tilde, "Expected '~' after function definition.");
    auto func = std::make_shared<FunctionDeclaration>();
    func->name = name.value;
    func->params = params;
    func->body = body;
    return func;
}

std::shared_ptr<Expr> Parser::parseCallExpr(std::shared_ptr<Expr> callee)
{
    auto call = std::make_shared<CallExpr>();
    call->callee = callee;
    eat();
    std::vector<std::shared_ptr<Expr>> args;
    while (at().type != TokenType::Tilde)
    {
        args.push_back(parseExpr());
        if (at().type == TokenType::Comma)
            eat();
    }
    call->arguments = args;
    return call;
}