#pragma once

#include "graphboard/guid.hpp"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

namespace graphboard {

// Checked little-endian counterpart to BinaryReader.  The GraphBoard formats
// contain several absolute file offsets, so reserveU32/patchU32 are first-class
// operations instead of ad-hoc vector indexing in individual serializers.
class BinaryWriter {
public:
    std::size_t position() const { return bytes_.size(); }
    const std::vector<std::uint8_t>& bytes() const { return bytes_; }
    std::vector<std::uint8_t> takeBytes() { return std::move(bytes_); }

    void writeU8(std::uint8_t value);
    void writeU16(std::uint16_t value);
    void writeU32(std::uint32_t value);
    void writeI32(std::int32_t value) { writeU32(static_cast<std::uint32_t>(value)); }
    void writeGuid(const Guid& guid);
    void writeBytes(const void* bytes, std::size_t byteCount);
    void writeBytes(const std::vector<std::uint8_t>& bytes) {
        writeBytes(bytes.data(), bytes.size());
    }
    void writeFixedString(const std::string& value, std::size_t byteCount);

    // MFC CArchive CString framing used by every recovered host/component
    // serializer: u8 length, 0xff+u16, or 0xff+0xffff+u32.
    void writeArchiveString(const std::string& value);

    std::size_t reserveU32();
    void patchU32(std::size_t offset, std::uint32_t value);

    // Write to a sibling temporary file and rename only after the full byte
    // stream is durable. Existing files are replaced; parent directories are
    // created when necessary.
    void writeFileAtomic(const std::filesystem::path& path) const;

private:
    std::vector<std::uint8_t> bytes_;
};

} // namespace graphboard
