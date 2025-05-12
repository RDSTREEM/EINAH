#include "lexer.h"

// Ensure the function implementation matches the declaration in the header file

std::vector<Token> tokenize(std::string sourceCode)
{
    std::vector<Token> tokens;
    std::vector<char> src(sourceCode.begin(), sourceCode.end());

    // Weird way to back trackingly get tokens
    while (!src.empty())
    {
        char current = src.front();
        if (current == '(')
        {
            tokens.push_back(createToken(TokenType::OpenParen, std::string(1, current)));
            src.erase(src.begin()); // Just like pop but with no return
        }
        else if (current == ')')
        {
            tokens.push_back(createToken(TokenType::CloseParen, std::string(1, current)));
            src.erase(src.begin());
        }
        else if (current == '+' || current == '-' || current == '*' || current == '/')
        {
            tokens.push_back(createToken(TokenType::BinaryOperator, std::string(1, current)));
            src.erase(src.begin());
        }
        else if (current == '=')
        {
            tokens.push_back(createToken(TokenType::Equals, std::string(1, current)));
            src.erase(src.begin());
        }
        else
        {
            if (std::isdigit(current))
            {
                std::string num = "";
                while (std::isdigit(current) && !src.empty())
                {
                    num.append(std::string(1, current));
                    src.erase(src.begin());
                }
                tokens.push_back(createToken(TokenType::NumericLiteral, num));
            }
            else if (std::isalpha(current) && !src.empty())
            {
                std::string ident = "";
                while (std::isalpha(current))
                {
                    ident.append(std::string(1, current));
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
            else
            {
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
