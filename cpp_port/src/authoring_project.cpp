#include "graphboard/authoring.hpp"

#include "graphboard/binary_reader.hpp"
#include "graphboard/binary_writer.hpp"
#include "graphboard/runtime/page.hpp"
#include "graphboard/text.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <map>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <system_error>

namespace graphboard::authoring {

void CommandStack::record(ProjectDocument before) {
    undo_.push_back(std::move(before));
    if (undo_.size() > limit_) undo_.erase(undo_.begin());
    redo_.clear();
}

bool CommandStack::undo(ProjectDocument& document) {
    if (undo_.empty()) return false;
    redo_.push_back(std::move(document));
    document = std::move(undo_.back());
    undo_.pop_back();
    return true;
}

bool CommandStack::redo(ProjectDocument& document) {
    if (redo_.empty()) return false;
    undo_.push_back(std::move(document));
    document = std::move(redo_.back());
    redo_.pop_back();
    return true;
}

void CommandStack::clear() { undo_.clear(); redo_.clear(); }

std::string newDocumentId() {
    std::random_device random;Guid guid;for(auto& byte:guid.bytes)byte=static_cast<std::uint8_t>(random());
    guid.bytes[7]=static_cast<std::uint8_t>((guid.bytes[7]&0x0f)|0x40);guid.bytes[8]=static_cast<std::uint8_t>((guid.bytes[8]&0x3f)|0x80);return guid.toString();
}

namespace {

using json = nlohmann::json;

std::vector<std::uint8_t> readFile(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) throw std::runtime_error("Unable to open file: " + path.string());
    stream.seekg(0, std::ios::end);
    const auto end = stream.tellg();
    if (end < 0) throw std::runtime_error("Unable to determine file size: " + path.string());
    stream.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(end));
    if (!bytes.empty()) stream.read(reinterpret_cast<char*>(bytes.data()),
                                    static_cast<std::streamsize>(bytes.size()));
    if (!stream) throw std::runtime_error("Unable to read complete file: " + path.string());
    return bytes;
}

std::string lowerAscii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

ComponentWrapper wrapperToUtf8(ComponentWrapper wrapper) {
    wrapper.displayName = cp1250ToUtf8(wrapper.displayName);
    for (auto& function : wrapper.functions) {
        function.typeOrReturnName = cp1250ToUtf8(function.typeOrReturnName);
        function.displayName = cp1250ToUtf8(function.displayName);
        function.descriptionOrHelpName = cp1250ToUtf8(function.descriptionOrHelpName);
    }
    for (auto& property : wrapper.properties)
        property.displayName = cp1250ToUtf8(property.displayName);
    return wrapper;
}

std::string contentId(const std::vector<std::uint8_t>& bytes) {
    // Stable FNV-1a content address. Include the size in the printed id so an
    // accidental 64-bit collision is also required to have identical length.
    std::uint64_t hash = 1469598103934665603ull;
    for (const auto byte : bytes) {
        hash ^= byte;
        hash *= 1099511628211ull;
    }
    std::ostringstream out;
    out << std::hex << std::setfill('0') << std::setw(16) << hash << '-' << bytes.size();
    return out.str();
}

struct ProjectAssets {
    std::filesystem::path root;
    bool write = false;

    std::string storeIn(const std::vector<std::uint8_t>& bytes, const std::string& extension,const char* folder) const {
        const auto relative = std::filesystem::path(folder) / (contentId(bytes) + extension);
        if (write) {
            BinaryWriter writer;
            writer.writeBytes(bytes);
            writer.writeFileAtomic(root / relative);
        }
        return relative.generic_string();
    }
    std::string store(const std::vector<std::uint8_t>& bytes,const std::string& extension)const{return storeIn(bytes,extension,"assets");}
    std::string storeOpaque(const std::vector<std::uint8_t>& bytes,const std::string& extension)const{return storeIn(bytes,extension,"opaque");}

    std::vector<std::uint8_t> load(const std::string& relative) const {
        const auto path=std::filesystem::path(relative).lexically_normal();if(path.is_absolute()||path.empty()||*path.begin()=="..")throw std::runtime_error("Project asset path escapes the project directory");return readFile(root/path);
    }
};

std::vector<std::uint8_t> imageBytes(const IndexedImage& image) {
    BinaryWriter writer;
    writer.writeBytes("GBI3", 4);
    writer.writeU32(image.width);
    writer.writeU32(image.height);
    writer.writeU8(image.transparentIndex);
    writer.writeU8(image.alphaCutoff);
    writer.writeBytes(image.pixels);
    writer.writeU32(static_cast<std::uint32_t>(image.sourceRgba.size()));
    writer.writeBytes(image.sourceRgba);
    return writer.takeBytes();
}

IndexedImage decodeImage(const std::vector<std::uint8_t>& bytes) {
    BinaryReader reader(bytes);
    const auto magic = reader.readBytesAsString(4);
    if (magic != "GBI1" && magic != "GBI2" && magic != "GBI3") throw std::runtime_error("Invalid .gbi asset");
    IndexedImage image;
    image.width = reader.readU32();
    image.height = reader.readU32();
    image.transparentIndex = reader.readU8();
    if(magic=="GBI3")image.alphaCutoff=reader.readU8();
    const auto count = static_cast<std::uint64_t>(image.width) * image.height;
    if (count > reader.remaining()) throw std::runtime_error("Truncated .gbi asset");
    image.pixels = reader.readBytes(static_cast<std::size_t>(count));
    if (magic == "GBI2" || magic == "GBI3") {
        const auto sourceSize = reader.readU32();
        if (sourceSize > reader.remaining()) throw std::runtime_error("Truncated .gbi source artwork");
        image.sourceRgba = reader.readBytes(sourceSize);
        if (!image.sourceRgba.empty() && image.sourceRgba.size() != count * 4)
            throw std::runtime_error("Invalid .gbi source artwork dimensions");
    }
    return image;
}

