#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <unordered_map>
#include <utils.h>

enum class TokenType
{
    Identifier,
    NumericLiteral,
    Equals,
    OpenParen,
    CloseParen,
    BinaryOperator,
    Let,
    EOFTk,
};

struct Token
{
    TokenType type;
    std::string value;
};

std::unordered_map<std::string, TokenType> KEYWORDS = {{"let", TokenType::Let}};

std::vector<Token> tokenize(std::string sourceCode);

Token createToken(TokenType type, std::string value);

#endif