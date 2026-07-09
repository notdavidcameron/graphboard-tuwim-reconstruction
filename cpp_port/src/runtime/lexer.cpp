#include "graphboard/runtime/lexer.hpp"

namespace graphboard::runtime {

namespace {

bool isIdentifierStart(unsigned char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

bool isIdentifierPart(unsigned char c) {
    return isIdentifierStart(c) || (c >= '0' && c <= '9');
}

bool isDigit(unsigned char c) {
    return c >= '0' && c <= '9';
}

bool isSpace(unsigned char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f';
}

// Two-character operators the language uses. Order does not matter; each is
// matched greedily before falling back to a single-character punctuator.
bool isTwoCharOperator(char a, char b) {
    switch (a) {
        case '=': return b == '=';
        case '!': return b == '=';
        case '<': return b == '=';
        case '>': return b == '=';
        case '&': return b == '&';
        case '|': return b == '|';
        case '+': return b == '+' || b == '=';
        case '-': return b == '-' || b == '=';
        case '*': return b == '=';
        case '/': return b == '=';
        default: return false;
    }
}

} // namespace

std::vector<Token> tokenize(const std::string& source) {
    std::vector<Token> tokens;
    const std::size_t n = source.size();
    std::size_t i = 0;

    while (i < n) {
        const unsigned char c = static_cast<unsigned char>(source[i]);

        if (isSpace(c)) {
            ++i;
            continue;
        }

        // Line comment.
        if (c == '/' && i + 1 < n && source[i + 1] == '/') {
            i += 2;
            while (i < n && source[i] != '\n') {
                ++i;
            }
            continue;
        }

        // Block comment (tolerate unterminated).
        if (c == '/' && i + 1 < n && source[i + 1] == '*') {
            i += 2;
            while (i + 1 < n && !(source[i] == '*' && source[i + 1] == '/')) {
                ++i;
            }
            i = (i + 1 < n) ? i + 2 : n;
            continue;
        }

        // String literal. Text excludes the surrounding quotes; length includes
        // them. Backslash escapes are consumed so an escaped quote does not end
        // the string.
        if (c == '"') {
            const std::size_t start = i;
            ++i;
            std::string text;
            while (i < n && source[i] != '"') {
                if (source[i] == '\\' && i + 1 < n) {
                    text.push_back(source[i]);
                    text.push_back(source[i + 1]);
                    i += 2;
                    continue;
                }
                text.push_back(source[i]);
                ++i;
            }
            const bool terminated = i < n;
            if (terminated) {
                ++i;  // closing quote
            }
            tokens.push_back({TokenKind::String, std::move(text), start, i - start});
            continue;
        }

        if (isIdentifierStart(c)) {
            const std::size_t start = i;
            while (i < n && isIdentifierPart(static_cast<unsigned char>(source[i]))) {
                ++i;
            }
            tokens.push_back({TokenKind::Identifier, source.substr(start, i - start), start, i - start});
            continue;
        }

        if (isDigit(c)) {
            const std::size_t start = i;
            // Accept decimal and hex integer/float spellings; the scanner keeps
            // the raw text and leaves numeric interpretation to the parser.
            while (i < n && (isIdentifierPart(static_cast<unsigned char>(source[i])) ||
                             source[i] == '.')) {
                ++i;
            }
            tokens.push_back({TokenKind::Number, source.substr(start, i - start), start, i - start});
            continue;
        }

        // Punctuator (two-character operator, else single character).
        const std::size_t start = i;
        if (i + 1 < n && isTwoCharOperator(source[i], source[i + 1])) {
            tokens.push_back({TokenKind::Punctuator, source.substr(start, 2), start, 2});
            i += 2;
        } else {
            tokens.push_back({TokenKind::Punctuator, source.substr(start, 1), start, 1});
            ++i;
        }
    }

    tokens.push_back({TokenKind::End, "", n, 0});
    return tokens;
}

} // namespace graphboard::runtime
