#ifndef VALUES_H
#define VALUES_H

#include <string>
#include <memory>

/**
 * @brief Value types for the Einah runtime.
 */
enum class ValueType
{
    Null,    // zip
    Number,  // numeric
    Boolean, // yup/nope
    String   // string value
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

#endif