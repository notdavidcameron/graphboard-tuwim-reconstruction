#include "graphboard/runtime/script.hpp"

#include <algorithm>

namespace graphboard::runtime {

namespace {

bool isPunct(const Token& token, const char* text) {
    return token.kind == TokenKind::Punctuator && token.text == text;
}

// Parse a parameter list from the tokens strictly between '(' and ')'.
std::vector<ScriptParameter> parseParameters(const std::vector<Token>& tokens,
                                             std::size_t open, std::size_t close) {
    std::vector<ScriptParameter> params;
    std::size_t i = open + 1;
    while (i < close) {
        // Collect identifiers/numbers up to the next top-level comma.
        std::vector<std::string> words;
        while (i < close && !isPunct(tokens[i], ",")) {
            if (tokens[i].kind == TokenKind::Identifier) {
                words.push_back(tokens[i].text);
            }
            ++i;
        }
        if (i < close) {
            ++i;  // skip comma
        }
        if (words.empty()) {
            continue;
        }
        if (words.size() == 1 && words[0] == "void") {
            continue;  // "(void)" means no parameters
        }
        ScriptParameter param;
        param.type = words[0];
        if (words.size() >= 2) {
            param.name = words.back();
        }
        params.push_back(std::move(param));
    }
    return params;
}

} // namespace

const std::vector<std::string>& pageEventNames() {
    // The authoritative page-event lookup table, GraphBrdScript_PageEventNameTable
    // (Tuwim.exe:0043c444), in the host's own order. Membership is what matters.
    static const std::vector<std::string> names = {
        "OnTimer",         "OnKeyDown",       "OnKeyUp",         "OnLButtonDown",
        "OnLButtonUp",     "OnRButtonDown",   "OnRButtonUp",     "OnClosePage",
        "OnOpenPage",      "OnMouseMoveStart", "OnMouseMoveStop",
    };
    return names;
}

bool isPageEventName(const std::string& name) {
    const auto& names = pageEventNames();
    return std::find(names.begin(), names.end(), name) != names.end();
}

const std::vector<std::string>& hostBuiltinNames() {
    // The authoritative host builtin dispatch table,
    // GraphBrdScript_HostBuiltinNameTable (Tuwim.exe:0043b62c), in index order.
    // An undotted script call resolves to a user function first (if defined in
    // the page), else one of these builtins.
    static const std::vector<std::string> names = {
        "MessageBox",         "Random",             "FadeScreen",
        "LoadPage",           "Exit",               "SetTimer",
        "SetCursor",          "CreateDirectSound",  "ReleaseDirectSound",
        "EnableScreen",       "DisableScreen",      "ShowBuffer",
        "IsProject",          "Debug",              "LoadGroup",
        "CloseGroup",         "SetDisplayMode",     "PrintBuffer",
        "Execute",            "GetProgramPath",     "GetCommandLine",
        "DeleteFile",         "ShellExecute",       "GetResourcePath",
    };
    return names;
}

bool isHostBuiltinName(const std::string& name) {
    const auto& names = hostBuiltinNames();
    return std::find(names.begin(), names.end(), name) != names.end();
}

std::vector<ScriptHandler> discoverHandlers(const std::string& source,
                                            const std::vector<Token>& tokens) {
    std::vector<ScriptHandler> handlers;
    int braceDepth = 0;

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        if (isPunct(token, "{")) {
            ++braceDepth;
            continue;
        }
        if (isPunct(token, "}")) {
            if (braceDepth > 0) {
                --braceDepth;
            }
            continue;
        }
        if (braceDepth != 0 || !isPunct(token, "(")) {
            continue;
        }

        // A '(' at brace depth 0. Identify the name immediately before it.
        if (i == 0 || tokens[i - 1].kind != TokenKind::Identifier) {
            continue;
        }
        const std::size_t methodTok = i - 1;
        std::size_t nameStartTok = methodTok;
        while (nameStartTok >= 2 && isPunct(tokens[nameStartTok - 1], ".") &&
               tokens[nameStartTok - 2].kind == TokenKind::Identifier) {
            nameStartTok -= 2;
        }
        const bool dotted = nameStartTok != methodTok;

        // Find the matching ')'.
        int parenDepth = 0;
        std::size_t close = i;
        for (std::size_t j = i; j < tokens.size(); ++j) {
            if (isPunct(tokens[j], "(")) {
                ++parenDepth;
            } else if (isPunct(tokens[j], ")")) {
                --parenDepth;
                if (parenDepth == 0) {
                    close = j;
                    break;
                }
            } else if (tokens[j].kind == TokenKind::End) {
                break;
            }
        }
        if (parenDepth != 0) {
            continue;  // unbalanced; not a definition
        }

        // A function definition requires '{' immediately after ')'.
        if (close + 1 >= tokens.size() || !isPunct(tokens[close + 1], "{")) {
            continue;
        }

        ScriptHandler handler;
        handler.nameOffset = tokens[nameStartTok].offset;
        if (dotted) {
            handler.kind = HandlerKind::ComponentCallback;
            for (std::size_t part = nameStartTok; part + 1 < methodTok; part += 2) {
                if (!handler.component.empty()) handler.component += ".";
                handler.component += tokens[part].text;
            }
            handler.method = tokens[methodTok].text;
            handler.name = handler.component + "." + handler.method;
        } else {
            handler.method = tokens[methodTok].text;
            handler.name = handler.method;
            handler.kind = isPageEventName(handler.method) ? HandlerKind::PageEvent
                                                           : HandlerKind::UserFunction;
        }

        // Optional return type: an identifier immediately before the name.
        if (nameStartTok > 0 && tokens[nameStartTok - 1].kind == TokenKind::Identifier) {
            handler.returnType = tokens[nameStartTok - 1].text;
        }

        handler.parameters = parseParameters(tokens, i, close);

        // Body span: from '{' to its matching '}'.
        const std::size_t bodyOpen = close + 1;
        handler.bodyOffset = tokens[bodyOpen].offset;
        int bd = 0;
        std::size_t bodyClose = bodyOpen;
        for (std::size_t j = bodyOpen; j < tokens.size(); ++j) {
            if (isPunct(tokens[j], "{")) {
                ++bd;
            } else if (isPunct(tokens[j], "}")) {
                --bd;
                if (bd == 0) {
                    bodyClose = j;
                    break;
                }
            } else if (tokens[j].kind == TokenKind::End) {
                break;
            }
        }
        handler.bodyEndOffset = tokens[bodyClose].offset + tokens[bodyClose].length;
        handlers.push_back(std::move(handler));

        // Continue scanning from just inside the body; the global brace counter
        // resumes tracking so nested definitions are never misread.
        (void)source;
    }

