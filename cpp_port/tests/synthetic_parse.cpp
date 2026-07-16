#include "graphboard/component.hpp"
#include "graphboard/format.hpp"
#include "graphboard/guid.hpp"
#include "graphboard/holders.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace {

void appendU8(std::vector<std::uint8_t>& bytes, std::uint8_t value) {
    bytes.push_back(value);
}

void appendU16(std::vector<std::uint8_t>& bytes, std::uint16_t value) {
    bytes.push_back(static_cast<std::uint8_t>(value & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
}

void appendU32(std::vector<std::uint8_t>& bytes, std::uint32_t value) {
    bytes.push_back(static_cast<std::uint8_t>(value & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 16) & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 24) & 0xff));
}

void appendArchiveString(std::vector<std::uint8_t>& bytes, const std::string& value) {
    assert(value.size() < 0xff);
    appendU8(bytes, static_cast<std::uint8_t>(value.size()));
    bytes.insert(bytes.end(), value.begin(), value.end());
}

// Reflected member names: u32 byteCount then byteCount+1 bytes incl. trailing NUL.
void appendCountedNulName(std::vector<std::uint8_t>& bytes, const std::string& value) {
    appendU32(bytes, static_cast<std::uint32_t>(value.size()));
    bytes.insert(bytes.end(), value.begin(), value.end());
    bytes.push_back(0);
}

void appendGuid(std::vector<std::uint8_t>& bytes, const graphboard::Guid& guid) {
    bytes.insert(bytes.end(), guid.bytes.begin(), guid.bytes.end());
}

void appendFixedString(std::vector<std::uint8_t>& bytes, const std::string& value, std::size_t width) {
    assert(value.size() <= width);
    bytes.insert(bytes.end(), value.begin(), value.end());
    bytes.resize(bytes.size() + (width - value.size()), 0);
}

std::string encodeSignature(const std::string& value) {
    std::string out;
    for (const unsigned char c : value) {
        out.push_back(static_cast<char>(c + 0x21));
    }
    return out;
}

void testProjectManifest() {
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 1);
    appendArchiveString(bytes, "intro.bdf");
    appendArchiveString(bytes, "");
    appendU32(bytes, 11);
    appendU32(bytes, 2);
    appendArchiveString(bytes, "intro.bdf");
    appendArchiveString(bytes, "radio.bdf");
    appendU32(bytes, 1);
    appendArchiveString(bytes, "cursors.grp");
    appendArchiveString(bytes, encodeSignature("Julian Tuwim"));
    appendU32(bytes, 1);                                    // globalScriptVersion
    appendArchiveString(bytes, "int global1=0;");           // globalScript

    graphboard::BinaryReader reader(bytes);
    const auto project = graphboard::parseProjectManifest(reader);
    assert(project.version == 1);
    assert(project.startupPage == "intro.bdf");
    assert(project.audioPresetIndex == 11);
    assert(project.pageNames.size() == 2);
    assert(project.groupNames.size() == 1);
    assert(project.decodedSignature == "Julian Tuwim");
    assert(project.globalScriptVersion == 1);
    assert(project.globalScript == "int global1=0;");
    assert(reader.eof());

    // Brzechwa's version-0 layout has no shifted signature CString. The global
    // script block begins immediately after the group table.
    std::vector<std::uint8_t> version0;
    appendU32(version0, 0);
    appendArchiveString(version0, "intro.bdf");
    appendArchiveString(version0, "");
    appendU32(version0, 11);
    appendU32(version0, 1);
    appendArchiveString(version0, "wybor.bdf");
    appendU32(version0, 0);
    appendU32(version0, 1);
    appendArchiveString(version0, "CString mHistory; int doTanca=0;");

    graphboard::BinaryReader version0Reader(version0);
    const auto brzechwa = graphboard::parseProjectManifest(version0Reader);
    assert(brzechwa.version == 0);
    assert(brzechwa.encodedSignature.empty());
    assert(brzechwa.decodedSignature.empty());
    assert(brzechwa.globalScriptVersion == 1);
    assert(brzechwa.globalScript == "CString mHistory; int doTanca=0;");
    assert(version0Reader.eof());
}

