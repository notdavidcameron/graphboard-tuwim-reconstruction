#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace graphboard {

// A 16-byte OLE class id (CLSID), stored on disk exactly as MFC writes it:
// Data1 (u32) and Data2/Data3 (u16) are little-endian; Data4[8] is a raw byte
// sequence. The canonical string form byte-swaps the first three fields, e.g.
// bytes {16 81 76 DA 41 53 D0 11 B4 44 00 80 48 EB 5D 40} render as
// "DA768116-5341-11D0-B444-008048EB5D40".
struct Guid {
    std::array<std::uint8_t, 16> bytes{};

    bool operator==(const Guid& other) const { return bytes == other.bytes; }
    bool operator!=(const Guid& other) const { return bytes != other.bytes; }
    bool operator<(const Guid& other) const { return bytes < other.bytes; }

    std::string toString() const;
    static Guid fromString(const std::string& text);
};

namespace detail {

inline char hexNibble(std::uint8_t value) {
    return static_cast<char>(value < 10 ? '0' + value : 'A' + (value - 10));
}

inline std::uint8_t parseNibble(char c) {
    if (c >= '0' && c <= '9') return static_cast<std::uint8_t>(c - '0');
    if (c >= 'a' && c <= 'f') return static_cast<std::uint8_t>(c - 'a' + 10);
    if (c >= 'A' && c <= 'F') return static_cast<std::uint8_t>(c - 'A' + 10);
    throw std::invalid_argument("Guid::fromString: non-hex digit");
}

inline void appendByte(std::string& out, std::uint8_t value) {
    out.push_back(hexNibble(static_cast<std::uint8_t>(value >> 4)));
    out.push_back(hexNibble(static_cast<std::uint8_t>(value & 0x0f)));
}

} // namespace detail

inline std::string Guid::toString() const {
    std::string out;
    out.reserve(36);
    // Data1: little-endian u32 -> print big-endian.
    for (int i = 3; i >= 0; --i) detail::appendByte(out, bytes[static_cast<std::size_t>(i)]);
    out.push_back('-');
    // Data2: little-endian u16.
    detail::appendByte(out, bytes[5]);
    detail::appendByte(out, bytes[4]);
    out.push_back('-');
    // Data3: little-endian u16.
    detail::appendByte(out, bytes[7]);
    detail::appendByte(out, bytes[6]);
    out.push_back('-');
    // Data4[0..1]: raw order.
    detail::appendByte(out, bytes[8]);
    detail::appendByte(out, bytes[9]);
    out.push_back('-');
    // Data4[2..7]: raw order.
    for (std::size_t i = 10; i < 16; ++i) detail::appendByte(out, bytes[i]);
    return out;
}

inline Guid Guid::fromString(const std::string& text) {
    // Accept canonical "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" (dashes optional).
    std::string hex;
    hex.reserve(32);
    for (const char c : text) {
        if (c == '-' || c == '{' || c == '}') continue;
        hex.push_back(c);
    }
    if (hex.size() != 32) {
        throw std::invalid_argument("Guid::fromString: expected 32 hex digits");
    }

    std::array<std::uint8_t, 16> big{};
    for (std::size_t i = 0; i < 16; ++i) {
        big[i] = static_cast<std::uint8_t>(
            (detail::parseNibble(hex[i * 2]) << 4) | detail::parseNibble(hex[i * 2 + 1]));
    }

    Guid guid;
    // Reverse the little-endian fields back to on-disk byte order.
    guid.bytes[0] = big[3];
    guid.bytes[1] = big[2];
    guid.bytes[2] = big[1];
    guid.bytes[3] = big[0];
    guid.bytes[4] = big[5];
    guid.bytes[5] = big[4];
    guid.bytes[6] = big[7];
    guid.bytes[7] = big[6];
    for (std::size_t i = 8; i < 16; ++i) guid.bytes[i] = big[i];
    return guid;
}

} // namespace graphboard