json paletteJson(const Palette& palette) {
    json result = json::array();
    for (const auto& color : palette) result.push_back({color.r, color.g, color.b, color.reserved});
    return result;
}

Palette parsePalette(const json& value) {
    if (!value.is_array() || value.size() != 256) throw std::runtime_error("Project palette must have 256 entries");
    Palette palette{};
    for (std::size_t i = 0; i < palette.size(); ++i) {
        palette[i].r = value[i].at(0).get<std::uint8_t>();
        palette[i].g = value[i].at(1).get<std::uint8_t>();
        palette[i].b = value[i].at(2).get<std::uint8_t>();
        palette[i].reserved = value[i].size() > 3 ? value[i].at(3).get<std::uint8_t>() : 0;
    }
    return palette;
}

json wrapperJson(const ComponentWrapper& wrapper) {
    json functions = json::array();
    for (const auto& function : wrapper.functions) {
        std::vector<std::uint8_t> raw(function.rawName.begin(), function.rawName.end());
        functions.push_back({{"offset", function.dispatchIdOrOffset}, {"raw", raw},
                             {"return", function.typeOrReturnName}, {"name", function.displayName},
                             {"signature", function.descriptionOrHelpName}, {"flags", function.flagsOrInvokeKind}});
    }
    json properties = json::array();
    for (const auto& property : wrapper.properties) {
        std::vector<std::uint8_t> raw(property.rawName.begin(), property.rawName.end());
        properties.push_back({{"raw", raw}, {"variant", property.variantTypeOrDispatchMetadata},
                              {"name", property.displayName}, {"flags", property.flagsOrInvokeKind}});
    }
    return {{"version", wrapper.wrapperVersion}, {"displayName", wrapper.displayName},
            {"clsid", wrapper.clsid.toString()}, {"functions", functions}, {"properties", properties}};
}

ComponentWrapper parseWrapper(const json& value) {
    ComponentWrapper wrapper;
    wrapper.wrapperVersion = value.value("version", 1u);
    wrapper.displayName = value.at("displayName").get<std::string>();
    wrapper.clsid = Guid::fromString(value.at("clsid").get<std::string>());
    for (const auto& item : value.at("functions")) {
        FunctionMember function;
        function.dispatchIdOrOffset = item.value("offset", 0u);
        const auto raw = item.value("raw", std::vector<std::uint8_t>{});
        function.rawName.assign(raw.begin(), raw.end());
        function.typeOrReturnName = item.value("return", std::string{});
        function.displayName = item.value("name", std::string{});
        function.descriptionOrHelpName = item.value("signature", std::string{});
        function.flagsOrInvokeKind = item.value("flags", static_cast<std::uint16_t>(24));
        wrapper.functions.push_back(std::move(function));
    }
    for (const auto& item : value.at("properties")) {
        PropertyMember property;
        const auto raw = item.value("raw", std::vector<std::uint8_t>{});
        property.rawName.assign(raw.begin(), raw.end());
        property.variantTypeOrDispatchMetadata = item.value("variant", 0u);
        property.displayName = item.value("name", std::string{});
        property.flagsOrInvokeKind = item.value("flags", static_cast<std::uint16_t>(24));
        wrapper.properties.push_back(std::move(property));
    }
    return wrapper;
}

json imageJson(const IndexedImage& image, const ProjectAssets& assets) {
    return {{"asset", assets.store(imageBytes(image), ".gbi")}};
}

IndexedImage parseImage(const json& value, const ProjectAssets& assets) {
    return decodeImage(assets.load(value.at("asset").get<std::string>()));
}