void testBdfHeader() {
    std::vector<std::uint8_t> bytes;
    appendFixedString(bytes, "YDP Board data file. Ver:1 1996.07.", 100);
    appendU32(bytes, 1);
    appendU32(bytes, 0);
    appendU32(bytes, 0);
    appendU32(bytes, 640);
    appendU32(bytes, 480);
    appendU32(bytes, 0xffffffffu);
    appendU32(bytes, 8);
    appendU32(bytes, 1);
    appendU8(bytes, 132);
    appendU32(bytes, 4);
    appendU8(bytes, 1);
    appendU8(bytes, 2);
    appendU8(bytes, 3);
    appendU8(bytes, 4);
    appendU32(bytes, 3);
    appendU8(bytes, 9);
    appendU8(bytes, 8);
    appendU8(bytes, 7);
    appendU32(bytes, 1);

    graphboard::BinaryReader reader(bytes);
    const auto header = graphboard::parseBdfHeader(reader);
    assert(header.version == 1);
    assert(header.pageRect.right == 640);
    assert(header.pageRect.bottom == 480);
    assert(header.minLayer == -1);
    assert(header.backgroundColorIndex == 132);
    assert(header.paletteByteCount == 4);
    assert(header.dibByteCount == 3);
    assert(header.componentListOffset == 148);
}

void testGroupDocument() {
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 1);                 // cursor count
    std::vector<std::uint8_t> record(0x2c, 0);
    auto put = [&record](std::size_t off, std::uint32_t value) {
        record[off] = static_cast<std::uint8_t>(value);
        record[off + 1] = static_cast<std::uint8_t>(value >> 8);
        record[off + 2] = static_cast<std::uint8_t>(value >> 16);
        record[off + 3] = static_cast<std::uint8_t>(value >> 24);
    };
    put(0x00, 1); put(0x04, 7); put(0x08, 2); put(0x0c, 2);
    put(0x10, 1); put(0x14, 0);
    const std::string name = "action";
    std::copy(name.begin(), name.end(), record.begin() + 0x1c);
    bytes.insert(bytes.end(), record.begin(), record.end());
    appendU32(bytes, 4);
    bytes.insert(bytes.end(), {7, 1, 2, 7});
    const std::size_t componentOffset = bytes.size();
    appendU32(bytes, 1);                 // component-list version
    appendU32(bytes, 0);                 // component count

    graphboard::BinaryReader reader(bytes);
    const auto group = graphboard::parseGroupDocument(reader);
    assert(group.cursors.size() == 1);
    assert(group.cursors[0].name == "action");
    assert(group.cursors[0].width == 2 && group.cursors[0].height == 2);
    assert(group.cursors[0].hotX == 1 && group.cursors[0].transparentIndex == 7);
    assert(group.cursors[0].pixels == std::vector<std::uint8_t>({7, 1, 2, 7}));
    assert(group.componentListOffset == componentOffset);
    const auto components = graphboard::parseComponentListHeader(reader);
    assert(components.version == 1 && components.count == 0);
    assert(reader.eof());
}

void testGuidRoundTrip() {
    const std::string text = "DA768116-5341-11D0-B444-008048EB5D40";
    const auto guid = graphboard::Guid::fromString(text);
    // On-disk byte order: Data1/Data2/Data3 little-endian, Data4 raw.
    assert(guid.bytes[0] == 0x16);
    assert(guid.bytes[1] == 0x81);
    assert(guid.bytes[2] == 0x76);
    assert(guid.bytes[3] == 0xDA);
    assert(guid.bytes[4] == 0x41);
    assert(guid.bytes[5] == 0x53);
    assert(guid.bytes[6] == 0xD0);
    assert(guid.bytes[7] == 0x11);
    assert(guid.bytes[8] == 0xB4);
    assert(guid.bytes[15] == 0x40);
    assert(guid.toString() == text);

    const auto* info = graphboard::lookupHolder(guid);
    assert(info != nullptr);
    assert(info->kind == graphboard::HolderKind::HotSpotHolder);
    assert(info->displayName == "HotSpot_Holder");
}

