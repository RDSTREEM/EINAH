#include "frontend/lexer.h"
#include <iostream>
#include <cstdlib>
#include <third_party/magic_enum.hpp>

std::vector<Token> tokenize(const std::string &sourceCode)
{
    std::vector<Token> tokens;
    std::vector<char> src(sourceCode.begin(), sourceCode.end());

    // Main tokenization loop
    while (!src.empty())
    {
        // Handle multi-character tokens first
        if (src.size() >= 2 && src[0] == '~' && src[1] == '~') // '~~' equality
        {
            tokens.push_back(createToken(TokenType::Eq, "~~"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.size() >= 2 && src[0] == '!' && src[1] == '~') // '!~' not equal
        {
            tokens.push_back(createToken(TokenType::Neq, "!~"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.size() >= 2 && src[0] == '~' && src[1] == '!') // '~!' not
        {
            tokens.push_back(createToken(TokenType::Not, "~!"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.front() == '~') // '~' statement terminator
        {
            tokens.push_back(createToken(TokenType::Tilde, "~"));
            src.erase(src.begin());
        }
        else if (src.size() >= 2 && src[0] == '-' && src[1] == '>') // '->' assignment
        {
            tokens.push_back(createToken(TokenType::Arrow, "->"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.size() >= 2 && src[0] == '=' && src[1] == '>') // '=>' fat arrow
        {
            tokens.push_back(createToken(TokenType::FatArrow, "=>"));
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
        else if (src.front() == '-' && src.size() > 1 && std::isdigit(src[1]))
        {
            std::string number = "-";
            src.erase(src.begin());
            while (!src.empty() && (std::isdigit(src.front()) || (src.front() == '.' && number.find('.') == std::string::npos)))
            {
                number += src.front();
                src.erase(src.begin());
            }
            tokens.push_back(createToken(TokenType::Number, number));
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
        else if (src.size() >= 2 && src[0] == '&' && src[1] == '=')
        {
            tokens.push_back(createToken(TokenType::And, "&="));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.size() >= 2 && src[0] == '|' && src[1] == '=')
        {
            tokens.push_back(createToken(TokenType::Or, "|=")); // logical or operator
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.size() >= 2 && src[0] == '<' && src[1] == '~')
        {
            tokens.push_back(createToken(TokenType::LessEq, "<~"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.size() >= 2 && src[0] == '>' && src[1] == '~')
        {
            tokens.push_back(createToken(TokenType::GreaterEq, ">~"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.size() >= 2 && src[0] == '<' && src[1] == '<')
        {
            tokens.push_back(createToken(TokenType::AngleOpen, "<<"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.size() >= 2 && src[0] == '>' && src[1] == '>')
        {
            tokens.push_back(createToken(TokenType::AngleClose, ">>"));
            src.erase(src.begin(), src.begin() + 2);
        }
        else if (src.front() == '<')
        {
            tokens.push_back(createToken(TokenType::Less, "<"));
            src.erase(src.begin());
        }
        else if (src.front() == '>')
        {
            tokens.push_back(createToken(TokenType::Greater, ">"));
            src.erase(src.begin());
        }
        else if (src.front() == '[')
        {
            tokens.push_back(createToken(TokenType::OpenBracket, "["));
            src.erase(src.begin());
        }
        else if (src.front() == ']')
        {
            tokens.push_back(createToken(TokenType::CloseBracket, "]"));
            src.erase(src.begin());
        }
        else if (src.front() == '|')
        {
            tokens.push_back(createToken(TokenType::Pipe, "|"));
            src.erase(src.begin());
        }
        else if (src.front() == '#')
        {
            src.erase(src.begin());
            std::string str = "";
            while (!src.empty() && src.front() != '#')
            {
                str += src.front();
                src.erase(src.begin());
            }
            if (!src.empty() && src.front() == '#')
            {
                src.erase(src.begin());
                tokens.push_back(createToken(TokenType::String, str));
            }
            else
            {
                std::cerr << "Unterminated string literal" << std::endl;
                exit(1);
            }
        }
        else if (std::isalpha(src.front())) // identifier or keyword
        {
            std::string ident = "";
            while (!src.empty() && std::isalpha(src.front()))
            {
                ident += src.front();
                src.erase(src.begin());
            }
            if (ident == "hush")
            {
                while (!src.empty() && src.front() != '\n')
                {
                    src.erase(src.begin());
                }
                if (!src.empty() && src.front() == '\n')
                {
                    src.erase(src.begin());
                }
                continue;
            }
            // Recognize booleans
            if (ident == "yup" || ident == "nope")
            {
                tokens.push_back(createToken(TokenType::Boolean, ident));
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
        else if (src.front() == '$')
        {
            tokens.push_back(createToken(TokenType::Dollar, "$"));
            src.erase(src.begin());
        }
        else if (src.front() == ',')
        {
            tokens.push_back(createToken(TokenType::Comma, ","));
            src.erase(src.begin());
        }
        else if (src.front() == '.')
        {
            tokens.push_back(createToken(TokenType::Dot, "."));
            src.erase(src.begin());
        }
        else if (src.front() == '@')
        {
            tokens.push_back(createToken(TokenType::At, "@"));
            src.erase(src.begin());
        }
        else if (src.front() == '^')
        {
            tokens.push_back(createToken(TokenType::Caret, "^"));
            src.erase(src.begin());
        }
        else if (src.front() == ';')
        {
            tokens.push_back(createToken(TokenType::Semicolon, ";"));
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