json componentJson(const ComponentDocument& component, const ProjectAssets& assets) {
    json out = {{"wrapper", wrapperJson(component.wrapper)}, {"editable", component.editable},
                {"privateDirty", component.privateDirty}, {"editorVisible", component.editorVisible},
                {"editorLocked", component.editorLocked}};
    if (!component.sourcePrivateState.empty())
        out["sourcePrivate"] = assets.storeOpaque(component.sourcePrivateState, ".private.bin");
    std::visit([&](const auto& state) {
        using T = std::decay_t<decltype(state)>;
        if constexpr (std::is_same_v<T, HotSpotComponent>) {
            out["kind"] = "hotspot";
            out["flags"] = {state.flag0, state.flag1, state.flag2};
            out["activeIndex"] = state.activeIndex;
            out["auxStateWord"] = state.auxStateWord;
            out["items"] = json::array();
            for (const auto& item : state.items)
                out["items"].push_back({{"rect", {item.left,item.top,item.right,item.bottom}},
                                        {"id",item.id},{"layer",item.layer},{"enabled",item.enabled},{"name",item.name}});
        } else if constexpr (std::is_same_v<T, SpriteComponent>) {
            out["kind"] = "sprite"; out["definitions"] = json::array(); out["instances"] = json::array();
            for (const auto& definition : state.definitions) {
                json frames = json::array();
                for (const auto& frame : definition.frames)
                    frames.push_back({{"image",imageJson(frame.image,assets)}, {"durationMs",frame.durationMs},
                                      {"pixelHitTest",frame.pixelHitTest}});
                out["definitions"].push_back({{"name",definition.name},{"pixelHitTest",definition.pixelHitTest},{"frames",frames}});
            }
            for (const auto& item : state.instances)
                out["instances"].push_back({{"definition",item.definitionIndex},{"x",item.x},{"y",item.y},
                                             {"layer",item.layer},{"phase",item.phase},{"visible",item.visible},
                                             {"draggable",item.draggable}});
        } else if constexpr (std::is_same_v<T, BitmapComponent>) {
            out["kind"] = "bitmap"; out["items"] = json::array();
            for (const auto& item : state.items)
                out["items"].push_back({{"name",item.name},{"x",item.x},{"y",item.y},{"layer",item.layer},
                                         {"initiallyHidden",item.initiallyHidden},{"pixelHitTest",item.pixelHitTest},
                                         {"image",imageJson(item.image,assets)}});
        } else if constexpr (std::is_same_v<T, MultiBitmapComponent>) {
            out["kind"] = "multibitmap"; out["items"] = json::array();
            for (const auto& item : state.items)
                out["items"].push_back({{"name",item.name},{"x",item.x},{"y",item.y},{"layer",item.layer},
                                         {"shown",item.shown},{"image",imageJson(item.image,assets)}});
        } else if constexpr (std::is_same_v<T, SoundComponent>) {
            out["kind"] = "sound"; out["items"] = json::array();
            for (const auto& item : state.items)
                out["items"].push_back({{"name",item.name},{"asset",assets.store(item.wav,".wav")}});
        } else if constexpr (std::is_same_v<T, VideoComponent>) {
            out["kind"] = "video"; out["items"] = json::array();
            for (const auto& item : state.items)
                out["items"].push_back({{"fileName",item.fileName},{"x",item.x},{"y",item.y},{"previewWidth",item.previewWidth},{"previewHeight",item.previewHeight}});
        } else if constexpr (std::is_same_v<T, OpaqueComponent>) {
            out["kind"] = "opaque"; out["holderKind"] = static_cast<int>(state.kind);
            out["asset"] = assets.storeOpaque(state.privateState,".bin");
        }
    }, component.state);
    return out;
}

ComponentDocument parseComponent(const json& value, const ProjectAssets& assets) {
    ComponentDocument component;
    component.wrapper = parseWrapper(value.at("wrapper"));
    component.editable = value.value("editable", true);
    component.privateDirty = value.value("privateDirty", true);
    component.editorVisible = value.value("editorVisible", true);
    component.editorLocked = value.value("editorLocked", false);
    if (value.contains("sourcePrivate"))
        component.sourcePrivateState = assets.load(value.at("sourcePrivate"));
    const auto kind = value.at("kind").get<std::string>();
    if (kind == "hotspot") {
        HotSpotComponent state;
        const auto flags = value.value("flags", std::vector<int>{0,0,255});
        if (flags.size() >= 3) { state.flag0=flags[0]; state.flag1=flags[1]; state.flag2=flags[2]; }
        state.activeIndex=value.value("activeIndex",0u); state.auxStateWord=value.value("auxStateWord",0u);
        for (const auto& entry : value.at("items")) {
            const auto r=entry.at("rect"); HotSpotItem item;
            item.left=r[0]; item.top=r[1]; item.right=r[2]; item.bottom=r[3]; item.id=entry.at("id");
            item.layer=entry.at("layer"); item.enabled=entry.value("enabled",true); item.name=entry.value("name",std::string{});
            state.items.push_back(std::move(item));
        }
        component.state=std::move(state);
    } else if (kind == "sprite") {
        SpriteComponent state;
        for (const auto& d : value.at("definitions")) {
            SpriteDefinitionDocument definition; definition.name=d.at("name"); definition.pixelHitTest=d.value("pixelHitTest",false);
            for (const auto& f : d.at("frames")) {
                SpriteFrameDocument frame; frame.image=parseImage(f.at("image"),assets);
                frame.durationMs=f.value("durationMs",50u); frame.pixelHitTest=f.value("pixelHitTest",false);
                definition.frames.push_back(std::move(frame));
            }
            state.definitions.push_back(std::move(definition));
        }
        for (const auto& e : value.at("instances")) {
            SpriteInstanceDocument item; item.definitionIndex=e.at("definition"); item.x=e.at("x"); item.y=e.at("y");
            item.layer=e.at("layer"); item.phase=e.value("phase",0); item.visible=e.value("visible",true);
            item.draggable=e.value("draggable",false); state.instances.push_back(item);
        }
        component.state=std::move(state);
    } else if (kind == "bitmap") {
        BitmapComponent state;
        for (const auto& e : value.at("items")) { BitmapItemDocument item; item.name=e.at("name"); item.x=e.at("x"); item.y=e.at("y");
            item.layer=e.at("layer"); item.initiallyHidden=e.value("initiallyHidden",false); item.pixelHitTest=e.value("pixelHitTest",false);
            item.image=parseImage(e.at("image"),assets); state.items.push_back(std::move(item)); }
        component.state=std::move(state);
    } else if (kind == "multibitmap") {
        MultiBitmapComponent state;
        for (const auto& e : value.at("items")) { MultiBitmapItemDocument item; item.name=e.at("name"); item.x=e.at("x"); item.y=e.at("y");
            item.layer=e.at("layer"); item.shown=e.value("shown",true); item.image=parseImage(e.at("image"),assets); state.items.push_back(std::move(item)); }
        component.state=std::move(state);
    } else if (kind == "sound") {
        SoundComponent state;
        for (const auto& e : value.at("items")) state.items.push_back({e.at("name"),assets.load(e.at("asset"))});
        component.state=std::move(state);
    } else if (kind == "video") {
        VideoComponent state;
        for (const auto& e : value.at("items")) state.items.push_back({e.at("fileName"),e.at("x"),e.at("y"),e.value("previewWidth",160),e.value("previewHeight",120)});
        component.state=std::move(state);
    } else if (kind == "opaque") {
        OpaqueComponent state; state.kind=static_cast<HolderKind>(value.value("holderKind",0)); state.privateState=assets.load(value.at("asset"));
        component.state=std::move(state); component.editable=false;
    } else throw std::runtime_error("Unknown project component kind: " + kind);
    return component;
}