void testComponentWrapper() {
    const auto clsid = graphboard::Guid::fromString("DA768116-5341-11D0-B444-008048EB5D40");
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 1);                 // wrapperVersion
    appendU32(bytes, 1);                 // functionCount
    appendArchiveString(bytes, "HotSpot_Holder");
    appendU32(bytes, 1);                 // propertyCount
    appendGuid(bytes, clsid);
    // one function member
    appendU32(bytes, 0x60020000);        // dispatchIdOrOffset
    appendCountedNulName(bytes, "EnableHotSpot");
    appendArchiveString(bytes, "void");
    appendArchiveString(bytes, "EnableHotSpot");
    appendArchiveString(bytes, "");
    appendU16(bytes, 1);                 // flagsOrInvokeKind
    // one property member
    appendCountedNulName(bytes, "CompDisableDrag");
    appendU32(bytes, 0x0b);              // variantType
    appendArchiveString(bytes, "CompDisableDrag");
    appendU16(bytes, 4);                 // flagsOrInvokeKind

    graphboard::BinaryReader reader(bytes);
    const auto wrapper = graphboard::parseComponentWrapper(reader);
    assert(wrapper.wrapperVersion == 1);
    assert(wrapper.displayName == "HotSpot_Holder");
    assert(wrapper.clsid == clsid);
    assert(wrapper.functions.size() == 1);
    assert(wrapper.functions[0].rawName == "EnableHotSpot");
    assert(wrapper.functions[0].typeOrReturnName == "void");
    assert(wrapper.properties.size() == 1);
    assert(wrapper.properties[0].rawName == "CompDisableDrag");
    assert(wrapper.properties[0].variantTypeOrDispatchMetadata == 0x0b);
    assert(reader.eof());
}

void testComponentListItem() {
    const auto clsid = graphboard::Guid::fromString("DA768116-5341-11D0-B444-008048EB5D40");
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 1);                 // list version
    appendU32(bytes, 1);                 // list count
    // item = container CLSID + CntrItem record + private state
    appendGuid(bytes, clsid);            // leading container-written CLSID
    appendU32(bytes, 1);                 // wrapperVersion
    appendU32(bytes, 0);                 // functionCount
    appendArchiveString(bytes, "HotSpot_Holder");
    appendU32(bytes, 0);                 // propertyCount
    appendGuid(bytes, clsid);            // duplicate CLSID inside the wrapper

    graphboard::BinaryReader reader(bytes);
    const auto header = graphboard::parseComponentListHeader(reader);
    assert(header.version == 1);
    assert(header.count == 1);
    const auto item = graphboard::parseComponentListItem(reader);
    assert(item.clsid == clsid);
    assert(item.wrapper.clsid == clsid);
    assert(item.wrapper.displayName == "HotSpot_Holder");
    const auto* info = graphboard::lookupHolder(item.clsid);
    assert(info != nullptr && info->kind == graphboard::HolderKind::HotSpotHolder);
    assert(reader.eof());
}

void appendHotSpotRecord(std::vector<std::uint8_t>& bytes, std::int32_t left, std::int32_t top,
                         std::int32_t right, std::int32_t bottom, std::int32_t layer,
                         std::int32_t enabled) {
    std::vector<std::uint8_t> record(100, 0);
    auto put = [&record](std::size_t off, std::int32_t v) {
        const auto u = static_cast<std::uint32_t>(v);
        record[off] = static_cast<std::uint8_t>(u & 0xff);
        record[off + 1] = static_cast<std::uint8_t>((u >> 8) & 0xff);
        record[off + 2] = static_cast<std::uint8_t>((u >> 16) & 0xff);
        record[off + 3] = static_cast<std::uint8_t>((u >> 24) & 0xff);
    };
    put(0x00, left);
    put(0x04, top);
    put(0x08, right);
    put(0x0c, bottom);
    put(0x1c, layer);
    put(0x20, enabled);
    bytes.insert(bytes.end(), record.begin(), record.end());
}

void testHotSpotHolderState() {
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 0);                 // version
    appendU8(bytes, 0);                  // flag0
    appendU8(bytes, 1);                  // flag1
    appendU8(bytes, 0);                  // flag2
    appendU32(bytes, 2);                 // hotspotCount
    appendHotSpotRecord(bytes, 589, 1, 639, 476, 3, 1);
    appendArchiveString(bytes, "prawo");
    appendHotSpotRecord(bytes, 1, 4, 47, 477, 3, 0);
    appendArchiveString(bytes, "lewo");
    appendU32(bytes, 0x11);              // activeIndex
    appendU32(bytes, 0x22);              // auxStateWord

    graphboard::BinaryReader reader(bytes);
    const auto state = graphboard::parseHotSpotHolderState(reader);
    assert(state.version == 0);
    assert(state.flag1 == 1);
    assert(state.hotspots.size() == 2);
    assert(state.hotspots[0].left == 589);
    assert(state.hotspots[0].bottom == 476);
    assert(state.hotspots[0].layer == 3);
    assert(state.hotspots[0].enabled == 1);
    assert(state.hotspots[0].name == "prawo");
    assert(state.hotspots[1].enabled == 0);
    assert(state.hotspots[1].name == "lewo");
    assert(state.activeIndex == 0x11);
    assert(state.auxStateWord == 0x22);
    assert(reader.eof());
}

