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

std::string formatCString(const std::vector<Value>& args) {
    if (args.empty()) return {};
    const std::string format = args[0].toString();
    std::string result;
    std::size_t argument = 1;
    for (std::size_t i = 0; i < format.size(); ++i) {
        if (format[i] != '%' || i + 1 >= format.size()) {
            result.push_back(format[i]);
            continue;
        }
        const char specifier = format[++i];
        if (specifier == '%') {
            result.push_back('%');
        } else if ((specifier == 's' || specifier == 'd') && argument < args.size()) {
            result += specifier == 'd' ? std::to_string(args[argument].toInt())
                                       : args[argument].toString();
            ++argument;
        } else {
            result.push_back('%');
            result.push_back(specifier);
        }
    }
    return result;
}

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

        // The definition starts at the name, or one token earlier when a return
        // type was written ("void OnTimer" / "int square"). A dotted callback
        // name ("Sound_Holder.TheEnd") starts at its component token, which is
        // where nameOffset points.
        const auto nameIt = offsetToTok.find(handler.nameOffset);
        if (nameIt != offsetToTok.end()) {
            std::size_t defStart = nameIt->second;
            if (!handler.returnType.empty() && defStart > 0) {
                --defStart;
            }
            functionSpans_[defStart] = fn.bodyClose;
        }
    }
}

void Interpreter::runGlobalSetup() {
    // Top-level statements only: the global block is a list of declarations.
    // Function definitions are stepped over, never executed.
    const bool savedDeclareToGlobal = declareToGlobal_;
    declareToGlobal_ = true;  // no scope is pushed here, but be explicit
    pos_ = 0;
    while (pos_ < tokens_.size() && tok().kind != TokenKind::End) {
        const auto span = functionSpans_.find(pos_);
        if (span != functionSpans_.end()) {
            pos_ = span->second + 1;  // just past the body's '}'
            continue;
        }
        const std::size_t before = pos_;
        execStatement();
        if (pos_ == before) {
            ++pos_;  // guarantee progress on anything unmodeled
        }
    }
    declareToGlobal_ = savedDeclareToGlobal;
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

void Interpreter::setGlobal(const std::string& name, Value value) {
    scopes_.front()[name] = std::move(value);
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

    // OnOpenPage is the page initializer: the variables it declares become
    // page-globals (see declareToGlobal_). Save/restore around the run so a
    // user function OnOpenPage calls still gets ordinary locals.
    const bool savedDeclareToGlobal = declareToGlobal_;
    declareToGlobal_ = (name == "OnOpenPage");

    const std::size_t savedPos = pos_;
    const Value savedReturn = returnValue_;
    returnValue_ = Value();
    execBlock(fn.bodyOpen);
    Value result = returnValue_;

    returnValue_ = savedReturn;
    pos_ = savedPos;
    declareToGlobal_ = savedDeclareToGlobal;
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
    // Sparse array/object fields belong to the same scope as their root
    // declaration. This keeps a local `CRect rect` local while allowing
    // OnOpenPage's promoted `filmy[25]` fields to persist as page globals.
    const auto separator = name.find_first_of("[.");
    if (separator != std::string::npos) {
        const std::string root = name.substr(0, separator);
        for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
            if (it->find(root) != it->end()) {
                (*it)[name] = std::move(value);
                return;
            }
        }
    }
    scopes_.front()[name] = std::move(value);  // undeclared -> page global
}

