#pragma once

#include "graphboard/runtime/lexer.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace graphboard::runtime {

// How a top-level script function is bound by the runtime.
enum class HandlerKind {
    PageEvent,          // OnOpenPage, OnLButtonDown, OnTimer, ...
    ComponentCallback,  // dotted name: Component.Method (e.g. Sound_Holder.TheEnd)
    UserFunction,       // any other top-level function
};

struct ScriptParameter {
    std::string type;   // "int", "CString", ... ("" if none written)
    std::string name;   // parameter name ("" for "void" / empty lists)
};

// A top-level function definition discovered in the page script. `component`
// and `method` are set only for ComponentCallback; otherwise `method` holds the
// bare function name.
struct ScriptHandler {
    HandlerKind kind = HandlerKind::UserFunction;
    std::string returnType;   // "" when omitted (e.g. OnOpenPage())
    std::string name;         // full name as written ("Sound_Holder.TheEnd")
    std::string component;    // ComponentCallback only
    std::string method;       // method / function name
    std::vector<ScriptParameter> parameters;
    std::size_t nameOffset = 0;  // byte offset of the name in the source
    std::size_t bodyOffset = 0;  // byte offset of the opening '{'
    std::size_t bodyEndOffset = 0;  // byte offset just past the matching '}'
};

// The authoritative page-event lookup table
// (GraphBrdScript_PageEventNameTable, Tuwim.exe:0043c444). A discovered
// top-level function whose (undotted) name is in this set is a PageEvent.
bool isPageEventName(const std::string& name);
const std::vector<std::string>& pageEventNames();

// The authoritative host builtin dispatch table
// (GraphBrdScript_HostBuiltinNameTable, Tuwim.exe:0043b62c), in index order.
// These are the 24 undotted calls the host implements directly; anything else
// undotted is a page-local user function.
bool isHostBuiltinName(const std::string& name);
const std::vector<std::string>& hostBuiltinNames();

// Discover top-level function definitions in page-script source. Robust to the
// optional return type and to nested braces/strings/comments inside bodies.
std::vector<ScriptHandler> discoverHandlers(const std::string& source);

// Convenience overload when tokens are already available.
std::vector<ScriptHandler> discoverHandlers(const std::string& source,
                                            const std::vector<Token>& tokens);

// A call site found inside a function body: either a builtin (undotted, e.g.
// LoadPage) or a component method (dotted, e.g. Sound_Holder.PlayDSound). These
// are the runtime API surface the interpreter must implement. Control-flow
// keywords (if/while/for/switch/return/sizeof) are not reported.
struct ScriptCall {
    bool isBuiltin = true;   // false => component method (component set)
    std::string component;   // component method only
    std::string name;        // builtin name or method name
    std::size_t offset = 0;  // byte offset of the callee identifier
};

std::vector<ScriptCall> collectCalls(const std::string& source);
std::vector<ScriptCall> collectCalls(const std::string& source,
                                     const std::vector<Token>& tokens);

} // namespace graphboard::runtime
