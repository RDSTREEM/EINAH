#include "runtime/native.h"
#include "runtime/values.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

void registerNativeFunctions(std::shared_ptr<Environment> env)
{
    env->declareVar("length", mkNative("length", 1, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                       {
            if (args[0]->_type == ValueType::Array) {
                auto arr = std::static_pointer_cast<ArrayVal>(args[0]);
                return mkNumber(static_cast<double>(arr->val.size()));
            } else if (args[0]->_type == ValueType::String) {
                auto str = std::static_pointer_cast<StringVal>(args[0]);
                return mkNumber(static_cast<double>(str->val.size()));
            }
            throw std::runtime_error("length() expects array or string"); }),
                    true);

    env->declareVar("push", mkNative("push", 2, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                     {
            if (args[0]->_type != ValueType::Array)
                throw std::runtime_error("push() expects array as first argument");
            auto arr = std::static_pointer_cast<ArrayVal>(args[0]);
            auto newArr = std::make_shared<ArrayVal>(arr->val);
            newArr->val.push_back(args[1]);
            return newArr; }),
                    true);

    env->declareVar("pop", mkNative("pop", 1, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                    {
            if (args[0]->_type != ValueType::Array)
                throw std::runtime_error("pop() expects array");
            auto arr = std::static_pointer_cast<ArrayVal>(args[0]);
            if (arr->val.empty())
                throw std::runtime_error("pop() on empty array");
            auto newArr = std::make_shared<ArrayVal>(arr->val);
            newArr->val.pop_back();
            return newArr; }),
                    true);

    env->declareVar("slice", mkNative("slice", 3, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                      {
            if (args[0]->_type != ValueType::Array)
                throw std::runtime_error("slice() expects array");
            auto arr = std::static_pointer_cast<ArrayVal>(args[0]);
            int a = static_cast<int>(std::static_pointer_cast<NumberVal>(args[1])->val);
            int b = static_cast<int>(std::static_pointer_cast<NumberVal>(args[2])->val);
            if (a < 0) a = 0;
            if (b > (int)arr->val.size()) b = (int)arr->val.size();
            if (a > b) a = b;
            std::vector<std::shared_ptr<RuntimeVal>> sub(arr->val.begin() + a, arr->val.begin() + b);
            return std::make_shared<ArrayVal>(sub); }),
                    true);

    env->declareVar("keys", mkNative("keys", 1, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                     {
            if (args[0]->_type != ValueType::Object)
                throw std::runtime_error("keys() expects object");
            auto obj = std::static_pointer_cast<ObjectVal>(args[0]);
            std::vector<std::shared_ptr<RuntimeVal>> keys;
            for (const auto& kv : obj->val) {
                keys.push_back(mkString(kv.first));
            }
            return mkArray(keys); }),
                    true);

    env->declareVar("has", mkNative("has", 2, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                    {
            if (args[0]->_type != ValueType::Object)
                throw std::runtime_error("has() expects object");
            auto obj = std::static_pointer_cast<ObjectVal>(args[0]);
            std::string key;
            if (args[1]->_type == ValueType::String)
                key = std::static_pointer_cast<StringVal>(args[1])->val;
            else if (args[1]->_type == ValueType::Number)
                key = std::to_string(static_cast<int>(std::static_pointer_cast<NumberVal>(args[1])->val));
            else
                throw std::runtime_error("has() expects string or number as key");
            return mkBool(obj->val.count(key) > 0); }),
                    true);

    env->declareVar("split", mkNative("split", 2, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                      {
            if (args[0]->_type != ValueType::String || args[1]->_type != ValueType::String)
                throw std::runtime_error("split() expects (string, string)");
            auto str = std::static_pointer_cast<StringVal>(args[0])->val;
            auto sep = std::static_pointer_cast<StringVal>(args[1])->val;
            std::vector<std::shared_ptr<RuntimeVal>> parts;
            size_t pos = 0, found;
            while ((found = str.find(sep, pos)) != std::string::npos) {
                parts.push_back(mkString(str.substr(pos, found - pos)));
                pos = found + sep.length();
            }
            parts.push_back(mkString(str.substr(pos)));
            return mkArray(parts); }),
                    true);

    env->declareVar("random", mkNative("random", 0, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                       {
            static bool seeded = false;
            if (!seeded) { std::srand(static_cast<unsigned>(std::time(nullptr))); seeded = true; }
            return mkNumber((double)std::rand() / RAND_MAX); }),
                    true);

    env->declareVar("floor", mkNative("floor", 1, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                      {
            if (args[0]->_type != ValueType::Number)
                throw std::runtime_error("floor() expects number");
            return mkNumber(std::floor(std::static_pointer_cast<NumberVal>(args[0])->val)); }),
                    true);

    env->declareVar("ceil", mkNative("ceil", 1, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                     {
            if (args[0]->_type != ValueType::Number)
                throw std::runtime_error("ceil() expects number");
            return mkNumber(std::ceil(std::static_pointer_cast<NumberVal>(args[0])->val)); }),
                    true);

    env->declareVar("type", mkNative("type", 1, [](const std::vector<std::shared_ptr<RuntimeVal>> &args) -> std::shared_ptr<RuntimeVal>
                                     {
            switch (args[0]->_type) {
                case ValueType::Null: return mkString("null");
                case ValueType::Number: return mkString("num");
                case ValueType::Boolean: return mkString("bool");
                case ValueType::String: return mkString("str");
                case ValueType::Array: return mkString("array");
                case ValueType::Function: return mkString("func");
                case ValueType::Object: return mkString("obj");
                default: return mkString("unknown");
            } }),
                    true);
}
