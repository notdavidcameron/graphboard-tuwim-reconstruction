#include "graphboard/runtime/interpreter.hpp"

#include <cstdlib>
#include <unordered_map>
#include <unordered_set>

namespace graphboard::runtime {

namespace {

const std::unordered_set<std::string>& typeKeywords() {
    // The authoritative value/type keyword table
    // (GraphBrdScript_ValueTypeNameTable, Tuwim.exe:0043c3a0) plus a few common
    // C aliases the scripts also use in declarations.
    static const std::unordered_set<std::string> kw = {
        "char", "int", "long", "float", "unsigned", "UINT", "BYTE", "BOOL",
        "void", "CString", "CRect", "CVarStorage",
        "double", "short", "DWORD", "WORD", "bool",
    };
    return kw;
}

bool isAssignOp(const Token& t) {
    if (t.kind != TokenKind::Punctuator) return false;
    return t.text == "=" || t.text == "+=" || t.text == "-=" || t.text == "*=" ||
           t.text == "/=";
}

constexpr int kMaxCallDepth = 256;
constexpr long kMaxLoopIterations = 2000000;

} // namespace

Interpreter::Interpreter(std::string source, Host& host)
    : source_(std::move(source)), host_(host), tokens_(tokenize(source_)) {
    scopes_.emplace_back();  // global scope

    // Map byte offset -> token index so handler body offsets become indices.
    std::unordered_map<std::size_t, std::size_t> offsetToTok;
    offsetToTok.reserve(tokens_.size());
    for (std::size_t i = 0; i < tokens_.size(); ++i) {
        offsetToTok[tokens_[i].offset] = i;
    }

    for (const auto& handler : discoverHandlers(source_, tokens_)) {
        Function fn;
        for (const auto& param : handler.parameters) {
            fn.params.push_back(param.name);
        }
        const auto openIt = offsetToTok.find(handler.bodyOffset);
        if (openIt == offsetToTok.end()) {
            continue;
        }
        fn.bodyOpen = openIt->second;
        // Matching close brace for the body.
        int depth = 0;
        fn.bodyClose = fn.bodyOpen;
        for (std::size_t j = fn.bodyOpen; j < tokens_.size(); ++j) {
            if (tokens_[j].kind == TokenKind::Punctuator && tokens_[j].text == "{") {
                ++depth;
            } else if (tokens_[j].kind == TokenKind::Punctuator && tokens_[j].text == "}") {
                --depth;
                if (depth == 0) {
                    fn.bodyClose = j;
                    break;
                }
            }
        }
        functions_[handler.name] = fn;
    }
}

bool Interpreter::hasHandler(const std::string& name) const {
    return functions_.find(name) != functions_.end();
}

bool Interpreter::hasGlobal(const std::string& name) const {
    return scopes_.front().find(name) != scopes_.front().end();
}

Value Interpreter::getGlobal(const std::string& name) const {
    const auto it = scopes_.front().find(name);
    return it != scopes_.front().end() ? it->second : Value();
}

Value Interpreter::runHandler(const std::string& name, const std::vector<Value>& args) {
    const auto it = functions_.find(name);
    if (it == functions_.end()) {
        return Value();
    }
    const Function fn = it->second;

    scopes_.emplace_back();
    for (std::size_t i = 0; i < fn.params.size(); ++i) {
        scopes_.back()[fn.params[i]] = i < args.size() ? args[i] : Value();
    }

    const std::size_t savedPos = pos_;
    const Value savedReturn = returnValue_;
    returnValue_ = Value();
    execBlock(fn.bodyOpen);
    Value result = returnValue_;

    returnValue_ = savedReturn;
    pos_ = savedPos;
    scopes_.pop_back();
    return result;
}

// -------------------------------------------------------------- token helpers

bool Interpreter::atPunct(const char* text) const {
    return tok().kind == TokenKind::Punctuator && tok().text == text;
}

bool Interpreter::atKeyword(const char* word) const {
    return tok().kind == TokenKind::Identifier && tok().text == word;
}

// --------------------------------------------------------------------- scopes

Value Interpreter::lookup(const std::string& name) const {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        const auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }
    return Value();
}

