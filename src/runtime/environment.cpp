#include "runtime/environment.h"
#include <iostream>

std::shared_ptr<RuntimeVal> Environment::declareVar(const std::string &varName, std::shared_ptr<RuntimeVal> value)
{
    if (variables.find(varName) != variables.end())
    {
        throw std::runtime_error("Cannot declare variable '" + varName + "'. It is already defined.");
    }
    variables[varName] = value;
    return value;
}

std::shared_ptr<RuntimeVal> Environment::assignVar(const std::string &varName, std::shared_ptr<RuntimeVal> value)
{
    auto env = resolve(varName);
    env->variables[varName] = value;

    return value;
}

std::shared_ptr<Environment> Environment::resolve(const std::string &varName)
{
    if (variables.find(varName) != variables.end())
    {
        std::shared_ptr<Environment> env = shared_from_this();
        return env;
    }

    if (parent)
    {
        return parent->resolve(varName);
    }

    throw std::runtime_error("Variable not defined: " + varName);
}

std::shared_ptr<RuntimeVal> Environment::lookUp(const std::string &varName)
{
    std::shared_ptr<Environment> env = resolve(varName);
    return env->variables.at(varName);
}