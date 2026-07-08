#pragma once

#include "graphboard/binary_reader.hpp"
#include "graphboard/guid.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace graphboard {

// One reflected IDispatch function member, as cached in a component wrapper.
// Layout from GraphBrdCntrItem_Serialize (Tuwim.exe:0042b920).
struct FunctionMember {
    std::uint32_t dispatchIdOrOffset = 0;
    std::string rawName;               // NUL-terminated on disk; NUL stripped here
    std::string typeOrReturnName;
    std::string displayName;
    std::string descriptionOrHelpName;
    std::uint16_t flagsOrInvokeKind = 0;
};

// One reflected IDispatch property member.
struct PropertyMember {
    std::string rawName;               // NUL-terminated on disk; NUL stripped here
    std::uint32_t variantTypeOrDispatchMetadata = 0;
    std::string displayName;
    std::uint16_t flagsOrInvokeKind = 0;
};

// The host-side reflection record written for every component instance inside a
// page/group component list. This is the wrapper metadata only; the
// component-private payload that follows it is holder-specific (see holders.hpp).
//
// Serializer: GraphBrdCntrItem_Serialize (Tuwim.exe:0042b920).
//   u32 wrapperVersion (=1)
//   u32 functionCount
//   CString displayName
//   u32 propertyCount
//   GUID clsid                        (16 raw bytes)
//   FunctionMember[functionCount]
//   PropertyMember[propertyCount]
//   <component-private state>         (parsed separately, by CLSID)
struct ComponentWrapper {
    std::uint32_t wrapperVersion = 0;
    std::string displayName;
    Guid clsid;
    std::vector<FunctionMember> functions;
    std::vector<PropertyMember> properties;

    // Byte range of the reflection record itself (excludes private payload).
    std::size_t wrapperOffset = 0;
    std::size_t privateStateOffset = 0;
};

// Read a raw 16-byte OLE CLSID at the reader's current position.
Guid parseGuid(BinaryReader& reader);

// Parse a single CntrItem reflection record (GraphBrdCntrItem_Serialize) at the
// reader's current position. On return the reader is positioned at the start of
// the component-private payload (ComponentWrapper::privateStateOffset).
//
// Note: this parses ONLY the CntrItem::Serialize record. Inside a component list
// each item is preceded by a container-written 16-byte CLSID (see
// parseComponentListItem); call parseGuid first when walking a list.
ComponentWrapper parseComponentWrapper(BinaryReader& reader);

// A page/group component list header: version + count, then `count` items.
//
// Confirmed against RZECZKA.BDF (component list at 0x048d): 8-byte header
// (version, count), then each item is
//     GUID clsid           (16 bytes, written by ComponentList_Serialize)
//     CntrItem record      (parseComponentWrapper; carries a duplicate clsid)
//     component-private     (holder-specific; see holders.hpp)
// laid out back-to-back with no additional inter-item tag.
struct ComponentListHeader {
    std::uint32_t version = 0;
    std::uint32_t count = 0;
    std::size_t firstItemOffset = 0;
};

ComponentListHeader parseComponentListHeader(BinaryReader& reader);

// One fully-parsed component-list item: the container CLSID plus the CntrItem
// reflection record. On return the reader sits at the component-private payload.
struct ComponentListItem {
    Guid clsid;                 // container-written CLSID that selects the holder
    ComponentWrapper wrapper;   // CntrItem record (wrapper.clsid duplicates clsid)
};

ComponentListItem parseComponentListItem(BinaryReader& reader);

} // namespace graphboard
