#pragma once

#include "graphboard/runtime/lexer.hpp"
#include "graphboard/runtime/script.hpp"
#include "graphboard/runtime/value.hpp"

#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace graphboard::runtime {

// The runtime environment a page script talks to. `callBuiltin` handles undotted
// host calls (LoadPage, SetCursor, FadeScreen, ...); `callComponent` handles
// dotted script->component dispatch (Sound_Holder.PlayDSound, ...). A real port
// backs these with the holder objects and the MFC view; tests back them with a
// recorder. User-function calls are resolved and executed by the interpreter
// itself and never reach the host.
class Host {
public:
    virtual ~Host() = default;
    virtual Value callBuiltin(const std::string& name, const std::vector<Value>& args) = 0;
    virtual Value callComponent(const std::string& component, const std::string& method,
                                const std::vector<Value>& args) = 0;
};

// A tree-walking interpreter for a single page's script text. Mirrors the
// recovered engine's model (see docs/runtime_recovery_notes.md): the script text
// is the program; top-level functions are event handlers / component callbacks /
// user functions; execution walks the tokens directly.
//
// Deliberate simplifications from the original engine, documented so a later
// pass can close them: expressions do not short-circuit && / || (both sides are
// evaluated); arrays, CRect/storage member objects, and float values are not yet
// modeled; the switch parse-cache is not used (semantics are identical, just
// unoptimized).
class Interpreter {
public:
    Interpreter(std::string source, Host& host);

    // Is `name` (e.g. "OnOpenPage" or "Sound_Holder.TheEnd") a defined
    // top-level function in this script?
    bool hasHandler(const std::string& name) const;

    // Run a top-level handler with the given arguments (bound positionally to
    // its parameters). Returns the handler's return value (Empty if none).
    // No-op returning Empty if the handler is not defined.
    Value runHandler(const std::string& name, const std::vector<Value>& args);

    // Global variables (script globals live in the bottom scope). Useful for
    // driving a page and asserting script-visible state in tests.
    const std::map<std::string, Value>& globals() const { return scopes_.front(); }
    bool hasGlobal(const std::string& name) const;
    Value getGlobal(const std::string& name) const;

private:
    enum class Flow { Normal, Return, Break };

    struct Function {
        std::vector<std::string> params;
        std::size_t bodyOpen = 0;   // token index of '{'
        std::size_t bodyClose = 0;  // token index of matching '}'
    };

    // Execution over the token stream.
    Flow execBlock(std::size_t openBrace);
    Flow execStatement();
    Flow execIf();
    Flow execWhile();
    Flow execSwitch();
    Flow execDeclaration();
    void skipStatement();
    void skipParens();
    void skipBraces();

    // Expression evaluation (precedence climbing).
    Value parseExpression();
    Value parseOr();
    Value parseAnd();
    Value parseEquality();
    Value parseRelational();
    Value parseAdditive();
    Value parseMultiplicative();
    Value parseUnary();
    Value parsePrimary();
    std::vector<Value> parseArguments();  // pos_ at '(' -> consumes through ')'
    Value callFunctionOrHost(const std::string& name, const std::vector<Value>& args);

    // Scope helpers.
    Value lookup(const std::string& name) const;
    void assign(const std::string& name, Value value);   // nearest existing, else global
    void declare(const std::string& name, Value value);  // current scope

    const Token& tok() const { return tokens_[pos_]; }
    bool atPunct(const char* text) const;
    bool atPunctAt(std::size_t index, const char* text) const;
    bool atKeyword(const char* word) const;

    std::string source_;
    Host& host_;
    std::vector<Token> tokens_;
    std::map<std::string, Function> functions_;
    std::vector<std::map<std::string, Value>> scopes_;

    std::size_t pos_ = 0;
    Value returnValue_;
    int callDepth_ = 0;

    // True only while OnOpenPage's own body runs: the page initializer, whose
    // variable declarations the engine promotes to page-globals visible to
    // every other handler (the scripts' own comment: "All variables definied on
    // this function are global for this page"). Nested user-function calls reset
    // it via runHandler so their locals stay local.
    bool declareToGlobal_ = false;
};

} // namespace graphboard::runtime