void Interpreter::assign(const std::string& name, Value value) {
    for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
        const auto found = it->find(name);
        if (found != it->end()) {
            found->second = std::move(value);
            return;
        }
    }
    scopes_.front()[name] = std::move(value);  // undeclared -> page global
}

void Interpreter::declare(const std::string& name, Value value) {
    scopes_.back()[name] = std::move(value);
}

// ------------------------------------------------------------------ statements

Interpreter::Flow Interpreter::execBlock(std::size_t openBrace) {
    // Find the matching close brace once, so pos_ always ends just past it.
    int depth = 0;
    std::size_t close = openBrace;
    for (std::size_t j = openBrace; j < tokens_.size(); ++j) {
        if (tokens_[j].kind == TokenKind::Punctuator && tokens_[j].text == "{") {
            ++depth;
        } else if (tokens_[j].kind == TokenKind::Punctuator && tokens_[j].text == "}") {
            --depth;
            if (depth == 0) {
                close = j;
                break;
            }
        }
    }

    pos_ = openBrace + 1;
    scopes_.emplace_back();
    Flow flow = Flow::Normal;
    while (pos_ < close && tok().kind != TokenKind::End) {
        flow = execStatement();
        if (flow != Flow::Normal) {
            break;
        }
    }
    scopes_.pop_back();
    pos_ = close + 1;
    return flow;
}

Interpreter::Flow Interpreter::execStatement() {
    if (atPunct(";")) {
        ++pos_;
        return Flow::Normal;
    }
    if (atPunct("{")) {
        return execBlock(pos_);
    }
    if (atKeyword("if")) {
        return execIf();
    }
    if (atKeyword("while")) {
        return execWhile();
    }
    if (atKeyword("switch")) {
        return execSwitch();
    }
    if (atKeyword("return")) {
        ++pos_;
        if (!atPunct(";")) {
            returnValue_ = parseExpression();
        } else {
            returnValue_ = Value();
        }
        while (pos_ < tokens_.size() && !atPunct(";") && !atPunct("}") &&
               tok().kind != TokenKind::End) {
            ++pos_;
        }
        if (atPunct(";")) {
            ++pos_;
        }
        return Flow::Return;
    }
    if (atKeyword("break")) {
        ++pos_;
        if (atPunct(";")) {
            ++pos_;
        }
        return Flow::Break;
    }
    // Declaration: type keyword followed by an identifier.
    if (tok().kind == TokenKind::Identifier && typeKeywords().count(tok().text) &&
        pos_ + 1 < tokens_.size() && tokens_[pos_ + 1].kind == TokenKind::Identifier) {
        return execDeclaration();
    }

    // Assignment: identifier (not dotted, not indexed) followed by an assign op.
    if (tok().kind == TokenKind::Identifier && pos_ + 1 < tokens_.size() &&
        isAssignOp(tokens_[pos_ + 1])) {
        const std::string name = tok().text;
        const std::string op = tokens_[pos_ + 1].text;
        pos_ += 2;
        const Value rhs = parseExpression();
        Value result;
        if (op == "=") {
            result = rhs;
        } else {
            const Value current = lookup(name);
            result = op == "+=" ? addValues(current, rhs)
                                : arithmetic(op[0], current, rhs);
        }
        assign(name, std::move(result));
    } else {
        // Expression statement (calls, dotted property/method, etc.).
        parseExpression();
    }

    // Consume through the terminating ';'. Stop at a block close so a
    // malformed/unmodeled statement can never run past the enclosing '}'.
    while (pos_ < tokens_.size() && !atPunct(";") && !atPunct("}") &&
           tok().kind != TokenKind::End) {
        ++pos_;
    }
    if (atPunct(";")) {
        ++pos_;
    }
    return Flow::Normal;
}

Interpreter::Flow Interpreter::execDeclaration() {
    ++pos_;  // type keyword
    while (pos_ < tokens_.size() && tok().kind == TokenKind::Identifier) {
        const std::string name = tok().text;
        ++pos_;
        // Array dimension(s): int puzz[28]; — consume the bracketed size so the
        // declaration stays in sync (array elements are not modeled yet).
        while (atPunct("[")) {
            int depth = 0;
            while (pos_ < tokens_.size() && tok().kind != TokenKind::End) {
                if (atPunct("[")) {
                    ++depth;
                } else if (atPunct("]")) {
                    --depth;
                    ++pos_;
                    if (depth == 0) break;
                    continue;
                }
                ++pos_;
            }
        }
        Value init;
        if (atPunct("=")) {
            ++pos_;
            init = parseExpression();
        }
        declare(name, std::move(init));
        if (atPunct(",")) {
            ++pos_;
            continue;
        }
        break;
    }
    if (atPunct(";")) {
        ++pos_;
    }
    return Flow::Normal;
}

