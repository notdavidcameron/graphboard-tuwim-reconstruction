#pragma once

#include <cstdint>
#include <string>

namespace graphboard::runtime {

// A GraphBoard script value. The engine's variant record is int-or-string
// dominated (CRect/storage/class members exist too — see runtime notes — but
// the page scripts are overwhelmingly int and CString). This models those two
// with the coercions the recovered compare/assign operators imply:
//   - '+' concatenates when either side is a string, else adds as integers.
//   - other arithmetic is integer.
//   - '==' etc. compare numerically when both sides are integers, else as
//     strings (an integer is coerced to its decimal spelling).
//   - truthiness is (integer value != 0); a string coerces via its integer.
class Value {
public:
    enum class Type { Empty, Int, Str };

    Value() = default;
    explicit Value(std::int64_t v) : type_(Type::Int), int_(v) {}
    explicit Value(std::string v) : type_(Type::Str), str_(std::move(v)) {}

    static Value integer(std::int64_t v) { return Value(v); }
    static Value string(std::string v) { return Value(std::move(v)); }

    Type type() const { return type_; }
    bool isInt() const { return type_ == Type::Int; }
    bool isString() const { return type_ == Type::Str; }
    bool isEmpty() const { return type_ == Type::Empty; }

    std::int64_t toInt() const;
    std::string toString() const;
    bool toBool() const { return toInt() != 0; }

private:
    Type type_ = Type::Empty;
    std::int64_t int_ = 0;
    std::string str_;
};

// Binary operators used by the interpreter. `applyBinary` implements the
// coercion rules above; comparisons return an integer 0/1 value.
Value addValues(const Value& a, const Value& b);          // '+'
Value arithmetic(char op, const Value& a, const Value& b);  // '-', '*', '/', '%'
Value compare(const std::string& op, const Value& a, const Value& b);  // ==,!=,<,<=,>,>=

} // namespace graphboard::runtime
