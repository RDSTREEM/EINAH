#include "frontend/parser.h"
#include "runtime/interpreter.h"
#include <iostream>
#include <third_party/magic_enum.hpp>
#include <fstream>
#include <sstream>

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
        printIndent(indent);
        std::cout << "Expr kind: " << magic_enum::enum_name(expr->kind) << "\n";
        switch (expr->kind)
        {
        case NodeType::Identifier:
        {
            auto id = std::static_pointer_cast<Identifier>(expr);
            printIndent(indent + 1);
            std::cout << "Identifier: '" << id->symbol << "'\n";
            break;
        }
        case NodeType::NumericLiteral:
        {
            auto num = std::static_pointer_cast<NumericLiteral>(expr);
            printIndent(indent + 1);
            std::cout << "NumericLiteral: " << num->value << "\n";
            break;
        }
        case NodeType::BinaryExpr:
        {
            auto bin = std::static_pointer_cast<BinaryExpr>(expr);
            printIndent(indent + 1);
            std::cout << "BinaryExpr (operator: '" << bin->op << "')\n";
            printIndent(indent + 2);
            std::cout << "Left:\n";
            printExpr(bin->left, indent + 3);
            printIndent(indent + 2);
            std::cout << "Right:\n";
            printExpr(bin->right, indent + 3);
            break;
        }
        case NodeType::AssignmentExpr:
        {
            auto assign = std::static_pointer_cast<AssignmentExpr>(expr);
            printIndent(indent + 1);
            std::cout << "AssignmentExpr\n";
            printIndent(indent + 2);
            std::cout << "Asignee:\n";
            printExpr(assign->asignee, indent + 3);
            printIndent(indent + 2);
            std::cout << "Value:\n";
            printExpr(assign->value, indent + 3);
            break;
        }
        default:
            printIndent(indent + 1);
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
        printIndent(indent);
        std::cout << "Stmt kind: " << magic_enum::enum_name(stmt->kind) << "\n";
        switch (stmt->kind)
        {
        case NodeType::VarDeclaration:
        {
            auto var = std::static_pointer_cast<VarDeclaration>(stmt);
            printIndent(indent + 1);
            std::cout << (var->constant ? "Const " : "Var ") << "Declaration: '" << var->ident << "'\n";
            printIndent(indent + 2);
            std::cout << "Value:\n";
            printExpr(var->value, indent + 3);
            break;
        }
        case NodeType::ExprStatement:
        {
            auto exprStmt = std::static_pointer_cast<ExprStatement>(stmt);
            printIndent(indent + 1);
            std::cout << "ExprStatement:\n";
            printExpr(exprStmt->expr, indent + 2);
            break;
        }
        default:
            printExpr(std::static_pointer_cast<Expr>(stmt), indent + 1);
            break;
        }
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
        std::cout << "{ \"type\": \"" << magic_enum::enum_name(val->_type) << "\"";
        switch (val->_type)
        {
        case ValueType::Number:
        {
            auto num = std::static_pointer_cast<NumberVal>(val);
            std::cout << ", \"value\": " << num->val << " }" << std::endl;
            break;
        }
        case ValueType::Boolean:
        {
            auto b = std::static_pointer_cast<BooleanVal>(val);
            std::cout << ", \"value\": " << (b->val ? "true" : "false") << " }" << std::endl;
            break;
        }
        case ValueType::Null:
        {
            std::cout << ", \"value\": null }" << std::endl;
            break;
        }
        default:
            std::cout << " }" << std::endl;
            break;
        }
    }

}

void printWelcome()
{
    std::cout << "EINAH v0.1\n";
    std::cout << "Type 'help' for available commands.\n";
}

void printHelp()
{
    std::cout << "Available commands:\n";
    std::cout << "  help         Show this help message\n";
    std::cout << "  clear        Clear the screen\n";
    std::cout << "  exit, quit   Exit the REPL\n";
    std::cout << "  #ast         Print the AST for the input\n";
    std::cout << "  (empty line) Repeat last input\n";
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void repl()
{
    Parser parser;
    std::shared_ptr<Environment> env = std::make_shared<Environment>();
    printWelcome();

    std::string lastInput;
    while (true)
    {
        std::string input;
        std::cout << "> ";
        if (!std::getline(std::cin, input))
        {
            break;
        }
        // Trim whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);
        if (input.empty())
        {
            if (lastInput.empty())
                continue;
            input = lastInput;
            std::cout << ">> " << input << std::endl;
        }
        else
        {
            lastInput = input;
        }
        if (input == "help")
        {
            printHelp();
            continue;
        }
        if (input == "clear")
        {
            clearScreen();
            continue;
        }
        if (input == "exit" || input == "quit")
        {
            std::cout << "Exiting...\n";
            break;
        }
        bool showAst = false;
        size_t astPos = input.find("#ast");
        if (astPos != std::string::npos)
        {
            showAst = true;
            input = input.substr(0, astPos);
            input.erase(input.find_last_not_of(" \t\n\r") + 1);
        }
        try
        {
            std::shared_ptr<Program> program = parser.produceAST(input);
            auto result = evaluate(program, env);
            if (showAst)
            {
                printHelper::printProgram(program);
            }
            bool isSpit = false;
            if (!program->body.empty())
            {
                auto last = program->body.back();
                if (last->kind == NodeType::PrintStatement)
                {
                    isSpit = true;
                }
            }
            if (!isSpit && result)
            {
                printHelper::printRuntimeVal(result);
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "[Error] " << e.what() << std::endl;
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc > 1)
    {
        std::string filename = argv[1];
        if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".exn")
        {
            std::ifstream file(filename);
            if (!file)
            {
                std::cerr << "Could not open file: " << filename << std::endl;
                return 1;
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string code = buffer.str();
            Parser parser;
            std::shared_ptr<Environment> env = std::make_shared<Environment>();
            try
            {
                std::shared_ptr<Program> program = parser.produceAST(code);
                evaluate(program, env);
            }
            catch (const std::exception &e)
            {
                std::cout << "[Error] " << e.what() << std::endl;
                return 1;
            }
            return 0;
        }
        else
        {
            std::cerr << "Only .exn files can be executed directly." << std::endl;
            return 1;
        }
    }
    else
        repl();
    return 0;
}