void testSpriteHolderState() {
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 1);                 // version
    appendU32(bytes, 1);                 // definitionCount
    appendU32(bytes, 2);                 // instanceCount
    // one definition blob: 0x90 bytes, name at +0x04, width/height +0x80/+0x84
    appendU32(bytes, 0x90);              // blobByteCount
    std::vector<std::uint8_t> blob(0x90, 0);
    const std::string name = "rzeczka";
    for (std::size_t i = 0; i < name.size(); ++i) blob[0x04 + i] = static_cast<std::uint8_t>(name[i]);
    blob[0x80] = 73;                     // width
    blob[0x84] = 59;                     // height
    bytes.insert(bytes.end(), blob.begin(), blob.end());
    // two instance records
    for (std::uint32_t i = 0; i < 2; ++i) {
        std::vector<std::uint8_t> record(0x8c, 0);
        record[0x00] = 0;                                       // definitionIndex
        record[0x04] = static_cast<std::uint8_t>(8 - i);        // field04
        record[0x08] = static_cast<std::uint8_t>(10 + i);       // posX
        record[0x0c] = static_cast<std::uint8_t>(20 + i);       // posY
        bytes.insert(bytes.end(), record.begin(), record.end());
    }

    graphboard::BinaryReader reader(bytes);
    const auto state = graphboard::parseSpriteHolderState(reader);
    assert(state.version == 1);
    assert(state.definitions.size() == 1);
    assert(state.definitions[0].name == "rzeczka");
    assert(state.definitions[0].width == 73);
    assert(state.definitions[0].height == 59);
    assert(state.instances.size() == 2);
    assert(state.instances[0].field04 == 8);
    assert(state.instances[1].posX == 11);
    assert(state.instances[1].posY == 21);
    assert(reader.eof());
}

void testMultiBitmapState() {
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 1);                 // version
    appendU32(bytes, 1);                 // recordCount
    appendU32(bytes, 8);                 // pixelByteCount (stride 4 * height 2)
    for (std::uint8_t i = 0; i < 8; ++i) appendU8(bytes, i);
    std::vector<std::uint8_t> metadata(0xc0, 0);
    metadata[0x08] = 10;                 // left
    metadata[0x0c] = 20;                 // top
    metadata[0x10] = 13;                 // right -> width 3
    metadata[0x14] = 22;                 // bottom -> height 2
    const std::string name = "przycisk";
    for (std::size_t i = 0; i < name.size(); ++i) metadata[0x28 + i] = static_cast<std::uint8_t>(name[i]);
    bytes.insert(bytes.end(), metadata.begin(), metadata.end());

    graphboard::BinaryReader reader(bytes);
    const auto state = graphboard::parseMultiBitmapState(reader);
    assert(state.version == 1);
    assert(state.records.size() == 1);
    assert(state.records[0].width == 3);
    assert(state.records[0].height == 2);
    assert(state.records[0].left == 10);
    assert(state.records[0].top == 20);
    assert(state.records[0].name == "przycisk");
    assert(state.records[0].pixelsAreRawIndexed);
    assert(state.records[0].pixelOffset == 12);
    assert(reader.eof());
}

