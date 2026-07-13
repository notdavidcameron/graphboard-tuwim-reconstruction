#pragma once

#include "graphboard/component.hpp"
#include "graphboard/format.hpp"
#include "graphboard/holders.hpp"

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace graphboard::authoring {

struct PaletteColor {
    std::uint8_t r = 0, g = 0, b = 0, reserved = 0;
    bool operator==(const PaletteColor& other) const {
        return r == other.r && g == other.g && b == other.b && reserved == other.reserved;
    }
    bool operator!=(const PaletteColor& other) const { return !(*this == other); }
};
using Palette = std::array<PaletteColor, 256>;

struct IndexedImage {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::vector<std::uint8_t> pixels; // top-down, tightly packed
    std::uint8_t transparentIndex = 0;
    // Kept in .gbproj for deterministic re-quantization after palette edits.
    // Imported legacy artwork has no source pixels and leaves this empty.
    std::vector<std::uint8_t> sourceRgba;
    std::uint8_t alphaCutoff = 128;
};

struct RgbaImage {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::vector<std::uint8_t> pixels; // top-down RGBA
};

struct PaletteConversionDiagnostic {
    std::uint64_t opaquePixels = 0;
    std::uint64_t transparentPixels = 0;
    double meanSquaredRgbError = 0.0;
};

struct AssetConversionResult {
    IndexedImage indexed;
    PaletteConversionDiagnostic diagnostic;
    std::vector<std::uint8_t> bottomUpStride4Pixels;
};

struct HotSpotItem {
    std::int32_t left = 0, top = 0, right = 1, bottom = 1;
    std::int32_t id = 0;
    std::int32_t layer = 0;
    bool enabled = true;
    std::string name;
};
struct HotSpotComponent {
    std::uint8_t flag0 = 0, flag1 = 0, flag2 = 0xff;
    std::uint32_t activeIndex = 0;
    std::uint32_t auxStateWord = 0;
    std::vector<HotSpotItem> items;
};

struct SpriteFrameDocument {
    IndexedImage image;
    std::uint32_t durationMs = 50;
    bool pixelHitTest = false;
};
struct SpriteDefinitionDocument {
    std::string name;
    bool pixelHitTest = false;
    std::vector<SpriteFrameDocument> frames;
};
struct SpriteInstanceDocument {
    std::uint32_t definitionIndex = 0;
    std::int32_t x = 0, y = 0, layer = 0, phase = 0;
    bool visible = true;
    bool draggable = false;
};
struct SpriteComponent {
    std::vector<SpriteDefinitionDocument> definitions;
    std::vector<SpriteInstanceDocument> instances;
};

struct BitmapItemDocument {
    std::string name;
    std::int32_t x = 0, y = 0, layer = 0;
    bool initiallyHidden = false;
    bool pixelHitTest = false;
    IndexedImage image;
};
struct BitmapComponent { std::vector<BitmapItemDocument> items; };

struct MultiBitmapItemDocument {
    std::string name;
    std::int32_t x = 0, y = 0, layer = 0;
    bool shown = true;
    IndexedImage image;
};
struct MultiBitmapComponent { std::vector<MultiBitmapItemDocument> items; };

struct SoundItemDocument {
    std::string name;
    std::vector<std::uint8_t> wav;
};
struct SoundComponent { std::vector<SoundItemDocument> items; };

struct VideoItemDocument {
    std::string fileName;
    std::int32_t x = 0, y = 0;
    std::int32_t previewWidth = 160, previewHeight = 120; // editor-only metadata
};
struct VideoComponent { std::vector<VideoItemDocument> items; };

// A known but not authorable private block. `wrapper` remains semantic so
// component callback offsets can be rebuilt when the page script changes.
struct OpaqueComponent {
    HolderKind kind = HolderKind::Unknown;
    std::vector<std::uint8_t> privateState;
};

using ComponentState = std::variant<HotSpotComponent, SpriteComponent, BitmapComponent,
                                    MultiBitmapComponent, SoundComponent, VideoComponent,
                                    OpaqueComponent>;

struct ComponentDocument {
    ComponentWrapper wrapper;
    ComponentState state;
    bool editable = true;
    // Imported typed components retain their original private block until the
    // user changes that component. This preserves every still-unknown flag and
    // cache word during script/page-only edits.
    std::vector<std::uint8_t> sourcePrivateState;
    bool privateDirty = true;
    bool editorVisible = true;
    bool editorLocked = false;
};

struct CursorDocument {
    std::uint32_t flags = 1;
    std::int32_t hotX = 0, hotY = 0;
    std::string name;
    IndexedImage image;
};

struct PageDocument {
    std::string id;
    std::string fileName;
    Rect rect{0, 0, 640, 480};
    bool listedInManifest = true;
    Palette palette{};
    bool usesProjectPalette = true;
    std::uint8_t backgroundColorIndex = 0;
    std::optional<IndexedImage> background;
    std::vector<ComponentDocument> components;
    std::string script = "void OnOpenPage(void)\r\n{\r\n}\r\n\r\nvoid OnClosePage(void)\r\n{\r\n}\r\n";
    std::vector<std::uint8_t> sourceBytes;
    bool dirty = true;
    bool immutablePassthrough = false;
};

struct GroupDocument {
    std::string id;
    std::string fileName;
    bool listedInManifest = true;
    std::vector<CursorDocument> cursors;
    std::vector<ComponentDocument> components;
    std::vector<std::uint8_t> sourceBytes;
    bool dirty = true;
    bool immutablePassthrough = false;
};

struct LooseAsset {
    std::string fileName;
    std::vector<std::uint8_t> bytes;
};

class AssetStore {
public:
    explicit AssetStore(std::filesystem::path projectRoot) : root_(std::move(projectRoot)) {}
    std::string put(const std::vector<std::uint8_t>& bytes, const std::string& extension) const;
    std::vector<std::uint8_t> get(const std::string& relativePath) const;
    const std::filesystem::path& root() const { return root_; }
private:
    std::filesystem::path root_;
};

struct ProjectDocument {
    std::uint32_t schemaVersion = 1;
    std::uint32_t manifestVersion = 1;
    std::uint32_t globalScriptVersion = 1;
    std::string title = "New GraphBoard Project";
    std::string signature = "GraphBoard Studio";
    std::uint32_t audioPresetIndex = 11;
    std::string startupPage;
    std::string currentPageOrGroupState;
    std::string globalScript;
    Palette palette{};
    std::vector<PageDocument> pages;
    std::vector<GroupDocument> groups;
    std::vector<LooseAsset> looseAssets;
    std::vector<std::uint8_t> sourceManifestBytes;
    std::vector<std::uint8_t> sourceManifestCanonicalBytes;
};

// UI-independent document history. Commands store the pre-change document;
// this intentionally favors robust, testable undo over editor-widget coupling.
class CommandStack {
public:
    explicit CommandStack(std::size_t limit = 100) : limit_(limit) {}
    void record(ProjectDocument before);
    bool undo(ProjectDocument& document);
    bool redo(ProjectDocument& document);
    bool canUndo() const { return !undo_.empty(); }
    bool canRedo() const { return !redo_.empty(); }
    void clear();
private:
    std::size_t limit_ = 100;
    std::vector<ProjectDocument> undo_;
    std::vector<ProjectDocument> redo_;
};

std::string newDocumentId();

enum class DiagnosticSeverity { Info, Warning, Error };
struct Diagnostic {
    DiagnosticSeverity severity = DiagnosticSeverity::Error;
    std::string code;
    std::string message;
    std::string object;
};

struct ExportContext {
    Palette projectPalette{};
    bool preserveUnchanged = true;
    std::string bannerDate; // empty => current local date
};

Palette makeRgb332Palette();
Palette generatePalette(const std::vector<RgbaImage>& images,
                        std::uint8_t transparentIndex = 0,
                        std::uint8_t alphaCutoff = 128);
IndexedImage quantizeImage(const RgbaImage& image, const Palette& palette,
                           std::uint8_t transparentIndex = 0,
                           std::uint8_t alphaCutoff = 128);
AssetConversionResult convertAsset(const RgbaImage& image, const Palette& palette,
                                   std::uint8_t transparentIndex = 0,
                                   std::uint8_t alphaCutoff = 128);
ComponentWrapper makeDefaultWrapper(HolderKind kind);
ScriptEngineState compileScriptMetadata(const std::string& script,
                                        std::vector<ComponentDocument>* components = nullptr);
std::vector<std::uint8_t> serializeProjectManifest(const ProjectDocument& project);
std::vector<std::uint8_t> serializePage(const PageDocument& page, const ExportContext& context);
std::vector<std::uint8_t> serializeGroup(const GroupDocument& group, const ExportContext& context);

std::vector<Diagnostic> validateProject(const ProjectDocument& project);
ProjectDocument importLegacyProject(const std::filesystem::path& startPrjOrBoard);
void saveProject(const std::filesystem::path& projectDirectory, const ProjectDocument& project);
ProjectDocument loadProject(const std::filesystem::path& projectDirectoryOrFile);
void exportLegacyProject(const std::filesystem::path& outputDirectory,
                         const ProjectDocument& project);

} // namespace graphboard::authoring
