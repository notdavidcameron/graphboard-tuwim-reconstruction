#include "graphboard/component.hpp"

namespace graphboard {

namespace {

// Reflected member names are written as: u32 byteCount, then byteCount+1 bytes
// including a trailing NUL (GraphBrdCntrItem_Serialize reads rawNameByteCount+1).
std::string readCountedNulName(BinaryReader& reader) {
    const auto byteCount = reader.readU32();
    std::string name = reader.readBytesAsString(static_cast<std::size_t>(byteCount) + 1);
    if (!name.empty() && name.back() == '\0') {
        name.pop_back();
    }
    return name;
}

} // namespace

Guid parseGuid(BinaryReader& reader) {
    Guid guid;
    const auto bytes = reader.readBytes(16);
    for (std::size_t i = 0; i < 16; ++i) {
        guid.bytes[i] = bytes[i];
    }
    return guid;
}

ComponentWrapper parseComponentWrapper(BinaryReader& reader) {
    ComponentWrapper wrapper;
    wrapper.wrapperOffset = reader.position();

    wrapper.wrapperVersion = reader.readU32();
    const auto functionCount = reader.readU32();
    wrapper.displayName = reader.readArchiveString();
    const auto propertyCount = reader.readU32();
    wrapper.clsid = parseGuid(reader);

    wrapper.functions.reserve(functionCount);
    for (std::uint32_t i = 0; i < functionCount; ++i) {
        FunctionMember member;
        member.dispatchIdOrOffset = reader.readU32();
        member.rawName = readCountedNulName(reader);
        member.typeOrReturnName = reader.readArchiveString();
        member.displayName = reader.readArchiveString();
        member.descriptionOrHelpName = reader.readArchiveString();
        member.flagsOrInvokeKind = reader.readU16();
        wrapper.functions.push_back(std::move(member));
    }

    wrapper.properties.reserve(propertyCount);
    for (std::uint32_t i = 0; i < propertyCount; ++i) {
        PropertyMember member;
        member.rawName = readCountedNulName(reader);
        member.variantTypeOrDispatchMetadata = reader.readU32();
        member.displayName = reader.readArchiveString();
        member.flagsOrInvokeKind = reader.readU16();
        wrapper.properties.push_back(std::move(member));
    }

    wrapper.privateStateOffset = reader.position();
    return wrapper;
}

ComponentListHeader parseComponentListHeader(BinaryReader& reader) {
    ComponentListHeader header;
    header.version = reader.readU32();
    header.count = reader.readU32();
    header.firstItemOffset = reader.position();
    return header;
}

ComponentListItem parseComponentListItem(BinaryReader& reader) {
    ComponentListItem item;
    item.clsid = parseGuid(reader);
    item.wrapper = parseComponentWrapper(reader);
    return item;
}

} // namespace graphboard
