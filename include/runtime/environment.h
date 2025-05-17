#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <memory>
#include <unordered_map>
#include "runtime/values.h"

class Environment : std::enable_shared_from_this<Environment>
{
private:
    std::shared_ptr<Environment> parent;
    std::unordered_map<std::string, std::shared_ptr<RuntimeVal>> variables;

public:
    Environment(std::shared_ptr<Environment> parentEnv = nullptr) : parent(std::move(parentEnv)) {}
    std::shared_ptr<RuntimeVal> declareVar(std::string varName, std::shared_ptr<RuntimeVal> value);
    std::shared_ptr<RuntimeVal> assignVar(std::string varName, std::shared_ptr<RuntimeVal> value);
    std::shared_ptr<Environment> resolve(std::string varName);
    std::shared_ptr<RuntimeVal> lookUp(std::string varName);
};
#endif