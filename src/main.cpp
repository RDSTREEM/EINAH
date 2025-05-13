#include <lexer.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    while (true)
    {
        std::string src;
        std::cout << std::endl;
        std::cout << "> ";
        std::cin >> src;

        std::vector<Token> tokens = tokenize(src);
        for (size_t i = 0; i < tokens.size(); i++)
        {
            Token tk = tokens.at(i);
            if (tk.type == TokenType::BinaryOperator)
            {
                std::cout << "BinaryOperator";
            }
            else if (tk.type == TokenType::CloseParen)
            {
                std::cout << "CloseParen";
            }
            else if (tk.type == TokenType::OpenParen)
            {
                std::cout << "OpenParen";
            }
            else if (tk.type == TokenType::Number)
            {
                std::cout << "NumericLiteral";
            }
            else if (tk.type == TokenType::Let)
            {
                std::cout << "Let";
            }
            else if (tk.type == TokenType::Equals)
            {
                std::cout << "Equals";
            }
            else if (tk.type == TokenType::Identifier)
            {
                std::cout << "Identifier";
            }
            if (!((i + 1) == tokens.size()))
            {
                std::cout << ", ";
            }
        }
    }
    return 0;
}
