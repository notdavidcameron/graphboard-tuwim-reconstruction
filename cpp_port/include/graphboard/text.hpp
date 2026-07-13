#pragma once

#include <cstdint>
#include <string>

namespace graphboard {

// GraphBoard strings (page names, holder names, script text, the poem texts)
// are Windows-1250 (Central European) byte strings. This converts them to
// UTF-8 for display/JSON. Bytes 0x00-0x7f pass through; the upper half uses
// the standard CP1250 table with U+FFFD for the five unassigned codes.
inline std::string cp1250ToUtf8(const std::string& input) {
    static const std::uint16_t kUpper[128] = {
        0x20AC, 0xFFFD, 0x201A, 0xFFFD, 0x201E, 0x2026, 0x2020, 0x2021,  // 0x80
        0xFFFD, 0x2030, 0x0160, 0x2039, 0x015A, 0x0164, 0x017D, 0x0179,  // 0x88
        0xFFFD, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,  // 0x90
        0xFFFD, 0x2122, 0x0161, 0x203A, 0x015B, 0x0165, 0x017E, 0x017A,  // 0x98
        0x00A0, 0x02C7, 0x02D8, 0x0141, 0x00A4, 0x0104, 0x00A6, 0x00A7,  // 0xa0
        0x00A8, 0x00A9, 0x015E, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x017B,  // 0xa8
        0x00B0, 0x00B1, 0x02DB, 0x0142, 0x00B4, 0x00B5, 0x00B6, 0x00B7,  // 0xb0
        0x00B8, 0x0105, 0x015F, 0x00BB, 0x013D, 0x02DD, 0x013E, 0x017C,  // 0xb8
        0x0154, 0x00C1, 0x00C2, 0x0102, 0x00C4, 0x0139, 0x0106, 0x00C7,  // 0xc0
        0x010C, 0x00C9, 0x0118, 0x00CB, 0x011A, 0x00CD, 0x00CE, 0x010E,  // 0xc8
        0x0110, 0x0143, 0x0147, 0x00D3, 0x00D4, 0x0150, 0x00D6, 0x00D7,  // 0xd0
        0x0158, 0x016E, 0x00DA, 0x0170, 0x00DC, 0x00DD, 0x0162, 0x00DF,  // 0xd8
        0x0155, 0x00E1, 0x00E2, 0x0103, 0x00E4, 0x013A, 0x0107, 0x00E7,  // 0xe0
        0x010D, 0x00E9, 0x0119, 0x00EB, 0x011B, 0x00ED, 0x00EE, 0x010F,  // 0xe8
        0x0111, 0x0144, 0x0148, 0x00F3, 0x00F4, 0x0151, 0x00F6, 0x00F7,  // 0xf0
        0x0159, 0x016F, 0x00FA, 0x0171, 0x00FC, 0x00FD, 0x0163, 0x02D9,  // 0xf8
    };

    std::string out;
    out.reserve(input.size());
    for (const char raw : input) {
        const auto byte = static_cast<unsigned char>(raw);
        if (byte < 0x80) {
            out.push_back(raw);
            continue;
        }
        const std::uint16_t cp = kUpper[byte - 0x80];
        if (cp < 0x800) {
            out.push_back(static_cast<char>(0xc0 | (cp >> 6)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3f)));
        } else {
            out.push_back(static_cast<char>(0xe0 | (cp >> 12)));
            out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3f)));
            out.push_back(static_cast<char>(0x80 | (cp & 0x3f)));
        }
    }
    return out;
}

