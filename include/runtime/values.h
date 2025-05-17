#ifndef VALUES_H
#define VALUES_H

#include <string>
#include <memory>

enum class ValueType
{
    Null,
    Number,
    Boolean
};

struct RuntimeVal
{
    ValueType _type;
};

struct NullVal : RuntimeVal
{
    std::nullptr_t val;
    NullVal()
    {
        _type = ValueType::Null;
        val = nullptr;
    }
};

struct NumberVal : RuntimeVal
{
    double val;
    NumberVal(double val) : val(val)
    {
        _type = ValueType::Number;
    }
};

struct BooleanVal : RuntimeVal
{
    bool val;
    BooleanVal(bool val = true) : val(val)
    {
        _type = ValueType::Boolean;
    }
};

std::shared_ptr<NumberVal> mkNumber(double val);
std::shared_ptr<NullVal> mkNull();
std::shared_ptr<BooleanVal> mkBool(bool val);

#endif