#include "lexer.h"
#include <iostream>
#include <cstdlib>

std::vector<Token> tokenize(std::string sourceCode)
{
    std::vector<Token> tokens;
    std::vector<char> src(sourceCode.begin(), sourceCode.end());

    // Weird way to back trackingly get tokens
    while (!src.empty())
    {
        if (src.front() == '(')
        {
            tokens.push_back(createToken(TokenType::OpenParen, std::string(1, src.front())));
            src.erase(src.begin()); // Just like pop but with no return
        }
        else if (src.front() == ')')
        {
            tokens.push_back(createToken(TokenType::CloseParen, std::string(1, src.front())));
            src.erase(src.begin());
        }
        else if (src.front() == '+' || src.front() == '-' || src.front() == '*' || src.front() == '/' || src.front() == '%')
        {
            tokens.push_back(createToken(TokenType::BinaryOperator, std::string(1, src.front())));
            src.erase(src.begin());
        }
        else if (src.front() == '=')
        {
            tokens.push_back(createToken(TokenType::Equals, std::string(1, src.front())));
            src.erase(src.begin());
        }
        else
        {
            if (std::isdigit(src.front()))
            {
                std::string num = "";
                while (!src.empty() && std::isdigit(src.front()))
                {
                    num.append(std::string(1, src.front()));
                    src.erase(src.begin());
                }
                tokens.push_back(createToken(TokenType::Number, num));
            }
            else if (std::isalpha(src.front()))
            {
                std::string ident = "";
                while (!src.empty() && std::isalpha(src.front()))
                {
                    ident += src.front();
                    src.erase(src.begin());
                }

                auto reserved = KEYWORDS.find(ident);
                if (reserved == KEYWORDS.end())
                {
                    tokens.push_back(createToken(TokenType::Identifier, ident));
                }
                else
                {
                    tokens.push_back(createToken(reserved->second, ident));
                }
            }
            else if (isSkippable(src.front()))
            {
                src.erase(src.begin());
            }
            else
            {
                std::cerr << "Unreconized character found in source: " << src.front();
                exit(1);
            }
        }
    }

    tokens.push_back(createToken(TokenType::EOFTk, ""));
    return tokens;
}

Token createToken(TokenType type, std::string value)
{
    return {type, value};
}
