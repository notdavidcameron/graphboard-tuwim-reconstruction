#include "graphboard/binary_reader.hpp"

#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace graphboard {

ParseError::ParseError(const std::string& message) : std::runtime_error(message) {}

BinaryReader::BinaryReader(std::vector<std::uint8_t> bytes) : bytes_(std::move(bytes)) {}

BinaryReader BinaryReader::fromFile(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        throw ParseError("Unable to open file: " + path.string());
    }
    stream.seekg(0, std::ios::end);
    const auto end = stream.tellg();
    if (end < 0) {
        throw ParseError("Unable to determine file size: " + path.string());
    }
    stream.seekg(0, std::ios::beg);

    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(end));
    if (!bytes.empty()) {
        stream.read(reinterpret_cast<char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        if (!stream) {
            throw ParseError("Unable to read complete file: " + path.string());
        }
    }
    return BinaryReader(std::move(bytes));
}

std::size_t BinaryReader::position() const {
    return pos_;
}

std::size_t BinaryReader::size() const {
    return bytes_.size();
}

std::size_t BinaryReader::remaining() const {
    return bytes_.size() - pos_;
}

bool BinaryReader::eof() const {
    return pos_ == bytes_.size();
}

void BinaryReader::seek(std::size_t offset) {
    if (offset > bytes_.size()) {
        throw ParseError("Seek past end of input");
    }
    pos_ = offset;
}

void BinaryReader::skip(std::size_t byteCount) {
    require(byteCount);
    pos_ += byteCount;
}

std::uint8_t BinaryReader::readU8() {
    require(1);
    return bytes_[pos_++];
}

std::uint16_t BinaryReader::readU16() {
    require(2);
    const std::uint16_t value =
        static_cast<std::uint16_t>(bytes_[pos_]) |
        (static_cast<std::uint16_t>(bytes_[pos_ + 1]) << 8);
    pos_ += 2;
    return value;
}

std::uint32_t BinaryReader::readU32() {
    require(4);
    const std::uint32_t value =
        static_cast<std::uint32_t>(bytes_[pos_]) |
        (static_cast<std::uint32_t>(bytes_[pos_ + 1]) << 8) |
        (static_cast<std::uint32_t>(bytes_[pos_ + 2]) << 16) |
        (static_cast<std::uint32_t>(bytes_[pos_ + 3]) << 24);
    pos_ += 4;
    return value;
}

std::int32_t BinaryReader::readI32() {
    return static_cast<std::int32_t>(readU32());
}

std::string BinaryReader::readBytesAsString(std::size_t byteCount) {
    const auto bytes = readBytes(byteCount);
    return std::string(bytes.begin(), bytes.end());
}

std::vector<std::uint8_t> BinaryReader::readBytes(std::size_t byteCount) {
    require(byteCount);
    std::vector<std::uint8_t> out(bytes_.begin() + static_cast<std::ptrdiff_t>(pos_),
                                  bytes_.begin() + static_cast<std::ptrdiff_t>(pos_ + byteCount));
    pos_ += byteCount;
    return out;
}

std::string BinaryReader::readArchiveString() {
    std::uint32_t length = readU8();
    if (length == 0xff) {
        length = readU16();
        if (length == 0xffff) {
            length = readU32();
        }
    }
    if (length > remaining()) {
        throw ParseError("Archive string length exceeds remaining input");
    }
    return readBytesAsString(length);
}

void BinaryReader::require(std::size_t byteCount) const {
    if (byteCount > remaining()) {
        std::ostringstream message;
        message << "Unexpected end of input at offset " << pos_
                << ", need " << byteCount << " bytes, have " << remaining();
        throw ParseError(message.str());
    }
}

} // namespace graphboard
