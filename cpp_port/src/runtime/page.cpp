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
            for (std::size_t i = 0; i < sp.instances.size(); ++i) {
                auto& item = state.items[static_cast<int>(i)];
                item["definition"] = Value::integer(sp.instances[i].definitionIndex);
                item["x"] = Value::integer(sp.instances[i].posX);
                item["y"] = Value::integer(sp.instances[i].posY);
            }
            break;
        }
        case HolderKind::MultiBitmap: parseMultiBitmapState(reader); break;
        case HolderKind::TransparentVideoHolder: parseTransparentVideoHolderState(reader); break;
        case HolderKind::SoundHolder: parseSoundHolderState(reader); break;
        case HolderKind::TextHolder: parseTextHolderState(reader); break;
        case HolderKind::BitmapHolder: parseBitmapHolderState(reader); break;
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

    if (method == "MoveTo" || method == "MoveSprite") {
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

// Mirrors HotSpotHolder::LButtonDown (HotSpotHolder.dll @ 100050a0), which the
// board calls once per layer with that layer in `deep`:
//
//   * only records whose layer == deep are considered;
//   * records are scanned last -> first, so a later record wins an overlap;
//   * the rect test is inclusive on every edge (left<=x<=right, top<=y<=bottom);
//   * only enabled records fire, and the value handed to the script is the
//     record's stored id (+0x18), not its array index.
//
// The board walks layers top-down (it keeps the page's min/max layer, which
// GraphBrdViewCallback_RecomputeComponentTimingBounds aggregates from every
// component's MinMaxDeep), so the highest layer containing the point wins.
// Note IsYou (@ 100047c0) -- the engine's *query* entry point -- uses a
// bottom-exclusive rect (top<=y<bottom) and scans first->last; this routine
// deliberately follows the click path, not the query path.
Page::HotSpotHit Page::findHotSpotHit(int x, int y) const {
    HotSpotHit hit;
    std::int32_t bestLayer = 0;
    for (const auto& state : components_) {
        if (state.kind != HolderKind::HotSpotHolder) {
            continue;
        }
        // Reverse scan: within a layer the last matching record wins.
        for (auto it = state.hotspots.rbegin(); it != state.hotspots.rend(); ++it) {
            const auto& h = *it;
            if (h.enabled == 0) {
                continue;
            }
            if (x < h.left || x > h.right || y < h.top || y > h.bottom) {
                continue;
            }
            // Strictly-greater keeps the first record seen at the winning
            // layer: reverse order means that is the last record on it, and
            // across holders it means the earliest holder wins a tie (the board
            // stops at the first component that handles the layer).
            if (hit.index == -1 || h.layer > bestLayer) {
                hit.index = h.id;
                hit.component = state.displayName;
                bestLayer = h.layer;
            }
        }
    }
    return hit;
}

int Page::hitTestHotSpot(int x, int y) const {
    return findHotSpotHit(x, y).index;
}

void Page::lButtonDown(int x, int y) {
    runEvent("OnLButtonDown", {Value::integer(x), Value::integer(y)});
    const auto hit = findHotSpotHit(x, y);
    if (hit.index != -1) {
        runEvent(hit.component + ".LeftButtonClickOn", {Value::integer(hit.index)});
    }
}

void Page::rButtonDown(int x, int y) {
    runEvent("OnRButtonDown", {Value::integer(x), Value::integer(y)});
    const auto hit = findHotSpotHit(x, y);
    if (hit.index != -1) {
        runEvent(hit.component + ".RightButtonClickOn", {Value::integer(hit.index)});
    }
}

void Page::mouseMove(int x, int y) {
    const auto hit = findHotSpotHit(x, y);
    if (hit.index == hoverHotSpotIndex_ && hit.component == hoverHotSpotComponent_) {
        return;
    }
    if (hoverHotSpotIndex_ != -1) {
        runEvent(hoverHotSpotComponent_ + ".MouseMoveOut", {Value::integer(hoverHotSpotIndex_)});
    }
    if (hit.index != -1) {
        runEvent(hit.component + ".MouseMoveIn", {Value::integer(hit.index)});
    }
    hoverHotSpotIndex_ = hit.index;
    hoverHotSpotComponent_ = hit.component;
}

} // namespace graphboard::runtime