void parseOpaquePrivate(BinaryReader& reader, HolderKind kind) {
    switch (kind) {
        case HolderKind::HotSpotHolder: parseHotSpotHolderState(reader); break;
        case HolderKind::SpriteHolder: parseSpriteHolderState(reader); break;
        case HolderKind::BitmapHolder: parseBitmapHolderState(reader); break;
        case HolderKind::MultiBitmap: parseMultiBitmapState(reader); break;
        case HolderKind::SoundHolder: parseSoundHolderState(reader); break;
        case HolderKind::VideoHolder: parseVideoHolderState(reader); break;
        case HolderKind::TransparentVideoHolder: parseTransparentVideoHolderState(reader); break;
        case HolderKind::TextHolder: parseTextHolderState(reader); break;
        case HolderKind::PanoramaHolder: parsePanoramaHolderState(reader); break;
        case HolderKind::Panorama: parsePanoramaState(reader); break;
        case HolderKind::Puzzle: parsePuzzleState(reader); break;
        case HolderKind::Recorder: parseRecorderState(reader); break;
        case HolderKind::Unknown: throw std::runtime_error("Unknown holder has no bounded private parser");
    }
}

IndexedImage indexedFromFrame(const SpriteFrame& frame) {
    return {frame.width, frame.height, frame.pixels, frame.transparentIndex};
}

ComponentDocument importComponent(BinaryReader& reader, const ComponentListItem& item) {
    const auto* info = lookupHolder(item.clsid);
    if (!info) throw std::runtime_error("Unknown component CLSID: " + item.clsid.toString());
    ComponentDocument component; component.wrapper=wrapperToUtf8(item.wrapper);
    const auto start=reader.position();
    switch (info->kind) {
        case HolderKind::HotSpotHolder: {
            const auto parsed=parseHotSpotHolderState(reader); HotSpotComponent state;
            state.flag0=parsed.flag0; state.flag1=parsed.flag1; state.flag2=parsed.flag2;
            state.activeIndex=parsed.activeIndex; state.auxStateWord=parsed.auxStateWord;
            for (const auto& h:parsed.hotspots) state.items.push_back({h.left,h.top,h.right,h.bottom,h.id,h.layer,h.enabled!=0,cp1250ToUtf8(h.name)});
            component.state=std::move(state); break;
        }
        case HolderKind::SpriteHolder: {
            const auto parsed=parseSpriteHolderState(reader); SpriteComponent state;
            for (const auto& d:parsed.definitions) { SpriteDefinitionDocument definition; definition.name=cp1250ToUtf8(d.name);
                for (const auto& f:d.frames) { SpriteFrameDocument frame; frame.image=indexedFromFrame(f); frame.pixelHitTest=!f.opaque.empty();
                    definition.pixelHitTest=definition.pixelHitTest||frame.pixelHitTest; definition.frames.push_back(std::move(frame)); }
                state.definitions.push_back(std::move(definition)); }
            for (const auto& s:parsed.instances) state.instances.push_back({s.definitionIndex,s.posX,s.posY,s.layer,s.phase,s.visible!=0,s.dragEnabled});
            component.state=std::move(state); break;
        }
        case HolderKind::BitmapHolder: {
            const auto parsed=parseBitmapHolderState(reader); BitmapComponent state;
            for (const auto& b:parsed.bitmaps) { IndexedImage image{static_cast<std::uint32_t>(b.right-b.left),static_cast<std::uint32_t>(b.bottom-b.top),b.pixels,b.transparentIndex};
                state.items.push_back({cp1250ToUtf8(b.name),b.left,b.top,b.layer,b.initiallyHidden,!b.opaque.empty(),std::move(image)}); }
            component.state=std::move(state); break;
        }
        case HolderKind::MultiBitmap: {
            const auto parsed=parseMultiBitmapState(reader); MultiBitmapComponent state;
            const auto& bytes=reader.bytes();
            for (const auto& b:parsed.records) { IndexedImage image; image.width=b.width; image.height=b.height;
                const auto pitch=(static_cast<std::size_t>(b.width)+3)&~std::size_t{3}; image.pixels.assign(static_cast<std::size_t>(b.width)*b.height,0);
                if (b.pixelOffset+b.pixelByteCount<=bytes.size()) for(std::uint32_t y=0;y<b.height;++y){ const auto src=b.pixelOffset+(b.height-1-y)*pitch;
                    if(src+b.width<=bytes.size()) std::copy_n(bytes.begin()+static_cast<std::ptrdiff_t>(src),b.width,image.pixels.begin()+static_cast<std::ptrdiff_t>(y*b.width)); }
                state.items.push_back({cp1250ToUtf8(b.name),b.left,b.top,b.layer,b.shown!=0,std::move(image)}); }
            component.state=std::move(state); break;
        }
        case HolderKind::SoundHolder: {
            const auto parsed=parseSoundHolderState(reader); SoundComponent state; const auto& bytes=reader.bytes();
            for(const auto& s:parsed.sounds){ if(s.soundOffset+s.soundByteCount>bytes.size()) throw std::runtime_error("Truncated sound payload");
                state.items.push_back({cp1250ToUtf8(s.name),std::vector<std::uint8_t>(bytes.begin()+static_cast<std::ptrdiff_t>(s.soundOffset),bytes.begin()+static_cast<std::ptrdiff_t>(s.soundOffset+s.soundByteCount))}); }
            component.state=std::move(state); break;
        }
        case HolderKind::VideoHolder: {
            const auto parsed=parseVideoHolderState(reader); VideoComponent state;
            for(const auto& v:parsed.entries) state.items.push_back({cp1250ToUtf8(v.name),v.posX,v.posY}); component.state=std::move(state); break;
        }
        default: {
            parseOpaquePrivate(reader,info->kind); const auto end=reader.position(); OpaqueComponent state; state.kind=info->kind;
            const auto& bytes=reader.bytes(); state.privateState.assign(bytes.begin()+static_cast<std::ptrdiff_t>(start),bytes.begin()+static_cast<std::ptrdiff_t>(end));
            component.state=std::move(state); component.editable=false; break;
        }
    }
    const auto end = reader.position();
    const auto& allBytes = reader.bytes();
    component.sourcePrivateState.assign(allBytes.begin() + static_cast<std::ptrdiff_t>(start),
                                        allBytes.begin() + static_cast<std::ptrdiff_t>(end));
    component.privateDirty = false;
    return component;
}

