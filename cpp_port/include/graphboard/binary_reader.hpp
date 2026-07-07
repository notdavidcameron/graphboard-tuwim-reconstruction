#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

namespace graphboard {

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message);
};

class BinaryReader {
public:
    explicit BinaryReader(std::vector<std::uint8_t> bytes);

    static BinaryReader fromFile(const std::filesystem::path& path);

    std::size_t position() const;
    std::size_t size() const;
    std::size_t remaining() const;
    bool eof() const;

    void seek(std::size_t offset);
    void skip(std::size_t byteCount);

    std::uint8_t readU8();
    std::uint16_t readU16();
    std::uint32_t readU32();
    std::int32_t readI32();
    std::string readBytesAsString(std::size_t byteCount);
    std::vector<std::uint8_t> readBytes(std::size_t byteCount);

    // MFC CArchive CString length prefix:
    // u8 length, or 0xff + u16 length, or 0xff 0xffff + u32 length.
    std::string readArchiveString();

private:
    void require(std::size_t byteCount) const;

    std::vector<std::uint8_t> bytes_;
    std::size_t pos_ = 0;
};

} // namespace graphboard
