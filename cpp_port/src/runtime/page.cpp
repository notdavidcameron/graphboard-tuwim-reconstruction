#include "graphboard/runtime/page.hpp"

#include "graphboard/component.hpp"
#include "graphboard/format.hpp"

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
                state.items[h.id]["enabled"] = Value::integer(h.enabled);
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
        case HolderKind::MultiBitmap: parseMultiBitmapState(reader); break;
        case HolderKind::TransparentVideoHolder: parseTransparentVideoHolderState(reader); break;
        case HolderKind::SoundHolder: parseSoundHolderState(reader); break;
        case HolderKind::TextHolder: parseTextHolderState(reader); break;
        case HolderKind::BitmapHolder: {
            const auto bh = parseBitmapHolderState(reader);
            // Bitmaps are addressed by index; the click path hit-tests by rect +
            // layer with no visibility gate (BitmapHolder @ 10003f50).
            for (std::size_t i = 0; i < bh.bitmaps.size(); ++i) {
                const auto& bm = bh.bitmaps[i];
                auto& item = state.items[static_cast<int>(i)];
                item["x"] = Value::integer(bm.left);
                item["y"] = Value::integer(bm.top);
                BitmapGeometry geom;
                geom.layer = bm.layer;
                geom.width = bm.right - bm.left;
                geom.height = bm.bottom - bm.top;
                geom.opaque = bm.opaque;
                state.bitmaps.push_back(std::move(geom));
            }
            break;
        }
        case HolderKind::Puzzle: parsePuzzleState(reader); break;
        case HolderKind::Recorder: parseRecorderState(reader); break;
        case HolderKind::VideoHolder: parseVideoHolderState(reader); break;
        case HolderKind::PanoramaHolder: parsePanoramaHolderState(reader); break;
        case HolderKind::Panorama: parsePanoramaState(reader); break;
        case HolderKind::Unknown:
            throw ParseError("Page::load: unknown holder kind '" + state.displayName + "'");
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

    const auto header = parseComponentListHeader(reader);
    for (std::uint32_t i = 0; i < header.count; ++i) {
        const auto item = parseComponentListItem(reader);
        ComponentState state;
        state.displayName = item.wrapper.displayName;
        const auto* info = lookupHolder(item.clsid);
        state.kind = info ? info->kind : HolderKind::Unknown;
        seedFromPrivateState(reader, state);

        // First instance of a display name wins for lookup; the vector keeps
        // every instance in order.
        byName_.emplace(state.displayName, components_.size());
        components_.push_back(std::move(state));
    }

    script_ = parseScriptText(reader).text;
}

std::unique_ptr<Page> Page::loadFromReader(BinaryReader& reader, std::string /*sourceLabel*/) {
    std::unique_ptr<Page> page(new Page());
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
    // Scripts may qualify a component as Group.HotSpot_Holder; the instance is
    // keyed by its bare display name (the segment after the last '.').
    const auto dot = componentPath.rfind('.');
    const std::string name =
        dot == std::string::npos ? componentPath : componentPath.substr(dot + 1);
    const auto it = byName_.find(name);
    return it != byName_.end() ? &components_[it->second] : nullptr;
}

const ComponentState* Page::component(const std::string& name) const {
    const auto it = byName_.find(name);
    return it != byName_.end() ? &components_[it->second] : nullptr;
}

std::vector<std::string> Page::componentNames() const {
    std::vector<std::string> names;
    names.reserve(components_.size());
    for (const auto& state : components_) {
        names.push_back(state.displayName);
    }
    return names;
}

Value Page::callComponent(const std::string& component, const std::string& method,
                          const std::vector<Value>& args) {
    callLog_.push_back({false, component, method, args});

    ComponentState* state = resolve(component);
    if (state == nullptr) {
        return Value();
    }

    // Index-addressed mutators. Method semantics follow the reflected member
    // names (verified against the recovered wrappers); unknown methods are
    // recorded but change no state.
    const int id = static_cast<int>(argInt(args, 0));
    auto& item = state->items[id];

    if (method == "MoveTo" || method == "MoveSprite" || method == "GotoXY") {
        item["x"] = Value::integer(argInt(args, 1));
        item["y"] = Value::integer(argInt(args, 2));
    } else if (method == "ShowBitmap" || method == "ShowText" || method == "Show" ||
               method == "ShowSprite") {
        item["visible"] = Value::integer(1);
    } else if (method == "HideBitmap" || method == "HideText" || method == "Hide" ||
               method == "HideSprite") {
        item["visible"] = Value::integer(0);
    } else if (method == "ChangePhase") {
        item["phase"] = Value::integer(argInt(args, 1));
    } else if (method == "StartAnimation") {
        item["animating"] = Value::integer(1);
    } else if (method == "StopAnimation") {
        item["animating"] = Value::integer(0);
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
    } else if (method == "Stop" || method == "StopAll" || method == "StopDSound") {
        state->props["playing"] = Value();
    }

    return Value();
}

Value Page::callBuiltin(const std::string& name, const std::vector<Value>& args) {
    callLog_.push_back({true, "", name, args});

    if (name == "LoadPage") {
        pendingPage_ = args.empty() ? std::string() : args[0].toString();
    } else if (name == "LoadGroup") {
        currentGroup_ = args.empty() ? std::string() : args[0].toString();
    } else if (name == "CloseGroup") {
        currentGroup_.clear();
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

            if (best.index == -1 || geom.layer > best.layer) {
                best.index = id;
                best.layer = geom.layer;
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
            if (best.index == -1 || geom.layer > best.layer) {
                best.index = id;
                best.layer = geom.layer;
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
    for (const auto& state : components_) {
        const auto candidate = topItemIn(state, x, y);
        if (candidate.index == -1) continue;
        if (hit.index == -1 || candidate.layer > bestLayer) {
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
        const auto* c = component(hit.component);
        if (c != nullptr && hit.index >= 0 &&
            static_cast<std::size_t>(hit.index) < c->sprites.size() &&
            c->sprites[hit.index].draggable) {
            dragging_ = true;
            dragId_ = hit.index;
            const auto ci = c->items.find(hit.index);
            const int sx = ci != c->items.end() ? static_cast<int>(
                               ci->second.count("x") ? ci->second.at("x").toInt() : 0) : 0;
            const int sy = ci != c->items.end() ? static_cast<int>(
                               ci->second.count("y") ? ci->second.at("y").toInt() : 0) : 0;
            grabOffsetX_ = x - sx;
            grabOffsetY_ = y - sy;
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

    const auto hit = findHit(x, y);
    if (hit.index == hover_.index && hit.component == hover_.component) {
        return;
    }
    // Both HotSpot_Holder and Sprite_Holder expose MouseMoveIn/MouseMoveOut.
    fireHitEvent(hover_, "MouseMoveOut");
    fireHitEvent(hit, "MouseMoveIn");
    hover_ = hit;
}

} // namespace graphboard::runtime