    return handlers;
}

std::vector<ScriptHandler> discoverHandlers(const std::string& source) {
    return discoverHandlers(source, tokenize(source));
}

namespace {

bool isControlKeyword(const std::string& word) {
    return word == "if" || word == "while" || word == "for" || word == "switch" ||
           word == "return" || word == "sizeof" || word == "else" || word == "do";
}

} // namespace

std::vector<ScriptCall> collectCalls(const std::string& source,
                                     const std::vector<Token>& tokens) {
    std::vector<ScriptCall> calls;
    int braceDepth = 0;

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        if (isPunct(token, "{")) {
            ++braceDepth;
            continue;
        }
        if (isPunct(token, "}")) {
            if (braceDepth > 0) {
                --braceDepth;
            }
            continue;
        }
        // Only look for calls inside a body; top-level '(' are definitions.
        if (braceDepth == 0 || !isPunct(token, "(")) {
            continue;
        }
        if (i == 0 || tokens[i - 1].kind != TokenKind::Identifier) {
            continue;
        }
        const std::string& callee = tokens[i - 1].text;
        if (isControlKeyword(callee)) {
            continue;
        }

        ScriptCall call;
        call.name = callee;
        call.offset = tokens[i - 1].offset;
        if (i >= 3 && isPunct(tokens[i - 2], ".") &&
            tokens[i - 3].kind == TokenKind::Identifier) {
            call.isBuiltin = false;
            call.component = tokens[i - 3].text;
        }
        calls.push_back(std::move(call));
    }

    (void)source;
    return calls;
}

std::vector<ScriptCall> collectCalls(const std::string& source) {
    return collectCalls(source, tokenize(source));
}

} // namespace graphboard::runtime
