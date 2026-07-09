#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace graphboard::runtime {

// Token kinds for the GraphBoard page-script language. The language is a small
// C dialect (see docs/runtime_recovery_notes.md): // and /* */ comments,
// int/CString declarations, if/else/while/switch, user functions, and dotted
// member calls like Sound_Holder.PlayDSound(0). Identifiers and numbers are
// ASCII; Polish (cp1250) bytes appear only inside string literals and comments.
enum class TokenKind {
    End,
    Identifier,
    Number,
    String,
    Punctuator,
};

struct Token {
    TokenKind kind = TokenKind::End;
    std::string text;        // spelling (string literals keep raw bytes, no quotes)
    std::size_t offset = 0;  // byte offset of the token start in the source
    std::size_t length = 0;  // byte length in the source (includes quotes for strings)
};

// Tokenize page-script source. Comments and whitespace are skipped; the final
// token is always TokenKind::End. Multi-character operators (==, !=, <=, >=,
// &&, ||, ++, --, +=, -=, *=, /=) are recognized as single punctuator tokens.
// Unterminated strings/comments are tolerated (consumed to end of input) so the
// scanner never throws on real, occasionally-sloppy scripts.
std::vector<Token> tokenize(const std::string& source);

} // namespace graphboard::runtime
