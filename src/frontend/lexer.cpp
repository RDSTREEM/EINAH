#include "frontend/lexer.h"
#include <iostream>
#include <cstdlib>
#include <third_party/magic_enum.hpp>

std::vector<Token> tokenize(const std::string &sourceCode)
{
    std::vector<Token> tokens;
    std::vector<char> src(sourceCode.begin(), sourceCode.end());

    // Weird way to back trackingly get tokens
    while (!src.empty())
    {
        if (src.size() >= 2 && src[0] == '~' && src[1] == '~')
        {
            tokens.push_back(createToken(TokenType::Eq, "~~"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.front() == '~')
        {
            tokens.push_back(createToken(TokenType::Tilde, "~"));
            src.erase(src.begin());
        }
        else if (src.size() >= 2 && src[0] == '-' && src[1] == '>')
        {
            tokens.push_back(createToken(TokenType::Arrow, "->"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.front() == '(')
        {
            tokens.push_back(createToken(TokenType::OpenParen, std::string(1, src.front())));
            src.erase(src.begin());
        }
        else if (src.front() == ')')
        {
            tokens.push_back(createToken(TokenType::CloseParen, std::string(1, src.front())));
            src.erase(src.begin());
        }
        else if (src.front() == '+')
        {
            tokens.push_back(createToken(TokenType::Plus, "+"));
            src.erase(src.begin());
        }
        else if (src.front() == '-')
        {
            tokens.push_back(createToken(TokenType::Minus, "-"));
            src.erase(src.begin());
        }
        else if (src.front() == '*')
        {
            tokens.push_back(createToken(TokenType::Star, "*"));
            src.erase(src.begin());
        }
        else if (src.front() == '/')
        {
            tokens.push_back(createToken(TokenType::Slash, "/"));
            src.erase(src.begin());
        }
        else if (src.front() == '%')
        {
            tokens.push_back(createToken(TokenType::Percent, "%"));
            src.erase(src.begin());
        }
        else if (src.size() >= 2 && src[0] == '~' && src[1] == '~')
        {
            tokens.push_back(createToken(TokenType::Eq, "~~"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.size() >= 2 && src[0] == '!' && src[1] == '~')
        {
            tokens.push_back(createToken(TokenType::Neq, "!~"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (std::isalpha(src.front()))
        {
            std::string ident = "";
            while (!src.empty() && std::isalpha(src.front()))
            {
                ident += src.front();
                src.erase(src.begin());
            }
            if (ident == "true" || ident == "false")
            {
                tokens.push_back(createToken(TokenType::Boolean, ident));
            }
            else if (ident == "null")
            {
                tokens.push_back(createToken(TokenType::Null, ident));
            }
            else if (ident == "and")
            {
                tokens.push_back(createToken(TokenType::And, ident));
            }
            else if (ident == "or")
            {
                tokens.push_back(createToken(TokenType::Or, ident));
            }
            else if (ident == "not")
            {
                tokens.push_back(createToken(TokenType::Not, ident));
            }
            else
            {
                auto reserved = KEYWORDS.find(ident);
                if (reserved == KEYWORDS.end())
                    tokens.push_back(createToken(TokenType::Identifier, ident));
                else
                    tokens.push_back(createToken(reserved->second, ident));
            }
        }
        else if (std::isdigit(src.front()))
        {
            std::string number = "";
            bool hasDot = false;
            while (!src.empty() && (std::isdigit(src.front()) || (!hasDot && src.front() == '.')))
            {
                if (src.front() == '.')
                    hasDot = true;
                number += src.front();
                src.erase(src.begin());
            }
            tokens.push_back(createToken(TokenType::Number, number));
        }
        else if (isSkippable(src.front()))
        {
            src.erase(src.begin());
        }
        else
        {
            std::cerr << "Unreconized character found in source: " << src.front() << std::endl;
            exit(1);
        }
    }

    tokens.push_back(createToken(TokenType::EOFTk, ""));
    return tokens;
}

Token createToken(TokenType type, const std::string &value)
{
    return {type, value};
}