Interpreter::Flow Interpreter::execIf() {
    ++pos_;  // 'if'
    // condition
    if (atPunct("(")) {
        ++pos_;
    }
    const Value cond = parseExpression();
    if (atPunct(")")) {
        ++pos_;
    }

    Flow flow = Flow::Normal;
    if (cond.toBool()) {
        flow = execStatement();
        if (atKeyword("else")) {
            ++pos_;
            skipStatement();
        }
    } else {
        skipStatement();
        if (atKeyword("else")) {
            ++pos_;
            flow = execStatement();
        }
    }
    return flow;
}

Interpreter::Flow Interpreter::execWhile() {
    ++pos_;  // 'while'
    const std::size_t condStart = pos_;

    long iterations = 0;
    while (true) {
        pos_ = condStart;
        if (atPunct("(")) {
            ++pos_;
        }
        const Value cond = parseExpression();
        if (atPunct(")")) {
            ++pos_;
        }
        const std::size_t bodyStart = pos_;

        if (!cond.toBool() || ++iterations > kMaxLoopIterations) {
            pos_ = bodyStart;
            skipStatement();
            break;
        }
        const Flow flow = execStatement();
        if (flow == Flow::Break) {
            break;
        }
        if (flow == Flow::Return) {
            return Flow::Return;
        }
    }
    return Flow::Normal;
}

Interpreter::Flow Interpreter::execSwitch() {
    ++pos_;  // 'switch'
    if (atPunct("(")) {
        ++pos_;
    }
    const Value selector = parseExpression();
    if (atPunct(")")) {
        ++pos_;
    }
    if (!atPunct("{")) {
        return Flow::Normal;
    }
    const std::size_t openBrace = pos_;

    // Find matching close.
    int depth = 0;
    std::size_t close = openBrace;
    for (std::size_t j = openBrace; j < tokens_.size(); ++j) {
        if (atPunctAt(j, "{")) {
            ++depth;
        } else if (atPunctAt(j, "}")) {
            --depth;
            if (depth == 0) {
                close = j;
                break;
            }
        }
    }

    // Locate the matching case (or default) at switch-body top level.
    std::size_t execFrom = close;  // default: fall through to end (no-op)
    std::size_t defaultFrom = close;
    depth = 0;
    for (std::size_t j = openBrace + 1; j < close; ++j) {
        if (atPunctAt(j, "{")) {
            ++depth;
        } else if (atPunctAt(j, "}")) {
            --depth;
        } else if (depth == 0 && tokens_[j].kind == TokenKind::Identifier &&
                   tokens_[j].text == "case") {
            const std::size_t saved = pos_;
            pos_ = j + 1;
            const Value caseValue = parseExpression();
            if (atPunct(":")) {
                ++pos_;
            }
            const std::size_t bodyPos = pos_;
            pos_ = saved;
            if (compare("==", selector, caseValue).toBool()) {
                execFrom = bodyPos;
                break;
            }
        } else if (depth == 0 && tokens_[j].kind == TokenKind::Identifier &&
                   tokens_[j].text == "default") {
            std::size_t k = j + 1;
            if (k < close && atPunctAt(k, ":")) {
                ++k;
            }
            defaultFrom = k;
        }
    }
    if (execFrom == close) {
        execFrom = defaultFrom;
    }

    scopes_.emplace_back();
    pos_ = execFrom;
    Flow flow = Flow::Normal;
    while (pos_ < close && tok().kind != TokenKind::End) {
        // A 'case'/'default' label mid-stream is just a fallthrough target.
        if ((atKeyword("case")) ) {
            ++pos_;
            parseExpression();
            if (atPunct(":")) ++pos_;
            continue;
        }
        if (atKeyword("default")) {
            ++pos_;
            if (atPunct(":")) ++pos_;
            continue;
        }
        flow = execStatement();
        if (flow == Flow::Break) {
            flow = Flow::Normal;
            break;
        }
        if (flow == Flow::Return) {
            break;
        }
    }
    scopes_.pop_back();
    pos_ = close + 1;
    return flow;
}

