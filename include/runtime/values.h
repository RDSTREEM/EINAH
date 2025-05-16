#ifndef VALUES_H
#define VALUES_H

#include <string>

enum class ValueType
{
    Null,
    Number
};

struct RuntimeVal
{
    ValueType _type;
};

struct NullVal : RuntimeVal
{
    std::string val;
    NullVal()
    {
        _type = ValueType::Null;
        val = "null";
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

#endif