#include "runtime/values.h"

std::shared_ptr<NumberVal> mkNumber(double val)
{
    return std::make_shared<NumberVal>(val);
}

std::shared_ptr<NullVal> mkNull()
{
    return std::make_shared<NullVal>();
}

std::shared_ptr<BooleanVal> mkBool(bool val = true)
{
    return std::make_shared<BooleanVal>(val);
}

std::shared_ptr<StringVal> mkString(const std::string &val)
{
    return std::make_shared<StringVal>(val);
}

std::shared_ptr<ArrayVal> mkArray(const std::vector<std::shared_ptr<RuntimeVal>> &val)
{
    return std::make_shared<ArrayVal>(val);
}