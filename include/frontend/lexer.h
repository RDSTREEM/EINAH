#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <unordered_map>
#include <utils.h>

enum class TokenType
{
    Tilde,     // the terminator substituing for "l"
    Semicolon, // to not have errors
    Identifier,
    Number,
    Equals,
    Arrow, // the equivalent of "="
    OpenParen,
    CloseParen,
    Plus,
    Minus,
    Star,
    Slash,
    Percent,
    Eq,  // ~
    Neq, // "!~"
    And, // and
    Or,  // or
    Not, // not
    Let,
    Const,
    Boolean,
    Null,
    Yeet, // for printing
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
    {"and", TokenType::And},
    {"or", TokenType::Or},
    {"not", TokenType::Not},
};

std::vector<Token> tokenize(const std::string &sourceCode);

Token createToken(TokenType type, const std::string &value);

#endif