void lockAmbiguousDuplicates(std::vector<ComponentDocument>& components) {
    std::map<std::string, std::size_t> counts;
    for (const auto& component : components) ++counts[lowerAscii(component.wrapper.displayName)];
    for (auto& component : components)
        if (counts[lowerAscii(component.wrapper.displayName)] > 1) component.editable = false;
}

Palette paletteFromBdf(const BdfHeader& header, const std::vector<std::uint8_t>& bytes) {
    Palette palette=makeRgb332Palette();
    if(header.paletteByteCount>=1024 && header.paletteOffset+1024<=bytes.size()) for(std::size_t i=0;i<256;++i){
        palette[i].r=bytes[header.paletteOffset+i*4]; palette[i].g=bytes[header.paletteOffset+i*4+1];
        palette[i].b=bytes[header.paletteOffset+i*4+2]; palette[i].reserved=bytes[header.paletteOffset+i*4+3]; }
    return palette;
}

std::optional<IndexedImage> backgroundFromBdf(const BdfHeader& header,const std::vector<std::uint8_t>& bytes) {
    if(header.dibByteCount<40 || header.dibOffset+40>bytes.size()) return std::nullopt;
    auto u32=[&](std::size_t p){return static_cast<std::uint32_t>(bytes[p])|(static_cast<std::uint32_t>(bytes[p+1])<<8)|
        (static_cast<std::uint32_t>(bytes[p+2])<<16)|(static_cast<std::uint32_t>(bytes[p+3])<<24);};
    const auto base=header.dibOffset;
    const auto headerSize=u32(base);
    const auto width=static_cast<std::int32_t>(u32(base+4));
    const auto rawHeight=static_cast<std::int32_t>(u32(base+8)); const auto height=rawHeight<0?-rawHeight:rawHeight;
    const auto bpp=static_cast<std::uint16_t>(bytes[base+14]|(bytes[base+15]<<8)); if(width<=0||height<=0||bpp!=8) return std::nullopt;
    const auto colors=u32(base+32)?u32(base+32):256u; const auto pixel=base+headerSize+colors*4; const auto pitch=(static_cast<std::size_t>(width)+3)&~std::size_t{3};
    if(pixel+pitch*height>bytes.size()) return std::nullopt; IndexedImage image; image.width=width; image.height=height; image.pixels.resize(static_cast<std::size_t>(width)*height);
    for(int y=0;y<height;++y){const auto row=rawHeight<0?y:height-1-y;std::copy_n(bytes.begin()+static_cast<std::ptrdiff_t>(pixel+row*pitch),width,image.pixels.begin()+static_cast<std::ptrdiff_t>(y*width));}
    return image;
}

