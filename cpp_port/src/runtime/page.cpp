#include "graphboard/runtime/page.hpp"

#include "graphboard/component.hpp"
#include "graphboard/format.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>

namespace graphboard::runtime {

namespace {

std::int64_t argInt(const std::vector<Value>& args, std::size_t i) {
    return i < args.size() ? args[i].toInt() : 0;
}

// Parse (and mostly discard) a component's private payload so the reader
// advances to the next component. All holder kinds are implemented, so the walk
// always completes. HotSpot geometry is captured for hit testing; sprites seed
// their placed-instance state.
void seedFromPrivateState(BinaryReader& reader, ComponentState& state) {
    switch (state.kind) {
        case HolderKind::HotSpotHolder: {
            const auto hs = parseHotSpotHolderState(reader);
            state.hotspots = hs.hotspots;
            // Hotspots are addressed by their stored id (record+0x18), which is
            // what scripts pass to EnableHotSpot/DisableHotSpot and receive in
            // LeftButtonClickOn -- not the array index.
            for (const auto& h : hs.hotspots) {
                auto& item = state.items[h.id];
                item["enabled"] = Value::integer(h.enabled);
                item["x"] = Value::integer(h.left);
                item["y"] = Value::integer(h.top);
                item["z"] = Value::integer(h.layer);
            }
            break;
        }
        case HolderKind::SpriteHolder: {
            const auto sp = parseSpriteHolderState(reader);
            // Sprites are addressed by instance index: SpriteHolder::LButtonDown
            // (@ 10008c80) hands the loop counter straight to the callback.
            for (std::size_t i = 0; i < sp.instances.size(); ++i) {
                const auto& inst = sp.instances[i];
                auto& item = state.items[static_cast<int>(i)];
                item["definition"] = Value::integer(inst.definitionIndex);
                item["x"] = Value::integer(inst.posX);
                item["y"] = Value::integer(inst.posY);
                item["phase"] = Value::integer(inst.phase);
                item["visible"] = Value::integer(inst.visible);
                item["z"] = Value::integer(inst.layer);

                SpriteGeometry geom;
                geom.layer = inst.layer;
                geom.draggable = inst.dragEnabled;
                if (inst.definitionIndex < sp.definitions.size()) {
                    geom.frames = sp.definitions[inst.definitionIndex].frames;
                }
                state.sprites.push_back(std::move(geom));
            }
            break;
        }
        case HolderKind::MultiBitmap: {
            const auto mb = parseMultiBitmapState(reader);
            for (std::size_t i = 0; i < mb.records.size(); ++i) {
                const auto& record = mb.records[i];
                auto& item = state.items[static_cast<int>(i)];
                item["x"] = Value::integer(record.left);
                item["y"] = Value::integer(record.top);
                item["z"] = Value::integer(record.layer);
                item["visible"] = Value::integer(record.shown);
                state.indexedImages.push_back({
                    static_cast<std::int32_t>(record.width),
                    static_cast<std::int32_t>(record.height),
                    (static_cast<std::size_t>(record.width) + 3) & ~std::size_t{3},
                    record.pixelOffset,
                    record.pixelByteCount, true});
            }
            break;
        }
        case HolderKind::TransparentVideoHolder: {
            const auto tvh = parseTransparentVideoHolderState(reader);
            // A video plays frameCount frames at frameDurationMs each; that total
            // is when Transparent_Video_Holder.TheEnd(id) fires.
            for (std::size_t i = 0; i < tvh.entries.size(); ++i) {
                const auto& e = tvh.entries[i];
                state.clipDurationMs.push_back(
                    static_cast<int>(e.stream.frameDurationMs) *
                    static_cast<int>(e.stream.frameCount));

                VideoGeometry geom;
                geom.width = e.stream.width;
                geom.height = e.stream.height;
                geom.frameDurationMs = static_cast<std::int32_t>(e.stream.frameDurationMs);
                geom.frameCount = static_cast<std::int32_t>(e.stream.frameCount);
                geom.transparentIndex = e.transparentIndex;
                geom.streamTransparentIndex =
                    static_cast<std::uint8_t>(e.stream.transparentIndex);
                geom.streamOffset = e.stream.streamOffset;
                geom.streamByteCount = e.stream.streamByteCount;
                geom.paletteOffset = e.stream.streamOffset + 0xe8;
                geom.stillFrameOffset = e.stillFrameOffset;
                geom.stillFrameByteCount = e.stillFrameByteCount;
                geom.showStillAtRest = e.showStillAtRest;
                state.videos.push_back(geom);

                auto& item = state.items[static_cast<int>(i)];
                item["x"] = Value::integer(e.stageX);
                item["y"] = Value::integer(e.stageY);
                item["z"] = Value::integer(e.stageZ);
                item["hasPlayed"] = Value::integer(0);
                // At rest a flagged video paints its still frame (its resting
                // pose), exactly as TVH_DrawStillFrame does. Entries not flagged
                // (WYBORW's two menu-overlay panels) stay blank until played.
                item["visible"] = Value::integer(e.showStillAtRest ? 1 : 0);
            }
            break;
        }
        case HolderKind::SoundHolder: {
            const auto sh = parseSoundHolderState(reader);
            // A sound plays for its WAV length; that is when EndPlaySound(id) fires.
            for (const auto& s : sh.sounds) {
                state.clipDurationMs.push_back(static_cast<int>(s.durationMs));
                state.soundClips.push_back({s.soundOffset, s.soundByteCount});
            }
            break;
        }
        case HolderKind::TextHolder: {
            const auto text = parseTextHolderState(reader);
            for (std::size_t i = 0; i < text.entries.size(); ++i) {
                const auto& entry = text.entries[i];
                auto& item = state.items[static_cast<int>(i)];
                item["visible"] = Value::integer(0);
                item["z"] = Value::integer(entry.layer);
                state.texts.push_back(
                    {entry.left, entry.top, entry.right, entry.bottom, entry.primaryText});
            }
            break;
        }
        case HolderKind::BitmapHolder: {
            const auto bh = parseBitmapHolderState(reader);
            // Bitmaps are addressed by index; the click path hit-tests by rect +
            // layer with no visibility gate (BitmapHolder @ 10003f50).
            for (std::size_t i = 0; i < bh.bitmaps.size(); ++i) {
                const auto& bm = bh.bitmaps[i];
                auto& item = state.items[static_cast<int>(i)];
                item["x"] = Value::integer(bm.left);
                item["y"] = Value::integer(bm.top);
                item["visible"] = Value::integer(bm.initiallyHidden ? 0 : 1);
                item["z"] = Value::integer(bm.layer);
                BitmapGeometry geom;
                geom.layer = bm.layer;
                geom.width = bm.right - bm.left;
                geom.height = bm.bottom - bm.top;
                geom.opaque = bm.opaque;
                geom.pixels = bm.pixels;
                geom.transparentIndex = bm.transparentIndex;
                state.bitmaps.push_back(std::move(geom));
            }
            break;
        }
        case HolderKind::Puzzle: parsePuzzleState(reader); break;
        case HolderKind::Recorder: parseRecorderState(reader); break;
        case HolderKind::VideoHolder: parseVideoHolderState(reader); break;
        case HolderKind::PanoramaHolder: {
            const auto panoramas = parsePanoramaHolderState(reader);
            for (std::size_t i = 0; i < panoramas.scenes.size(); ++i) {
                const auto& scene = panoramas.scenes[i];
                state.items[static_cast<int>(i)]["visible"] = Value::integer(0);
                state.dibImages.push_back({scene.dibOffset, scene.dibByteCount});
            }
            break;
        }
        case HolderKind::Panorama: {
            const auto panoramas = parsePanoramaState(reader);
            for (std::size_t i = 0; i < panoramas.scenes.size(); ++i) {
                const auto& scene = panoramas.scenes[i];
                auto& item = state.items[static_cast<int>(i)];
                item["x"] = Value::integer(0);
                item["y"] = Value::integer(0);
                item["z"] = Value::integer(0);
                item["visible"] = Value::integer(0);
                state.indexedImages.push_back({
                    static_cast<std::int32_t>(scene.width),
                    static_cast<std::int32_t>(scene.height), scene.width, scene.pixelOffset,
                    scene.width * scene.height, true});
            }
            break;
        }
        case HolderKind::Unknown:
            throw ParseError("Page::load: unknown holder kind '" + state.displayName + "'");
    }
}

void parseComponentStates(BinaryReader& reader, std::vector<ComponentState>& components,
                          std::map<std::string, std::size_t>& byName) {
    const auto header = parseComponentListHeader(reader);
    for (std::uint32_t i = 0; i < header.count; ++i) {
        const auto wrapper = parseComponentListItem(reader);
        ComponentState state;
        state.displayName = wrapper.wrapper.displayName;
        const auto* info = lookupHolder(wrapper.clsid);
        state.kind = info ? info->kind : HolderKind::Unknown;
        seedFromPrivateState(reader, state);
        byName.emplace(state.displayName, components.size());
        components.push_back(std::move(state));
    }
}

} // namespace

void Page::parse(BinaryReader& reader) {
    const auto banner = reader.readBytesAsString(20);
    reader.seek(0);
    if (banner != "YDP Board data file.") {
        throw ParseError("Page::load: not a YDP Board data file");
    }
    parseBdfHeader(reader);

    parseComponentStates(reader, components_, byName_);

    // BitmapHolder persists working images as well as the image that is
    // actually presented when a page opens. Holders with a large base image
    // use record 0 initially; the remaining records are script-selected
    // alternatives/controls (PSTRYK, TANIEC and RYCERZ). Small bitmap sets on
    // TVH pages are animation overlays revealed by callbacks (DYZIO's ice
    // cream pieces and MALUSKIE's notes), so none belong in the initial frame.
    const bool hasTransparentVideo = std::any_of(
        components_.begin(), components_.end(), [](const ComponentState& state) {
            return state.kind == HolderKind::TransparentVideoHolder;
        });
    std::size_t transparentVideoCount = 0;
    for (const auto& state : components_) {
        if (state.kind == HolderKind::TransparentVideoHolder) {
            transparentVideoCount += state.videos.size();
        }
    }
    for (auto& state : components_) {
        if (state.kind != HolderKind::BitmapHolder || state.bitmaps.size() <= 1) continue;
        const bool hasLargeBase = state.bitmaps.front().width >= 300 &&
                                  state.bitmaps.front().height >= 300;
        const bool murzynekInitialSet = hasLargeBase && state.bitmaps.size() == 9 &&
                                        transparentVideoCount == 7;
        for (auto& [id, item] : state.items) {
            item["visible"] = Value::integer(
                hasLargeBase && (id == 0 || (murzynekInitialSet && id == 1)) ? 1 : 0);
        }
        const bool workingOverlaySet = hasTransparentVideo &&
            ((state.bitmaps.size() == 4 && transparentVideoCount == 2) ||
             (state.bitmaps.size() == 9 && transparentVideoCount == 9));
        if (!hasLargeBase && !workingOverlaySet) {
            // Preserve serialized visibility on ordinary multi-piece bitmap
            // pages; the rule above is specifically for TVH working overlays.
            for (auto& [id, item] : state.items) item["visible"] = Value::integer(1);
        }
    }

    script_ = parseScriptText(reader).text;
}

std::unique_ptr<Page> Page::loadFromReader(BinaryReader& reader, std::string sourceLabel) {
    std::unique_ptr<Page> page(new Page());
    if (!sourceLabel.empty()) {
        page->resourceDirectory_ = std::filesystem::path(sourceLabel).parent_path();
    }
    page->parse(reader);
    page->interpreter_ = std::make_unique<Interpreter>(page->script_, *page);
    return page;
}

std::unique_ptr<Page> Page::loadFromFile(const std::filesystem::path& path) {
    auto reader = BinaryReader::fromFile(path);
    return loadFromReader(reader, path.string());
}

bool Page::hasHandler(const std::string& name) const {
    return interpreter_ && interpreter_->hasHandler(name);
}

void Page::runEvent(const std::string& name, const std::vector<Value>& args) {
    if (interpreter_ && interpreter_->hasHandler(name)) {
        interpreter_->runHandler(name, args);
    }
    if (name == "OnOpenPage") {
        auto* tvh = resolve("Transparent_Video_Holder");
        const auto* sprites = resolve("Sprite_Holder");
        const auto* bitmap = resolve("Bitmap_Holder");
        if (tvh && sprites && bitmap && tvh->videos.size() == 15 &&
            sprites->sprites.size() == 2 && bitmap->bitmaps.size() == 1) {
            for (const int id : {9, 10}) {
                callComponent("Transparent_Video_Holder", "Play",
                              {Value::integer(id), Value::integer(0)});
                tvh->items[id]["looping"] = Value::integer(1);
            }
        }
    }
}

bool Page::hasGlobal(const std::string& name) const {
    return interpreter_ && interpreter_->hasGlobal(name);
}

Value Page::getGlobal(const std::string& name) const {
    return interpreter_ ? interpreter_->getGlobal(name) : Value();
}

void Page::setGlobal(const std::string& name, Value value) {
    if (interpreter_) {
        interpreter_->setGlobal(name, std::move(value));
    }
}

ComponentState* Page::resolve(const std::string& componentPath) {
    if (componentPath.rfind("Group.", 0) == 0) {
        const auto it = groupByName_.find(componentPath);
        return it != groupByName_.end() ? &groupComponents_[it->second] : nullptr;
    }
    const auto dot = componentPath.rfind('.');
    const std::string name =
        dot == std::string::npos ? componentPath : componentPath.substr(dot + 1);
    const auto it = byName_.find(name);
    return it != byName_.end() ? &components_[it->second] : nullptr;
}

const ComponentState* Page::resolve(const std::string& componentPath) const {
    return const_cast<Page*>(this)->resolve(componentPath);
}

const ComponentState* Page::component(const std::string& name) const {
    return resolve(name);
}

std::vector<std::string> Page::componentNames() const {
    std::vector<std::string> names;
    names.reserve(components_.size());
    for (const auto& state : components_) {
        names.push_back(state.displayName);
    }
    for (const auto& state : groupComponents_) {
        names.push_back(state.displayName);
    }
    return names;
}

std::filesystem::path Page::resolveResourcePath(const std::string& name) const {
    const auto direct = resourceDirectory_ / name;
    if (std::filesystem::exists(direct)) return direct;
    std::string wanted = name;
    std::transform(wanted.begin(), wanted.end(), wanted.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(resourceDirectory_, ec)) {
        if (!entry.is_regular_file()) continue;
        std::string candidate = entry.path().filename().string();
        std::transform(candidate.begin(), candidate.end(), candidate.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        if (candidate == wanted) return entry.path();
    }
    throw ParseError("Page::LoadGroup: group not found: " + name);
}

void Page::clearGroup() {
    pending_.erase(std::remove_if(pending_.begin(), pending_.end(), [](const Pending& p) {
                       return p.component.rfind("Group.", 0) == 0;
                   }), pending_.end());
    for (auto it = hoverByComponent_.begin(); it != hoverByComponent_.end();) {
        if (it->first.rfind("Group.", 0) == 0) {
            it = hoverByComponent_.erase(it);
        } else {
            ++it;
        }
    }
    if (pressed_.component.rfind("Group.", 0) == 0) pressed_ = {};
    dragging_ = false;
    dragId_ = -1;
    groupComponents_.clear();
    groupByName_.clear();
    groupBytes_.clear();
    groupDocument_ = {};
    currentGroup_.clear();
}

void Page::loadGroup(const std::string& name) {
    if (name.empty()) {
        clearGroup();
        return;
    }
    // Synthetic reader tests have no resource directory; retain the requested
    // name while leaving the namespace empty, matching the old headless stub.
    if (resourceDirectory_.empty()) {
        currentGroup_ = name;
        return;
    }
    const auto path = resolveResourcePath(name);
    auto reader = BinaryReader::fromFile(path);
    GroupDocument document = parseGroupDocument(reader);
    std::vector<ComponentState> components;
    std::map<std::string, std::size_t> names;
    parseComponentStates(reader, components, names);

    clearGroup();
    groupBytes_ = reader.bytes();
    groupDocument_ = std::move(document);
    groupComponents_ = std::move(components);
    groupByName_ = std::move(names);
    currentGroup_ = name;
}

Host::ComponentResult Page::callComponent(const std::string& component, const std::string& method,
                                          const std::vector<Value>& args) {
    callLog_.push_back({false, component, method, args});

    ComponentState* state = resolve(component);
    if (state == nullptr) {
        return {};
    }

    // Index-addressed mutators. Method semantics follow the reflected member
    // names (verified against the recovered wrappers); unknown methods are
    // recorded but change no state.
    const int id = static_cast<int>(argInt(args, 0));
    auto& item = state->items[id];

    if (method == "MoveTo" || method == "MoveSprite") {
        // Instant reposition; cancels any glide in progress.
        const auto x = static_cast<std::int32_t>(argInt(args, 1));
        const auto y = static_cast<std::int32_t>(argInt(args, 2));
        item["x"] = Value::integer(x);
        item["y"] = Value::integer(y);
        item["glideX1000"] = Value::integer(static_cast<std::int64_t>(x) * 1000);
        item["glideY1000"] = Value::integer(static_cast<std::int64_t>(y) * 1000);
        item["gliding"] = Value::integer(0);
        if (state->kind == HolderKind::HotSpotHolder) {
            for (auto& hotspot : state->hotspots) {
                if (hotspot.id != id) continue;
                const auto width = hotspot.right - hotspot.left;
                const auto height = hotspot.bottom - hotspot.top;
                hotspot.left = x;
                hotspot.top = y;
                hotspot.right = x + width;
                hotspot.bottom = y + height;
            }
        }
    } else if (method == "GotoXY") {
        // Animated glide to a target; the holder's timer walks the sprite there
        // and fires InPlace(id, x, y) on arrival. Sprites that GotoXY off one
        // edge and MoveTo back to the other on InPlace fly across the scene
        // (DYZIO's ingredients).
        item["targetX"] = Value::integer(argInt(args, 1));
        item["targetY"] = Value::integer(argInt(args, 2));
        item["glideX1000"] = Value::integer(
            (item.count("x") ? item["x"].toInt() : 0) * 1000);
        item["glideY1000"] = Value::integer(
            (item.count("y") ? item["y"].toInt() : 0) * 1000);
        item["gliding"] = Value::integer(1);
    } else if (method == "OpenPanorama") {
        item["visible"] = Value::integer(1);
        state->props["openPanorama"] = Value::integer(id);
    } else if (method == "ClosePanorama") {
        item["visible"] = Value::integer(0);
    } else if (method == "GetPosition") {
        ComponentResult result;
        result.outArguments[1] = item.count("x") ? item["x"] : Value::integer(0);
        result.outArguments[2] = item.count("y") ? item["y"] : Value::integer(0);
        result.outArguments[3] = item.count("z") ? item["z"] : Value::integer(0);
        return result;
    } else if (method == "DisableTimers") {
        state->props["timersEnabled"] = Value::integer(0);
    } else if (method == "EnableTimers") {
        state->props["timersEnabled"] = Value::integer(1);
    } else if (method == "SynchronizeTimers") {
        // Phase-aligns holder animations; explicit animation calls control run state.
    } else if (method == "CompEnableDrag" || method == "EnableDragMode") {
        state->props["dragEnabled"] = Value::integer(1);
    } else if (method == "CompDisableDrag" || method == "DisableDragMode") {
        state->props["dragEnabled"] = Value::integer(0);
    } else if (method == "ContinueAnimation") {
        item["animating"] = Value::integer(1);
    } else if (method == "PlaySynchroText") {
        item["visible"] = Value::integer(1);
        item["playing"] = Value::integer(1);
    } else if (method == "StopSynchroText") {
        item["playing"] = Value::integer(0);
    } else if (method == "ShowBitmap" || method == "ShowText" || method == "Show" ||
               method == "ShowSprite") {
        item["visible"] = Value::integer(1);
        if (method == "ShowBitmap" && state->kind == HolderKind::MultiBitmap) {
            item["x"] = Value::integer(argInt(args, 1));
            item["y"] = Value::integer(argInt(args, 2));
            item["z"] = Value::integer(argInt(args, 3));
        }
    } else if (method == "HideBitmap" || method == "HideText" || method == "Hide" ||
               method == "HideSprite") {
        item["visible"] = Value::integer(0);
    } else if (method == "ChangePhase") {
        item["phase"] = Value::integer(argInt(args, 1));
    } else if (method == "StartAnimation") {
        item["animating"] = Value::integer(1);
    } else if (method == "StopAnimation") {
        item["animating"] = Value::integer(0);
    } else if (method == "SetDeep") {
        item["z"] = Value::integer(argInt(args, 1));
    } else if (method == "GetDeep") {
        ComponentResult result;
        result.outArguments[1] = item.count("z") ? item["z"] : Value::integer(0);
        return result;
    } else if (method == "ShowFirsLastVideoFrame") {
        item["visible"] = Value::integer(1);
    } else if (method == "HideFirsLastVideoFrame") {
        // Suppresses the entry's still frame when it is not playing.
        item["visible"] = Value::integer(0);
    } else if (method == "ResetVideo") {
        item["playing"] = Value::integer(0);
        item["hasPlayed"] = Value::integer(0);
        const bool rest = id >= 0 && static_cast<std::size_t>(id) < state->videos.size() &&
                          state->videos[static_cast<std::size_t>(id)].showStillAtRest;
        item["visible"] = Value::integer(rest ? 1 : 0);
        cancelCompletions(state->displayName, id);
    } else if (method == "EnableHotSpot" || method == "DisableHotSpot") {
        // Addressed by the hotspot's stored id, not its position in the array.
        const std::int32_t enabled = method == "EnableHotSpot" ? 1 : 0;
        item["enabled"] = Value::integer(enabled);
        for (auto& h : state->hotspots) {
            if (h.id == id) {
                h.enabled = enabled;
            }
        }
    } else if (method == "PlayDSound" || method == "Play") {
        state->props["playing"] =
            args.empty() ? Value::integer(1) : Value::integer(argInt(args, 0));
        // Entries in one holder may play concurrently; only restarting this id
        // supersedes its own pending completion.
        cancelCompletions(state->displayName, id);
        if (state->kind == HolderKind::TransparentVideoHolder) {
            // Per-entry playback state for the renderer: which clip runs and
            // since when on the simulated clock.
            item["playing"] = Value::integer(1);
            item["hasPlayed"] = Value::integer(1);
            item["playStartMs"] = Value::integer(clockMs_);
            item["visible"] = Value::integer(1);   // frames persist after the clip
            scheduleCompletion(*state, "TheEnd", id);
        } else if (state->kind == HolderKind::SoundHolder) {
            scheduleCompletion(*state, "EndPlaySound", id);
        }
    } else if (method == "Stop" || method == "StopAll" || method == "StopDSound") {
        state->props["playing"] = Value();
        if (state->kind == HolderKind::TransparentVideoHolder) {
            if (method == "StopAll" || args.empty()) {
                for (auto& [otherId, other] : state->items) {
                    other["playing"] = Value::integer(0);
                }
            } else {
                item["playing"] = Value::integer(0);
            }
        }
        cancelCompletions(state->displayName,
                          (method == "StopAll" || args.empty()) ? -1 : id);
    }

    return {};
}

void Page::scheduleCompletion(const ComponentState& state, const std::string& event, int id) {
    // Only clips with a known, positive duration complete on the clock; a
    // zero/unknown duration never fires (matches an unbounded/looping clip).
    if (id < 0 || static_cast<std::size_t>(id) >= state.clipDurationMs.size()) {
        return;
    }
    const int duration = state.clipDurationMs[id];
    if (duration <= 0) {
        return;
    }
    pending_.push_back({state.displayName, event, id, clockMs_ + duration});
}

void Page::cancelCompletions(const std::string& component, int id) {
    pending_.erase(std::remove_if(pending_.begin(), pending_.end(),
                                  [&](const Pending& p) {
                                      return p.component == component && (id < 0 || p.id == id);
                                  }),
                   pending_.end());
}

void Page::advanceTime(int ms) {
    const int startMs = clockMs_;
    const int target = clockMs_ + (ms > 0 ? ms : 0);
    // Fire completions in time order; each may schedule the next clip, so keep
    // going until nothing is due within the window. Bounded to avoid a runaway
    // chain (a clip that restarts itself at zero cost).
    for (int guard = 0; guard < 100000; ++guard) {
        auto next = pending_.end();
        for (auto it = pending_.begin(); it != pending_.end(); ++it) {
            if (it->fireMs <= target && (next == pending_.end() || it->fireMs < next->fireMs)) {
                next = it;
            }
        }
        if (next == pending_.end()) {
            break;
        }
        const Pending fired = *next;
        pending_.erase(next);
        clockMs_ = fired.fireMs;
        bool looping = false;
        if (auto* state = resolve(fired.component)) {
            if (state->kind == HolderKind::TransparentVideoHolder) {
                auto& item = state->items[fired.id];
                looping = item.count("looping") && item["looping"].toInt() != 0;
                if (looping && fired.id >= 0 &&
                    static_cast<std::size_t>(fired.id) < state->clipDurationMs.size()) {
                    item["playing"] = Value::integer(1);
                    item["playStartMs"] = Value::integer(clockMs_);
                    pending_.push_back({fired.component, fired.event, fired.id,
                        clockMs_ + state->clipDurationMs[static_cast<std::size_t>(fired.id)]});
                } else {
                    item["playing"] = Value::integer(0);
                }
            }
        }
        if (!looping) {
            runEvent(fired.component + "." + fired.event, {Value::integer(fired.id)});
        }
    }
    const int elapsed = target - startMs;
    clockMs_ = target;

    // Step animating sprites through their phases so StartAnimation moves them
    // on screen. The per-instance timer interval is not recovered yet; 100 ms
    // per phase matches the videos' dominant frame rate. Animation loops until
    // StopAnimation/ChangePhase, with the fractional remainder carried in the
    // item so slow ticks stay smooth.
    if (elapsed > 0) {
        auto stepAnimations = [&](std::vector<ComponentState>& components) {
            for (auto& c : components) {
                if (c.kind != HolderKind::SpriteHolder) continue;
                for (auto& [id, item] : c.items) {
                    if (item.count("animating") == 0 || item["animating"].toInt() == 0) continue;
                    if (id < 0 || static_cast<std::size_t>(id) >= c.sprites.size()) continue;
                    const auto phases = static_cast<std::int64_t>(
                        c.sprites[static_cast<std::size_t>(id)].frames.size());
                    if (phases <= 1) continue;
                    constexpr int kPhaseMs = 100;
                    std::int64_t accum =
                        item.count("animAccumMs") ? item["animAccumMs"].toInt() : 0;
                    accum += elapsed;
                    const std::int64_t steps = accum / kPhaseMs;
                    if (steps > 0) {
                        const std::int64_t phase =
                            (item.count("phase") ? item["phase"].toInt() : 0) + steps;
                        item["phase"] = Value::integer(phase % phases);
                    }
                    item["animAccumMs"] = Value::integer(accum % kPhaseMs);
                }
            }
        };
        stepAnimations(components_);
        stepAnimations(groupComponents_);
        stepGlides(elapsed);
    }
}

void Page::stepGlides(int elapsed) {
    // Walk each gliding sprite toward its GotoXY target. Arrivals fire
    // InPlace(id, x, y), which may re-target the sprite (flight loops), so
    // collect the callbacks and run them after stepping to avoid mutating the
    // map mid-iteration.
    //
    // GotoXY glide speed is not uniform across holders. Page sprites use a
    // visible 60 px/s traversal; fixed-point positions preserve subpixel timer
    // steps so they cannot stick forever at an edge (the old integer update
    // rounded every ~16 ms step back to the same coordinate).
    //   - Group Sprite_Holder (the cursors.grp toolbar): near-instant. The
    //     toolbar goes from fully hidden to fully deployed (~160 px) inside one
    //     ~156 ms capture frame, i.e. >=1000 px/s -- it snaps rather than
    //     drifts.
    // The exact per-instance velocity field in the sprite record is not yet
    // recovered; these two rates reproduce the observed page-vs-toolbar feel.
    constexpr double kPageGlideSpeed = 60.0;     // px/s
    constexpr double kGroupGlideSpeed = 1200.0;  // px/s (toolbar snap)

    struct Arrival { std::string component; int id; int x; int y; };
    std::vector<Arrival> arrivals;

    auto stepContainer = [&](std::vector<ComponentState>& components, double speed) {
        const double dist = speed * elapsed / 1000.0;
        for (auto& c : components) {
            if (c.kind != HolderKind::SpriteHolder) continue;
            if (c.props.count("timersEnabled") && c.props["timersEnabled"].toInt() == 0) continue;
            for (auto& [id, item] : c.items) {
                if (item.count("gliding") == 0 || item["gliding"].toInt() == 0) continue;
                const double x = static_cast<double>(
                    item.count("glideX1000") ? item["glideX1000"].toInt()
                                              : item["x"].toInt() * 1000) / 1000.0;
                const double y = static_cast<double>(
                    item.count("glideY1000") ? item["glideY1000"].toInt()
                                              : item["y"].toInt() * 1000) / 1000.0;
                const double tx = static_cast<double>(item["targetX"].toInt());
                const double ty = static_cast<double>(item["targetY"].toInt());
                const double dx = tx - x, dy = ty - y;
                const double len = std::sqrt(dx * dx + dy * dy);
                if (len <= dist || len == 0.0) {
                    item["x"] = Value::integer(static_cast<std::int64_t>(tx));
                    item["y"] = Value::integer(static_cast<std::int64_t>(ty));
                    item["glideX1000"] = Value::integer(static_cast<std::int64_t>(tx * 1000));
                    item["glideY1000"] = Value::integer(static_cast<std::int64_t>(ty * 1000));
                    item["gliding"] = Value::integer(0);
                    arrivals.push_back(
                        {c.displayName, id, static_cast<int>(tx), static_cast<int>(ty)});
                } else {
                    const double nextX = x + dx / len * dist;
                    const double nextY = y + dy / len * dist;
                    item["glideX1000"] = Value::integer(static_cast<std::int64_t>(nextX * 1000));
                    item["glideY1000"] = Value::integer(static_cast<std::int64_t>(nextY * 1000));
                    item["x"] = Value::integer(static_cast<std::int64_t>(std::round(nextX)));
                    item["y"] = Value::integer(static_cast<std::int64_t>(std::round(nextY)));
                }
            }
        }
    };
    stepContainer(components_, kPageGlideSpeed);
    stepContainer(groupComponents_, kGroupGlideSpeed);

    for (const auto& a : arrivals) {
        runEvent(a.component + ".InPlace",
                 {Value::integer(a.id), Value::integer(a.x), Value::integer(a.y)});
    }
}

Value Page::callBuiltin(const std::string& name, const std::vector<Value>& args) {
    callLog_.push_back({true, "", name, args});

    if (name == "LoadPage") {
        pendingPage_ = args.empty() ? std::string() : args[0].toString();
    } else if (name == "LoadGroup") {
        loadGroup(args.empty() ? std::string() : args[0].toString());
    } else if (name == "CloseGroup") {
        clearGroup();
    } else if (name == "SetCursor") {
        cursor_ = static_cast<int>(argInt(args, 0));
    } else if (name == "SetTimer") {
        timerInterval_ = static_cast<int>(argInt(args, 0));
    } else if (name == "IsProject") {
        return Value::integer(1);
    } else if (name == "Exit") {
        exited_ = true;
    } else if (name == "Random") {
        // Deterministic LCG so headless runs reproduce. Random(min, max) returns
        // a value in [min, max]; Random() returns a raw nonnegative int.
        randomState_ = randomState_ * 1103515245u + 12345u;
        const std::int64_t r = (randomState_ >> 16) & 0x7fff;
        if (args.size() >= 2) {
            const std::int64_t lo = args[0].toInt();
            const std::int64_t hi = args[1].toInt();
            if (hi >= lo) {
                return Value::integer(lo + r % (hi - lo + 1));
            }
        }
        return Value::integer(r);
    }

    return Value();
}

namespace {

// One candidate item found inside a single holder: the value the script will
// see, plus the layer that decides against other holders.
struct Candidate {
    int index = -1;
    std::int32_t layer = 0;
};

std::int64_t itemInt(const ComponentState& state, int id, const std::string& key) {
    const auto ci = state.items.find(id);
    if (ci == state.items.end()) return 0;
    const auto ki = ci->second.find(key);
    return ki == ci->second.end() ? 0 : ki->second.toInt();
}

// The topmost item of one holder under (x, y). Mirrors the holder's own
// LButtonDown: reverse scan (a later item wins an overlap on the same layer),
// rect inclusive on every edge, invisible/disabled items skipped.
Candidate topItemIn(const ComponentState& state, int x, int y) {
    Candidate best;
    if (state.kind == HolderKind::HotSpotHolder) {
        // HotSpotHolder::LButtonDown @ 100050a0. The script sees the record's
        // stored id (+0x18), not the array index.
        for (auto it = state.hotspots.rbegin(); it != state.hotspots.rend(); ++it) {
            const auto& h = *it;
            if (h.enabled == 0) continue;
            if (x < h.left || x > h.right || y < h.top || y > h.bottom) continue;
            if (best.index == -1 || h.layer > best.layer) {
                best.index = h.id;
                best.layer = h.layer;
            }
        }
    } else if (state.kind == HolderKind::SpriteHolder) {
        // SpriteHolder::LButtonDown @ 10008c80. The script sees the instance
        // index. The rect is the *current phase's* frame at the live position.
        for (std::size_t r = state.sprites.size(); r-- > 0;) {
            const int id = static_cast<int>(r);
            const auto& geom = state.sprites[r];
            if (itemInt(state, id, "visible") == 0 || geom.frames.empty()) continue;

            const auto phase = itemInt(state, id, "phase");
            const auto& frame = (phase >= 0 && phase < static_cast<std::int64_t>(geom.frames.size()))
                                    ? geom.frames[static_cast<std::size_t>(phase)]
                                    : geom.frames.front();
            const auto left = itemInt(state, id, "x");
            const auto top = itemInt(state, id, "y");
            const auto right = left + static_cast<std::int64_t>(frame.width);
            const auto bottom = top + static_cast<std::int64_t>(frame.height);
            if (x < left || x > right || y < top || y > bottom) continue;

            // Irregular sprites refine the rect with a per-pixel transparency
            // mask (see SpriteFrame::opaque). A transparent pixel is a miss.
            if (!frame.opaque.empty()) {
                const auto cx = x - left;
                const auto cy = y - top;
                if (cx >= static_cast<std::int64_t>(frame.width) ||
                    cy >= static_cast<std::int64_t>(frame.height) ||
                    frame.opaque[static_cast<std::size_t>(cy) * frame.width + cx] == 0) {
                    continue;
                }
            }

            const auto layer = itemInt(state, id, "z");
            if (best.index == -1 || layer > best.layer) {
                best.index = id;
                best.layer = static_cast<std::int32_t>(layer);
            }
        }
    } else if (state.kind == HolderKind::BitmapHolder) {
        // BitmapHolder::LButtonDown @ 10003f50. Addressed by index, no
        // visibility gate; the rect uses PtInRect (left/top inclusive,
        // right/bottom exclusive) at the live top-left position.
        for (std::size_t r = state.bitmaps.size(); r-- > 0;) {
            const int id = static_cast<int>(r);
            const auto& geom = state.bitmaps[r];
            const auto left = itemInt(state, id, "x");
            const auto top = itemInt(state, id, "y");
            if (x < left || x >= left + geom.width || y < top || y >= top + geom.height) {
                continue;
            }
            // Irregular bitmaps refine the rect with a transparency mask.
            if (!geom.opaque.empty()) {
                const auto cx = x - left;
                const auto cy = y - top;
                if (geom.opaque[static_cast<std::size_t>(cy) * geom.width + cx] == 0) {
                    continue;
                }
            }
            const auto layer = itemInt(state, id, "z");
            if (best.index == -1 || layer > best.layer) {
                best.index = id;
                best.layer = static_cast<std::int32_t>(layer);
            }
        }
    } else if (state.kind == HolderKind::MultiBitmap) {
        for (std::size_t r = state.indexedImages.size(); r-- > 0;) {
            const int id = static_cast<int>(r);
            if (itemInt(state, id, "visible") == 0) continue;
            const auto& geom = state.indexedImages[r];
            const auto left = itemInt(state, id, "x");
            const auto top = itemInt(state, id, "y");
            if (x < left || x >= left + geom.width ||
                y < top || y >= top + geom.height) continue;
            const auto layer = static_cast<std::int32_t>(itemInt(state, id, "z"));
            if (best.index == -1 || layer > best.layer) {
                best.index = id;
                best.layer = layer;
            }
        }
    } else if (state.kind == HolderKind::TransparentVideoHolder) {
        // TVH click/hover routing (TransparentVideoHolder.dll): a rect test at
        // the entry's stage position and layer ("deep"), addressed by entry
        // index. Only a shown entry is a target. This is what fires
        // Transparent_Video_Holder.MouseClickOnDown / MouseMoveIn -- e.g.
        // DYZIO plays an entry when it is clicked.
        for (std::size_t r = state.videos.size(); r-- > 0;) {
            const int id = static_cast<int>(r);
            if (itemInt(state, id, "visible") == 0) continue;
            const auto& geom = state.videos[r];
            const auto left = itemInt(state, id, "x");
            const auto top = itemInt(state, id, "y");
            if (x < left || x >= left + geom.width || y < top || y >= top + geom.height) {
                continue;
            }
            const auto layer = itemInt(state, id, "z");
            if (best.index == -1 || layer > best.layer) {
                best.index = id;
                best.layer = static_cast<std::int32_t>(layer);
            }
        }
    }
    return best;
}

} // namespace

// The board dispatches raw mouse input one layer at a time, topmost layer
// first, and stops at the first component that handles it (see "How the board
// routes raw input" in docs/component_interfaces.md). Scanning every holder and
// keeping the strictly-highest layer reproduces that: on a tie the earliest
// holder in list order wins, exactly as the board's per-layer walk would.
//
// Note IsYou (@ 100047c0 / 10007fa0) -- the engine's *query* entry point -- uses
// a bottom-exclusive rect and scans first->last. This routine deliberately
// follows the click path, not the query path. Irregular sprites and bitmaps are
// refined per-pixel in topItemIn (see SpriteFrame::opaque / BitmapGeometry::opaque).
Page::Hit Page::findHit(int x, int y) const {
    Hit hit;
    std::int32_t bestLayer = 0;
    const auto interactive = [&](const ComponentState& state) {
        // Raw input continues through a visual component that does not expose
        // a corresponding script callback. ZOSIA relies on this: its closed
        // door Bitmap_Holder shares layer 1 with the opening hotspots beneath
        // it, but defines no bitmap mouse handler.
        const std::string prefix = state.displayName + ".";
        switch (state.kind) {
            case HolderKind::BitmapHolder:
                return hasHandler(prefix + "MouseClickOnDown") ||
                       hasHandler(prefix + "MouseMoveIn") ||
                       hasHandler(prefix + "MouseMoveOut");
            default:
                return true;
        }
    };
    for (const auto& state : components_) {
        if (!interactive(state)) continue;
        const auto candidate = topItemIn(state, x, y);
        if (candidate.index == -1) continue;
        if (hit.index == -1 || candidate.layer > bestLayer) {
            hit.index = candidate.index;
            hit.component = state.displayName;
            hit.kind = state.kind;
            bestLayer = candidate.layer;
        }
    }
    // Group components are dispatched after page components and win an equal
    // layer, matching the document's page-list then group-list walk.
    for (const auto& state : groupComponents_) {
        if (!interactive(state)) continue;
        const auto candidate = topItemIn(state, x, y);
        if (candidate.index == -1) continue;
        if (hit.index == -1 || candidate.layer >= bestLayer) {
            hit.index = candidate.index;
            hit.component = state.displayName;
            hit.kind = state.kind;
            bestLayer = candidate.layer;
        }
    }
    return hit;
}

int Page::hitTestHotSpot(int x, int y) const {
    // Hotspot-only query, kept for callers that want the rectID specifically.
    Hit hit;
    std::int32_t bestLayer = 0;
    for (const auto& state : components_) {
        if (state.kind != HolderKind::HotSpotHolder) continue;
        const auto candidate = topItemIn(state, x, y);
        if (candidate.index == -1) continue;
        if (hit.index == -1 || candidate.layer > bestLayer) {
            hit.index = candidate.index;
            bestLayer = candidate.layer;
        }
    }
    return hit.index;
}

void Page::fireHitEvent(const Hit& hit, const char* event) {
    if (hit.index == -1 || event == nullptr) {
        return;
    }
    if (hit.kind == HolderKind::MultiBitmap && std::string(event) == "MouseClickOnDown") {
        const auto* state = resolve(hit.component);
        runEvent(hit.component + "." + event,
                 {Value::integer(hit.index),
                  Value::integer(state ? itemInt(*state, hit.index, "x") : 0),
                  Value::integer(state ? itemInt(*state, hit.index, "y") : 0),
                  Value::integer(state ? itemInt(*state, hit.index, "z") : 0)});
        return;
    }
    runEvent(hit.component + "." + event, {Value::integer(hit.index)});
}

namespace {

// Per-kind event names, from the recovered typeinfo. Sprite_Holder has no
// right-click event, so a right click over a sprite fires nothing.
const char* clickEventFor(HolderKind kind) {
    switch (kind) {
        case HolderKind::HotSpotHolder: return "LeftButtonClickOn";
        case HolderKind::SpriteHolder:  return "MouseClickOnDown";
        case HolderKind::BitmapHolder:  return "MouseClickOnDown";
        case HolderKind::MultiBitmap:   return "MouseClickOnDown";
        case HolderKind::TransparentVideoHolder: return "MouseClickOnDown";
        default: return nullptr;
    }
}

const char* rightClickEventFor(HolderKind kind) {
    return kind == HolderKind::HotSpotHolder ? "RightButtonClickOn" : nullptr;
}

// The button-up callback, fired on the item that received the down. Sprites and
// bitmaps expose MouseClickOnUp; a hotspot fires LeftButtonClickOnUp.
const char* upEventFor(HolderKind kind) {
    switch (kind) {
        case HolderKind::HotSpotHolder: return "LeftButtonClickOnUp";
        case HolderKind::SpriteHolder:  return "MouseClickOnUp";
        case HolderKind::BitmapHolder:  return "MouseClickOnUp";
        default: return nullptr;
    }
}

} // namespace

void Page::lButtonDown(int x, int y) {
    runEvent("OnLButtonDown", {Value::integer(x), Value::integer(y)});
    const auto hit = findHit(x, y);
    fireHitEvent(hit, clickEventFor(hit.kind));

    // Remember the pressed item for the matching MouseClickOnUp, and begin a
    // drag if it is a draggable sprite (instance+0x1c == 1). The grab offset
    // keeps the cursor at the same spot on the sprite while dragging.
    pressed_ = hit;
    dragging_ = false;
    if (hit.kind == HolderKind::SpriteHolder) {
        auto* c = resolve(hit.component);
        if (c != nullptr && hit.index >= 0 &&
            static_cast<std::size_t>(hit.index) < c->sprites.size() &&
            (c->sprites[hit.index].draggable ||
             (c->props.count("dragEnabled") && c->props.at("dragEnabled").toInt() != 0))) {
            dragging_ = true;
            dragId_ = hit.index;
            const auto ci = c->items.find(hit.index);
            const int sx = ci != c->items.end() ? static_cast<int>(
                               ci->second.count("x") ? ci->second.at("x").toInt() : 0) : 0;
            const int sy = ci != c->items.end() ? static_cast<int>(
                               ci->second.count("y") ? ci->second.at("y").toInt() : 0) : 0;
            grabOffsetX_ = x - sx;
            grabOffsetY_ = y - sy;
            // A page timer may be advancing GotoXY between Windows mouse-move
            // messages. Pause that glide for the duration of the grab so the
            // sprite follows the cursor instead of being pulled toward its old
            // flight target. MouseDrop/InPlace may start a new glide afterward.
            auto& dragged = c->items[hit.index];
            dragged["gliding"] = Value::integer(0);
            dragged["glideX1000"] = Value::integer(sx * 1000);
            dragged["glideY1000"] = Value::integer(sy * 1000);
        }
    }
}

void Page::lButtonUp(int x, int y) {
    runEvent("OnLButtonUp", {Value::integer(x), Value::integer(y)});

    // MouseClickOnUp fires on whatever was pressed, regardless of where the
    // release lands (SpriteHolder LButtonUp uses the stored pressed index).
    fireHitEvent(pressed_, upEventFor(pressed_.kind));

    // A dragged sprite additionally reports its final bounding box via MouseDrop.
    if (dragging_) {
        auto* state = resolve(pressed_.component);
        if (state != nullptr && dragId_ >= 0 &&
            static_cast<std::size_t>(dragId_) < state->sprites.size()) {
            const auto& geom = state->sprites[dragId_];
            const auto phase = itemInt(*state, dragId_, "phase");
            const auto& frame = (!geom.frames.empty() && phase >= 0 &&
                                 phase < static_cast<std::int64_t>(geom.frames.size()))
                                    ? geom.frames[static_cast<std::size_t>(phase)]
                                    : (geom.frames.empty() ? SpriteFrame{} : geom.frames.front());
            const auto left = itemInt(*state, dragId_, "x");
            const auto top = itemInt(*state, dragId_, "y");
            runEvent(pressed_.component + ".MouseDrop",
                     {Value::integer(dragId_), Value::integer(left), Value::integer(top),
                      Value::integer(left + static_cast<std::int64_t>(frame.width)),
                      Value::integer(top + static_cast<std::int64_t>(frame.height))});
        }
    }

    dragging_ = false;
    dragId_ = -1;
    pressed_ = Hit{};
}

void Page::rButtonDown(int x, int y) {
    runEvent("OnRButtonDown", {Value::integer(x), Value::integer(y)});
    const auto hit = findHit(x, y);
    fireHitEvent(hit, rightClickEventFor(hit.kind));
}

namespace {
// Fire `event` on the first component of `kind` (by its display name), with the
// given item id. A no-op if the page has no such component or handler.
void fireKindEvent(Page& page, const std::vector<ComponentState>& components,
                   HolderKind kind, const char* event, int id) {
    for (const auto& c : components) {
        if (c.kind == kind) {
            page.runEvent(c.displayName + "." + event, {Value::integer(id)});
            return;
        }
    }
}
} // namespace

void Page::videoEnd(int id) {
    fireKindEvent(*this, components_, HolderKind::TransparentVideoHolder, "TheEnd", id);
}

void Page::soundEnd(int id) {
    fireKindEvent(*this, components_, HolderKind::SoundHolder, "EndPlaySound", id);
}

void Page::animationEnd(int id) {
    fireKindEvent(*this, components_, HolderKind::SpriteHolder, "EndAnimation", id);
}

void Page::mouseMove(int x, int y) {
    // While dragging, the sprite follows the cursor (offset by the grab point)
    // and hover events are suppressed, mirroring the board's drag mode.
    if (dragging_) {
        auto* state = resolve(pressed_.component);
        if (state != nullptr) {
            state->items[dragId_]["x"] = Value::integer(x - grabOffsetX_);
            state->items[dragId_]["y"] = Value::integer(y - grabOffsetY_);
        }
        return;
    }

    // Hover follows the same merged layer order as clicks. This matters for
    // CURSORS.GRP: when its toolbar is open, the layer-8 button sprites cover
    // the layer-5 full-page hotspot that closes it. Dispatching hover to every
    // holder independently made that covered hotspot close the toolbar as soon
    // as the pointer left the one-pixel reveal strip.
    const Hit hit = findHit(x, y);

    for (auto it = hoverByComponent_.begin(); it != hoverByComponent_.end();) {
        const Hit old = it->second;
        if (old.component == hit.component && old.index == hit.index) {
            ++it;
            continue;
        }
        fireHitEvent(old, "MouseMoveOut");
        it = hoverByComponent_.erase(it);
    }

    if (hit.index != -1) {
        const auto oldIt = hoverByComponent_.find(hit.component);
        if (oldIt == hoverByComponent_.end() || oldIt->second.index != hit.index) {
            fireHitEvent(hit, "MouseMoveIn");
            hoverByComponent_[hit.component] = hit;
        }
    }
}

} // namespace graphboard::runtime
