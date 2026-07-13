#include "graphboard/binary_writer.hpp"

#include <fstream>
#include <limits>
#include <stdexcept>
#include <system_error>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace graphboard {

void BinaryWriter::writeU8(std::uint8_t value) {
    bytes_.push_back(value);
}

void BinaryWriter::writeU16(std::uint16_t value) {
    bytes_.push_back(static_cast<std::uint8_t>(value));
    bytes_.push_back(static_cast<std::uint8_t>(value >> 8));
}

void BinaryWriter::writeU32(std::uint32_t value) {
    bytes_.push_back(static_cast<std::uint8_t>(value));
    bytes_.push_back(static_cast<std::uint8_t>(value >> 8));
    bytes_.push_back(static_cast<std::uint8_t>(value >> 16));
    bytes_.push_back(static_cast<std::uint8_t>(value >> 24));
}

void BinaryWriter::writeGuid(const Guid& guid) {
    writeBytes(guid.bytes.data(), guid.bytes.size());
}

void BinaryWriter::writeBytes(const void* bytes, std::size_t byteCount) {
    if (byteCount == 0) return;
    if (!bytes) throw std::invalid_argument("BinaryWriter::writeBytes: null source");
    if (byteCount > bytes_.max_size() - bytes_.size()) {
        throw std::length_error("BinaryWriter byte stream exceeds vector capacity");
    }
    const auto* first = static_cast<const std::uint8_t*>(bytes);
    bytes_.insert(bytes_.end(), first, first + byteCount);
}

void BinaryWriter::writeFixedString(const std::string& value, std::size_t byteCount) {
    if (value.size() > byteCount) {
        throw std::invalid_argument("Fixed string exceeds its serialized field");
    }
    writeBytes(value.data(), value.size());
    bytes_.insert(bytes_.end(), byteCount - value.size(), 0);
}

void BinaryWriter::writeArchiveString(const std::string& value) {
    if (value.size() > std::numeric_limits<std::uint32_t>::max()) {
        throw std::length_error("Archive string exceeds the MFC u32 length limit");
    }
    const auto length = static_cast<std::uint32_t>(value.size());
    if (length < 0xff) {
        writeU8(static_cast<std::uint8_t>(length));
    } else if (length < 0xffff) {
        writeU8(0xff);
        writeU16(static_cast<std::uint16_t>(length));
    } else {
        writeU8(0xff);
        writeU16(0xffff);
        writeU32(length);
    }
    writeBytes(value.data(), value.size());
}

std::size_t BinaryWriter::reserveU32() {
    const auto offset = position();
    writeU32(0);
    return offset;
}

void BinaryWriter::patchU32(std::size_t offset, std::uint32_t value) {
    if (offset > bytes_.size() || bytes_.size() - offset < 4) {
        throw std::out_of_range("BinaryWriter::patchU32 offset is outside the stream");
    }
    bytes_[offset + 0] = static_cast<std::uint8_t>(value);
    bytes_[offset + 1] = static_cast<std::uint8_t>(value >> 8);
    bytes_[offset + 2] = static_cast<std::uint8_t>(value >> 16);
    bytes_[offset + 3] = static_cast<std::uint8_t>(value >> 24);
}

void BinaryWriter::writeFileAtomic(const std::filesystem::path& path) const {
    if (path.empty()) throw std::invalid_argument("Output path is empty");
    std::error_code ec;
    if (!path.parent_path().empty()) {
        std::filesystem::create_directories(path.parent_path(), ec);
        if (ec) throw std::runtime_error("Unable to create output directory: " + ec.message());
    }
    auto temporary = path;
    temporary += ".tmp";
    {
        std::ofstream stream(temporary, std::ios::binary | std::ios::trunc);
        if (!stream) throw std::runtime_error("Unable to open temporary output file");
        if (!bytes_.empty()) {
            stream.write(reinterpret_cast<const char*>(bytes_.data()),
                         static_cast<std::streamsize>(bytes_.size()));
        }
        stream.flush();
        if (!stream) throw std::runtime_error("Unable to write complete output file");
    }
#ifdef _WIN32
    if (!MoveFileExW(temporary.c_str(), path.c_str(),
                     MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        const auto code = GetLastError();
        std::filesystem::remove(temporary, ec);
        throw std::runtime_error("Unable to atomically replace output file (Win32 " +
                                 std::to_string(code) + ")");
    }
#else
    std::filesystem::rename(temporary, path, ec); // POSIX rename replaces atomically.
    if (ec) {
        std::filesystem::remove(temporary);
        throw std::runtime_error("Unable to replace output file: " + ec.message());
    }
#endif
}

} // namespace graphboard