// Strict inverse used by the authoring/export path. Returns an empty string
// only when the input is empty; `ok` is cleared for malformed UTF-8 or a code
// point with no CP1250 representation. Unrepresentable bytes are not silently
// substituted because doing so would corrupt script offsets and filenames.
inline std::string utf8ToCp1250(const std::string& input, bool* ok = nullptr) {
    static const std::uint16_t kUpper[128] = {
        0x20AC,0xFFFD,0x201A,0xFFFD,0x201E,0x2026,0x2020,0x2021,
        0xFFFD,0x2030,0x0160,0x2039,0x015A,0x0164,0x017D,0x0179,
        0xFFFD,0x2018,0x2019,0x201C,0x201D,0x2022,0x2013,0x2014,
        0xFFFD,0x2122,0x0161,0x203A,0x015B,0x0165,0x017E,0x017A,
        0x00A0,0x02C7,0x02D8,0x0141,0x00A4,0x0104,0x00A6,0x00A7,
        0x00A8,0x00A9,0x015E,0x00AB,0x00AC,0x00AD,0x00AE,0x017B,
        0x00B0,0x00B1,0x02DB,0x0142,0x00B4,0x00B5,0x00B6,0x00B7,
        0x00B8,0x0105,0x015F,0x00BB,0x013D,0x02DD,0x013E,0x017C,
        0x0154,0x00C1,0x00C2,0x0102,0x00C4,0x0139,0x0106,0x00C7,
        0x010C,0x00C9,0x0118,0x00CB,0x011A,0x00CD,0x00CE,0x010E,
        0x0110,0x0143,0x0147,0x00D3,0x00D4,0x0150,0x00D6,0x00D7,
        0x0158,0x016E,0x00DA,0x0170,0x00DC,0x00DD,0x0162,0x00DF,
        0x0155,0x00E1,0x00E2,0x0103,0x00E4,0x013A,0x0107,0x00E7,
        0x010D,0x00E9,0x0119,0x00EB,0x011B,0x00ED,0x00EE,0x010F,
        0x0111,0x0144,0x0148,0x00F3,0x00F4,0x0151,0x00F6,0x00F7,
        0x0159,0x016F,0x00FA,0x0171,0x00FC,0x00FD,0x0163,0x02D9,
    };
    bool valid = true;
    std::string out;
    for (std::size_t i = 0; i < input.size();) {
        const auto first = static_cast<unsigned char>(input[i]);
        std::uint32_t cp = 0;
        std::size_t bytes = 0;
        std::uint32_t minimum = 0;
        if (first < 0x80) { cp = first; bytes = 1; }
        else if ((first & 0xe0) == 0xc0) { cp = first & 0x1f; bytes = 2; minimum = 0x80; }
        else if ((first & 0xf0) == 0xe0) { cp = first & 0x0f; bytes = 3; minimum = 0x800; }
        else if ((first & 0xf8) == 0xf0) { cp = first & 0x07; bytes = 4; minimum = 0x10000; }
        else { valid = false; break; }
        if (i + bytes > input.size()) { valid = false; break; }
        for (std::size_t j = 1; j < bytes; ++j) {
            const auto continuation = static_cast<unsigned char>(input[i + j]);
            if ((continuation & 0xc0) != 0x80) { valid = false; break; }
            cp = (cp << 6) | (continuation & 0x3f);
        }
        if (!valid) break;
        if (cp < minimum || cp > 0x10ffff || (cp >= 0xd800 && cp <= 0xdfff)) {
            valid = false;
            break;
        }
        if (cp < 0x80) out.push_back(static_cast<char>(cp));
        else {
            std::size_t found = 128;
            for (std::size_t u = 0; u < 128; ++u) if (kUpper[u] == cp) { found = u; break; }
            if (found == 128) { valid = false; break; }
            out.push_back(static_cast<char>(0x80 + found));
        }
        i += bytes;
    }
    if (ok) *ok = valid;
    return valid ? out : std::string{};
}

inline bool isCp1250EncodableUtf8(const std::string& input) {
    bool ok = false;
    (void)utf8ToCp1250(input, &ok);
    return ok;
}

// Case-insensitive key matching Windows-1250 filenames as the original
// Windows engine and the host filesystem do. The returned key is an internal
// byte string; it is not intended for display.
inline std::string cp1250CaseFoldKey(const std::string& utf8) {
    bool ok=false;auto key=utf8ToCp1250(utf8,&ok);if(!ok)key=utf8;
    for(char& raw:key){auto byte=static_cast<unsigned char>(raw);if(byte>='A'&&byte<='Z')byte=static_cast<unsigned char>(byte-'A'+'a');else switch(byte){
        case 0xA5:byte=0xB9;break; // Ą -> ą
        case 0xC6:byte=0xE6;break; // Ć -> ć
        case 0xCA:byte=0xEA;break; // Ę -> ę
        case 0xA3:byte=0xB3;break; // Ł -> ł
        case 0xD1:byte=0xF1;break; // Ń -> ń
        case 0xD3:byte=0xF3;break; // Ó -> ó
        case 0x8C:byte=0x9C;break; // Ś -> ś
        case 0x8F:byte=0x9F;break; // Ź -> ź
        case 0xAF:byte=0xBF;break; // Ż -> ż
        default:break;}raw=static_cast<char>(byte);}
    return key;
}

} // namespace graphboard
