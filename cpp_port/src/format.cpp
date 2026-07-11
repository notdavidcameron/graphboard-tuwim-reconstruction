#include "graphboard/format.hpp"

namespace graphboard {

namespace {

constexpr std::size_t kBdfBannerBytes = 100;
constexpr std::size_t kCursorRecordBytes = 0x2c;

std::vector<std::string> readStringTable(BinaryReader& reader) {
    const auto count = reader.readU32();
    std::vector<std::string> values;
    values.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        values.push_back(reader.readArchiveString());
    }
    return values;
}

std::string trimTrailingNulAndSpace(std::string value) {
    while (!value.empty() && (value.back() == '\0' || value.back() == ' ')) {
        value.pop_back();
    }
    return value;
}

} // namespace

ProjectManifest parseProjectManifest(BinaryReader& reader) {
    ProjectManifest project;
    project.version = reader.readU32();
    project.startupPage = reader.readArchiveString();
    project.currentPageOrGroupState = reader.readArchiveString();
    project.audioPresetIndex = reader.readU32();
    project.pageNames = readStringTable(reader);
    project.groupNames = readStringTable(reader);
    project.encodedSignature = reader.readArchiveString();
    project.decodedSignature = decodeShiftedSignature(project.encodedSignature);
    // Group/global script editor block (GraphBrdScriptEditor_SerializeText):
    // u32 version, then the project-wide global script as a CString.
    project.globalScriptVersion = reader.readU32();
    project.globalScript = reader.readArchiveString();
    return project;
}

BdfHeader parseBdfHeader(BinaryReader& reader) {
    BdfHeader header;
    header.banner = trimTrailingNulAndSpace(reader.readBytesAsString(kBdfBannerBytes));
    header.version = reader.readU32();
    header.pageRect.left = reader.readI32();
    header.pageRect.top = reader.readI32();
    header.pageRect.right = reader.readI32();
    header.pageRect.bottom = reader.readI32();
    header.minLayer = reader.readI32();
    header.maxLayer = reader.readI32();
    header.backgroundFlag = reader.readU32();
    header.backgroundColorIndex = reader.readU8();
    header.paletteByteCount = reader.readU32();
    header.paletteOffset = reader.position();
    reader.skip(header.paletteByteCount);
    header.dibByteCount = reader.readU32();
    header.dibOffset = reader.position();
    reader.skip(header.dibByteCount);
    header.componentListOffset = reader.position();
    return header;
}

GroupDocument parseGroupDocument(BinaryReader& reader) {
    GroupDocument group;
    const auto count = reader.readU32();
    group.cursors.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        const auto record = reader.readBytes(kCursorRecordBytes);
        auto u32 = [&](std::size_t offset) -> std::uint32_t {
            return static_cast<std::uint32_t>(record[offset]) |
                   (static_cast<std::uint32_t>(record[offset + 1]) << 8) |
                   (static_cast<std::uint32_t>(record[offset + 2]) << 16) |
                   (static_cast<std::uint32_t>(record[offset + 3]) << 24);
        };
        CursorBitmap cursor;
        cursor.flags = u32(0x00);
        cursor.transparentIndex = u32(0x04);
        cursor.width = u32(0x08);
        cursor.height = u32(0x0c);
        cursor.hotX = static_cast<std::int32_t>(u32(0x10));
        cursor.hotY = static_cast<std::int32_t>(u32(0x14));
        cursor.name.assign(reinterpret_cast<const char*>(record.data() + 0x1c), 12);
        cursor.name = trimTrailingNulAndSpace(std::move(cursor.name));
        const auto pixelBytes = reader.readU32();
        cursor.pixels = reader.readBytes(pixelBytes);
        group.cursors.push_back(std::move(cursor));
    }
    group.componentListOffset = reader.position();
    return group;
}

ScriptText parseScriptText(BinaryReader& reader) {
    ScriptText script;
    script.version = reader.readU32();
    script.text = reader.readArchiveString();
    return script;
}

ScriptEngineState parseScriptEngineState(BinaryReader& reader) {
    ScriptEngineState state;
    state.schemaVersion = reader.readU32();
    const auto switchBlockCount = reader.readU32();
    for (auto& field : state.parserState) {
        field = reader.readU32();
    }

    state.switchBlocks.reserve(switchBlockCount);
    for (std::uint32_t i = 0; i < switchBlockCount; ++i) {
        ScriptSwitchBlock block;
        block.blockStart = reader.readU32();
        block.blockStart = reader.readU32();   // duplicate on disk; second wins
        const auto caseCount = reader.readU32();
        block.blockEnd = reader.readU32();
        block.defaultBodyOffset = reader.readU32();
        block.cases.reserve(caseCount);
        for (std::uint32_t c = 0; c < caseCount; ++c) {
            ScriptSwitchCase entry;
            entry.caseValue = reader.readU32();
            entry.caseBodyOffset = reader.readU32();
            block.cases.push_back(entry);
        }
        state.switchBlocks.push_back(std::move(block));
    }

    const auto builtinCallCount = reader.readU32();
    state.builtinTokenOffsets.reserve(builtinCallCount);
    for (std::uint32_t i = 0; i < builtinCallCount; ++i) {
        state.builtinTokenOffsets.push_back(reader.readU32());
    }
    state.builtinCallKinds.reserve(builtinCallCount);
    for (std::uint32_t i = 0; i < builtinCallCount; ++i) {
        state.builtinCallKinds.push_back(reader.readU32());
    }

    if (state.schemaVersion > 1) {
        for (auto& field : state.schema2Fields) {
            field = reader.readU32();
        }
    }
    if (state.schemaVersion > 2) {
        state.schema3Field = reader.readU32();
    }
    if (state.schemaVersion > 3) {
        state.schema4Field = reader.readU32();
    }
    return state;
}

std::string decodeShiftedSignature(const std::string& encoded) {
    std::string decoded;
    decoded.reserve(encoded.size());
    for (const unsigned char c : encoded) {
        decoded.push_back(static_cast<char>(c - 0x21));
    }
    return decoded;
}

} // namespace graphboard
