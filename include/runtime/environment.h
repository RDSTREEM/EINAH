#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <memory>
#include <unordered_map>
#include <set>
#include "runtime/values.h"

class Environment : public std::enable_shared_from_this<Environment>
{
private:
    std::shared_ptr<Environment> parent;
    std::unordered_map<std::string, std::shared_ptr<RuntimeVal>> variables;
    std::set<std::string> constants;

public:
    Environment(std::shared_ptr<Environment> parentEnv = nullptr) : parent(std::move(parentEnv)) {}
    std::shared_ptr<RuntimeVal> declareVar(const std::string &varName, std::shared_ptr<RuntimeVal> value, bool constant);
    std::shared_ptr<RuntimeVal> assignVar(const std::string &varName, std::shared_ptr<RuntimeVal> value);
    std::shared_ptr<Environment> resolve(const std::string &varName);
    std::shared_ptr<RuntimeVal> lookUp(const std::string &varName);
};
#endif