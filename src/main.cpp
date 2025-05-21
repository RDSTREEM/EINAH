#include "frontend/parser.h"
#include "runtime/interpreter.h"
#include <iostream>

namespace printHelper
{

    void printIndent(int indent)
    {
        for (int i = 0; i < indent; ++i)
            std::cout << "  ";
    }

    void printExpr(const std::shared_ptr<Expr> &expr, int indent)
    {
        if (!expr)
        {
            printIndent(indent);
            std::cout << "null expr\n";
            return;
        }

        switch (expr->kind)
        {
        case NodeType::Identifier:
        {
            auto id = std::static_pointer_cast<Identifier>(expr);
            printIndent(indent);
            std::cout << "Identifier: " << id->symbol << "\n";
            break;
        }
        case NodeType::NumericLiteral:
        {
            auto num = std::static_pointer_cast<NumericLiteral>(expr);
            printIndent(indent);
            std::cout << "NumericLiteral: " << num->value << "\n";
            break;
        }
        case NodeType::BinaryExpr:
        {
            auto bin = std::static_pointer_cast<BinaryExpr>(expr);
            printIndent(indent);
            std::cout << "BinaryExpr (operator: " << bin->op << "):\n";
            printExpr(bin->left, indent + 1);
            printExpr(bin->right, indent + 1);
            break;
        }
        // case NodeType::NullLiteral:
        // {
        //     printIndent(indent);
        //     std::cout << "NullLiteral\n";
        //     break;
        // }
        default:
            printIndent(indent);
            std::cout << "Unknown Expr node type\n";
            break;
        }
    }

    void printStmt(const std::shared_ptr<Stmt> &stmt, int indent = 0)
    {
        if (!stmt)
        {
            printIndent(indent);
            std::cout << "null stmt\n";
            return;
        }
        printExpr(std::static_pointer_cast<Expr>(stmt), indent);
    }

    void printProgram(const std::shared_ptr<Program> &program)
    {
        if (!program)
        {
            std::cout << "Empty program\n";
            return;
        }

        std::cout << "Program AST:\n";
        for (const auto &stmt : program->body)
        {
            printStmt(stmt, 1);
        }
    }

    void printRuntimeVal(const std::shared_ptr<RuntimeVal> &val)
    {
        if (!val)
        {
            std::cout << "null\n";
            return;
        }

        switch (val->_type)
        {
        case ValueType::Number:
        {
            auto num = std::static_pointer_cast<NumberVal>(val);
            std::cout << "{ \"type\": \"number\", \"value\": " << num->val << " }" << std::endl;
            break;
        }
        case ValueType::Null:
        {
            std::cout << "{ \"type\": \"null\", \"value\": null }" << std::endl;
            break;
        }
        default:
            std::cout << "{ \"type\": \"unknown\" }" << std::endl;
            break;
        }
    }

}

void repl()
{
    Parser parser;
    std::shared_ptr<Environment> env = std::make_shared<Environment>();
    env->declareVar("true", mkBool(true), true);
    env->declareVar("false", mkBool(false), true);
    env->declareVar("null", mkNull(), true);
    std::cout << "EINAH v0.1" << std::endl;

    while (true)
    {
        std::string input;
        std::cout << "> ";

        if (!std::getline(std::cin, input))
        {
            break;
        }

        if (input.empty() || input.find("exit") != std::string::npos)
        {
            std::cout << "Exiting...\n";
            break;
        }

        std::shared_ptr<Program> program = parser.produceAST(input);
        auto result = evaluate(program, env);
        printHelper::printRuntimeVal(result);
    }
}

int main(int argc, char const *argv[])
{
    repl();
    return 0;
}
