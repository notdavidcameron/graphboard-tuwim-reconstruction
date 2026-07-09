#include "graphboard/runtime/value.hpp"

#include <cstdlib>

namespace graphboard::runtime {

namespace {

// Parse a leading integer from a string the way atoi/atol does: optional sign,
// then digits, stopping at the first non-digit. Non-numeric strings yield 0.
std::int64_t parseLeadingInt(const std::string& s) {
    return static_cast<std::int64_t>(std::strtoll(s.c_str(), nullptr, 10));
}

} // namespace

std::int64_t Value::toInt() const {
    switch (type_) {
        case Type::Int: return int_;
        case Type::Str: return parseLeadingInt(str_);
        case Type::Empty: return 0;
    }
    return 0;
}

std::string Value::toString() const {
    switch (type_) {
        case Type::Int: return std::to_string(int_);
        case Type::Str: return str_;
        case Type::Empty: return std::string();
    }
    return std::string();
}

Value addValues(const Value& a, const Value& b) {
    if (a.isString() || b.isString()) {
        return Value::string(a.toString() + b.toString());
    }
    return Value::integer(a.toInt() + b.toInt());
}

Value arithmetic(char op, const Value& a, const Value& b) {
    const std::int64_t x = a.toInt();
    const std::int64_t y = b.toInt();
    switch (op) {
        case '-': return Value::integer(x - y);
        case '*': return Value::integer(x * y);
        case '/': return Value::integer(y != 0 ? x / y : 0);
        case '%': return Value::integer(y != 0 ? x % y : 0);
        default: return Value::integer(0);
    }
}

Value compare(const std::string& op, const Value& a, const Value& b) {
    // Both integers -> numeric; otherwise string comparison (int coerced to text).
    bool result = false;
    if (a.isInt() && b.isInt()) {
        const std::int64_t x = a.toInt();
        const std::int64_t y = b.toInt();
        if (op == "==") result = x == y;
        else if (op == "!=") result = x != y;
        else if (op == "<") result = x < y;
        else if (op == "<=") result = x <= y;
        else if (op == ">") result = x > y;
        else if (op == ">=") result = x >= y;
    } else {
        const std::string x = a.toString();
        const std::string y = b.toString();
        if (op == "==") result = x == y;
        else if (op == "!=") result = x != y;
        else if (op == "<") result = x < y;
        else if (op == "<=") result = x <= y;
        else if (op == ">") result = x > y;
        else if (op == ">=") result = x >= y;
    }
    return Value::integer(result ? 1 : 0);
}

} // namespace graphboard::runtime
