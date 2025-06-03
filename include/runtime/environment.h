#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <memory>
#include <unordered_map>
#include <set>
#include "runtime/values.h"

/**
 * @brief The Environment class represents a variable scope (with optional parent for nested scopes).
 */
class Environment : public std::enable_shared_from_this<Environment>
{
private:
    std::shared_ptr<Environment> parent;                                    // Parent environment (for nested scopes)
    std::unordered_map<std::string, std::shared_ptr<RuntimeVal>> variables; // Variable table
    std::set<std::string> constants;                                        // Set of constant variable names

public:
    /**
     * @brief Create a new environment, optionally with a parent.
     * @param parentEnv The parent environment (optional).
     */
    Environment(std::shared_ptr<Environment> parentEnv = nullptr) : parent(std::move(parentEnv)) {}
    /**
     * @brief Declare a new variable (throws if already exists).
     * @param varName The variable name.
     * @param value The value to assign.
     * @param constant Whether the variable is a constant.
     * @return std::shared_ptr<RuntimeVal> The value assigned.
     */
    std::shared_ptr<RuntimeVal> declareVar(const std::string &varName, std::shared_ptr<RuntimeVal> value, bool constant);
    /**
     * @brief Assign to an existing variable (throws if constant or not found).
     * @param varName The variable name.
     * @param value The value to assign.
     * @return std::shared_ptr<RuntimeVal> The value assigned.
     */
    std::shared_ptr<RuntimeVal> assignVar(const std::string &varName, std::shared_ptr<RuntimeVal> value);
    /**
     * @brief Find the environment where a variable is defined (throws if not found).
     * @param varName The variable name.
     * @return std::shared_ptr<Environment> The environment containing the variable.
     */
    std::shared_ptr<Environment> resolve(const std::string &varName);
    /**
     * @brief Look up a variable's value (throws if not found).
     * @param varName The variable name.
     * @return std::shared_ptr<RuntimeVal> The value of the variable.
     */
    std::shared_ptr<RuntimeVal> lookUp(const std::string &varName);
};
#endif