#include "runtime/environment.h"
#include <iostream>

std::shared_ptr<RuntimeVal> Environment::declareVar(std::string varName, std::shared_ptr<RuntimeVal> value)
{
    if (variables.find(varName) != variables.end())
    {
        throw std::runtime_error("Cannot declare variable '" + varName + "'. It is already defined.");
    }
    variables[varName] = value;
    return value;
}

std::shared_ptr<RuntimeVal> Environment::assignVar(std::string varName, std::shared_ptr<RuntimeVal> value)
{
    auto env = resolve(varName);
    env->variables[varName] = value;

    return value;
}

std::shared_ptr<Environment> Environment::resolve(std::string varName)
{
    if (variables.count(varName))
    {
        return shared_from_this();
    }

    if (!parent)
    {
        throw std::runtime_error("Cannot resolve variable '" + varName + "' as it does not exist.");
    }

    return parent->resolve(varName);
}

std::shared_ptr<RuntimeVal> Environment::lookUp(std::string varName)
{
    auto env = resolve(varName);
    return env->variables.at(varName);
}