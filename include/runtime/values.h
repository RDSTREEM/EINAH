#ifndef VALUES_H
#define VALUES_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <frontend/ast.h>
#include <functional>

class Environment;

/**
 * @brief Value types for the Einah runtime.
 */
enum class ValueType
{
    Null,     // zip
    Number,   // numeric
    Boolean,  // yup/nope
    String,   // string value
    Array,    // array value
    Function, // function value
    Object    // object/map value
};

/**
 * @brief Base runtime value.
 */
struct RuntimeVal
{
    ValueType _type; // The type of the runtime value.
};

/**
 * @brief Null value (zip).
 */
struct NullVal : RuntimeVal
{
    std::nullptr_t val; // Always nullptr.
    NullVal()
    {
        _type = ValueType::Null;
        val = nullptr;
    }
};

/**
 * @brief Number value.
 */
struct NumberVal : RuntimeVal
{
    double val; // The numeric value.
    NumberVal(double val) : val(val)
    {
        _type = ValueType::Number;
    }
};

/**
 * @brief Boolean value (yup/nope).
 */
struct BooleanVal : RuntimeVal
{
    bool val; // The boolean value.
    BooleanVal(bool val = true) : val(val)
    {
        _type = ValueType::Boolean;
    }
};

/**
 * @brief String value.
 */
struct StringVal : RuntimeVal
{
    std::string val; // The string value.
    StringVal(const std::string &v) : val(v)
    {
        _type = ValueType::String;
    }
};

/**
 * @brief Array value.
 */
struct ArrayVal : RuntimeVal
{
    std::vector<std::shared_ptr<RuntimeVal>> val; // The array elements.
    ArrayVal(const std::vector<std::shared_ptr<RuntimeVal>> &v) : val(v)
    {
        _type = ValueType::Array;
    }
};

/**
 * @brief Function value.
 */
struct FunctionVal : RuntimeVal
{
    std::vector<std::string> params;
    std::vector<std::shared_ptr<Stmt>> body;
    std::shared_ptr<Environment> closure;
    FunctionVal(const std::vector<std::string> &params,
                const std::vector<std::shared_ptr<Stmt>> &body,
                std::shared_ptr<Environment> closure)
        : params(params), body(body), closure(closure)
    {
        _type = ValueType::Function;
    }
};

/**
 * @brief Object value (map).
 */
struct ObjectVal : RuntimeVal
{
    std::unordered_map<std::string, std::shared_ptr<RuntimeVal>> val;
    ObjectVal(const std::unordered_map<std::string, std::shared_ptr<RuntimeVal>> &v) : val(v)
    {
        _type = ValueType::Object;
    }
};

/**
 * @brief Native function value.
 */
struct NativeFunctionVal : RuntimeVal
{
    std::string name;
    size_t arity;
    std::function<std::shared_ptr<RuntimeVal>(const std::vector<std::shared_ptr<RuntimeVal>> &)> fn;
    NativeFunctionVal(const std::string &n, size_t a,
                      std::function<std::shared_ptr<RuntimeVal>(const std::vector<std::shared_ptr<RuntimeVal>> &)> f)
        : name(n), arity(a), fn(f) { _type = ValueType::Function; }
};

/**
 * @brief Create a new number value.
 * @param val The numeric value.
 * @return std::shared_ptr<NumberVal> The created number value.
 */
std::shared_ptr<NumberVal> mkNumber(double val);
/**
 * @brief Create a new null value.
 * @return std::shared_ptr<NullVal> The created null value.
 */
std::shared_ptr<NullVal> mkNull();
/**
 * @brief Create a new boolean value.
 * @param val The boolean value.
 * @return std::shared_ptr<BooleanVal> The created boolean value.
 */
std::shared_ptr<BooleanVal> mkBool(bool val);
/**
 * @brief Create a new string value.
 * @param val The string value.
 * @return std::shared_ptr<StringVal> The created string value.
 */
std::shared_ptr<StringVal> mkString(const std::string &val);
/**
 * @brief Create a new array value.
 * @param val The array elements.
 * @return std::shared_ptr<ArrayVal> The created array value.
 */
std::shared_ptr<ArrayVal> mkArray(const std::vector<std::shared_ptr<RuntimeVal>> &val);
/**
 * @brief Create a new function value.
 * @param params The function parameters.
 * @param body The function body.
 * @param closure The closure for the function.
 * @return std::shared_ptr<FunctionVal> The created function value.
 */
std::shared_ptr<FunctionVal> mkFunction(const std::vector<std::string> &params, const std::vector<std::shared_ptr<Stmt>> &body, std::shared_ptr<Environment> closure);
/**
 * @brief Create a new object value.
 * @param val The object/map elements.
 * @return std::shared_ptr<ObjectVal> The created object value.
 */
std::shared_ptr<ObjectVal> mkObject(const std::unordered_map<std::string, std::shared_ptr<RuntimeVal>> &val);
/**
 * @brief Create a new native function value.
 * @param name The name of the function.
 * @param arity The number of parameters the function takes.
 * @param fn The function implementation.
 * @return std::shared_ptr<NativeFunctionVal> The created native function value.
 */
std::shared_ptr<NativeFunctionVal> mkNative(const std::string &name, size_t arity, std::function<std::shared_ptr<RuntimeVal>(const std::vector<std::shared_ptr<RuntimeVal>> &)> fn);

#endif