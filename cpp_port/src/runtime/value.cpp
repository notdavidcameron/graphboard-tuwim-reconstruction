#include "graphboard/runtime/value.hpp"

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <sstream>

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
        case Type::Real: return static_cast<std::int64_t>(real_);
        case Type::Str: return parseLeadingInt(str_);
        case Type::Empty: return 0;
    }
    return 0;
}

double Value::toDouble() const {
    switch (type_) {
        case Type::Int: return static_cast<double>(int_);
        case Type::Real: return real_;
        case Type::Str: return std::strtod(str_.c_str(), nullptr);
        case Type::Empty: return 0.0;
    }
    return 0.0;
}

std::string Value::toString() const {
    switch (type_) {
        case Type::Int: return std::to_string(int_);
        case Type::Real: {
            std::ostringstream out;
            out << std::setprecision(15) << real_;
            return out.str();
        }
        case Type::Str: return str_;
        case Type::Empty: return std::string();
    }
    return std::string();
}

Value addValues(const Value& a, const Value& b) {
    if (a.isString() || b.isString()) {
        return Value::string(a.toString() + b.toString());
    }
    if (a.isReal() || b.isReal()) {
        return Value::real(a.toDouble() + b.toDouble());
    }
    return Value::integer(a.toInt() + b.toInt());
}

Value arithmetic(char op, const Value& a, const Value& b) {
    if (a.isReal() || b.isReal()) {
        const double x = a.toDouble();
        const double y = b.toDouble();
        switch (op) {
            case '-': return Value::real(x - y);
            case '*': return Value::real(x * y);
            case '/': return Value::real(y != 0.0 ? x / y : 0.0);
            case '%': return Value::real(y != 0.0 ? std::fmod(x, y) : 0.0);
            default: return Value::real(0.0);
        }
    }
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
    // Both numeric -> numeric; otherwise string comparison (number coerced to text).
    bool result = false;
    if (a.isNumeric() && b.isNumeric()) {
        const double x = a.toDouble();
        const double y = b.toDouble();
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
