#pragma once

#include "graphboard/binary_reader.hpp"

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace graphboard {

struct Rect {
    std::int32_t left = 0;
    std::int32_t top = 0;
    std::int32_t right = 0;
    std::int32_t bottom = 0;
};

struct ProjectManifest {
    std::uint32_t version = 0;
    std::string startupPage;
    std::string currentPageOrGroupState;
    std::uint32_t audioPresetIndex = 0;   // audioManager+0x20; only project field it owns
    std::vector<std::string> pageNames;
    std::vector<std::string> groupNames;
    std::string encodedSignature;
    std::string decodedSignature;
    // Trailing block is the group/global script editor's text (doc +0xa0),
    // written by GraphBrdScriptEditor_SerializeText, NOT audio-manager state.
    std::uint32_t globalScriptVersion = 0;
    std::string globalScript;
};

struct BdfHeader {
    std::string banner;
    std::uint32_t version = 0;
    Rect pageRect;
    std::int32_t minLayer = 0;
    std::int32_t maxLayer = 0;
    std::uint32_t backgroundFlag = 0;
    std::uint8_t backgroundColorIndex = 0;
    std::uint32_t paletteByteCount = 0;
    std::size_t paletteOffset = 0;
    std::uint32_t dibByteCount = 0;
    std::size_t dibOffset = 0;
    std::size_t componentListOffset = 0;
};

// Cursor bitmap prefix of a .GRP file. GraphBrdDoc_SerializeCursorAndGroupState
// writes these records first, followed immediately by a normal ComponentList.
struct CursorBitmap {
    std::uint32_t flags = 0;
    std::uint32_t transparentIndex = 0;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::int32_t hotX = 0;
    std::int32_t hotY = 0;
    std::string name;
    std::vector<std::uint8_t> pixels;
};

struct GroupDocument {
    std::vector<CursorBitmap> cursors;
    std::size_t componentListOffset = 0;
};

// Script editor text block (GraphBrdScriptEditor_SerializeText,
// Tuwim.exe:004230a0). Used for both the per-page script (after the page
// component list in .BDF) and the global project script (end of START.PRJ).
struct ScriptText {
    std::uint32_t version = 0;   // currently 1
    std::string text;            // cp1250 script source
};

// Script engine parse cache (GraphBrdScriptEngine_Serialize,
// Tuwim.exe:0041aad0). Follows the page script text in .BDF. All offsets are
// character offsets into the page script text.
struct ScriptSwitchCase {
    std::uint32_t caseValue = 0;
    std::uint32_t caseBodyOffset = 0;
};

struct ScriptSwitchBlock {
    std::uint32_t blockStart = 0;       // written twice on disk; second copy wins
    std::uint32_t blockEnd = 0;
    std::uint32_t defaultBodyOffset = 0;
    std::vector<ScriptSwitchCase> cases;
};

struct ScriptEngineState {
    std::uint32_t schemaVersion = 0;    // current exe writes 4; RZECZKA has 2
    std::uint32_t parserState[4] = {0, 0, 0, 0};  // engine +0x38,+0x3c,+0x30,+0x34
    std::vector<ScriptSwitchBlock> switchBlocks;
    std::vector<std::uint32_t> builtinTokenOffsets;  // engine +0x11b4
    std::vector<std::uint32_t> builtinCallKinds;     // engine +0x1344
    std::uint32_t schema2Fields[4] = {0, 0, 0, 0};   // engine +0x44..+0x50
    std::uint32_t schema3Field = 0;                  // engine +0x54
    std::uint32_t schema4Field = 0;                  // engine +0x58
};

ProjectManifest parseProjectManifest(BinaryReader& reader);
BdfHeader parseBdfHeader(BinaryReader& reader);
GroupDocument parseGroupDocument(BinaryReader& reader);
ScriptText parseScriptText(BinaryReader& reader);
ScriptEngineState parseScriptEngineState(BinaryReader& reader);

std::string decodeShiftedSignature(const std::string& encoded);

} // namespace graphboard