PageDocument importPageFile(const std::filesystem::path& path) {
    const auto bytes=readFile(path); BinaryReader reader(bytes); const auto header=parseBdfHeader(reader);
    if(header.banner.rfind("YDP Board data file.",0)!=0) throw std::runtime_error("Not a GraphBoard BDF");
    PageDocument page; page.id=newDocumentId(); page.fileName=path.filename().u8string(); page.rect=header.pageRect;
    page.palette=paletteFromBdf(header,bytes); page.usesProjectPalette=false; page.backgroundColorIndex=header.backgroundColorIndex;
    page.background=backgroundFromBdf(header,bytes); page.sourceBytes=bytes; page.dirty=false;
    try { const auto list=parseComponentListHeader(reader); page.components.reserve(list.count);
        for(std::uint32_t i=0;i<list.count;++i) page.components.push_back(importComponent(reader,parseComponentListItem(reader)));
        lockAmbiguousDuplicates(page.components);
        page.script=cp1250ToUtf8(parseScriptText(reader).text); parseScriptEngineState(reader);
    } catch(const std::exception&) { page.components.clear(); page.immutablePassthrough=true; }
    return page;
}

GroupDocument importGroupFile(const std::filesystem::path& path) {
    const auto bytes=readFile(path); BinaryReader reader(bytes); const auto parsed=parseGroupDocument(reader);
    GroupDocument group; group.id=newDocumentId(); group.fileName=path.filename().u8string(); group.sourceBytes=bytes; group.dirty=false;
    for(const auto& c:parsed.cursors) group.cursors.push_back({c.flags,c.hotX,c.hotY,cp1250ToUtf8(c.name),{c.width,c.height,c.pixels,static_cast<std::uint8_t>(c.transparentIndex)}});
    try { const auto list=parseComponentListHeader(reader); for(std::uint32_t i=0;i<list.count;++i) group.components.push_back(importComponent(reader,parseComponentListItem(reader))); lockAmbiguousDuplicates(group.components); }
    catch(const std::exception&) { group.components.clear(); group.immutablePassthrough=true; }
    return group;
}

std::filesystem::path projectFilePath(const std::filesystem::path& input) {
    if(lowerAscii(input.extension().string())==".gbproj") return input;
    return input/"project.gbproj";
}

} // namespace

std::string AssetStore::put(const std::vector<std::uint8_t>& bytes,const std::string& extension) const {
    const auto relative=std::filesystem::path("assets")/(contentId(bytes)+extension);BinaryWriter writer;writer.writeBytes(bytes);writer.writeFileAtomic(root_/relative);return relative.generic_string();
}

std::vector<std::uint8_t> AssetStore::get(const std::string& relativePath) const {
    const auto relative=std::filesystem::path(relativePath).lexically_normal();if(relative.is_absolute()||relative.empty()||*relative.begin()=="..")throw std::invalid_argument("Asset path escapes the project root");return readFile(root_/relative);
}

void saveProject(const std::filesystem::path& projectDirectory, const ProjectDocument& project) {
    const auto file=projectFilePath(projectDirectory); const auto root=file.parent_path(); ProjectAssets assets{root,true};
    json doc={{"schemaVersion",project.schemaVersion},{"manifestVersion",project.manifestVersion},{"globalScriptVersion",project.globalScriptVersion},{"title",project.title},{"signature",project.signature},
              {"audioPresetIndex",project.audioPresetIndex},{"startupPage",project.startupPage},
              {"currentPageOrGroupState",project.currentPageOrGroupState},{"globalScript",project.globalScript},{"palette",paletteJson(project.palette)}};
    if(!project.sourceManifestBytes.empty())doc["sourceManifest"]=assets.storeOpaque(project.sourceManifestBytes,".prj.raw");if(!project.sourceManifestCanonicalBytes.empty())doc["sourceManifestCanonical"]=assets.storeOpaque(project.sourceManifestCanonicalBytes,".prj.canonical");
    doc["pages"]=json::array();
    for(const auto& page:project.pages){ json p={{"id",page.id},{"fileName",page.fileName},{"listedInManifest",page.listedInManifest},
        {"rect",{page.rect.left,page.rect.top,page.rect.right,page.rect.bottom}},{"usesProjectPalette",page.usesProjectPalette},
        {"palette",paletteJson(page.palette)},{"backgroundColorIndex",page.backgroundColorIndex},{"script",page.script},
        {"dirty",page.dirty},{"immutable",page.immutablePassthrough}};
        if(page.background)p["background"]=imageJson(*page.background,assets); if(!page.sourceBytes.empty())p["source"]=assets.storeOpaque(page.sourceBytes,".bdf.raw");
        p["components"]=json::array();for(const auto& c:page.components)p["components"].push_back(componentJson(c,assets));doc["pages"].push_back(std::move(p));}
    doc["groups"]=json::array();
    for(const auto& group:project.groups){json g={{"id",group.id},{"fileName",group.fileName},{"listedInManifest",group.listedInManifest},
        {"dirty",group.dirty},{"immutable",group.immutablePassthrough}};if(!group.sourceBytes.empty())g["source"]=assets.storeOpaque(group.sourceBytes,".grp.raw");
        g["cursors"]=json::array();for(const auto& c:group.cursors)g["cursors"].push_back({{"flags",c.flags},{"hotX",c.hotX},{"hotY",c.hotY},{"name",c.name},{"image",imageJson(c.image,assets)}});
        g["components"]=json::array();for(const auto& c:group.components)g["components"].push_back(componentJson(c,assets));doc["groups"].push_back(std::move(g));}
    doc["looseAssets"]=json::array();for(const auto& a:project.looseAssets)doc["looseAssets"].push_back({{"fileName",a.fileName},{"asset",assets.store(a.bytes,std::filesystem::path(a.fileName).extension().string())}});
    const auto text=doc.dump(2)+"\n";BinaryWriter writer;writer.writeBytes(text.data(),text.size());writer.writeFileAtomic(file);
}