void testTransparentVideoHolderState() {
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 3);                 // version
    appendU32(bytes, 1);                 // entryCount
    // stream: fixed 0x4e8 header only (no chunk records)
    appendU32(bytes, 0x4e8);             // streamByteCount
    std::vector<std::uint8_t> header(0x4e8, 0);
    auto putU32 = [&header](std::size_t off, std::uint32_t v) {
        header[off] = static_cast<std::uint8_t>(v & 0xff);
        header[off + 1] = static_cast<std::uint8_t>((v >> 8) & 0xff);
        header[off + 2] = static_cast<std::uint8_t>((v >> 16) & 0xff);
        header[off + 3] = static_cast<std::uint8_t>((v >> 24) & 0xff);
    };
    putU32(0x68, 0xada77777u);           // magic
    putU32(0x74, 199);                   // authored transparent colour index
    putU32(0x7c, 100);                   // frameDurationMs
    putU32(0x80, 6);                     // width  (stride 8)
    putU32(0x84, 2);                     // height
    putU32(0x88, 4);                     // chunkRecordCount
    putU32(0x8c, 3);                     // frameCount
    putU32(0x90, 22050);                 // audioSampleRate
    bytes.insert(bytes.end(), header.begin(), header.end());
    // holder entry: header copy + placement fields
    std::vector<std::uint8_t> entry(0x568, 0);
    auto putEntryU32 = [&entry](std::size_t off, std::uint32_t v) {
        entry[off] = static_cast<std::uint8_t>(v & 0xff);
        entry[off + 1] = static_cast<std::uint8_t>((v >> 8) & 0xff);
        entry[off + 2] = static_cast<std::uint8_t>((v >> 16) & 0xff);
        entry[off + 3] = static_cast<std::uint8_t>((v >> 24) & 0xff);
    };
    putEntryU32(0x80, 6);                // entry width -> still frame stride 8
    putEntryU32(0x84, 2);                // entry height -> still 16 bytes
    putEntryU32(0x508, 12);              // originalX
    putEntryU32(0x50c, 34);              // originalY
    putEntryU32(0x54c, 56);              // stageX
    putEntryU32(0x550, 78);              // stageY
    bytes.insert(bytes.end(), entry.begin(), entry.end());
    // still frame: stride(6)=8, height 2 -> 16 bytes
    for (int i = 0; i < 16; ++i) appendU8(bytes, 0xaa);

    graphboard::BinaryReader reader(bytes);
    const auto state = graphboard::parseTransparentVideoHolderState(reader);
    assert(state.version == 3);
    assert(state.entries.size() == 1);
    const auto& e = state.entries[0];
    assert(e.stream.magic == 0xada77777u);
    assert(e.stream.width == 6 && e.stream.height == 2);
    assert(e.stream.frameCount == 3);
    assert(e.transparentIndex == 0xaa);  // repeated still-frame corner matte
    assert(e.stream.audioSampleRate == 22050);
    assert(e.originalX == 12 && e.originalY == 34);
    assert(e.stageX == 56 && e.stageY == 78);
    assert(e.stillFrameByteCount == 16);
    assert(reader.eof());
}

void testSoundHolderState() {
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 2);                 // version
    appendU32(bytes, 1);                 // soundCount
    // one sound: 12-byte fake RIFF, 0x58 record, u32 wfSize=2, 2 wf bytes
    appendU32(bytes, 12);
    const char riff[12] = {'R','I','F','F',4,0,0,0,'W','A','V','E'};
    for (const char c : riff) appendU8(bytes, static_cast<std::uint8_t>(c));
    std::vector<std::uint8_t> record(0x58, 0);
    record[0x04] = 8;                    // archiveStart
    record[0x08] = 20;                   // archiveEnd
    const std::string name = "kamyki";
    for (std::size_t i = 0; i < name.size(); ++i) record[0x0c + i] = static_cast<std::uint8_t>(name[i]);
    bytes.insert(bytes.end(), record.begin(), record.end());
    appendU32(bytes, 2);
    appendU8(bytes, 0xaa);
    appendU8(bytes, 0xbb);

    graphboard::BinaryReader reader(bytes);
    const auto state = graphboard::parseSoundHolderState(reader);
    assert(state.version == 2);
    assert(state.sounds.size() == 1);
    assert(state.sounds[0].looksLikeRiff);
    assert(state.sounds[0].name == "kamyki");
    assert(state.sounds[0].archiveStart == 8);
    assert(state.sounds[0].waveFormatByteCount == 2);
    assert(reader.eof());
}