void Interpreter::declare(const std::string& name, Value value) {
    // Inside OnOpenPage every declaration is a page-global (the engine's
    // documented rule), so it survives for later event handlers/callbacks.
    auto& scope = declareToGlobal_ ? scopes_.front() : scopes_.back();
    scope[name] = std::move(value);
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

    // Assignment to a scalar, sparse array element, or object member. Scan the
    // postfix chain without evaluating it first so an index expression is
    // executed exactly once when parseVariablePath consumes the real lvalue.
    bool pathAssignment = false;
    if (tok().kind == TokenKind::Identifier) {
        std::size_t scan = pos_ + 1;
        while (scan < tokens_.size()) {
            if (atPunctAt(scan, "[")) {
                int depth = 1;
                ++scan;
                while (scan < tokens_.size() && depth > 0) {
                    if (atPunctAt(scan, "[")) ++depth;
                    else if (atPunctAt(scan, "]")) --depth;
                    ++scan;
                }
                continue;
            }
            if (atPunctAt(scan, ".") && scan + 1 < tokens_.size() &&
                tokens_[scan + 1].kind == TokenKind::Identifier) {
                scan += 2;
                continue;
            }
            break;
        }
        pathAssignment = scan < tokens_.size() && isAssignOp(tokens_[scan]);
    }
    if (pathAssignment) {
        const std::string name = parseVariablePath();
        const std::string op = tok().text;
        ++pos_;
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
        // Array dimension(s): consume the bracketed size. Elements are created
        // lazily as scoped synthetic paths when they are assigned.
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
            const std::string name = parseVariablePath();
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

std::vector<Value> Interpreter::parseArguments(std::vector<OutReference>* outRefs) {
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
        // &name passes a variable by address (GetDeep(id, &deep)): send its
        // current value and record the name for the post-call write-back.
        if (atPunct("&") && pos_ + 1 < tokens_.size() &&
            tokens_[pos_ + 1].kind == TokenKind::Identifier) {
            ++pos_;
            const std::string refName = tok().text;
            ++pos_;
            if (outRefs) {
                outRefs->push_back({args.size(), refName});
            }
            args.push_back(lookup(refName));
            if (atPunct(",")) {
                ++pos_;
                continue;
            }
            break;
        }
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

std::string Interpreter::parseVariablePath() {
    if (tok().kind != TokenKind::Identifier) return {};
    std::string path = tok().text;
    ++pos_;
    while (pos_ < tokens_.size()) {
        if (atPunct("[")) {
            ++pos_;
            const auto index = parseExpression().toInt();
            if (atPunct("]")) ++pos_;
            path += "[" + std::to_string(index) + "]";
            continue;
        }
        if (atPunct(".") && pos_ + 1 < tokens_.size() &&
            tokens_[pos_ + 1].kind == TokenKind::Identifier) {
            ++pos_;
            path += "." + tok().text;
            ++pos_;
            continue;
        }
        break;
    }
    return path;
}

Value Interpreter::parsePrimary() {
    const Token& t = tok();

    if (t.kind == TokenKind::Number) {
        const bool real = t.text.find_first_of(".eE") != std::string::npos;
        const Value value = real
                                ? Value::real(std::strtod(t.text.c_str(), nullptr))
                                : Value::integer(static_cast<std::int64_t>(
                                      std::strtoll(t.text.c_str(), nullptr, 0)));
        ++pos_;
        return value;
    }
    if (t.kind == TokenKind::String) {
        Value v = Value::string(t.text);
        ++pos_;
        return v;
    }
    if (atPunct("(")) {
        ++pos_;
        Value v = parseExpression();
        if (atPunct(")")) ++pos_;
        return v;
    }
    if (t.kind == TokenKind::Identifier) {
        std::string path = t.text;
        ++pos_;

        // An undotted call is a user function or host builtin.
        if (atPunct("(")) {
            return callFunctionOrHost(path, parseArguments());
        }

        // Apply index and member postfixes in source order. A final member
        // followed by '(' is either a local CString/CRect method or a holder
        // call; a member without '(' remains part of the sparse variable key.
        while (pos_ < tokens_.size()) {
            if (atPunct("[")) {
                ++pos_;
                const auto index = parseExpression().toInt();
                if (atPunct("]")) ++pos_;
                path += "[" + std::to_string(index) + "]";
                continue;
            }
            if (!(atPunct(".") && pos_ + 1 < tokens_.size() &&
                  tokens_[pos_ + 1].kind == TokenKind::Identifier)) {
                break;
            }
            ++pos_;  // '.'
            const std::string member = tok().text;
            ++pos_;
            if (!atPunct("(")) {
                path += "." + member;
                continue;
            }

            std::vector<OutReference> outRefs;
            const auto args = parseArguments(&outRefs);
            if (member == "SetRect") {
                static const char* fields[] = {"left", "top", "right", "bottom"};
                for (std::size_t i = 0; i < 4; ++i) {
                    assign(path + "." + fields[i],
                           i < args.size() ? args[i] : Value::integer(0));
                }
                return Value();
            }
            if (member == "SetString") {
                const Value value = args.empty() ? Value::string("")
                                                 : Value::string(args[0].toString());
                assign(path, value);
                return value;
            }
            if (member == "Empty") {
                const Value value = Value::string("");
                assign(path, value);
                return value;
            }
            if (member == "GetLength") {
                return Value::integer(static_cast<std::int64_t>(lookup(path).toString().size()));
            }
            if (member == "GetString") {
                return Value::string(lookup(path).toString());
            }
            if (member == "Format") {
                const Value value = Value::string(formatCString(args));
                assign(path, value);
                return value;
            }

            auto result = host_.callComponent(path, member, args);
            for (const auto& [argument, variable] : outRefs) {
                const auto value = result.outArguments.find(argument);
                if (value != result.outArguments.end()) assign(variable, value->second);
            }
            return result.value;
        }

        const Value base = lookup(path);
        if (atPunct("++") || atPunct("--")) {
            const bool inc = tok().text == "++";
            ++pos_;
            assign(path, inc ? addValues(base, Value::integer(1))
                             : arithmetic('-', base, Value::integer(1)));
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
