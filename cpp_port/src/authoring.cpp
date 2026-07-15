#include "graphboard/authoring.hpp"

#include "graphboard/binary_reader.hpp"
#include "graphboard/binary_writer.hpp"
#include "graphboard/runtime/lexer.hpp"
#include "graphboard/runtime/script.hpp"
#include "graphboard/text.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>

namespace graphboard::authoring {

namespace {

constexpr std::size_t kSpriteInstanceBytes = 0x8c;
constexpr std::size_t kSpriteFrameTable = 0x6c;
constexpr std::size_t kSpriteFrameBytes = 0x4c;
constexpr std::size_t kSpritePixelBase = 0xb8;
constexpr std::size_t kBitmapPixelBase = 0x80;
constexpr std::size_t kBitmapTrailer = 0x10;
constexpr std::size_t kMultiBitmapRecordBytes = 0xc0;
constexpr std::size_t kSoundRecordBytes = 0x58; // shipped Tuwim Sound.dll
constexpr std::size_t kVideoRecordBytes = 0x6c;

void putU32(std::vector<std::uint8_t>& bytes, std::size_t offset, std::uint32_t value) {
    if (offset > bytes.size() || bytes.size() - offset < 4) {
        throw std::out_of_range("Fixed record field exceeds its byte buffer");
    }
    bytes[offset + 0] = static_cast<std::uint8_t>(value);
    bytes[offset + 1] = static_cast<std::uint8_t>(value >> 8);
    bytes[offset + 2] = static_cast<std::uint8_t>(value >> 16);
    bytes[offset + 3] = static_cast<std::uint8_t>(value >> 24);
}

std::uint32_t getU32(const std::vector<std::uint8_t>& bytes, std::size_t offset) {
    if (offset > bytes.size() || bytes.size() - offset < 4) {
        throw std::out_of_range("Fixed record read exceeds its byte buffer");
    }
    return static_cast<std::uint32_t>(bytes[offset]) |
           (static_cast<std::uint32_t>(bytes[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(bytes[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
}

void putFixedName(std::vector<std::uint8_t>& bytes, std::size_t offset, std::size_t capacity,
                  const std::string& name) {
    if (name.size() >= capacity) {
        throw std::invalid_argument("Holder item name exceeds its fixed on-disk field");
    }
    if (offset > bytes.size() || capacity > bytes.size() - offset) {
        throw std::out_of_range("Fixed name field exceeds its byte buffer");
    }
    std::fill(bytes.begin() + static_cast<std::ptrdiff_t>(offset),
              bytes.begin() + static_cast<std::ptrdiff_t>(offset + capacity), 0);
    std::copy(name.begin(), name.end(), bytes.begin() + static_cast<std::ptrdiff_t>(offset));
}

std::size_t stride4(std::size_t width) { return (width + 3) & ~std::size_t{3}; }

void requireImage(const IndexedImage& image, const char* context) {
    if (image.width == 0 || image.height == 0 ||
        static_cast<std::uint64_t>(image.width) * image.height != image.pixels.size()) {
        throw std::invalid_argument(std::string(context) + " has invalid indexed pixels");
    }
}

std::string rawBytes(std::initializer_list<std::uint8_t> values) {
    return std::string(values.begin(), values.end());
}

FunctionMember functionMember(std::string name, std::string returnType, std::string signature,
                              std::string rawName, std::uint16_t flags = 24) {
    FunctionMember value;
    value.rawName = std::move(rawName);
    value.typeOrReturnName = std::move(returnType);
    value.displayName = std::move(name);
    value.descriptionOrHelpName = std::move(signature);
    value.flagsOrInvokeKind = flags;
    return value;
}

PropertyMember propertyMember(std::string name, std::uint32_t variant, std::string rawName,
                              std::uint16_t flags = 24) {
    PropertyMember value;
    value.rawName = std::move(rawName);
    value.variantTypeOrDispatchMetadata = variant;
    value.displayName = std::move(name);
    value.flagsOrInvokeKind = flags;
    return value;
}

void addComPrefix(ComponentWrapper& wrapper) {
    wrapper.functions.push_back(functionMember("QueryInterface", "void", "(void* riid,void* ppvObj)",
                                                rawBytes({0x1a, 0x1a})));
    wrapper.functions.push_back(functionMember("AddRef", "UINT", "()", {}, 19));
    wrapper.functions.push_back(functionMember("Release", "UINT", "()", {}, 19));
    wrapper.functions.push_back(functionMember("GetTypeInfoCount", "void", "(void* pctinfo)",
                                                rawBytes({0x1a})));
    wrapper.functions.push_back(functionMember("GetTypeInfo", "void",
                                                "(UINT itinfo,UINT lcid,void* pptinfo)",
                                                rawBytes({0x17, 0x13, 0x1a})));
    wrapper.functions.push_back(functionMember("GetIDsOfNames", "void",
        "(void* riid,void* rgszNames,UINT cNames,UINT lcid,void* rgdispid)",
        rawBytes({0x1a, 0x1a, 0x17, 0x13, 0x1a})));
    wrapper.functions.push_back(functionMember("Invoke", "void",
        "(long dispidMember,void* riid,UINT lcid,unsigned short wFlags,void* pdispparams,void* pvarResult,void* pexcepinfo,void* puArgErr)",
        rawBytes({0x03, 0x1a, 0x13, 0x12, 0x1a, 0x1a, 0x1a, 0x1a})));

    wrapper.properties.push_back(propertyMember("QueryInterface", 0x60000000u,
                                                 rawBytes({0x43, 0x43})));
    wrapper.properties.push_back(propertyMember("AddRef", 0x60000001u, {}, 19));
    wrapper.properties.push_back(propertyMember("Release", 0x60000002u, {}, 19));
    wrapper.properties.push_back(propertyMember("GetTypeInfoCount", 0x60010000u,
                                                 rawBytes({0x43})));
    wrapper.properties.push_back(propertyMember("GetTypeInfo", 0x60010001u,
                                                 rawBytes({0x17, 0x13, 0x43})));
    wrapper.properties.push_back(propertyMember("GetIDsOfNames", 0x60010002u,
                                                 rawBytes({0x43, 0x43, 0x17, 0x13, 0x43})));
    wrapper.properties.push_back(propertyMember("Invoke", 0x60010003u,
                                                 rawBytes({0x03, 0x43, 0x13, 0x12,
                                                           0x43, 0x43, 0x43, 0x43})));
}

void addEvents(ComponentWrapper& wrapper,
               std::initializer_list<std::pair<const char*, const char*>> events) {
    for (const auto& event : events) {
        const std::string signature = event.second;
        const auto argumentCount = static_cast<std::size_t>(
            std::count(signature.begin(), signature.end(), ',') +
            (signature == "()" ? 0 : 1));
        wrapper.functions.push_back(functionMember(event.first, "void", signature,
                                                   std::string(argumentCount, '\x16')));
    }
}

void addMethods(ComponentWrapper& wrapper, std::initializer_list<const char*> methods) {
    std::uint32_t dispatch = 0x60030000u;
    for (const char* method : methods) {
        wrapper.properties.push_back(propertyMember(method, dispatch++, {}));
    }
}

std::string lowerAscii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

std::string cp1250(const std::string& utf8, const char* field) {
    bool ok = false;
    auto encoded = utf8ToCp1250(utf8, &ok);
    if (!ok) throw std::invalid_argument(std::string(field) + " contains characters outside Windows-1250");
    return encoded;
}

bool isPunct(const runtime::Token& token, const char* value) {
    return token.kind == runtime::TokenKind::Punctuator && token.text == value;
}

std::map<std::string, std::uint32_t> handlerParenOffsets(const std::string& script) {
    const auto tokens = runtime::tokenize(script);
    std::map<std::string, std::uint32_t> result;
    int braces = 0;
    for (std::size_t i = 0; i < tokens.size(); ++i) {
        if (isPunct(tokens[i], "{")) { ++braces; continue; }
        if (isPunct(tokens[i], "}")) { if (braces > 0) --braces; continue; }
        if (braces != 0 || !isPunct(tokens[i], "(") || i == 0 ||
            tokens[i - 1].kind != runtime::TokenKind::Identifier) continue;

        std::size_t method = i - 1;
        std::size_t start = method;
        while (start >= 2 && isPunct(tokens[start - 1], ".") &&
               tokens[start - 2].kind == runtime::TokenKind::Identifier) start -= 2;
        int depth = 0;
        std::size_t close = i;
        for (std::size_t j = i; j < tokens.size(); ++j) {
            if (isPunct(tokens[j], "(")) ++depth;
            else if (isPunct(tokens[j], ")") && --depth == 0) { close = j; break; }
        }
        if (close + 1 >= tokens.size() || !isPunct(tokens[close + 1], "{")) continue;
        std::string name;
        for (std::size_t j = start; j <= method; ++j) name += tokens[j].text;
        result[name] = static_cast<std::uint32_t>(tokens[i].offset);
    }
    return result;
}

void serializeWrapper(BinaryWriter& writer, const ComponentWrapper& wrapper) {
    writer.writeU32(1);
    writer.writeU32(static_cast<std::uint32_t>(wrapper.functions.size()));
    writer.writeArchiveString(cp1250(wrapper.displayName, "Component display name"));
    writer.writeU32(static_cast<std::uint32_t>(wrapper.properties.size()));
    writer.writeGuid(wrapper.clsid);
    for (const auto& fn : wrapper.functions) {
        writer.writeU32(fn.dispatchIdOrOffset);
        writer.writeU32(static_cast<std::uint32_t>(fn.rawName.size()));
        writer.writeBytes(fn.rawName.data(), fn.rawName.size());
        writer.writeU8(0);
        writer.writeArchiveString(cp1250(fn.typeOrReturnName, "Function return type"));
        writer.writeArchiveString(cp1250(fn.displayName, "Function name"));
        writer.writeArchiveString(cp1250(fn.descriptionOrHelpName, "Function signature"));
        writer.writeU16(fn.flagsOrInvokeKind);
    }
    for (const auto& property : wrapper.properties) {
        writer.writeU32(static_cast<std::uint32_t>(property.rawName.size()));
        writer.writeBytes(property.rawName.data(), property.rawName.size());
        writer.writeU8(0);
        writer.writeU32(property.variantTypeOrDispatchMetadata);
        writer.writeArchiveString(cp1250(property.displayName, "Property name"));
        writer.writeU16(property.flagsOrInvokeKind);
    }
}

void serializeHotSpots(BinaryWriter& writer, const HotSpotComponent& component) {
    writer.writeU32(0);
    writer.writeU8(component.flag0);
    writer.writeU8(component.flag1);
    writer.writeU8(component.flag2);
    writer.writeU32(static_cast<std::uint32_t>(component.items.size()));
    for (const auto& item : component.items) {
        std::vector<std::uint8_t> record(100, 0);
        putU32(record, 0x00, static_cast<std::uint32_t>(item.left));
        putU32(record, 0x04, static_cast<std::uint32_t>(item.top));
        putU32(record, 0x08, static_cast<std::uint32_t>(item.right));
        putU32(record, 0x0c, static_cast<std::uint32_t>(item.bottom));
        putU32(record, 0x18, static_cast<std::uint32_t>(item.id));
        putU32(record, 0x1c, static_cast<std::uint32_t>(item.layer));
        putU32(record, 0x20, item.enabled ? 1u : 0u);
        writer.writeBytes(record);
        writer.writeArchiveString(cp1250(item.name, "Hotspot name"));
    }
    writer.writeU32(component.activeIndex);
    writer.writeU32(component.auxStateWord);
}

std::vector<std::uint8_t> makeSpriteBlob(const SpriteDefinitionDocument& definition) {
    if (definition.frames.empty()) throw std::invalid_argument("Sprite definition has no frames");
    const std::size_t frameTableEnd = kSpriteFrameTable + definition.frames.size() * kSpriteFrameBytes;
    std::size_t cursor = std::max(kSpritePixelBase, frameTableEnd);
    std::vector<std::uint8_t> blob(cursor, 0);
    putU32(blob, 0x00, static_cast<std::uint32_t>(definition.frames.size()));
    putFixedName(blob, 0x04, 0x20, cp1250(definition.name, "Sprite definition name"));
    putU32(blob, 0x18, 1); // confirmed blank-definition default
    putU32(blob, 0x24, definition.pixelHitTest ? 1u : 0u);
    putU32(blob, 0x2c, 1); // confirmed blank-definition default

    for (std::size_t i = 0; i < definition.frames.size(); ++i) {
        const auto& frame = definition.frames[i];
        requireImage(frame.image, "Sprite frame");
        const std::size_t frameOffset = kSpriteFrameTable + i * kSpriteFrameBytes;
        const std::size_t pitch = stride4(frame.image.width);
        putU32(blob, frameOffset + 0x04, frame.image.transparentIndex);
        putU32(blob, frameOffset + 0x08, frame.pixelHitTest ? 1u : 0u);
        putU32(blob, frameOffset + 0x10, frame.image.width);
        putU32(blob, frameOffset + 0x14, frame.image.width);
        putU32(blob, frameOffset + 0x18, frame.image.height);
        putU32(blob, frameOffset + 0x20, frame.durationMs);
        putU32(blob, frameOffset + 0x48, static_cast<std::uint32_t>(cursor - kSpritePixelBase));
        const auto old = blob.size();
        blob.resize(old + pitch * frame.image.height, frame.image.transparentIndex);
        for (std::uint32_t y = 0; y < frame.image.height; ++y) {
            const auto src = static_cast<std::size_t>(frame.image.height - 1 - y) * frame.image.width;
            std::copy_n(frame.image.pixels.begin() + static_cast<std::ptrdiff_t>(src),
                        frame.image.width,
                        blob.begin() + static_cast<std::ptrdiff_t>(old + y * pitch));
        }
        cursor = blob.size();
    }
    return blob;
}

void serializeSprites(BinaryWriter& writer, const SpriteComponent& component) {
    writer.writeU32(1);
    writer.writeU32(static_cast<std::uint32_t>(component.definitions.size()));
    writer.writeU32(static_cast<std::uint32_t>(component.instances.size()));
    for (const auto& definition : component.definitions) {
        const auto blob = makeSpriteBlob(definition);
        writer.writeU32(static_cast<std::uint32_t>(blob.size()));
        writer.writeBytes(blob);
    }
    for (const auto& item : component.instances) {
        if (item.definitionIndex >= component.definitions.size()) {
            throw std::invalid_argument("Sprite instance references a missing definition");
        }
        std::vector<std::uint8_t> record(kSpriteInstanceBytes, 0);
        putU32(record, 0x00, item.definitionIndex);
        putU32(record, 0x04, static_cast<std::uint32_t>(item.layer));
        putU32(record, 0x08, static_cast<std::uint32_t>(item.x));
        putU32(record, 0x0c, static_cast<std::uint32_t>(item.y));
        putU32(record, 0x10, 1); // confirmed AddSpriteInstance default
        putU32(record, 0x18, static_cast<std::uint32_t>(item.layer));
        putU32(record, 0x1c, item.draggable ? 1u : 0u);
        putU32(record, 0x5c, static_cast<std::uint32_t>(item.phase));
        putU32(record, 0x88, item.visible ? 1u : 0u);
        writer.writeBytes(record);
    }
}

void serializeBitmaps(BinaryWriter& writer, const BitmapComponent& component) {
    writer.writeU32(1);
    writer.writeU32(static_cast<std::uint32_t>(component.items.size()));
    for (const auto& item : component.items) {
        requireImage(item.image, "Bitmap_Holder image");
        const std::size_t pitch = stride4(item.image.width);
        std::vector<std::uint8_t> blob(kBitmapPixelBase + pitch * item.image.height +
                                      kBitmapTrailer, 0);
        putU32(blob, 0x04, item.image.transparentIndex);
        putU32(blob, 0x08, static_cast<std::uint32_t>(item.x));
        putU32(blob, 0x0c, static_cast<std::uint32_t>(item.y));
        putU32(blob, 0x10, static_cast<std::uint32_t>(item.x + item.image.width));
        putU32(blob, 0x14, static_cast<std::uint32_t>(item.y + item.image.height));
        putU32(blob, 0x18, static_cast<std::uint32_t>(item.layer));
        putU32(blob, 0x20, item.pixelHitTest ? 1u : 0u);
        putU32(blob, 0x28, item.pixelHitTest ? 1u : 0u);
        putU32(blob, 0x2c, item.initiallyHidden ? 1u : 0u);
        putFixedName(blob, 0x34, 12, cp1250(item.name, "Bitmap name"));
        putU32(blob, 0x40, static_cast<std::uint32_t>(item.x));
        putU32(blob, 0x44, static_cast<std::uint32_t>(item.y));
        for (std::uint32_t y = 0; y < item.image.height; ++y) {
            const auto src = static_cast<std::size_t>(item.image.height - 1 - y) * item.image.width;
            std::copy_n(item.image.pixels.begin() + static_cast<std::ptrdiff_t>(src),
                        item.image.width,
                        blob.begin() + static_cast<std::ptrdiff_t>(kBitmapPixelBase + y * pitch));
        }
        writer.writeU32(static_cast<std::uint32_t>(blob.size()));
        writer.writeBytes(blob);
    }
}

void serializeMultiBitmaps(BinaryWriter& writer, const MultiBitmapComponent& component) {
    writer.writeU32(1);
    writer.writeU32(static_cast<std::uint32_t>(component.items.size()));
    for (const auto& item : component.items) {
        requireImage(item.image, "MultiBitmap image");
        const std::size_t pitch = stride4(item.image.width);
        std::vector<std::uint8_t> pixels(pitch * item.image.height,
                                         item.image.transparentIndex);
        for (std::uint32_t y = 0; y < item.image.height; ++y) {
            const auto src = static_cast<std::size_t>(item.image.height - 1 - y) * item.image.width;
            std::copy_n(item.image.pixels.begin() + static_cast<std::ptrdiff_t>(src),
                        item.image.width,
                        pixels.begin() + static_cast<std::ptrdiff_t>(y * pitch));
        }
        writer.writeU32(static_cast<std::uint32_t>(pixels.size()));
        writer.writeBytes(pixels);
        std::vector<std::uint8_t> record(kMultiBitmapRecordBytes, 0);
        putU32(record, 0x04, item.image.transparentIndex);
        putU32(record, 0x08, static_cast<std::uint32_t>(item.x));
        putU32(record, 0x0c, static_cast<std::uint32_t>(item.y));
        putU32(record, 0x10, static_cast<std::uint32_t>(item.x + item.image.width));
        putU32(record, 0x14, static_cast<std::uint32_t>(item.y + item.image.height));
        putU32(record, 0x18, static_cast<std::uint32_t>(item.layer));
        putFixedName(record, 0x28, 12, cp1250(item.name, "MultiBitmap name"));
        // The DLL clears this field on load; scripts opt records in via ShowBitmap.
        putU32(record, 0x3c, item.shown ? 1u : 0u);
        writer.writeBytes(record);
    }
}

void serializeSounds(BinaryWriter& writer, const SoundComponent& component) {
    writer.writeU32(2); // shipped Tuwim files use schema 2 + 0x58 records
    writer.writeU32(static_cast<std::uint32_t>(component.items.size()));
    for (const auto& item : component.items) {
        if (item.wav.size() > std::numeric_limits<std::uint32_t>::max()) {
            throw std::length_error("Sound entry exceeds 4 GiB");
        }
        writer.writeU32(static_cast<std::uint32_t>(item.wav.size()));
        const auto start = static_cast<std::uint32_t>(writer.position());
        writer.writeBytes(item.wav);
        const auto end = static_cast<std::uint32_t>(writer.position());
        std::vector<std::uint8_t> record(kSoundRecordBytes, 0);
        putU32(record, 0x04, start);
        putU32(record, 0x08, end);
        putFixedName(record, 0x0c, 0x20, cp1250(item.name, "Sound name"));
        writer.writeBytes(record);
        writer.writeU32(0); // shipped records carry no trailing WAVEFORMAT block
    }
}

void serializeVideos(BinaryWriter& writer, const VideoComponent& component) {
    writer.writeU32(1);
    writer.writeU32(static_cast<std::uint32_t>(component.items.size()));
    for (std::size_t i = 0; i < component.items.size(); ++i) {
        const auto& item = component.items[i];
        std::vector<std::uint8_t> record(kVideoRecordBytes, 0);
        putU32(record, 0x10, static_cast<std::uint32_t>(item.x));
        putU32(record, 0x14, static_cast<std::uint32_t>(item.y));
        putU32(record, 0x18, static_cast<std::uint32_t>(i));
        putU32(record, 0x1c, static_cast<std::uint32_t>(i));
        const auto encodedFileName = cp1250(item.fileName, "AVI filename");
        const auto slash = encodedFileName.find_last_of("/\\");
        const auto base = slash == std::string::npos ? encodedFileName : encodedFileName.substr(slash + 1);
        const auto dot = base.find_last_of('.');
        const auto title = dot == std::string::npos ? base : base.substr(0, dot);
        putFixedName(record, 0x20, 10, title.size() > 9 ? title.substr(0, 9) : title);
        writer.writeBytes(record);
        writer.writeArchiveString(encodedFileName);
    }
}

void relocateAndWritePrivate(BinaryWriter& writer, HolderKind kind,
                             std::vector<std::uint8_t> bytes) {
    if (kind == HolderKind::TransparentVideoHolder && bytes.size() >= 8) {
        std::size_t pos = 8;
        const auto count = getU32(bytes, 4);
        const auto privateStart = writer.position();
        for (std::uint32_t i = 0; i < count; ++i) {
            if (pos + 4 > bytes.size()) throw std::runtime_error("Truncated opaque TVH block");
            const auto streamBytes = getU32(bytes, pos);
            pos += 4;
            const auto streamRel = pos;
            if (streamBytes > bytes.size() - pos) throw std::runtime_error("Truncated TVH stream");
            pos += streamBytes;
            if (pos + 0x568 > bytes.size()) throw std::runtime_error("Truncated TVH entry");
            const auto entry = pos;
            putU32(bytes, entry + 0x558,
                   static_cast<std::uint32_t>(privateStart + streamRel));
            putU32(bytes, entry + 0x55c,
                   static_cast<std::uint32_t>(privateStart + streamRel + streamBytes));
            const auto width = getU32(bytes, entry + 0x80);
            const auto height = getU32(bytes, entry + 0x84);
            pos += 0x568;
            const auto stillBytes = stride4(width) * static_cast<std::size_t>(height);
            if (stillBytes > bytes.size() - pos) throw std::runtime_error("Truncated TVH still frame");
            pos += stillBytes;
        }
    } else if (kind == HolderKind::SoundHolder && bytes.size() >= 8) {
        std::size_t pos = 8;
        const auto count = getU32(bytes, 4);
        const auto privateStart = writer.position();
        for (std::uint32_t i = 0; i < count; ++i) {
            if (pos + 4 > bytes.size()) throw std::runtime_error("Truncated opaque sound block");
            const auto wavBytes = getU32(bytes, pos);
            pos += 4;
            const auto wavRel = pos;
            if (wavBytes > bytes.size() - pos) throw std::runtime_error("Truncated opaque WAV");
            pos += wavBytes;
            if (pos + kSoundRecordBytes + 4 > bytes.size())
                throw std::runtime_error("Truncated opaque sound record");
            putU32(bytes, pos + 0x04, static_cast<std::uint32_t>(privateStart + wavRel));
            putU32(bytes, pos + 0x08, static_cast<std::uint32_t>(privateStart + wavRel + wavBytes));
            pos += kSoundRecordBytes;
            const auto formatBytes = getU32(bytes, pos);
            pos += 4;
            if (formatBytes > bytes.size() - pos) throw std::runtime_error("Truncated sound format");
            pos += formatBytes;
        }
    }
    writer.writeBytes(bytes);
}

void serializePrivate(BinaryWriter& writer, const ComponentDocument& component) {
    if (!component.privateDirty && !component.sourcePrivateState.empty()) {
        const auto* info = lookupHolder(component.wrapper.clsid);
        relocateAndWritePrivate(writer, info ? info->kind : HolderKind::Unknown,
                                component.sourcePrivateState);
        return;
    }
    std::visit([&](const auto& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, HotSpotComponent>) serializeHotSpots(writer, value);
        else if constexpr (std::is_same_v<T, SpriteComponent>) serializeSprites(writer, value);
        else if constexpr (std::is_same_v<T, BitmapComponent>) serializeBitmaps(writer, value);
        else if constexpr (std::is_same_v<T, MultiBitmapComponent>) serializeMultiBitmaps(writer, value);
        else if constexpr (std::is_same_v<T, SoundComponent>) serializeSounds(writer, value);
        else if constexpr (std::is_same_v<T, VideoComponent>) serializeVideos(writer, value);
        else if constexpr (std::is_same_v<T, OpaqueComponent>)
            relocateAndWritePrivate(writer, value.kind, value.privateState);
    }, component.state);
}

void serializeComponentList(BinaryWriter& writer,
                            const std::vector<ComponentDocument>& components) {
    writer.writeU32(1);
    writer.writeU32(static_cast<std::uint32_t>(components.size()));
    for (const auto& component : components) {
        writer.writeGuid(component.wrapper.clsid);
        serializeWrapper(writer, component.wrapper);
        serializePrivate(writer, component);
    }
}

void writeScriptState(BinaryWriter& writer, const ScriptEngineState& state) {
    writer.writeU32(4);
    writer.writeU32(static_cast<std::uint32_t>(state.switchBlocks.size()));
    for (const auto field : state.parserState) writer.writeU32(field);
    for (const auto& block : state.switchBlocks) {
        writer.writeU32(block.blockStart);
        writer.writeU32(block.blockStart);
        writer.writeU32(static_cast<std::uint32_t>(block.cases.size()));
        writer.writeU32(block.blockEnd);
        writer.writeU32(block.defaultBodyOffset);
        for (const auto& entry : block.cases) {
            writer.writeU32(entry.caseValue);
            writer.writeU32(entry.caseBodyOffset);
        }
    }
    const auto calls = std::min(state.builtinTokenOffsets.size(), state.builtinCallKinds.size());
    writer.writeU32(static_cast<std::uint32_t>(calls));
    for (std::size_t i = 0; i < calls; ++i) writer.writeU32(state.builtinTokenOffsets[i]);
    for (std::size_t i = 0; i < calls; ++i) writer.writeU32(state.builtinCallKinds[i]);
    for (const auto field : state.schema2Fields) writer.writeU32(field);
    writer.writeU32(state.schema3Field);
    writer.writeU32(state.schema4Field);
}

std::pair<std::int32_t, std::int32_t> layerRange(const PageDocument& page) {
    std::int32_t minimum = -1, maximum = -1;bool any=false;
    auto add = [&](std::int32_t layer) { if(!any){minimum=maximum=layer;any=true;}else{minimum = std::min(minimum, layer); maximum = std::max(maximum, layer);} };
    for (const auto& component : page.components) {
        std::visit([&](const auto& value) {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, HotSpotComponent>) for (const auto& i : value.items) add(i.layer);
            else if constexpr (std::is_same_v<T, SpriteComponent>) for (const auto& i : value.instances) add(i.layer);
            else if constexpr (std::is_same_v<T, BitmapComponent>) for (const auto& i : value.items) add(i.layer);
            else if constexpr (std::is_same_v<T, MultiBitmapComponent>) for (const auto& i : value.items) add(i.layer);
        }, component.state);
    }
    return {minimum, maximum};
}

std::vector<std::uint8_t> packedBackgroundDib(const IndexedImage& image, const Palette& palette) {
    requireImage(image, "Page background");
    const auto pitch = stride4(image.width);
    std::vector<std::uint8_t> bytes(40 + 1024 + pitch * image.height, 0);
    putU32(bytes, 0, 40);
    putU32(bytes, 4, image.width);
    putU32(bytes, 8, image.height);
    bytes[12] = 1; // planes u16
    bytes[14] = 8; // bpp u16
    putU32(bytes, 20, static_cast<std::uint32_t>(pitch * image.height));
    putU32(bytes, 32, 256);
    for (std::size_t i = 0; i < palette.size(); ++i) {
        bytes[40 + i * 4 + 0] = palette[i].b;
        bytes[40 + i * 4 + 1] = palette[i].g;
        bytes[40 + i * 4 + 2] = palette[i].r;
        bytes[40 + i * 4 + 3] = palette[i].reserved;
    }
    const auto pixels = 40 + 1024;
    for (std::uint32_t y = 0; y < image.height; ++y) {
        const auto src = static_cast<std::size_t>(image.height - 1 - y) * image.width;
        std::copy_n(image.pixels.begin() + static_cast<std::ptrdiff_t>(src), image.width,
                    bytes.begin() + static_cast<std::ptrdiff_t>(pixels + y * pitch));
    }
    return bytes;
}

std::string pageBanner(const std::string& explicitDate) {
    std::string date = explicitDate;
    if (date.empty()) {
        const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm local{};
#ifdef _WIN32
        localtime_s(&local, &now);
#else
        localtime_r(&now, &local);
#endif
        std::ostringstream out;
        out.imbue(std::locale::classic());
        out << std::put_time(&local, "%A, %B %d, %Y");
        date = out.str();
    }
    return "YDP Board data file. Ver:1 1996.07.    File modification: " + date;
}

} // namespace

Palette makeRgb332Palette() {
    Palette result{};
    for (std::size_t i = 0; i < result.size(); ++i) {
        result[i].r = static_cast<std::uint8_t>(((i >> 5) & 7) * 255 / 7);
        result[i].g = static_cast<std::uint8_t>(((i >> 2) & 7) * 255 / 7);
        result[i].b = static_cast<std::uint8_t>((i & 3) * 255 / 3);
    }
    return result;
}

ComponentWrapper makeDefaultWrapper(HolderKind kind) {
    const auto& holders = knownHolders();
    const auto it = std::find_if(holders.begin(), holders.end(),
                                 [&](const HolderInfo& info) { return info.kind == kind; });
    if (it == holders.end()) throw std::invalid_argument("No known wrapper for holder kind");
    ComponentWrapper wrapper;
    wrapper.wrapperVersion = 1;
    wrapper.clsid = it->clsid;
    wrapper.displayName = it->displayName;
    addComPrefix(wrapper);
    switch (kind) {
        case HolderKind::HotSpotHolder:
            addEvents(wrapper, {{"LeftButtonClickOn", "(int rectID)"},
                                {"RightButtonClickOn", "(int rectID)"},
                                {"DBLClickOn", "(int rectID)"},
                                {"MouseMoveIn", "(int rectID)"},
                                {"MouseMoveOut", "(int rectID)"},
                                {"LeftButtonClickOnUp", "(int rectID)"}});
            addMethods(wrapper, {"EnableHotSpot", "DisableHotSpot", "MoveTo", "PointInHotSpot"});
            break;
        case HolderKind::SpriteHolder:
            addEvents(wrapper, {{"InPlace", "(int spriteID,int x,int y)"},
                                {"Crash", "(int spriteID,int x,int y,int angle)"},
                                {"EndAnimation", "(int spriteID)"},
                                {"MouseMoveIn", "(int spriteID)"},
                                {"MouseMoveOut", "(int spriteID)"},
                                {"MouseClickOnDown", "(int spriteID)"},
                                {"MouseClickOnUp", "(int spriteID)"},
                                {"MouseDrop", "(int spriteID,int left,int top,int right,int bottom)"}});
            addMethods(wrapper, {"GotoXY", "ChangePhase", "MoveTo", "ShowSprite", "HideSprite",
                                 "StopAnimation", "ContinueAnimation", "SetDeep", "GetPosition",
                                 "DisableDragMode", "DisableTimers", "EnableTimers", "SynchronizeTimers",
                                 "CompEnableDrag", "CompDisableDrag"});
            break;
        case HolderKind::BitmapHolder:
            addEvents(wrapper, {{"MouseMoveIn", "(int bitmapID)"}, {"MouseMoveOut", "(int bitmapID)"},
                                {"MouseClickOnDown", "(int bitmapID)"},
                                {"MouseClickOnUp", "(int bitmapID)"},
                                {"MouseDrop", "(int bitmapID,int left,int top,int right,int bottom)"}});
            addMethods(wrapper, {"ShowBitmap", "HideBitmap", "MoveTo", "GetPixel", "EnableDrag", "DisableDrag"});
            break;
        case HolderKind::MultiBitmap:
            addEvents(wrapper, {{"MouseMoveIn", "(int bitmapID)"}, {"MouseMoveOut", "(int bitmapID)"},
                                {"MouseClickOnDown", "(int bitmapID,int x,int y,int deep)"},
                                {"MouseClickOnUp", "(int bitmapID)"}});
            addMethods(wrapper, {"ShowBitmap", "HideBitmap", "MoveTo"});
            break;
        case HolderKind::SoundHolder:
            addEvents(wrapper, {{"EndPlaySound", "(int soundID)"}});
            addMethods(wrapper, {"PlayDSound", "PlayDSoundEx", "SetPlayDSoundParameters", "Stop", "StopAll"});
            break;
        case HolderKind::VideoHolder:
            addEvents(wrapper, {{"TheEnd", "(int videoID)"}, {"MouseClickOn", "(int videoID)"}});
            addMethods(wrapper, {"Play", "PlayFromTo", "Stop", "NewOpen", "NewPlay", "NewPlayFromTo", "NewStop", "NewClose"});
            break;
        default:
            break;
    }
    return wrapper;
}

static ScriptEngineState compileScriptMetadataEncoded(const std::string& script,
                                                       std::vector<ComponentDocument>* components) {
    ScriptEngineState state;
    state.schemaVersion = 4;
    const auto offsets = handlerParenOffsets(script);
    auto offset = [&](const char* name) -> std::uint32_t {
        const auto it = offsets.find(name);
        return it == offsets.end() ? 0u : it->second;
    };
    state.parserState[0] = offset("OnTimer");
    state.parserState[1] = offset("OnKeyDown");
    state.parserState[2] = offset("OnClosePage");
    state.parserState[3] = offset("OnOpenPage");
    state.schema2Fields[0] = offset("OnLButtonDown");
    state.schema2Fields[1] = offset("OnLButtonUp");
    state.schema2Fields[2] = offset("OnRButtonDown");
    state.schema2Fields[3] = offset("OnRButtonUp");
    state.schema3Field = offset("OnMouseMoveStart");
    state.schema4Field = offset("OnMouseMoveStop");

    if (components) {
        for (auto& component : *components) {
            for (auto& fn : component.wrapper.functions) {
                const auto it = offsets.find(component.wrapper.displayName + "." + fn.displayName);
                fn.dispatchIdOrOffset = it == offsets.end() ? 0u : it->second;
            }
        }
    }

    const auto tokens = runtime::tokenize(script);
    for (std::size_t i = 0; i < tokens.size() && state.switchBlocks.size() < 100; ++i) {
        if (tokens[i].kind != runtime::TokenKind::Identifier || tokens[i].text != "switch" ||
            i + 1 >= tokens.size() || !isPunct(tokens[i + 1], "(")) continue;
        std::size_t brace = i + 1;
        int parens = 0;
        for (; brace < tokens.size(); ++brace) {
            if (isPunct(tokens[brace], "(")) ++parens;
            else if (isPunct(tokens[brace], ")")) --parens;
            else if (parens == 0 && isPunct(tokens[brace], "{")) break;
        }
        if (brace >= tokens.size()) continue;
        int depth = 1;
        ScriptSwitchBlock block;
        block.blockStart = static_cast<std::uint32_t>(tokens[i + 1].offset);
        for (std::size_t j = brace + 1; j < tokens.size(); ++j) {
            if (isPunct(tokens[j], "{")) { ++depth; continue; }
            if (isPunct(tokens[j], "}")) {
                if (--depth == 0) {
                    block.blockEnd = static_cast<std::uint32_t>(tokens[j].offset + tokens[j].length);
                    break;
                }
                continue;
            }
            if (depth != 1 || tokens[j].kind != runtime::TokenKind::Identifier) continue;
            const bool isCase = tokens[j].text == "case";
            const bool isDefault = tokens[j].text == "default";
            if (!isCase && !isDefault) continue;
            std::size_t colon = j + 1;
            while (colon < tokens.size() && !isPunct(tokens[colon], ":")) ++colon;
            if (colon >= tokens.size()) continue;
            const std::uint32_t body = colon + 1 < tokens.size()
                ? static_cast<std::uint32_t>(tokens[colon + 1].offset) : 0u;
            if (isDefault) block.defaultBodyOffset = body;
            else if (j + 1 < tokens.size()) {
                try {
                    const auto value = static_cast<std::uint32_t>(std::stoll(tokens[j + 1].text, nullptr, 0));
                    block.cases.push_back({value, body});
                } catch (...) {
                    // The original cache accepts integer constants only. The
                    // runtime reparses non-constant cases from script text.
                }
            }
        }
        state.switchBlocks.push_back(std::move(block));
    }

    const auto calls = runtime::collectCalls(script, tokens);
    const auto& builtins = runtime::hostBuiltinNames();
    for (const auto& call : calls) {
        if (!call.isBuiltin || state.builtinTokenOffsets.size() >= 100) continue;
        const auto it = std::find(builtins.begin(), builtins.end(), call.name);
        if (it == builtins.end()) continue;
        state.builtinTokenOffsets.push_back(static_cast<std::uint32_t>(call.offset));
        state.builtinCallKinds.push_back(static_cast<std::uint32_t>(std::distance(builtins.begin(), it)));
    }
    return state;
}

ScriptEngineState compileScriptMetadata(const std::string& script,
                                        std::vector<ComponentDocument>* components) {
    return compileScriptMetadataEncoded(cp1250(script,"Page script"),components);
}

std::vector<std::uint8_t> serializeProjectManifest(const ProjectDocument& project) {
    BinaryWriter writer;
    writer.writeU32(project.manifestVersion);
    writer.writeArchiveString(cp1250(project.startupPage, "Startup page"));
    writer.writeArchiveString(cp1250(project.currentPageOrGroupState,"Current page/group state"));
    writer.writeU32(project.audioPresetIndex);
    std::vector<std::string> pages;
    for (const auto& page : project.pages) if (page.listedInManifest) pages.push_back(page.fileName);
    writer.writeU32(static_cast<std::uint32_t>(pages.size()));
    for (const auto& page : pages) writer.writeArchiveString(cp1250(page, "Page filename"));
    std::vector<std::string> groups;
    for (const auto& group : project.groups) if (group.listedInManifest) groups.push_back(group.fileName);
    writer.writeU32(static_cast<std::uint32_t>(groups.size()));
    for (const auto& group : groups) writer.writeArchiveString(cp1250(group, "Group filename"));
    if (project.manifestVersion != 0) {
        std::string signature;
        const auto rawSignature = cp1250(project.signature, "Project signature");
        signature.reserve(rawSignature.size());
        for (const unsigned char c : rawSignature) {
            signature.push_back(static_cast<char>(c + 0x21));
        }
        writer.writeArchiveString(signature);
    }
    writer.writeU32(project.globalScriptVersion);
    writer.writeArchiveString(cp1250(project.globalScript, "Global script"));
    auto generated=writer.takeBytes();if(!project.sourceManifestBytes.empty()&&!project.sourceManifestCanonicalBytes.empty()&&generated==project.sourceManifestCanonicalBytes)return project.sourceManifestBytes;return generated;
}

std::vector<std::uint8_t> serializePage(const PageDocument& page, const ExportContext& context) {
    const bool inheritedPaletteUnchanged=!page.usesProjectPalette||page.palette==context.projectPalette;
    if (!page.dirty && inheritedPaletteUnchanged && context.preserveUnchanged && !page.sourceBytes.empty()) return page.sourceBytes;
    if (page.immutablePassthrough) throw std::runtime_error("Cannot regenerate an immutable imported page");
    auto components = page.components;
    const auto encodedScript = cp1250(page.script, "Page script");
    const auto scriptState = compileScriptMetadata(page.script, &components);
    const Palette& palette = page.usesProjectPalette ? context.projectPalette : page.palette;
    BinaryWriter writer;
    writer.writeFixedString(pageBanner(context.bannerDate).substr(0, 100), 100);
    writer.writeU32(1);
    writer.writeI32(page.rect.left); writer.writeI32(page.rect.top);
    writer.writeI32(page.rect.right); writer.writeI32(page.rect.bottom);
    const auto layers = layerRange(page);
    writer.writeI32(layers.first); writer.writeI32(layers.second);
    writer.writeU32(1);
    writer.writeU8(page.backgroundColorIndex);
    writer.writeU32(1024);
    for (const auto& color : palette) {
        writer.writeU8(color.r); writer.writeU8(color.g);
        writer.writeU8(color.b); writer.writeU8(color.reserved);
    }
    if (page.background) {
        const auto dib = packedBackgroundDib(*page.background, palette);
        writer.writeU32(static_cast<std::uint32_t>(dib.size()));
        writer.writeBytes(dib);
    } else {
        writer.writeU32(0);
    }
    serializeComponentList(writer, components);
    writer.writeU32(1);
    writer.writeArchiveString(encodedScript);
    writeScriptState(writer, scriptState);
    writer.writeU32(0);
    return writer.takeBytes();
}

std::vector<std::uint8_t> serializeGroup(const GroupDocument& group, const ExportContext& context) {
    (void)context;
    if (!group.dirty && context.preserveUnchanged && !group.sourceBytes.empty()) return group.sourceBytes;
    if (group.immutablePassthrough) throw std::runtime_error("Cannot regenerate an immutable imported group");
    BinaryWriter writer;
    writer.writeU32(static_cast<std::uint32_t>(group.cursors.size()));
    for (const auto& cursor : group.cursors) {
        requireImage(cursor.image, "Cursor image");
        const auto cursorName = cp1250(cursor.name, "Cursor name");
        if (cursorName.size() >= 12) throw std::invalid_argument("Cursor name exceeds 11 bytes");
        std::vector<std::uint8_t> record(0x2c, 0);
        putU32(record, 0x00, cursor.flags);
        putU32(record, 0x04, cursor.image.transparentIndex);
        putU32(record, 0x08, cursor.image.width);
        putU32(record, 0x0c, cursor.image.height);
        putU32(record, 0x10, static_cast<std::uint32_t>(cursor.hotX));
        putU32(record, 0x14, static_cast<std::uint32_t>(cursor.hotY));
        putU32(record, 0x18, cursor.image.width);
        putFixedName(record, 0x1c, 12, cursorName);
        writer.writeBytes(record);
        writer.writeU32(static_cast<std::uint32_t>(cursor.image.pixels.size()));
        writer.writeBytes(cursor.image.pixels);
    }
    serializeComponentList(writer, group.components);
    return writer.takeBytes();
}

std::vector<Diagnostic> validateProject(const ProjectDocument& project) {
    std::vector<Diagnostic> diagnostics;
    auto error = [&](std::string code, std::string message, std::string object) {
        diagnostics.push_back({DiagnosticSeverity::Error, std::move(code), std::move(message),
                               std::move(object)});
    };
    auto warning = [&](std::string code, std::string message, std::string object) {
        diagnostics.push_back({DiagnosticSeverity::Warning, std::move(code), std::move(message),
                               std::move(object)});
    };
    auto encodedSize = [&](const std::string& value, const std::string& object) -> std::optional<std::size_t> {
        bool ok = false; const auto encoded = utf8ToCp1250(value, &ok);
        if (!ok) { error("text.cp1250", "Text is not representable in Windows-1250", object); return std::nullopt; }
        return encoded.size();
    };
    auto validLeafName = [&](const std::string& value, const std::string& object) {
        if (value.empty() || value.find_first_of("/\\:") != std::string::npos || value == "." || value == "..")
            error("filename.path", "Filename must be a non-empty leaf name", object);
    };
    std::set<std::string> looseNames;
    for(const auto& asset:project.looseAssets)looseNames.insert(cp1250CaseFoldKey(asset.fileName));
    auto checkImage = [&](const IndexedImage& image, const std::string& object) {
        const auto count = static_cast<std::uint64_t>(image.width) * image.height;
        if (!image.width || !image.height || count != image.pixels.size())
            error("image.dimensions", "Indexed pixel count does not match image dimensions", object);
        if (!image.sourceRgba.empty() && image.sourceRgba.size() != count * 4)
            error("image.source", "RGBA source pixel count does not match image dimensions", object);
    };
    auto checkVideo = [&](const VideoItemDocument& item,bool requireAsset=true) {
        validLeafName(item.fileName,item.fileName);encodedSize(item.fileName,item.fileName);
        if(std::filesystem::path(cp1250CaseFoldKey(item.fileName)).extension()!=".avi")error("video.extension","External video must use .avi",item.fileName);
        if(!looseNames.count(cp1250CaseFoldKey(item.fileName))){if(requireAsset)error("video.asset","External AVI is not packaged in project assets",item.fileName);else warning("video.asset","Legacy AVI is missing and will not be copied",item.fileName);}
        const auto slash=item.fileName.find_last_of("/\\");const auto base=slash==std::string::npos?item.fileName:item.fileName.substr(slash+1);const auto dot=base.find_last_of('.');const auto title=dot==std::string::npos?base:base.substr(0,dot);const auto size=encodedSize(title,item.fileName+":title");if(size&&*size>9)warning("video.title_truncated","Legacy VideoHolder stores only the first nine title bytes; the AVI filename remains intact",item.fileName);
    };
    std::set<std::string> names;
    std::set<std::string> pageNames;
    for (const auto& page : project.pages) pageNames.insert(cp1250CaseFoldKey(page.fileName));
    std::map<std::string, std::vector<const Palette*>> groupPalettes;
    for (const auto& page : project.pages) {
        const bool regenerate=page.dirty||page.sourceBytes.empty();
        const auto lower = cp1250CaseFoldKey(page.fileName);
        validLeafName(page.fileName, page.fileName);
        if (std::filesystem::path(lower).extension() != ".bdf")
            error("page.extension", "Page filename must end in .bdf", page.fileName);
        if (!names.insert(lower).second) error("filename.duplicate", "Filename is not unique case-insensitively", page.fileName);
        if (page.rect.right <= page.rect.left || page.rect.bottom <= page.rect.top)
            error("page.rect", "Page rectangle must have positive dimensions", page.fileName);
        const auto handlers = runtime::discoverHandlers(page.script);
        auto hasPageSignature = [&](const char* name) { return std::any_of(handlers.begin(), handlers.end(), [&](const runtime::ScriptHandler& h) {
            return h.name == name && h.returnType == "void" && h.parameters.empty(); }); };
        if (regenerate&&!hasPageSignature("OnOpenPage")) error("script.open", "Required signature is void OnOpenPage(void)", page.fileName);
        if (regenerate&&!hasPageSignature("OnClosePage")) error("script.close", "Required signature is void OnClosePage(void)", page.fileName);
        std::set<std::string> holders;
        std::map<std::string,const ComponentDocument*> holderDocuments;
        for (const auto& component : page.components) {
            if (!holders.insert(lowerAscii(component.wrapper.displayName)).second && component.editable)
                error("holder.duplicate", "Duplicate editable holder namespace", page.fileName + ":" + component.wrapper.displayName);
            holderDocuments.emplace(lowerAscii(component.wrapper.displayName),&component);
            if(regenerate)std::visit([&](const auto& state) { using T = std::decay_t<decltype(state)>;
                if constexpr (std::is_same_v<T, HotSpotComponent>) {
                    std::set<int> ids; for (const auto& item : state.items) {
                        if (!ids.insert(item.id).second) error("hotspot.id", "Hotspot IDs must be unique", page.fileName);
                        if (item.right <= item.left || item.bottom <= item.top) error("hotspot.rect", "Hotspot bounds must be positive", page.fileName);
                        encodedSize(item.name, page.fileName + ":hotspot");
                    }
                } else if constexpr (std::is_same_v<T, SpriteComponent>) {
                    for (const auto& definition : state.definitions) {
                        const auto size=encodedSize(definition.name,page.fileName+":sprite");if(size&&*size>=32)error("name.fixed","Sprite name exceeds 31 encoded bytes",definition.name);
                        if(definition.frames.empty())error("sprite.frames","Sprite definition needs at least one frame",definition.name);
                        for(const auto& frame:definition.frames)checkImage(frame.image,page.fileName+":"+definition.name);
                    }
                    for(const auto& item:state.instances)if(item.definitionIndex>=state.definitions.size())error("sprite.reference","Sprite instance references a missing definition",page.fileName);
                } else if constexpr (std::is_same_v<T, BitmapComponent> || std::is_same_v<T, MultiBitmapComponent>) {
                    for(const auto& item:state.items){const auto size=encodedSize(item.name,page.fileName+":bitmap");if(size&&*size>=12)error("name.fixed","Bitmap name exceeds 11 encoded bytes",item.name);checkImage(item.image,page.fileName+":"+item.name);}
                } else if constexpr (std::is_same_v<T, SoundComponent>) {
                    for(const auto& item:state.items){const auto size=encodedSize(item.name,page.fileName+":sound");if(size&&*size>=32)error("name.fixed","Sound name exceeds 31 encoded bytes",item.name);if(item.wav.size()<12||std::memcmp(item.wav.data(),"RIFF",4)!=0)error("sound.wav","Embedded sound must be a RIFF/WAVE file",item.name);}
                } else if constexpr (std::is_same_v<T, VideoComponent>) {
                    for(const auto& item:state.items)checkVideo(item);
                }
            }, component.state);
            else if(const auto* videos=std::get_if<VideoComponent>(&component.state))for(const auto& item:videos->items)checkVideo(item,false);
        }
        auto expectedParameterCount=[](const std::string& signature){if(signature.empty()||signature=="()"||signature=="(void)")return std::size_t{0};return static_cast<std::size_t>(std::count(signature.begin(),signature.end(),',')+1);};
        if(regenerate)for(const auto& handler:handlers)if(handler.kind==runtime::HandlerKind::ComponentCallback){const auto holder=holderDocuments.find(lowerAscii(handler.component));if(holder==holderDocuments.end())continue;const auto function=std::find_if(holder->second->wrapper.functions.begin(),holder->second->wrapper.functions.end(),[&](const FunctionMember& candidate){return candidate.displayName==handler.method;});if(function==holder->second->wrapper.functions.end()){error("script.callback","Callback is not exposed by the holder wrapper",page.fileName+":"+handler.name);continue;}if(handler.returnType!="void")error("script.callback_signature","Component callback must return void",page.fileName+":"+handler.name);const auto expected=expectedParameterCount(function->descriptionOrHelpName);if(handler.parameters.size()!=expected)error("script.callback_signature","Callback parameter count does not match the holder wrapper",page.fileName+":"+handler.name);}
        encodedSize(page.fileName,page.fileName);if(regenerate)encodedSize(page.script,page.fileName+":script");
        if(regenerate&&page.background)checkImage(*page.background,page.fileName+":background");
        const auto tokens=runtime::tokenize(page.script);
        for(std::size_t i=0;i+2<tokens.size();++i)if(tokens[i].kind==runtime::TokenKind::Identifier&&isPunct(tokens[i+1],"(")&&tokens[i+2].kind==runtime::TokenKind::String){
            if(tokens[i].text=="LoadPage"&&!pageNames.count(cp1250CaseFoldKey(tokens[i+2].text))){if(regenerate)error("script.page_reference","LoadPage target does not exist",page.fileName+":"+tokens[i+2].text);else warning("script.page_reference","Legacy LoadPage target was not imported",page.fileName+":"+tokens[i+2].text);}
            if(tokens[i].text=="LoadGroup")groupPalettes[cp1250CaseFoldKey(tokens[i+2].text)].push_back(page.usesProjectPalette?&project.palette:&page.palette);
        }
        if(regenerate)for(const auto& call:runtime::collectCalls(page.script))if(!call.isBuiltin&&!holders.count(lowerAscii(call.component))&&call.component.find("_Holder")!=std::string::npos){
            const auto groupPrefix="Group."+call.component+".";const auto isGroupCall=call.offset>=groupPrefix.size()&&page.script.compare(call.offset-groupPrefix.size(),groupPrefix.size(),groupPrefix)==0;
            if(!isGroupCall)error("script.holder_reference","Script references a holder namespace not present on the page",page.fileName+":"+call.component);
        }
    }
    for (const auto& group : project.groups) {
        const bool regenerate=group.dirty||group.sourceBytes.empty();
        const auto lower = cp1250CaseFoldKey(group.fileName);
        validLeafName(group.fileName,group.fileName);
        if (std::filesystem::path(lower).extension() != ".grp")
            error("group.extension", "Group filename must end in .grp", group.fileName);
        if (!names.insert(lower).second) error("filename.duplicate", "Filename is not unique case-insensitively", group.fileName);
        encodedSize(group.fileName,group.fileName);
        std::set<std::string> cursorNames;if(regenerate)for(const auto& cursor:group.cursors){const auto size=encodedSize(cursor.name,group.fileName+":cursor");if(size&&*size>=12)error("name.fixed","Cursor name exceeds 11 encoded bytes",cursor.name);if(!cursorNames.insert(cp1250CaseFoldKey(cursor.name)).second)error("cursor.duplicate","Cursor names must be unique",cursor.name);checkImage(cursor.image,group.fileName+":"+cursor.name);}
        std::set<std::string> groupHolders;for(const auto& component:group.components){if(regenerate&&!groupHolders.insert(lowerAscii(component.wrapper.displayName)).second&&component.editable)error("holder.duplicate","Duplicate editable holder namespace",group.fileName+":"+component.wrapper.displayName);if(regenerate)std::visit([&](const auto& state){using T=std::decay_t<decltype(state)>;
            if constexpr(std::is_same_v<T,HotSpotComponent>){std::set<int> ids;for(const auto& item:state.items){if(!ids.insert(item.id).second)error("hotspot.id","Hotspot IDs must be unique",group.fileName);if(item.right<=item.left||item.bottom<=item.top)error("hotspot.rect","Hotspot bounds must be positive",group.fileName);encodedSize(item.name,group.fileName+":hotspot");}}
            else if constexpr(std::is_same_v<T,SpriteComponent>){for(const auto& definition:state.definitions){const auto size=encodedSize(definition.name,group.fileName+":sprite");if(size&&*size>=32)error("name.fixed","Sprite name exceeds 31 encoded bytes",definition.name);if(definition.frames.empty())error("sprite.frames","Sprite definition needs at least one frame",definition.name);for(const auto& frame:definition.frames)checkImage(frame.image,group.fileName+":"+definition.name);}for(const auto& item:state.instances)if(item.definitionIndex>=state.definitions.size())error("sprite.reference","Sprite instance references a missing definition",group.fileName);}
            else if constexpr(std::is_same_v<T,BitmapComponent>||std::is_same_v<T,MultiBitmapComponent>){for(const auto& item:state.items){const auto size=encodedSize(item.name,group.fileName+":bitmap");if(size&&*size>=12)error("name.fixed","Bitmap name exceeds 11 encoded bytes",item.name);checkImage(item.image,group.fileName+":"+item.name);}}
            else if constexpr(std::is_same_v<T,SoundComponent>){for(const auto& item:state.items){const auto size=encodedSize(item.name,group.fileName+":sound");if(size&&*size>=32)error("name.fixed","Sound name exceeds 31 encoded bytes",item.name);if(item.wav.size()<12||std::memcmp(item.wav.data(),"RIFF",4)!=0)error("sound.wav","Embedded sound must be a RIFF/WAVE file",item.name);}}
            else if constexpr(std::is_same_v<T,VideoComponent>){for(const auto& item:state.items)checkVideo(item);}
        },component.state);else if(const auto* videos=std::get_if<VideoComponent>(&component.state))for(const auto& item:videos->items)checkVideo(item,false);}
        const auto palettes=groupPalettes[lower];if(palettes.size()>1)for(std::size_t i=1;i<palettes.size();++i)if(*palettes[i]!=*palettes[0]){warning("group.palette","Group is loaded by pages with incompatible palettes",group.fileName);break;}
    }
    for(const auto& reference:groupPalettes)if(std::none_of(project.groups.begin(),project.groups.end(),[&](const GroupDocument& group){return cp1250CaseFoldKey(group.fileName)==reference.first;}))error("script.group_reference","LoadGroup target does not exist",reference.first);
    for(const auto& asset:project.looseAssets){validLeafName(asset.fileName,asset.fileName);encodedSize(asset.fileName,asset.fileName);const auto lower=cp1250CaseFoldKey(asset.fileName);if(!names.insert(lower).second)error("asset.collision","Loose asset collides with another exported filename",asset.fileName);}
    encodedSize(project.globalScript,"project:global script");
    if(project.manifestVersion!=0)encodedSize(project.signature,"project:signature");
    if (!project.pages.empty() && project.startupPage.empty()) error("project.startup", "A startup page is required", "project");
    else if (std::none_of(project.pages.begin(), project.pages.end(), [&](const PageDocument& page) {
                 return cp1250CaseFoldKey(page.fileName) == cp1250CaseFoldKey(project.startupPage);
             }) && !project.pages.empty()) error("project.startup", "Startup page does not exist", project.startupPage);
    return diagnostics;
}

} // namespace graphboard::authoring