ProjectDocument loadProject(const std::filesystem::path& projectDirectoryOrFile) {
    const auto file=projectFilePath(projectDirectoryOrFile);const auto root=file.parent_path();ProjectAssets assets{root,false};
    std::ifstream stream(file);if(!stream)throw std::runtime_error("Unable to open project.gbproj");json doc;stream>>doc;
    ProjectDocument project;project.schemaVersion=doc.value("schemaVersion",1u);project.manifestVersion=doc.value("manifestVersion",1u);project.globalScriptVersion=doc.value("globalScriptVersion",1u);project.title=doc.value("title",std::string{});
    project.signature=doc.value("signature",std::string{});project.audioPresetIndex=doc.value("audioPresetIndex",11u);
    project.startupPage=doc.value("startupPage",std::string{});project.currentPageOrGroupState=doc.value("currentPageOrGroupState",std::string{});project.globalScript=doc.value("globalScript",std::string{});project.palette=parsePalette(doc.at("palette"));if(doc.contains("sourceManifest"))project.sourceManifestBytes=assets.load(doc.at("sourceManifest"));if(doc.contains("sourceManifestCanonical"))project.sourceManifestCanonicalBytes=assets.load(doc.at("sourceManifestCanonical"));
    for(const auto& p:doc.at("pages")){PageDocument page;page.id=p.value("id",std::string{});if(page.id.empty())page.id=newDocumentId();page.fileName=p.at("fileName");page.listedInManifest=p.value("listedInManifest",true);
        const auto r=p.at("rect");page.rect={r[0],r[1],r[2],r[3]};page.usesProjectPalette=p.value("usesProjectPalette",true);page.palette=parsePalette(p.at("palette"));
        page.backgroundColorIndex=p.value("backgroundColorIndex",0);if(p.contains("background"))page.background=parseImage(p.at("background"),assets);page.script=p.value("script",std::string{});
        page.dirty=p.value("dirty",true);page.immutablePassthrough=p.value("immutable",false);if(p.contains("source"))page.sourceBytes=assets.load(p.at("source"));
        for(const auto& c:p.at("components"))page.components.push_back(parseComponent(c,assets));project.pages.push_back(std::move(page));}
    for(const auto& g:doc.at("groups")){GroupDocument group;group.id=g.value("id",std::string{});if(group.id.empty())group.id=newDocumentId();group.fileName=g.at("fileName");group.listedInManifest=g.value("listedInManifest",true);
        group.dirty=g.value("dirty",true);group.immutablePassthrough=g.value("immutable",false);if(g.contains("source"))group.sourceBytes=assets.load(g.at("source"));
        for(const auto& c:g.at("cursors"))group.cursors.push_back({c.value("flags",1u),c.value("hotX",0),c.value("hotY",0),c.value("name",std::string{}),parseImage(c.at("image"),assets)});
        for(const auto& c:g.at("components"))group.components.push_back(parseComponent(c,assets));project.groups.push_back(std::move(group));}
    for(const auto& a:doc.value("looseAssets",json::array()))project.looseAssets.push_back({a.at("fileName"),assets.load(a.at("asset"))});
    return project;
}

