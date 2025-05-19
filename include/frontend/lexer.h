#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <unordered_map>
#include <utils.h>

enum class TokenType
{
    Semicolon,
    Identifier,
    Number,
    Equals,
    OpenParen,
    CloseParen,
    BinaryOperator,
    // Null
    Let,
    Const,
    EOFTk,
};

struct Token
{
    TokenType type;
    std::string value;
};

const inline std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"let", TokenType::Let},
    {"const", TokenType::Const},
};

std::vector<Token> tokenize(const std::string &sourceCode);

Token createToken(TokenType type, const std::string &value);

#endif