void testTextHolderState() {
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 2);                 // version
    appendU32(bytes, 1);                 // textCount
    appendU8(bytes, 0x17); appendU8(bytes, 0xbb); appendU8(bytes, 0x37); appendU8(bytes, 0x00);
    // entry record: both stale pointers set -> secondary-text branch
    std::vector<std::uint8_t> entry(0xc4, 0);
    entry[0x70] = 1;                     // pRenderCache present
    entry[0x90] = 1;                     // pSecondaryText present
    bytes.insert(bytes.end(), entry.begin(), entry.end());
    // renderCache: lineOffsetsPtr nonzero, lineCount 2
    std::vector<std::uint8_t> cache(0x68, 0);
    cache[0x0c] = 1;
    cache[0x10] = 2;
    bytes.insert(bytes.end(), cache.begin(), cache.end());
    appendU32(bytes, 10);
    appendU32(bytes, 25);
    appendArchiveString(bytes, "rzeczka.exs");
    appendArchiveString(bytes, "Rzeczka");
    // FontControl: one present slot with one glyph (width 5 -> stride 8, height 2)
    appendU32(bytes, 1);                 // slot 0 present
    std::vector<std::uint8_t> slot(0xc84, 0);
    slot[0x00] = 1;                      // glyph 0 present
    slot[0x800] = 5;                     // glyph 0 width
    slot[0xc00] = 2;                     // slot height
    bytes.insert(bytes.end(), slot.begin(), slot.end());
    for (int i = 0; i < 16; ++i) appendU8(bytes, 0xee);  // glyph bitmap
    for (int i = 0; i < 49; ++i) appendU32(bytes, 0);    // remaining absent slots

    graphboard::BinaryReader reader(bytes);
    const auto state = graphboard::parseTextHolderState(reader);
    assert(state.version == 2);
    assert(state.colors[1] == 0xbb);
    assert(state.entries.size() == 1);
    assert(state.entries[0].hasSecondaryText);
    assert(state.entries[0].lineCount == 2);
    assert(state.entries[0].lineOffsets[1] == 25);
    assert(state.entries[0].secondaryText == "rzeczka.exs");
    assert(state.entries[0].primaryText == "Rzeczka");
    assert(state.fontSlots.size() == 1);
    assert(state.fontSlots[0].height == 2);
    assert(state.fontSlots[0].glyphCount == 1);
    assert(reader.eof());
}

void testScriptBlocks() {
    std::vector<std::uint8_t> bytes;
    appendU32(bytes, 1);                 // script text version
    appendArchiveString(bytes, "switch(x){}");
    // engine state, schema 2
    appendU32(bytes, 2);                 // schemaVersion
    appendU32(bytes, 1);                 // switchBlockCount
    appendU32(bytes, 7); appendU32(bytes, 0); appendU32(bytes, 0); appendU32(bytes, 3);  // parserState
    appendU32(bytes, 5); appendU32(bytes, 6);   // blockStart twice (second wins)
    appendU32(bytes, 1);                 // caseCount
    appendU32(bytes, 11);                // blockEnd
    appendU32(bytes, 9);                 // defaultBodyOffset
    appendU32(bytes, 3); appendU32(bytes, 8);   // case value/offset
    appendU32(bytes, 1);                 // builtinCallCount
    appendU32(bytes, 4);                 // token offset
    appendU32(bytes, 2);                 // call kind
    appendU32(bytes, 100); appendU32(bytes, 0); appendU32(bytes, 200); appendU32(bytes, 0);  // schema2

    graphboard::BinaryReader reader(bytes);
    const auto script = graphboard::parseScriptText(reader);
    assert(script.version == 1);
    assert(script.text == "switch(x){}");
    const auto engine = graphboard::parseScriptEngineState(reader);
    assert(engine.schemaVersion == 2);
    assert(engine.parserState[0] == 7);
    assert(engine.switchBlocks.size() == 1);
    assert(engine.switchBlocks[0].blockStart == 6);
    assert(engine.switchBlocks[0].blockEnd == 11);
    assert(engine.switchBlocks[0].cases[0].caseValue == 3);
    assert(engine.builtinTokenOffsets.size() == 1);
    assert(engine.builtinCallKinds[0] == 2);
    assert(engine.schema2Fields[2] == 200);
    assert(reader.eof());
}

} // namespace

int main() {
    testProjectManifest();
    testBdfHeader();
    testGroupDocument();
    testGuidRoundTrip();
    testComponentWrapper();
    testComponentListItem();
    testHotSpotHolderState();
    testSpriteHolderState();
    testMultiBitmapState();
    testTransparentVideoHolderState();
    testSoundHolderState();
    testTextHolderState();
    testScriptBlocks();
    return 0;
}