ProjectDocument importLegacyProject(const std::filesystem::path& input) {
    ProjectDocument project;project.palette=makeRgb332Palette();const auto ext=lowerAscii(input.extension().string());const auto root=input.parent_path();
    if(ext==".prj"){BinaryReader reader=BinaryReader::fromFile(input);project.sourceManifestBytes=reader.bytes();const auto manifest=parseProjectManifest(reader);project.manifestVersion=manifest.version;project.globalScriptVersion=manifest.globalScriptVersion;project.title=cp1250ToUtf8(manifest.decodedSignature);
        project.signature=cp1250ToUtf8(manifest.decodedSignature);project.audioPresetIndex=manifest.audioPresetIndex;project.startupPage=cp1250ToUtf8(manifest.startupPage);project.currentPageOrGroupState=cp1250ToUtf8(manifest.currentPageOrGroupState);project.globalScript=cp1250ToUtf8(manifest.globalScript);
        std::vector<std::string> pageOrder,groupOrder;for(const auto& value:manifest.pageNames)pageOrder.push_back(cp1250ToUtf8(value));for(const auto& value:manifest.groupNames)groupOrder.push_back(cp1250ToUtf8(value));
        std::map<std::string,std::filesystem::path> pageFiles,groupFiles;for(const auto& entry:std::filesystem::directory_iterator(root)){if(!entry.is_regular_file())continue;const auto extension=lowerAscii(entry.path().extension().string()),name=cp1250CaseFoldKey(entry.path().filename().u8string());if(extension==".bdf")pageFiles.emplace(name,entry.path());else if(extension==".grp")groupFiles.emplace(name,entry.path());}
        std::set<std::string> loadedPages,loadedGroups;
        for(const auto& name:pageOrder){const auto key=cp1250CaseFoldKey(name);const auto found=pageFiles.find(key);if(found==pageFiles.end()||!loadedPages.insert(key).second)continue;try{auto page=importPageFile(found->second);page.listedInManifest=true;project.pages.push_back(std::move(page));}catch(const std::exception&){} }
        for(const auto& candidate:pageFiles)if(!loadedPages.count(candidate.first))try{auto page=importPageFile(candidate.second);page.listedInManifest=false;project.pages.push_back(std::move(page));}catch(const std::exception&){}
        for(const auto& name:groupOrder){const auto key=cp1250CaseFoldKey(name);const auto found=groupFiles.find(key);if(found==groupFiles.end()||!loadedGroups.insert(key).second)continue;try{auto group=importGroupFile(found->second);group.listedInManifest=true;project.groups.push_back(std::move(group));}catch(const std::exception&){} }
        for(const auto& candidate:groupFiles)if(!loadedGroups.count(candidate.first))try{auto group=importGroupFile(candidate.second);group.listedInManifest=false;project.groups.push_back(std::move(group));}catch(const std::exception&){}
    } else if(ext==".bdf"){auto page=importPageFile(input);project.startupPage=page.fileName;project.pages.push_back(std::move(page));}
    else if(ext==".grp")project.groups.push_back(importGroupFile(input));else throw std::invalid_argument("Expected START.PRJ, .BDF, or .GRP");
    if(!project.pages.empty())project.palette=project.pages.front().palette;for(auto& page:project.pages)page.usesProjectPalette=page.palette==project.palette;
    // External VideoHolder entries are filenames, not embedded payloads. Gather
    // matching sibling AVIs into the project so staged export is self-contained.
    std::map<std::string,std::filesystem::path> siblings;for(const auto& entry:std::filesystem::directory_iterator(root))if(entry.is_regular_file())siblings.emplace(cp1250CaseFoldKey(entry.path().filename().u8string()),entry.path());
    std::set<std::string> gathered;auto gatherVideos=[&](const std::vector<ComponentDocument>& components){for(const auto& component:components)if(const auto* videos=std::get_if<VideoComponent>(&component.state))for(const auto& video:videos->items){const auto lower=cp1250CaseFoldKey(video.fileName);const auto found=siblings.find(lower);if(found!=siblings.end()&&gathered.insert(lower).second)project.looseAssets.push_back({video.fileName,readFile(found->second)});}};for(const auto& page:project.pages)gatherVideos(page.components);for(const auto& group:project.groups)gatherVideos(group.components);if(ext==".prj")project.sourceManifestCanonicalBytes=serializeProjectManifest(project);
    return project;
}

void exportLegacyProject(const std::filesystem::path& outputDirectory,const ProjectDocument& project) {
    const auto diagnostics=validateProject(project);for(const auto& d:diagnostics)if(d.severity==DiagnosticSeverity::Error)throw std::runtime_error("Export validation failed ["+d.code+"]: "+d.message);
    std::error_code ec;std::filesystem::create_directories(outputDirectory,ec);if(ec)throw std::runtime_error("Unable to create export directory: "+ec.message());
    auto stage=outputDirectory/".gbeditor-stage";const auto absoluteOut=std::filesystem::weakly_canonical(outputDirectory,ec);ec.clear();const auto absoluteStage=std::filesystem::absolute(stage,ec);
    if(ec||absoluteStage.parent_path()!=std::filesystem::absolute(outputDirectory))throw std::runtime_error("Unsafe export staging path");
    std::filesystem::remove_all(stage,ec);ec.clear();std::filesystem::create_directories(stage,ec);if(ec)throw std::runtime_error("Unable to create export staging directory");
    ExportContext context;context.projectPalette=project.palette;context.preserveUnchanged=true;
    auto write=[&](const std::filesystem::path& path,const std::vector<std::uint8_t>& bytes){BinaryWriter writer;writer.writeBytes(bytes);writer.writeFileAtomic(path);};
    write(stage/"START.PRJ",serializeProjectManifest(project));
    for(const auto& page:project.pages)write(stage/page.fileName,serializePage(page,context));
    for(const auto& group:project.groups)write(stage/group.fileName,serializeGroup(group,context));
    for(const auto& asset:project.looseAssets)write(stage/asset.fileName,asset.bytes);
    // Reopen every authored file before touching destination files.
    {BinaryReader r=BinaryReader::fromFile(stage/"START.PRJ");parseProjectManifest(r);if(!r.eof())throw std::runtime_error("Generated START.PRJ has trailing bytes");}
    for(const auto& page:project.pages){auto p=runtime::Page::loadFromFile(stage/page.fileName);if(!p)throw std::runtime_error("Generated page failed runtime validation");}
    for(const auto& group:project.groups){BinaryReader r=BinaryReader::fromFile(stage/group.fileName);parseGroupDocument(r);const auto h=parseComponentListHeader(r);for(std::uint32_t i=0;i<h.count;++i){const auto item=parseComponentListItem(r);const auto* info=lookupHolder(item.clsid);if(!info)throw std::runtime_error("Generated group contains unknown holder");parseOpaquePrivate(r,info->kind);}}
    for(const auto& entry:std::filesystem::directory_iterator(stage)){const auto target=outputDirectory/entry.path().filename();std::filesystem::copy_file(entry.path(),target,std::filesystem::copy_options::overwrite_existing,ec);if(ec)throw std::runtime_error("Unable to publish export file: "+ec.message());}
    std::filesystem::remove_all(stage,ec);
}

} // namespace graphboard::authoring