// ----------------------------------------------------------------- statement skipping

void Interpreter::skipParens() {
    if (!atPunct("(")) {
        return;
    }
    int depth = 0;
    while (pos_ < tokens_.size() && tok().kind != TokenKind::End) {
        if (atPunct("(")) {
            ++depth;
        } else if (atPunct(")")) {
            --depth;
            ++pos_;
            if (depth == 0) {
                return;
            }
            continue;
        }
        ++pos_;
    }
}

void Interpreter::skipBraces() {
    if (!atPunct("{")) {
        return;
    }
    int depth = 0;
    while (pos_ < tokens_.size() && tok().kind != TokenKind::End) {
        if (atPunct("{")) {
            ++depth;
        } else if (atPunct("}")) {
            --depth;
            ++pos_;
            if (depth == 0) {
                return;
            }
            continue;
        }
        ++pos_;
    }
}

void Interpreter::skipStatement() {
    if (atPunct("{")) {
        skipBraces();
        return;
    }
    if (atKeyword("if")) {
        ++pos_;
        skipParens();
        skipStatement();
        if (atKeyword("else")) {
            ++pos_;
            skipStatement();
        }
        return;
    }
    if (atKeyword("while")) {
        ++pos_;
        skipParens();
        skipStatement();
        return;
    }
    if (atKeyword("switch")) {
        ++pos_;
        skipParens();
        skipBraces();
        return;
    }
    // Simple statement: consume through the terminating ';'.
    while (pos_ < tokens_.size() && !atPunct(";") && tok().kind != TokenKind::End) {
        if (atPunct("{")) {
            skipBraces();
            continue;
        }
        ++pos_;
    }
    if (atPunct(";")) {
        ++pos_;
    }
}

// ----------------------------------------------------------------- expressions

Value Interpreter::parseExpression() {
    return parseOr();
}

Value Interpreter::parseOr() {
    Value left = parseAnd();
    while (atPunct("||")) {
        ++pos_;
        const Value right = parseAnd();
        left = Value::integer((left.toBool() || right.toBool()) ? 1 : 0);
    }
    return left;
}

Value Interpreter::parseAnd() {
    Value left = parseEquality();
    while (atPunct("&&")) {
        ++pos_;
        const Value right = parseEquality();
        left = Value::integer((left.toBool() && right.toBool()) ? 1 : 0);
    }
    return left;
}

Value Interpreter::parseEquality() {
    Value left = parseRelational();
    while (atPunct("==") || atPunct("!=")) {
        const std::string op = tok().text;
        ++pos_;
        const Value right = parseRelational();
        left = compare(op, left, right);
    }
    return left;
}

Value Interpreter::parseRelational() {
    Value left = parseAdditive();
    while (atPunct("<") || atPunct("<=") || atPunct(">") || atPunct(">=")) {
        const std::string op = tok().text;
        ++pos_;
        const Value right = parseAdditive();
        left = compare(op, left, right);
    }
    return left;
}

Value Interpreter::parseAdditive() {
    Value left = parseMultiplicative();
    while (atPunct("+") || atPunct("-")) {
        const std::string op = tok().text;
        ++pos_;
        const Value right = parseMultiplicative();
        left = op == "+" ? addValues(left, right) : arithmetic('-', left, right);
    }
    return left;
}

Value Interpreter::parseMultiplicative() {
    Value left = parseUnary();
    while (atPunct("*") || atPunct("/") || atPunct("%")) {
        const char op = tok().text[0];
        ++pos_;
        const Value right = parseUnary();
        left = arithmetic(op, left, right);
    }
    return left;
}

