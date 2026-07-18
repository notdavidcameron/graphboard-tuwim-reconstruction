#include "graphboard/runtime/page.hpp"

#include "graphboard/component.hpp"
#include "graphboard/format.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <queue>
#include <unordered_map>

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
            // CUDA's five draggable butterflies are serialized as five
            // one-frame, drag-enabled sprites. The original SpriteHolder timer
            // applies a continuous wing-close/open cycle rather than storing
            // extra raster phases. Reconstruct the closed-wing phase inside
            // the same canvas and start the holder animation; DisableTimers in
            // text mode pauses it, while dragging does not.
            const bool proceduralButterflies = sp.instances.size() == 5 &&
                std::all_of(sp.instances.begin(), sp.instances.end(),
                            [](const SpriteInstance& instance) { return instance.dragEnabled; }) &&
                std::all_of(state.sprites.begin(), state.sprites.end(),
                            [](const SpriteGeometry& sprite) { return sprite.frames.size() == 1; });
            if (proceduralButterflies) {
                state.props["proceduralButterflies"] = Value::integer(1);
                for (std::size_t i = 0; i < state.sprites.size(); ++i) {
                    auto& sprite = state.sprites[i];
                    const auto& open = sprite.frames.front();
                    if (open.width == 0 || open.height == 0 || open.pixels.empty()) continue;
                    std::vector<std::uint8_t> closed(open.pixels.size(), open.transparentIndex);
                    std::vector<std::uint8_t> closedMask;
                    if (!open.opaque.empty()) closedMask.assign(open.opaque.size(), 0);
                    const std::uint32_t closedWidth = std::max<std::uint32_t>(1, open.width * 3 / 5);
                    const std::uint32_t left = (open.width - closedWidth) / 2;
                    for (std::uint32_t y = 0; y < open.height; ++y) {
                        for (std::uint32_t x = 0; x < closedWidth; ++x) {
                            const std::uint32_t sourceX = std::min<std::uint32_t>(
                                open.width - 1, x * open.width / closedWidth);
                            const auto source = static_cast<std::size_t>(y) * open.width + sourceX;
                            const auto destination = static_cast<std::size_t>(y) * open.width + left + x;
                            closed[destination] = open.pixels[source];
                            if (!open.opaque.empty()) closedMask[destination] = open.opaque[source];
                        }
                    }
                    sprite.frames.front().animationPixels = {open.pixels, std::move(closed)};
                    if (!open.opaque.empty()) {
                        sprite.frames.front().animationOpaque = {open.opaque, std::move(closedMask)};
                    }
                    sprite.frames.front().cellDurationMs = 180;
                    state.items[static_cast<int>(i)]["animating"] = Value::integer(1);
                }
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
                geom.drawTransparentIndex = e.drawTransparentIndex;
                geom.persistentBacking = e.stream.persistentBacking;
                geom.streamTransparencyEnabled = e.stream.transparencyEnabled;
                geom.transparencyEnabled = e.drawTransparencyEnabled;
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
            for (std::size_t i = 0; i < sh.sounds.size(); ++i) {
                const auto& s = sh.sounds[i];
                state.clipDurationMs.push_back(static_cast<int>(s.durationMs));
                state.soundClips.push_back({s.soundOffset, s.soundByteCount});
                state.items[static_cast<int>(i)]["looping"] = Value::integer(s.looping ? 1 : 0);
            }
            break;
        }
        case HolderKind::TextHolder: {
            const auto text = parseTextHolderState(reader);
            state.props["textColorR"] = Value::integer(text.colors[0]);
            state.props["textColorG"] = Value::integer(text.colors[1]);
            state.props["textColorB"] = Value::integer(text.colors[2]);
            const std::uint32_t lineHeight = text.fontSlots.empty() || text.fontSlots.front().height == 0
                                                 ? 18u
                                                 : text.fontSlots.front().height;
            for (std::size_t i = 0; i < text.entries.size(); ++i) {
                const auto& entry = text.entries[i];
                auto& item = state.items[static_cast<int>(i)];
                item["visible"] = Value::integer(entry.initiallyVisible ? 1 : 0);
                item["z"] = Value::integer(entry.layer);
                item["offsetX"] = Value::integer(0);
                item["offsetY"] = Value::integer(0);
                item["mouseEnabled"] = Value::integer(0);
                state.texts.push_back({entry.left, entry.top, entry.right, entry.bottom,
                                       entry.primaryText, entry.secondaryText,
                                       entry.lineCount, lineHeight});
                state.soundClips.push_back({entry.streamOffset, entry.streamByteCount});
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
                item["stateWord"] = Value::integer(bm.stateWord);
                BitmapGeometry geom;
                geom.name = bm.name;
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
        case HolderKind::Puzzle: {
            const auto puzzle = parsePuzzleState(reader);
            if (!puzzle.boards.empty()) {
                const auto& file = reader.bytes();
                const auto& board = puzzle.boards.front();
                for (std::size_t i = 0; i < board.chips.size(); ++i) {
                    const auto& chip = board.chips[i];
                    PuzzleChipGeometry geom;
                    geom.originalX = chip.left;
                    geom.originalY = chip.top;
                    geom.width = static_cast<std::int32_t>(chip.right) - chip.left;
                    geom.height = chip.bottom - chip.top;
                    const auto stride = geom.width > 0
                        ? (static_cast<std::size_t>(geom.width) + 3u) & ~std::size_t{3}
                        : 0;
                    if (geom.width > 0 && geom.height > 0 &&
                        chip.pixelByteCount >= stride * static_cast<std::size_t>(geom.height) &&
                        chip.pixelOffset + stride * static_cast<std::size_t>(geom.height) <= file.size()) {
                        // The chip record has no per-piece key. The matte is
                        // the dominant palette index on its outer perimeter;
                        // using only four corners fails when a jigsaw tab or
                        // artwork reaches a corner (leaving bright green mats).
                        std::array<std::uint32_t, 256> borderHistogram{};
                        auto countSource = [&](int sx, int sy) {
                            const auto source = chip.pixelOffset +
                                static_cast<std::size_t>(geom.height - 1 - sy) * stride +
                                static_cast<std::size_t>(sx);
                            ++borderHistogram[file[source]];
                        };
                        for (int x = 0; x < geom.width; ++x) {
                            countSource(x, 0);
                            if (geom.height > 1) countSource(x, geom.height - 1);
                        }
                        for (int y = 1; y + 1 < geom.height; ++y) {
                            countSource(0, y);
                            if (geom.width > 1) countSource(geom.width - 1, y);
                        }
                        geom.transparentIndex = static_cast<std::uint8_t>(
                            std::distance(borderHistogram.begin(),
                                std::max_element(borderHistogram.begin(), borderHistogram.end())));
                        geom.pixels.assign(static_cast<std::size_t>(geom.width) * geom.height, 0);
                        for (int y = 0; y < geom.height; ++y) {
                            const auto source = chip.pixelOffset +
                                static_cast<std::size_t>(geom.height - 1 - y) * stride;
                            std::copy_n(file.begin() + static_cast<std::ptrdiff_t>(source), geom.width,
                                        geom.pixels.begin() + static_cast<std::ptrdiff_t>(y) * geom.width);
                        }
                    }
                    auto& item = state.items[static_cast<int>(i)];
                    item["x"] = Value::integer(geom.originalX);
                    item["y"] = Value::integer(geom.originalY);
                    item["z"] = Value::integer(static_cast<std::int64_t>(i));
                    item["visible"] = Value::integer(0);
                    item["locked"] = Value::integer(0);
                    state.puzzleChips.push_back(std::move(geom));
                }

                // Puzzle chip records only store their loose editor positions.
                // The solved arrangement is encoded as neighbour constraints:
                // target[connection.chipId] = target[thisChip] + (dx, dy).
                // Walk that graph, then normalize it to a zero-based image.
                std::vector<bool> positioned(board.chips.size(), false);
                std::queue<std::size_t> pending;
                for (std::size_t root = 0; root < board.chips.size(); ++root) {
                    if (positioned[root]) continue;
                    positioned[root] = true;
                    pending.push(root);
                    while (!pending.empty()) {
                        const auto current = pending.front();
                        pending.pop();
                        for (const auto& connection : board.chips[current].connections) {
                            if (connection.chipId < 0 ||
                                static_cast<std::size_t>(connection.chipId) >= board.chips.size()) {
                                continue;
                            }
                            const auto neighbour = static_cast<std::size_t>(connection.chipId);
                            if (positioned[neighbour]) continue;
                            state.puzzleChips[neighbour].solutionX =
                                state.puzzleChips[current].solutionX + connection.dx;
                            state.puzzleChips[neighbour].solutionY =
                                state.puzzleChips[current].solutionY + connection.dy;
                            positioned[neighbour] = true;
                            pending.push(neighbour);
                        }
                    }
                }
                if (!state.puzzleChips.empty()) {
                    const auto minX = std::min_element(
                        state.puzzleChips.begin(), state.puzzleChips.end(),
                        [](const auto& a, const auto& b) { return a.solutionX < b.solutionX; })
                                          ->solutionX;
                    const auto minY = std::min_element(
                        state.puzzleChips.begin(), state.puzzleChips.end(),
                        [](const auto& a, const auto& b) { return a.solutionY < b.solutionY; })
                                          ->solutionY;
                    for (auto& geom : state.puzzleChips) {
                        geom.solutionX -= minX;
                        geom.solutionY -= minY;
                    }
                }
            }
            state.props["puzzleId"] = Value::integer(0);
            state.props["showFullBitmap"] = Value::integer(0);
            break;
        }
        case HolderKind::Recorder: parseRecorderState(reader); break;
        case HolderKind::VideoHolder: {
            const auto videos = parseVideoHolderState(reader);
            for (std::size_t i = 0; i < videos.entries.size(); ++i) {
                const auto& video = videos.entries[i];
                auto& item = state.items[static_cast<int>(i)];
                item["x"] = Value::integer(video.posX);
                item["y"] = Value::integer(video.posY);
                item["visible"] = Value::integer(0);
                item["playing"] = Value::integer(0);
                state.externalVideos.push_back({video.posX, video.posY, video.name});
            }
            break;
        }
        case HolderKind::PanoramaHolder: {
            const auto panoramas = parsePanoramaHolderState(reader);
            state.props["panX1000"] = Value::integer(0);
            state.props["panVelocity"] = Value::integer(0);
            state.props["panY1000"] = Value::integer(0);
            state.props["panVelocityY"] = Value::integer(0);
            state.props["panEnabled"] = Value::integer(1);
            for (std::size_t i = 0; i < panoramas.scenes.size(); ++i) {
                const auto& scene = panoramas.scenes[i];
                state.items[static_cast<int>(i)]["visible"] = Value::integer(0);
                state.dibImages.push_back({scene.dibOffset, scene.dibByteCount});
                if (i == 0 && scene.dibOffset + 8 <= reader.bytes().size()) {
                    const auto& bytes = reader.bytes();
                    const auto p = scene.dibOffset + 4;
                    const std::uint32_t width = static_cast<std::uint32_t>(bytes[p]) |
                        (static_cast<std::uint32_t>(bytes[p + 1]) << 8) |
                        (static_cast<std::uint32_t>(bytes[p + 2]) << 16) |
                        (static_cast<std::uint32_t>(bytes[p + 3]) << 24);
                    state.props["panSourceWidth"] = Value::integer(width);
                    if (p + 8 <= bytes.size()) {
                        const auto rawHeight = static_cast<std::int32_t>(
                            static_cast<std::uint32_t>(bytes[p + 4]) |
                            (static_cast<std::uint32_t>(bytes[p + 5]) << 8) |
                            (static_cast<std::uint32_t>(bytes[p + 6]) << 16) |
                            (static_cast<std::uint32_t>(bytes[p + 7]) << 24));
                        state.props["panSourceHeight"] = Value::integer(std::abs(rawHeight));
                    }
                    state.props["panMax1000"] = Value::integer(
                        static_cast<std::int64_t>(width > 640 ? width - 640 : 0) * 1000);
                }
            }
            break;
        }
        case HolderKind::Panorama: {
            const auto panoramas = parsePanoramaState(reader);
            state.props["panX1000"] = Value::integer(0);
            state.props["panVelocity"] = Value::integer(0);
            state.props["panY1000"] = Value::integer(0);
            state.props["panVelocityY"] = Value::integer(0);
            state.props["panEnabled"] = Value::integer(1);
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
                if (i == 0) {
                    state.props["panSourceWidth"] = Value::integer(scene.width);
                    state.props["panSourceHeight"] = Value::integer(scene.height);
                }
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
    const auto header = parseBdfHeader(reader);

    parseComponentStates(reader, components_, byName_);

    // BitmapHolder overlays persist their shown state at +0x00. Full-page
    // hit-test surfaces are the board image itself and must be present from
    // frame zero even though those backing records commonly store zero. Keep
    // the structural exceptions geometric rather than keyed to page names.
    std::size_t videoCount = 0;
    bool hasPanorama = false;
    std::int64_t minimumVideoLayer = std::numeric_limits<std::int64_t>::max();
    for (const auto& state : components_) {
        if (state.kind == HolderKind::Panorama || state.kind == HolderKind::PanoramaHolder) {
            hasPanorama = true;
        }
        if (state.kind != HolderKind::TransparentVideoHolder) continue;
        videoCount += state.videos.size();
        for (const auto& [id, item] : state.items) {
            const auto z = item.find("z");
            minimumVideoLayer = std::min(
                minimumVideoLayer, z == item.end() ? std::int64_t{0} : z->second.toInt());
        }
    }
    const int pageWidth = header.pageRect.right - header.pageRect.left;
    const int pageHeight = header.pageRect.bottom - header.pageRect.top;
    for (auto& state : components_) {
        if (state.kind != HolderKind::BitmapHolder) continue;
        bool hasFullPage = false;
        for (std::size_t i = 0; i < state.bitmaps.size(); ++i) {
            const auto& bitmap = state.bitmaps[i];
            auto& item = state.items[static_cast<int>(i)];
            const bool fullPage = bitmap.width >= pageWidth && bitmap.height >= pageHeight;
            hasFullPage = hasFullPage || fullPage;
            // record+0x00 is the persisted shown state for placed overlays.
            // Complete board surfaces are always the backing canvas even when
            // that word is zero; partial records respect it.
            const bool activityOverlay = videoCount > 0 && !hasPanorama && !fullPage;
            item["visible"] = Value::integer(
                fullPage || (!activityOverlay && item["stateWord"].toInt() != 0));
        }
        // Multiple bitmaps with the same authored rectangle are replacement
        // states, not simultaneous layers. The opening state is the shallowest
        // record (and, on an equal layer, the first serialized one); scripts
        // subsequently swap them with ShowBitmap/HideBitmap. This covers full
        // board light/dark variants as well as instrument hit/rest artwork.
        for (std::size_t i = 0; i < state.bitmaps.size(); ++i) {
            const auto& a = state.bitmaps[i];
            std::size_t opening = i;
            bool hasAlternate = false;
            for (std::size_t j = 0; j < state.bitmaps.size(); ++j) {
                const auto& b = state.bitmaps[j];
                if (i == j) continue;
                const auto ax = state.items[static_cast<int>(i)]["x"].toInt();
                const auto ay = state.items[static_cast<int>(i)]["y"].toInt();
                const auto bx = state.items[static_cast<int>(j)]["x"].toInt();
                const auto by = state.items[static_cast<int>(j)]["y"].toInt();
                const auto overlapW = std::max<std::int64_t>(
                    0, std::min(ax + a.width, bx + b.width) - std::max(ax, bx));
                const auto overlapH = std::max<std::int64_t>(
                    0, std::min(ay + a.height, by + b.height) - std::max(ay, by));
                const auto intersection = overlapW * overlapH;
                const auto unionArea = static_cast<std::int64_t>(a.width) * a.height +
                    static_cast<std::int64_t>(b.width) * b.height - intersection;
                if (unionArea <= 0 || intersection * 100 < unionArea * 90) continue;
                hasAlternate = true;
                if (b.layer < state.bitmaps[opening].layer ||
                    (b.layer == state.bitmaps[opening].layer && j < opening)) opening = j;
            }
            if (hasAlternate) {
                state.items[static_cast<int>(i)]["visible"] = Value::integer(opening == i);
            }
        }
        // Some authored motion sets use differently-sized replacement cells,
        // so rectangle overlap alone cannot identify them. A long run of the
        // same non-empty bitmap name is the editor's animation sequence. Its
        // first record is the base artwork and the next is the opening pose;
        // the remaining poses/effects are selected by script calls later.
        std::unordered_map<std::string, std::vector<std::size_t>> namedSequences;
        for (std::size_t i = 0; i < state.bitmaps.size(); ++i) {
            if (!state.bitmaps[i].name.empty()) {
                namedSequences[state.bitmaps[i].name].push_back(i);
            }
        }
        const std::vector<std::size_t>* dominantSequence = nullptr;
        for (const auto& [name, indices] : namedSequences) {
            if (indices.size() >= 6 &&
                (!dominantSequence || indices.size() > dominantSequence->size())) {
                dominantSequence = &indices;
            }
        }
        if (dominantSequence && dominantSequence->size() + 2 >= state.bitmaps.size()) {
            for (std::size_t i = 0; i < state.bitmaps.size(); ++i) {
                const auto& bitmap = state.bitmaps[i];
                if (bitmap.width < pageWidth || bitmap.height < pageHeight) {
                    state.items[static_cast<int>(i)]["visible"] = Value::integer(0);
                }
            }
            state.items[static_cast<int>((*dominantSequence)[0])]["visible"] = Value::integer(1);
            state.items[static_cast<int>((*dominantSequence)[1])]["visible"] = Value::integer(1);
        }
        // Composite activity boards store the complete board first, one large
        // opening foreground next, then small mutually-exclusive replacement
        // states. Select that unique large foreground by area; scripts control
        // the smaller alternatives thereafter.
        if (state.bitmaps.size() > 2 && videoCount > 0) {
            std::size_t largest = state.bitmaps.size();
            std::int64_t largestArea = 0;
            std::int64_t secondArea = 0;
            for (std::size_t i = 0; i < state.bitmaps.size(); ++i) {
                const auto& bitmap = state.bitmaps[i];
                if (bitmap.width >= pageWidth && bitmap.height >= pageHeight) continue;
                const std::int64_t area = static_cast<std::int64_t>(bitmap.width) * bitmap.height;
                if (area > largestArea) {
                    secondArea = largestArea;
                    largestArea = area;
                    largest = i;
                } else {
                    secondArea = std::max(secondArea, area);
                }
            }
            if (largest < state.bitmaps.size() && largestArea > secondArea * 2) {
                state.items[static_cast<int>(largest)]["visible"] = Value::integer(1);
            }
        }
        // A singleton large bitmap accompanying a dense animation collage is
        // its editor work surface. It is not part of the opening composition;
        // the animated entries already contain the authored backing pieces.
        if (state.bitmaps.size() == 1 && videoCount > 4 &&
            minimumVideoLayer != std::numeric_limits<std::int64_t>::max()) {
            const auto& bitmap = state.bitmaps.front();
            if (bitmap.width > pageWidth / 2 && bitmap.height > pageHeight / 2 &&
                bitmap.width < pageWidth && bitmap.height < pageHeight) {
                auto& item = state.items[0];
                item["visible"] = Value::integer(0);
            }
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

    if (state->kind == HolderKind::Recorder && method == "OpenFile") {
        state->props["file"] = args.empty() ? Value::string("") : args[0];
    } else if (state->kind == HolderKind::Recorder && method == "CloseFile") {
        state->props["file"] = Value::string("");
    } else if (state->kind == HolderKind::Recorder && method == "IsEmpty") {
        ComponentResult result;
        const std::string file = state->props.count("file") ? state->props["file"].toString() : "";
        const std::string key = "recorded:" + file;
        result.outArguments[0] = Value::integer(
            state->props.count(key) && state->props[key].toInt() != 0 ? 0 : 1);
        return result;
    } else if (state->kind == HolderKind::Recorder && method == "EmptyFile") {
        const std::string file = state->props.count("file") ? state->props["file"].toString() : "";
        state->props["recorded:" + file] = Value::integer(0);
    } else if (state->kind == HolderKind::Recorder && method == "Record") {
        state->props["recording"] = Value::integer(1);
        state->props["playing"] = Value::integer(0);
    } else if (state->kind == HolderKind::Recorder && method == "Play") {
        state->props["playing"] = Value::integer(1);
        state->props["recording"] = Value::integer(0);
    } else if (state->kind == HolderKind::Recorder && method == "Stop") {
        state->props["recording"] = Value::integer(0);
        state->props["playing"] = Value::integer(0);
    } else if ((state->kind == HolderKind::TextHolder && method == "SetSoundParameters") ||
               (state->kind == HolderKind::SoundHolder && method == "SetPlayDSoundParameters")) {
        item["volume"] = Value::integer(argInt(args, 1));
    } else if (state->kind == HolderKind::TextHolder && method == "SetText") {
        if (id >= 0 && static_cast<std::size_t>(id) < state->texts.size()) {
            auto& text = state->texts[static_cast<std::size_t>(id)];
            text.text = args.size() > 1 ? args[1].toString() : std::string();
            text.lineCount = 1;
            for (const char ch : text.text) {
                if (ch == '\n') ++text.lineCount;
            }
        }
    } else if (state->kind == HolderKind::TextHolder && method == "SetTextOffsets") {
        item["offsetX"] = Value::integer(argInt(args, 1));
        item["offsetY"] = Value::integer(argInt(args, 2));
    } else if (state->kind == HolderKind::TextHolder && method == "GetTextOffsets") {
        ComponentResult result;
        result.outArguments[1] = item.count("offsetX") ? item["offsetX"] : Value::integer(0);
        result.outArguments[2] = item.count("offsetY") ? item["offsetY"] : Value::integer(0);
        return result;
    } else if (state->kind == HolderKind::TextHolder && method == "EnableMouse") {
        item["mouseEnabled"] = Value::integer(1);
    } else if (state->kind == HolderKind::TextHolder && method == "DisableMouse") {
        item["mouseEnabled"] = Value::integer(0);
    } else if (method == "MoveTo" || method == "MoveSprite") {
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
        // A fresh destination is a new holder command and supersedes a prior
        // StopAnimation pause. Without this, RZECZKA can enter its edge hotspot
        // and queue a new panorama target that remains permanently paused.
        item["glidePaused"] = Value::integer(0);
    } else if (state->kind == HolderKind::Puzzle && method == "OpenPuzzle") {
        state->props["puzzleId"] = Value::integer(id);
        state->props["showFullBitmap"] = Value::integer(0);
        for (std::size_t chip = 0; chip < state->puzzleChips.size(); ++chip) {
            const auto& geom = state->puzzleChips[chip];
            auto& chipItem = state->items[static_cast<int>(chip)];
            chipItem["x"] = Value::integer(geom.originalX);
            chipItem["y"] = Value::integer(geom.originalY);
            chipItem["visible"] = Value::integer(1);
            chipItem["locked"] = Value::integer(0);
        }
    } else if (state->kind == HolderKind::Puzzle && method == "ClosePuzzle") {
        for (auto& [chip, chipItem] : state->items) chipItem["visible"] = Value::integer(0);
    } else if (state->kind == HolderKind::Puzzle && method == "Mix") {
        const int left = static_cast<int>(argInt(args, 1));
        const int right = std::max(left + 1, static_cast<int>(argInt(args, 2)));
        const int bottom = std::max(1, static_cast<int>(argInt(args, 3)));
        int assembledWidth = 0;
        int assembledHeight = 0;
        for (const auto& geom : state->puzzleChips) {
            assembledWidth = std::max(assembledWidth, geom.solutionX + geom.width);
            assembledHeight = std::max(assembledHeight, geom.solutionY + geom.height);
        }
        const int solutionOriginX = left + (right - left - assembledWidth) / 2;
        const int solutionOriginY = (bottom - assembledHeight) / 2;
        for (std::size_t chip = 0; chip < state->puzzleChips.size(); ++chip) {
            auto& geom = state->puzzleChips[chip];
            geom.originalX = solutionOriginX + geom.solutionX;
            geom.originalY = solutionOriginY + geom.solutionY;
            const int xRange = std::max(1, right - left - geom.width);
            const int yRange = std::max(1, bottom - geom.height);
            auto& chipItem = state->items[static_cast<int>(chip)];
            chipItem["x"] = Value::integer(left + static_cast<int>((chip * 97u + 37u) % xRange));
            chipItem["y"] = Value::integer(static_cast<int>((chip * 53u + 19u) % yRange));
            chipItem["z"] = Value::integer(static_cast<std::int64_t>(chip));
            chipItem["locked"] = Value::integer(0);
            chipItem["targetX"] = Value::integer(geom.originalX);
            chipItem["targetY"] = Value::integer(geom.originalY);
        }
    } else if (state->kind == HolderKind::Puzzle && method == "ShowFullBitmap") {
        state->props["showFullBitmap"] = Value::integer(1);
        state->props["fullBitmapX"] = Value::integer(argInt(args, 0));
        state->props["fullBitmapY"] = Value::integer(argInt(args, 1));
        state->props["fullBitmapZ"] = Value::integer(argInt(args, 2));
    } else if (state->kind == HolderKind::Puzzle && method == "EndShowFullBitmap") {
        state->props["showFullBitmap"] = Value::integer(0);
    } else if ((state->kind == HolderKind::Panorama ||
                state->kind == HolderKind::PanoramaHolder) &&
               (method == "SetHorAngle" || method == "SetHorizontalAngle")) {
        // Retain degrees in fixed point. PanoramaHolder treats scripted motion
        // as an offset from this authored starting angle, so its remaining
        // travel must exclude both the viewport and that starting offset.
        // Otherwise MROZ visually reaches the right edge, then keeps ticking
        // through an unchanging (clamped) frame for many seconds.
        const auto angle = argInt(args, 0);
        state->props["panAngle"] = Value::integer(angle);
        if (state->kind == HolderKind::PanoramaHolder &&
            state->props.count("panSourceWidth")) {
            const auto width = std::max<std::int64_t>(0, state->props["panSourceWidth"].toInt());
            const auto normalizedAngle = ((angle % 360) + 360) % 360;
            const auto start = width * normalizedAngle / 360;
            const auto remaining = std::max<std::int64_t>(0, width - 640 - start);
            state->props["panMax1000"] = Value::integer(remaining * 1000);
            state->props["panX1000"] = Value::integer(
                std::clamp<std::int64_t>(state->props["panX1000"].toInt(), 0, remaining * 1000));
        }
    } else if ((state->kind == HolderKind::Panorama ||
                state->kind == HolderKind::PanoramaHolder) && method == "GoRight") {
        state->props["panVelocity"] = Value::integer(std::max<std::int64_t>(1, argInt(args, 0)) * 15);
    } else if ((state->kind == HolderKind::Panorama ||
                state->kind == HolderKind::PanoramaHolder) && method == "GoLeft") {
        state->props["panVelocity"] = Value::integer(-std::max<std::int64_t>(1, argInt(args, 0)) * 15);
    } else if ((state->kind == HolderKind::Panorama ||
                state->kind == HolderKind::PanoramaHolder) && method == "Stop") {
        state->props["panVelocity"] = Value::integer(0);
        state->props["panVelocityY"] = Value::integer(0);
    } else if ((state->kind == HolderKind::Panorama ||
                state->kind == HolderKind::PanoramaHolder) && method == "Enable") {
        state->props["panEnabled"] = Value::integer(1);
    } else if ((state->kind == HolderKind::Panorama ||
                state->kind == HolderKind::PanoramaHolder) && method == "Disable") {
        state->props["panEnabled"] = Value::integer(0);
        state->props["panVelocity"] = Value::integer(0);
        state->props["panVelocityY"] = Value::integer(0);
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
        // This aligns timers that are already running; it does not start every
        // multi-phase definition. Multi-phase records are also used as static
        // puzzle/button states (KOTEK, MICHAL, MROZ and SLOWIK), so treating
        // synchronization as StartAnimation makes the whole board cycle by
        // itself.
        if (state->kind == HolderKind::SpriteHolder) {
            for (auto& [spriteId, spriteItem] : state->items) {
                if (spriteId < 0 || static_cast<std::size_t>(spriteId) >= state->sprites.size()) continue;
                const auto& frames = state->sprites[static_cast<std::size_t>(spriteId)].frames;
                const auto phase = spriteItem.count("phase") ? spriteItem["phase"].toInt() : 0;
                const bool hasCells = phase >= 0 && phase < static_cast<std::int64_t>(frames.size()) &&
                    frames[static_cast<std::size_t>(phase)].animationPixels.size() > 1;
                if (hasCells) {
                    spriteItem["animating"] = Value::integer(1);
                    spriteItem["cell"] = Value::integer(0);
                    spriteItem["animAccumMs"] = Value::integer(0);
                } else if ((spriteItem.count("animating") &&
                            spriteItem["animating"].toInt() != 0) ||
                           (spriteItem.count("oneShotAnimating") &&
                            spriteItem["oneShotAnimating"].toInt() != 0)) {
                    spriteItem["animAccumMs"] = Value::integer(0);
                }
            }
        }
    } else if (method == "CompEnableDrag" || method == "EnableDragMode") {
        state->props["dragEnabled"] = Value::integer(1);
    } else if (method == "CompDisableDrag" || method == "DisableDragMode") {
        state->props["dragEnabled"] = Value::integer(0);
    } else if (method == "ContinueAnimation") {
        item["animating"] = Value::integer(1);
        item["glidePaused"] = Value::integer(0);
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
        const auto phase = argInt(args, 1);
        item["phase"] = Value::integer(phase);
        if (state->kind == HolderKind::SpriteHolder &&
            state->displayName.rfind("Group.", 0) != 0) {
            const bool hasCells = id >= 0 && static_cast<std::size_t>(id) < state->sprites.size() &&
                phase >= 0 && static_cast<std::size_t>(phase) < state->sprites[id].frames.size() &&
                state->sprites[id].frames[static_cast<std::size_t>(phase)].animationPixels.size() > 1;
            item["animating"] = Value::integer(hasCells ? 1 : 0);
            item["cell"] = Value::integer(0);
            item["animAccumMs"] = Value::integer(0);
            item["oneShotAnimating"] = Value::integer(0);
        }
        // Toolbar buttons with a three-frame definition use phase 1 as a
        // one-shot press animation and navigate from EndAnimation at phase 2.
        // They do not call StartAnimation explicitly (ladybugs/puzzles across
        // CURSORS*.GRP), so treating ChangePhase as a static assignment leaves
        // those navbar actions permanently stuck.
        if (state->kind == HolderKind::SpriteHolder &&
            state->displayName.rfind("Group.", 0) == 0 && id >= 0 &&
            static_cast<std::size_t>(id) < state->sprites.size()) {
            const auto phases = state->sprites[static_cast<std::size_t>(id)].frames.size();
            if (phase > 0 && static_cast<std::size_t>(phase + 1) < phases) {
                item["oneShotAnimating"] = Value::integer(1);
                item["animAccumMs"] = Value::integer(0);
            } else {
                item["oneShotAnimating"] = Value::integer(0);
            }
        }
    } else if (method == "StartAnimation") {
        item["animating"] = Value::integer(1);
    } else if (method == "StopAnimation") {
        item["animating"] = Value::integer(0);
        if (item.count("gliding") && item["gliding"].toInt() != 0) {
            item["glidePaused"] = Value::integer(1);
        }
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
    } else if (method == "PlayDSound" || method == "Play" ||
               method == "PlayFromTo" || method == "NewPlay" ||
               method == "NewPlayFromTo") {
        state->props["playing"] =
            args.empty() ? Value::integer(1) : Value::integer(argInt(args, 0));
        // Restarting an id supersedes its pending completion. TVH additionally
        // enforces its single active playback channel below.
        cancelCompletions(state->displayName, id);
        if (state->kind == HolderKind::TransparentVideoHolder) {
            // The DLL has one active click-playback channel per holder. Starting
            // a new entry replaces the previous one and cancels its completion
            // callback.
            for (auto& [otherId, other] : state->items) {
                if (otherId == id) continue;
                if (other.count("playing") && other["playing"].toInt() != 0) {
                    other["playing"] = Value::integer(0);
                    other["hasPlayed"] = Value::integer(0);
                    const bool rest = otherId >= 0 &&
                        static_cast<std::size_t>(otherId) < state->videos.size() &&
                        state->videos[static_cast<std::size_t>(otherId)].showStillAtRest;
                    other["visible"] = Value::integer(rest ? 1 : 0);
                    cancelCompletions(state->displayName, otherId);
                }
            }
            // Per-entry playback state for the renderer: which clip runs and
            // since when on the simulated clock.
            item["playing"] = Value::integer(1);
            item["hasPlayed"] = Value::integer(1);
            item["playStartMs"] = Value::integer(clockMs_);
            item["visible"] = Value::integer(1);   // frames persist after the clip
            scheduleCompletion(*state, "TheEnd", id);
        } else if (state->kind == HolderKind::SoundHolder) {
            const bool looping = item.count("looping") && item["looping"].toInt() != 0;
            if (!looping) scheduleCompletion(*state, "EndPlaySound", id);
        } else if (state->kind == HolderKind::VideoHolder) {
            item["playing"] = Value::integer(1);
            item["visible"] = Value::integer(1);
        }
    } else if (method == "Stop" || method == "StopAll" || method == "StopDSound") {
        state->props["playing"] = Value();
        if (state->kind == HolderKind::TransparentVideoHolder) {
            if (method == "StopAll" || args.empty()) {
                for (auto& [otherId, other] : state->items) {
                    other["playing"] = Value::integer(0);
                    other["hasPlayed"] = Value::integer(0);
                    const bool rest = otherId >= 0 &&
                        static_cast<std::size_t>(otherId) < state->videos.size() &&
                        state->videos[static_cast<std::size_t>(otherId)].showStillAtRest;
                    other["visible"] = Value::integer(rest ? 1 : 0);
                }
            } else {
                item["playing"] = Value::integer(0);
                item["hasPlayed"] = Value::integer(0);
                const bool rest = id >= 0 && static_cast<std::size_t>(id) < state->videos.size() &&
                                  state->videos[static_cast<std::size_t>(id)].showStillAtRest;
                item["visible"] = Value::integer(rest ? 1 : 0);
            }
        } else if (state->kind == HolderKind::VideoHolder) {
            for (auto& [otherId, other] : state->items) {
                other["playing"] = Value::integer(0);
                other["visible"] = Value::integer(0);
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
        struct AnimationEnd { std::string component; int id; };
        std::vector<AnimationEnd> animationEnds;
        auto stepAnimations = [&](std::vector<ComponentState>& components) {
            for (auto& c : components) {
                if (c.kind != HolderKind::SpriteHolder) continue;
                if (c.props.count("timersEnabled") && c.props["timersEnabled"].toInt() == 0) {
                    continue;
                }
                for (auto& [id, item] : c.items) {
                    if (id < 0 || static_cast<std::size_t>(id) >= c.sprites.size()) continue;
                    const auto& frames = c.sprites[static_cast<std::size_t>(id)].frames;
                    const auto phases = static_cast<std::int64_t>(frames.size());
                    if (phases <= 0) continue;
                    constexpr int kPhaseMs = 100;
                    std::int64_t accum =
                        item.count("animAccumMs") ? item["animAccumMs"].toInt() : 0;
                    accum += elapsed;
                    const std::int64_t steps = accum / kPhaseMs;
                    if (item.count("oneShotAnimating") &&
                        item["oneShotAnimating"].toInt() != 0) {
                        if (steps > 0) {
                            const auto phase = item.count("phase") ? item["phase"].toInt() : 0;
                            const auto next = std::min<std::int64_t>(phases - 1, phase + steps);
                            item["phase"] = Value::integer(next);
                            if (next == phases - 1) {
                                item["oneShotAnimating"] = Value::integer(0);
                                animationEnds.push_back({c.displayName, id});
                            }
                        }
                        item["animAccumMs"] = Value::integer(accum % kPhaseMs);
                        continue;
                    }
                    if (item.count("animating") == 0 || item["animating"].toInt() == 0) continue;
                    const auto phase = item.count("phase") ? item["phase"].toInt() : 0;
                    if (phase < 0 || phase >= phases) continue;
                    const auto& selected = frames[static_cast<std::size_t>(phase)];
                    const auto cells = static_cast<std::int64_t>(selected.animationPixels.size());
                    if (cells <= 1) continue;
                    const auto duration = std::max<std::int64_t>(1, selected.cellDurationMs);
                    const auto cellSteps = accum / duration;
                    if (cellSteps > 0) {
                        const auto cell = (item.count("cell") ? item["cell"].toInt() : 0) + cellSteps;
                        item["cell"] = Value::integer(cell % cells);
                    }
                    item["animAccumMs"] = Value::integer(accum % duration);
                }
            }
        };
        stepAnimations(components_);
        stepAnimations(groupComponents_);
        auto stepPanoramas = [&](std::vector<ComponentState>& components) {
            for (auto& c : components) {
                if (c.kind != HolderKind::Panorama && c.kind != HolderKind::PanoramaHolder) continue;
                if (c.props["panEnabled"].toInt() == 0) continue;
                const auto velocity = c.props["panVelocity"].toInt();
                const auto velocityY = c.props["panVelocityY"].toInt();
                if (velocity == 0 && velocityY == 0) continue;
                auto position = c.props["panX1000"].toInt();
                auto next = position + velocity * elapsed;
                if (c.kind == HolderKind::PanoramaHolder && c.props.count("panMax1000")) {
                    const auto maximum = c.props["panMax1000"].toInt();
                    next = std::clamp<std::int64_t>(next, 0, maximum);
                    if ((next == 0 && velocity < 0) || (next == maximum && velocity > 0)) {
                        c.props["panVelocity"] = Value::integer(0);
                    }
                }
                c.props["panX1000"] = Value::integer(next);
                auto nextY = c.props["panY1000"].toInt() + velocityY * elapsed;
                const auto maxY = std::max<std::int64_t>(0,
                    c.props.count("panSourceHeight")
                        ? (c.props["panSourceHeight"].toInt() - 480) * 1000 : 0);
                nextY = std::clamp<std::int64_t>(nextY, 0, maxY);
                c.props["panY1000"] = Value::integer(nextY);
            }
        };
        stepPanoramas(components_);
        stepPanoramas(groupComponents_);
        for (const auto& ended : animationEnds) {
            runEvent(ended.component + ".EndAnimation", {Value::integer(ended.id)});
        }
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
    // moderate traversal; fixed-point positions preserve subpixel timer
    // steps so they cannot stick forever at an edge (the old integer update
    // rounded every ~16 ms step back to the same coordinate).
    //   - Group Sprite_Holder (the cursors.grp toolbar): near-instant. The
    //     toolbar goes from fully hidden to fully deployed (~160 px) inside one
    //     ~156 ms capture frame, i.e. >=1000 px/s -- it snaps rather than
    //     drifts.
    // The exact per-instance velocity field in the sprite record is not yet
    // recovered. In particular record+0x38 contains coordinate/state-like
    // values, not velocity; treating it as px/s made every moving page object
    // race at a different, often extreme speed.
    constexpr double kPageGlideSpeed = 60.0;     // px/s
    constexpr double kGroupGlideSpeed = 1200.0;  // px/s (toolbar snap)

    struct Arrival { std::string component; int id; int x; int y; };
    std::vector<Arrival> arrivals;

    auto stepContainer = [&](std::vector<ComponentState>& components, double fallbackSpeed) {
        for (auto& c : components) {
            if (c.kind != HolderKind::SpriteHolder) continue;
            if (c.props.count("timersEnabled") && c.props["timersEnabled"].toInt() == 0) continue;
            for (auto& [id, item] : c.items) {
                if (item.count("gliding") == 0 || item["gliding"].toInt() == 0) continue;
                if (item.count("glidePaused") && item["glidePaused"].toInt() != 0) continue;
                const double holderSpeed = fallbackSpeed;
                const double dist = holderSpeed * elapsed / 1000.0;
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
        // ABECADLO's letter-block shuffle is authored at 200 ms, which feels
        // markedly slower in the browser than the original machine. Identify
        // that board by its unique 25-sprite/four-hotspot layout and tighten
        // only its active shuffle timer; zero still cancels it normally.
        const auto* sprites = resolve("Sprite_Holder");
        const auto* hotspots = resolve("HotSpot_Holder");
        if (timerInterval_ == 200 && sprites && hotspots &&
            sprites->sprites.size() == 25 && hotspots->hotspots.size() == 4) {
            timerInterval_ = 120;
        }
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
            // The shared cursor groups open the toolbar from an invisible
            // 10-20 px strip at the right edge (hotspot 0). Give that one
            // specific trigger a little inward hover tolerance; do not alter
            // authored hotspot geometry or any other interactive rectangle.
            const bool toolbarEdgeTrigger =
                state.displayName.rfind("Group.", 0) == 0 && h.id == 0 &&
                h.left >= 600 && (h.right - h.left) <= 64;
            const auto left = h.left - (toolbarEdgeTrigger ? 32 : 0);
            if (x < left || x > h.right || y < h.top || y > h.bottom) continue;
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
            const auto cell = itemInt(state, id, "cell");
            const auto* opacity = &frame.opaque;
            if (cell >= 0 && cell < static_cast<std::int64_t>(frame.animationOpaque.size())) {
                opacity = &frame.animationOpaque[static_cast<std::size_t>(cell)];
            }
            if (!opacity->empty()) {
                const auto cx = x - left;
                const auto cy = y - top;
                if (cx >= static_cast<std::int64_t>(frame.width) ||
                    cy >= static_cast<std::int64_t>(frame.height) ||
                    (*opacity)[static_cast<std::size_t>(cy) * frame.width + cx] == 0) {
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
            // Colouring boards serialize six variants per paintable object and
            // may append one composite decoration (SLON's paint buckets). The
            // decoration has no variants and is not an input target in the
            // original holder; routing it through the six-colour script hides
            // it after an out-of-range ShowBitmap call.
            if (state.indexedImages.size() > 6 &&
                state.indexedImages.size() % 6 == 1 &&
                r + 1 == state.indexedImages.size()) continue;
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
    } else if (state.kind == HolderKind::TextHolder) {
        // TextHolder accepts hover/click input only while its authored mouse
        // mode is enabled. GRZESIU/WIES/MROZ use these callbacks to change the
        // cursor and toggle synchronised poem playback.
        for (std::size_t r = state.texts.size(); r-- > 0;) {
            const int id = static_cast<int>(r);
            if (itemInt(state, id, "visible") == 0 ||
                itemInt(state, id, "mouseEnabled") == 0) continue;
            const auto& geom = state.texts[r];
            if (x < geom.left || x >= geom.right || y < geom.top || y >= geom.bottom) continue;
            const auto layer = itemInt(state, id, "z");
            if (best.index == -1 || layer > best.layer) {
                best.index = id;
                best.layer = static_cast<std::int32_t>(layer);
            }
        }
    } else if (state.kind == HolderKind::Puzzle) {
        std::int64_t bestOrder = std::numeric_limits<std::int64_t>::min();
        for (const auto& [id, item] : state.items) {
            if (id < 0 || static_cast<std::size_t>(id) >= state.puzzleChips.size() ||
                itemInt(state, id, "visible") == 0 || itemInt(state, id, "locked") != 0) continue;
            const auto& geom = state.puzzleChips[static_cast<std::size_t>(id)];
            const auto left = itemInt(state, id, "x");
            const auto top = itemInt(state, id, "y");
            if (x < left || x >= left + geom.width || y < top || y >= top + geom.height) continue;
            const auto pixel = static_cast<std::size_t>(y - top) * geom.width +
                               static_cast<std::size_t>(x - left);
            if (pixel >= geom.pixels.size() || geom.pixels[pixel] == geom.transparentIndex) continue;
            const auto order = itemInt(state, id, "z");
            if (best.index == -1 || order > bestOrder) {
                best.index = id;
                best.layer = 1;
                bestOrder = order;
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
    // Group components are dispatched after page components. Keep the strict
    // comparison used above: the host stops at the first component that handles
    // an equal-layer point, so page components precede group components and an
    // earlier group holder precedes a later one. Brzechwa's navigation fingers
    // rely on their Sprite_Holder beating the overlapping HotSpot_Holder.
    for (const auto& state : groupComponents_) {
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
        case HolderKind::TextHolder: return "ClickOnText";
        case HolderKind::Puzzle: return "MouseStartDrag";
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
    } else if (hit.kind == HolderKind::Puzzle) {
        auto* c = resolve(hit.component);
        if (c != nullptr && hit.index >= 0 &&
            static_cast<std::size_t>(hit.index) < c->puzzleChips.size()) {
            dragging_ = true;
            dragId_ = hit.index;
            const int chipX = static_cast<int>(itemInt(*c, hit.index, "x"));
            const int chipY = static_cast<int>(itemInt(*c, hit.index, "y"));
            grabOffsetX_ = x - chipX;
            grabOffsetY_ = y - chipY;
            std::int64_t topOrder = 0;
            for (const auto& [otherId, other] : c->items) {
                topOrder = std::max(topOrder, itemInt(*c, otherId, "z"));
            }
            c->items[hit.index]["z"] = Value::integer(topOrder + 1);
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
        if (state != nullptr && pressed_.kind == HolderKind::SpriteHolder && dragId_ >= 0 &&
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
        } else if (state != nullptr && pressed_.kind == HolderKind::Puzzle && dragId_ >= 0 &&
                   static_cast<std::size_t>(dragId_) < state->puzzleChips.size()) {
            const auto& geom = state->puzzleChips[static_cast<std::size_t>(dragId_)];
            auto& chip = state->items[dragId_];
            const auto dx = itemInt(*state, dragId_, "x") - geom.originalX;
            const auto dy = itemInt(*state, dragId_, "y") - geom.originalY;
            if (dx * dx + dy * dy <= 20 * 20) {
                chip["x"] = Value::integer(geom.originalX);
                chip["y"] = Value::integer(geom.originalY);
                chip["locked"] = Value::integer(1);
                runEvent(pressed_.component + ".ChipLock", {});
                const bool complete = std::all_of(
                    state->items.begin(), state->items.end(), [&](const auto& entry) {
                        return itemInt(*state, entry.first, "locked") != 0;
                    });
                if (complete) {
                    runEvent(pressed_.component + ".GameOver",
                             {state->props.count("puzzleId") ? state->props["puzzleId"]
                                                             : Value::integer(0)});
                }
            } else {
                runEvent(pressed_.component + ".MouseDrop", {});
            }
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

void Page::externalVideoEnd(int id) {
    if (auto* state = resolve("Video_Holder")) {
        auto& item = state->items[id];
        item["playing"] = Value::integer(0);
    }
    fireKindEvent(*this, components_, HolderKind::VideoHolder, "TheEnd", id);
}

void Page::soundEnd(int id) {
    fireKindEvent(*this, components_, HolderKind::SoundHolder, "EndPlaySound", id);
}

void Page::textEnd(int id) {
    if (auto* state = resolve("Text_Holder")) {
        state->items[id]["playing"] = Value::integer(0);
    }
    fireKindEvent(*this, components_, HolderKind::TextHolder, "EndOfSynchroText", id);
}

void Page::recorderEndRecord(bool hasData) {
    if (auto* state = resolve("Recorder")) {
        state->props["recording"] = Value::integer(0);
        const std::string file = state->props.count("file") ? state->props["file"].toString() : "";
        state->props["recorded:" + file] = Value::integer(hasData ? 1 : 0);
    }
    runEvent("Recorder.EndRecordSound", {});
}

void Page::recorderEndPlay() {
    if (auto* state = resolve("Recorder")) state->props["playing"] = Value::integer(0);
    runEvent("Recorder.EndPlaySound", {});
}

void Page::recorderProgress(int percentFull) {
    runEvent("Recorder.Progress", {Value::integer(percentFull)});
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

    // The non-holder Panorama DLL supplies its own mouse steering when
    // enabled. Edge pressure changes direction; returning to the middle stops
    // it. Panorama_Holder pages drive the same state explicitly through their
    // scripted GoLeft/GoRight/Stop calls.
    for (auto& state : components_) {
        if (state.kind != HolderKind::Panorama || state.props["panEnabled"].toInt() == 0) continue;
        state.props["panVelocity"] = Value::integer(x < 160 ? -90 : (x > 480 ? 90 : 0));
        state.props["panVelocityY"] = Value::integer(y < 120 ? -90 : (y > 360 ? 90 : 0));
    }

    // Visual hover follows merged layer order, but page HotSpotHolder also
    // receives its own hover route. RZECZKA deliberately puts its scrolling
    // edge hotspot behind interactive sprites: the finger cursor belongs to
    // the sprite while the hotspot still starts the panorama glide. Group
    // hotspots remain merged so a deployed toolbar button can cover its
    // full-page close strip.
    const Hit hit = findHit(x, y);
    std::map<std::string, Hit> active;
    if (hit.index != -1) active[hit.component] = hit;
    for (const auto& state : components_) {
        if (state.kind != HolderKind::HotSpotHolder) continue;
        const auto candidate = topItemIn(state, x, y);
        if (candidate.index == -1) continue;
        active[state.displayName] = {state.displayName, state.kind, candidate.index};
    }

    for (auto it = hoverByComponent_.begin(); it != hoverByComponent_.end();) {
        const Hit old = it->second;
        const auto current = active.find(old.component);
        if (current != active.end() && current->second.index == old.index) {
            ++it;
            continue;
        }
        fireHitEvent(old, "MouseMoveOut");
        it = hoverByComponent_.erase(it);
    }

    for (const auto& [component, current] : active) {
        const auto oldIt = hoverByComponent_.find(component);
        if (oldIt == hoverByComponent_.end() || oldIt->second.index != current.index) {
            fireHitEvent(current, "MouseMoveIn");
            hoverByComponent_[component] = current;
        }
    }
}

bool Page::mouseWheel(int x, int y, int wheelDelta) {
    if (wheelDelta == 0) return false;

    // Text_Holder owns its scrolling and only accepts input after EnableMouse.
    // Prefer the highest-layer visible text entry under the pointer, matching
    // the holder routing used by the rest of the runtime.
    ComponentState* target = nullptr;
    int targetId = -1;
    std::int64_t bestLayer = std::numeric_limits<std::int64_t>::min();
    for (auto& state : components_) {
        if (state.kind != HolderKind::TextHolder) continue;
        for (auto& [id, item] : state.items) {
            if (id < 0 || static_cast<std::size_t>(id) >= state.texts.size()) continue;
            if (itemInt(state, id, "visible") == 0 || itemInt(state, id, "mouseEnabled") == 0) continue;
            const auto& text = state.texts[static_cast<std::size_t>(id)];
            if (x < text.left || x >= text.right || y < text.top || y >= text.bottom) continue;
            const auto layer = itemInt(state, id, "z");
            if (target == nullptr || layer > bestLayer) {
                target = &state;
                targetId = id;
                bestLayer = layer;
            }
        }
    }
    // The original holder receives wheel messages at the page window and then
    // scrolls its active text, even when the pointer is over surrounding art.
    // Fall back to the highest visible mouse-enabled entry when no text rect
    // is directly under the cursor.
    if (target == nullptr) {
        for (auto& state : components_) {
            if (state.kind != HolderKind::TextHolder) continue;
            for (auto& [id, item] : state.items) {
                if (id < 0 || static_cast<std::size_t>(id) >= state.texts.size()) continue;
                if (itemInt(state, id, "visible") == 0 ||
                    itemInt(state, id, "mouseEnabled") == 0) continue;
                const auto layer = itemInt(state, id, "z");
                if (target == nullptr || layer > bestLayer) {
                    target = &state;
                    targetId = id;
                    bestLayer = layer;
                }
            }
        }
    }
    if (target == nullptr) return false;

    auto& item = target->items[targetId];
    const auto& text = target->texts[static_cast<std::size_t>(targetId)];
    const int lineHeight = static_cast<int>(std::max<std::uint32_t>(1, text.lineHeight));
    const int viewportHeight = std::max(0, text.bottom - text.top);
    const int contentHeight = static_cast<int>(text.lineCount) * lineHeight;
    const int minimumOffset = std::min(0, viewportHeight - contentHeight);
    const int current = static_cast<int>(itemInt(*target, targetId, "offsetY"));
    const int direction = wheelDelta > 0 ? 1 : -1;
    const int next = std::clamp(current + direction * lineHeight * 3, minimumOffset, 0);
    item["offsetY"] = Value::integer(next);

    runEvent(target->displayName + (direction > 0 ? ".ScrollTextUp" : ".ScrollTextDown"),
             {Value::integer(targetId)});
    if (next == current) {
        runEvent(target->displayName + ".EndScrollText", {Value::integer(targetId)});
    }
    return true;
}

} // namespace graphboard::runtime
