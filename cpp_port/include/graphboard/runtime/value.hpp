#pragma once

#include <cstdint>
#include <string>

namespace graphboard::runtime {

// A GraphBoard script scalar value. CRect/storage members are represented by
// sparse paths in the interpreter; this type models the numeric and CString
// values stored at those paths
// with the coercions the recovered compare/assign operators imply:
//   - '+' concatenates when either side is a string, else adds numerically.
//   - arithmetic stays integral unless either side is real.
//   - '==' etc. compare numerically when both sides are numeric, else as strings.
//   - truthiness is (numeric value != 0); a string coerces numerically.
class Value {
public:
    enum class Type { Empty, Int, Real, Str };

    Value() = default;
    explicit Value(std::int64_t v) : type_(Type::Int), int_(v) {}
    explicit Value(double v) : type_(Type::Real), real_(v) {}
    explicit Value(std::string v) : type_(Type::Str), str_(std::move(v)) {}

    static Value integer(std::int64_t v) { return Value(v); }
    static Value real(double v) { return Value(v); }
    static Value string(std::string v) { return Value(std::move(v)); }

    Type type() const { return type_; }
    bool isInt() const { return type_ == Type::Int; }
    bool isReal() const { return type_ == Type::Real; }
    bool isNumeric() const { return isInt() || isReal(); }
    bool isString() const { return type_ == Type::Str; }
    bool isEmpty() const { return type_ == Type::Empty; }

    std::int64_t toInt() const;
    double toDouble() const;
    std::string toString() const;
    bool toBool() const { return toDouble() != 0.0; }

private:
    Type type_ = Type::Empty;
    std::int64_t int_ = 0;
    double real_ = 0.0;
    std::string str_;
};

// Binary operators used by the interpreter. `applyBinary` implements the
// coercion rules above; comparisons return an integer 0/1 value.
Value addValues(const Value& a, const Value& b);          // '+'
Value arithmetic(char op, const Value& a, const Value& b);  // '-', '*', '/', '%'
Value compare(const std::string& op, const Value& a, const Value& b);  // ==,!=,<,<=,>,>=

} // namespace graphboard::runtime