Value Interpreter::parseUnary() {
    // Prefix ++x / --x: mutate the variable, yield the new value.
    if (atPunct("++") || atPunct("--")) {
        const bool inc = tok().text == "++";
        ++pos_;
        if (tok().kind == TokenKind::Identifier) {
            const std::string name = tok().text;
            ++pos_;
            const Value updated = inc ? addValues(lookup(name), Value::integer(1))
                                      : arithmetic('-', lookup(name), Value::integer(1));
            assign(name, updated);
            return updated;
        }
        return parseUnary();  // not an lvalue; ignore the operator
    }
    if (atPunct("-")) {
        ++pos_;
        return arithmetic('-', Value::integer(0), parseUnary());
    }
    if (atPunct("!")) {
        ++pos_;
        return Value::integer(parseUnary().toBool() ? 0 : 1);
    }
    if (atPunct("+")) {
        ++pos_;
        return parseUnary();
    }
    return parsePrimary();
}

std::vector<Value> Interpreter::parseArguments() {
    std::vector<Value> args;
    if (!atPunct("(")) {
        return args;
    }
    ++pos_;  // '('
    if (atPunct(")")) {
        ++pos_;
        return args;
    }
    while (pos_ < tokens_.size() && tok().kind != TokenKind::End) {
        args.push_back(parseExpression());
        if (atPunct(",")) {
            ++pos_;
            continue;
        }
        break;
    }
    if (atPunct(")")) {
        ++pos_;
    }
    return args;
}

Value Interpreter::callFunctionOrHost(const std::string& name, const std::vector<Value>& args) {
    const auto it = functions_.find(name);
    if (it == functions_.end()) {
        return host_.callBuiltin(name, args);
    }
    if (callDepth_ >= kMaxCallDepth) {
        return Value();
    }
    ++callDepth_;
    const Value result = runHandler(name, args);
    --callDepth_;
    return result;
}

Value Interpreter::parsePrimary() {
    const Token& t = tok();

    if (t.kind == TokenKind::Number) {
        const std::int64_t v = static_cast<std::int64_t>(std::strtoll(t.text.c_str(), nullptr, 0));
        ++pos_;
        return Value::integer(v);
    }
    if (t.kind == TokenKind::String) {
        Value v = Value::string(t.text);
        ++pos_;
        return v;
    }
    if (atPunct("(")) {
        ++pos_;
        Value v = parseExpression();
        if (atPunct(")")) {
            ++pos_;
        }
        return v;
    }
    if (t.kind == TokenKind::Identifier) {
        const std::string name = t.text;
        ++pos_;

        // Dotted access, possibly a chain (Group.HotSpot_Holder.DisableHotSpot).
        // Consume the whole chain so parsing stays in sync; the last name is the
        // member/method and everything before it is the (possibly compound)
        // component path.
        if (atPunct(".") && pos_ + 1 < tokens_.size() &&
            tokens_[pos_ + 1].kind == TokenKind::Identifier) {
            std::string component = name;
            std::string member;
            while (atPunct(".") && pos_ + 1 < tokens_.size() &&
                   tokens_[pos_ + 1].kind == TokenKind::Identifier) {
                ++pos_;  // '.'
                member = tok().text;
                ++pos_;
                if (atPunct(".")) {
                    component += "." + member;
                    continue;
                }
                break;
            }
            if (atPunct("(")) {
                const auto args = parseArguments();
                return host_.callComponent(component, member, args);
            }
            return Value();  // property read: not modeled yet
        }

        Value base;
        if (atPunct("(")) {
            const auto args = parseArguments();
            base = callFunctionOrHost(name, args);
        } else if (atPunct("++") || atPunct("--")) {
            // Postfix x++ / x--: yield the old value, then mutate the variable.
            const bool inc = tok().text == "++";
            ++pos_;
            const Value old = lookup(name);
            assign(name, inc ? addValues(old, Value::integer(1))
                             : arithmetic('-', old, Value::integer(1)));
            return old;
        } else {
            base = lookup(name);
        }

        // Array indexing is unsupported; consume it so parsing stays in sync.
        while (atPunct("[")) {
            ++pos_;
            parseExpression();
            if (atPunct("]")) {
                ++pos_;
            }
            base = Value();
        }
        return base;
    }

    // Unknown token: consume to guarantee progress.
    ++pos_;
    return Value();
}

bool Interpreter::atPunctAt(std::size_t index, const char* text) const {
    return index < tokens_.size() && tokens_[index].kind == TokenKind::Punctuator &&
           tokens_[index].text == text;
}

} // namespace graphboard::runtime
