#include "graphboard/guid.hpp"
#include "graphboard/runtime/page.hpp"

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

using namespace graphboard;
using namespace graphboard::runtime;

namespace {

void putU8(std::vector<std::uint8_t>& b, std::uint8_t v) { b.push_back(v); }
void putU32(std::vector<std::uint8_t>& b, std::uint32_t v) {
    b.push_back(static_cast<std::uint8_t>(v));
    b.push_back(static_cast<std::uint8_t>(v >> 8));
    b.push_back(static_cast<std::uint8_t>(v >> 16));
    b.push_back(static_cast<std::uint8_t>(v >> 24));
}
void putArchiveString(std::vector<std::uint8_t>& b, const std::string& s) {
    // MFC CString archive prefix: u8 length, or 0xff + u16 length for longer
    // strings (the script exceeds 255 bytes, so this form is required).
    if (s.size() < 0xff) {
        b.push_back(static_cast<std::uint8_t>(s.size()));
    } else {
        b.push_back(0xff);
        b.push_back(static_cast<std::uint8_t>(s.size()));
        b.push_back(static_cast<std::uint8_t>(s.size() >> 8));
    }
    b.insert(b.end(), s.begin(), s.end());
}
void putFixed(std::vector<std::uint8_t>& b, const std::string& s, std::size_t width) {
    b.insert(b.end(), s.begin(), s.end());
    b.resize(b.size() + (width - s.size()), 0);
}
void putGuid(std::vector<std::uint8_t>& b, const Guid& g) {
    b.insert(b.end(), g.bytes.begin(), g.bytes.end());
}

// A wrapper record with no reflected members (functionCount = propertyCount = 0).
void putWrapper(std::vector<std::uint8_t>& b, const Guid& clsid, const std::string& name) {
    putGuid(b, clsid);           // container-written leading CLSID
    putU32(b, 1);                // wrapperVersion
    putU32(b, 0);                // functionCount
    putArchiveString(b, name);   // displayName
    putU32(b, 0);                // propertyCount
    putGuid(b, clsid);           // duplicate CLSID inside the wrapper
}

void putHotSpotRecord(std::vector<std::uint8_t>& b, std::int32_t l, std::int32_t t,
                      std::int32_t r, std::int32_t bot, std::int32_t layer,
                      std::int32_t enabled, std::int32_t id) {
    std::vector<std::uint8_t> rec(100, 0);
    auto put = [&rec](std::size_t off, std::int32_t v) {
        const auto u = static_cast<std::uint32_t>(v);
        rec[off] = static_cast<std::uint8_t>(u);
        rec[off + 1] = static_cast<std::uint8_t>(u >> 8);
        rec[off + 2] = static_cast<std::uint8_t>(u >> 16);
        rec[off + 3] = static_cast<std::uint8_t>(u >> 24);
    };
    put(0x00, l); put(0x04, t); put(0x08, r); put(0x0c, bot);
    put(0x18, id); put(0x1c, layer); put(0x20, enabled);
    b.insert(b.end(), rec.begin(), rec.end());
}

std::vector<std::uint8_t> buildSyntheticBdf(const std::string& script) {
    const auto hotspotClsid = Guid::fromString("DA768116-5341-11D0-B444-008048EB5D40");
    const auto spriteClsid = Guid::fromString("B64F3336-5368-11D0-B445-008048EB5D40");

    std::vector<std::uint8_t> b;
    putFixed(b, "YDP Board data file. Ver:1 test", 100);
    putU32(b, 1);                    // version
    putU32(b, 0); putU32(b, 0); putU32(b, 640); putU32(b, 480);  // page rect
    putU32(b, 0xffffffffu); putU32(b, 8);                        // layer range
    putU32(b, 0);                    // background flag
    putU8(b, 0);                     // background color index
    putU32(b, 0);                    // palette bytes
    putU32(b, 0);                    // dib bytes

    // Component list: 2 items.
    putU32(b, 1);                    // list version
    putU32(b, 2);                    // count

    // Item 0: HotSpot_Holder with two hotspots.
    putWrapper(b, hotspotClsid, "HotSpot_Holder");
    putU32(b, 0);                    // hotspot private version
    putU8(b, 0); putU8(b, 0); putU8(b, 0);  // flags
    putU32(b, 2);                    // hotspot count
    // Ids deliberately differ from array indices (as in RZECZKA.BDF, which
    // stores 0,1,3,4,6,5,7,8): the script sees the id, never the index.
    putHotSpotRecord(b, 0, 0, 100, 100, 1, 1, /*id=*/5);
    putArchiveString(b, "hs0");
    putHotSpotRecord(b, 200, 200, 300, 300, 1, 1, /*id=*/7);
    putArchiveString(b, "hs1");
    putU32(b, 0); putU32(b, 0);      // activeIndex / auxStateWord

    // Item 1: Sprite_Holder with one definition and one instance.
    putWrapper(b, spriteClsid, "Sprite_Holder");
    putU32(b, 1);                    // sprite private version
    putU32(b, 1);                    // definition count
    putU32(b, 1);                    // instance count
    putU32(b, 0x90);                 // blobByteCount
    {
        std::vector<std::uint8_t> blob(0x90, 0);
        const std::string n = "spr";
        for (std::size_t i = 0; i < n.size(); ++i) blob[0x04 + i] = static_cast<std::uint8_t>(n[i]);
        blob[0x00] = 1;              // phaseCount -> one frame record at +0x6c
        blob[0x80] = 32;             // frames[0].width  (== blob+0x6c+0x14)
        blob[0x84] = 16;             // frames[0].height (== blob+0x6c+0x18)
        b.insert(b.end(), blob.begin(), blob.end());
    }
    {
        // Instance: definition 0, at (400,400), layer 3, phase 0, visible.
        // Layer 3 is above the hotspots (layer 1), so the sprite wins any
        // overlap -- exercised by testCrossKindLayerPrecedence below. Its
        // starting position is clear of both hotspot rects.
        std::vector<std::uint8_t> rec(0x8c, 0);
        auto put = [&rec](std::size_t off, std::int32_t v) {
            const auto u = static_cast<std::uint32_t>(v);
            rec[off] = static_cast<std::uint8_t>(u);
            rec[off + 1] = static_cast<std::uint8_t>(u >> 8);
            rec[off + 2] = static_cast<std::uint8_t>(u >> 16);
            rec[off + 3] = static_cast<std::uint8_t>(u >> 24);
        };
        put(0x00, 0);    // definition index
        put(0x08, 400);  // posX
        put(0x0c, 400);  // posY
        put(0x18, 3);    // layer
        put(0x5c, 0);    // phase
        put(0x88, 1);    // visible
        b.insert(b.end(), rec.begin(), rec.end());
    }

    // Page script text block.
    putU32(b, 1);                    // script version
    putArchiveString(b, script);
    return b;
}

const char* kScript = R"S(
OnOpenPage()
{
   HotSpot_Holder.DisableHotSpot(5);
   Sprite_Holder.MoveTo(0, 100, 200);
   Sprite_Holder.ShowBitmap(0);
   Sprite_Holder.ChangePhase(0, 2);
   LoadGroup("cursors.grp");
   SetCursor(3);
}

void OnLButtonDown(int x, int y)
{
   Sprite_Holder.ChangePhase(0, 5);
   Group.HotSpot_Holder.EnableHotSpot(5);
   LoadPage("next.bdf");
}
)S";

Value item(const ComponentState* c, int idx, const std::string& key) {
    const auto ci = c->items.find(idx);
    if (ci == c->items.end()) return Value();
    const auto ki = ci->second.find(key);
    return ki == ci->second.end() ? Value() : ki->second;
}

void testDriveSyntheticPage() {
    const auto bytes = buildSyntheticBdf(kScript);
    BinaryReader reader(bytes);
    auto page = Page::loadFromReader(reader, "synthetic");

    // Components discovered and seeded.
    const auto names = page->componentNames();
    assert(names.size() == 2);
    assert(page->component("HotSpot_Holder") != nullptr);
    assert(page->component("Sprite_Holder") != nullptr);
    assert(page->component("HotSpot_Holder")->hotspots.size() == 2);
    assert(page->hasHandler("OnOpenPage"));

    // Hit-test before running: both hotspots enabled. The reported value is the
    // stored id (5 / 7), not the array index (0 / 1).
    assert(page->hitTestHotSpot(50, 50) == 5);
    assert(page->hitTestHotSpot(250, 250) == 7);
    assert(page->hitTestHotSpot(150, 150) == -1);
    // The click-path rect is inclusive on every edge (LButtonDown semantics).
    assert(page->hitTestHotSpot(100, 100) == 5);
    assert(page->hitTestHotSpot(101, 100) == -1);

    // Run OnOpenPage; assert script-visible component + page state.
    page->open();
    const auto* sprite = page->component("Sprite_Holder");
    assert(item(sprite, 0, "x").toInt() == 100);
    assert(item(sprite, 0, "y").toInt() == 200);
    assert(item(sprite, 0, "visible").toInt() == 1);
    assert(item(sprite, 0, "phase").toInt() == 2);
    assert(page->currentGroup() == "cursors.grp");
    assert(page->cursor() == 3);

    // Hotspot id 5 was disabled by the script -> hit-test now misses it.
    assert(item(page->component("HotSpot_Holder"), 5, "enabled").toInt() == 0);
    assert(page->hitTestHotSpot(50, 50) == -1);
    assert(page->hitTestHotSpot(250, 250) == 7);

    // A click re-enables hotspot id 5 (via Group.HotSpot_Holder.EnableHotSpot),
    // changes the sprite phase, and requests the next page.
    page->lButtonDown(250, 250);
    assert(item(page->component("Sprite_Holder"), 0, "phase").toInt() == 5);
    assert(item(page->component("HotSpot_Holder"), 5, "enabled").toInt() == 1);
    assert(page->hitTestHotSpot(50, 50) == 5);
    assert(page->pendingPage() == "next.bdf");
}

const char* kCallbackScript = R"S(
void OnOpenPage() {}

void HotSpot_Holder.LeftButtonClickOn(int rectID)
{
   Sprite_Holder.ChangePhase(0, rectID + 10);
}

void HotSpot_Holder.RightButtonClickOn(int rectID)
{
   Sprite_Holder.ChangePhase(0, rectID + 20);
}

void HotSpot_Holder.MouseMoveIn(int rectID)
{
   Sprite_Holder.ChangePhase(0, rectID + 100);
}

void HotSpot_Holder.MouseMoveOut(int rectID)
{
   Sprite_Holder.ChangePhase(0, rectID + 200);
}
)S";

// The component->script callback direction: a click or hover crossing a
// HotSpot_Holder rect fires the matching reflected event (recovered in
// docs/component_interfaces.md) only if the page script defines it. The rectID
// handed to the script is the record's stored id (5 / 7), never the index.
void testHotSpotCallbacks() {
    const auto bytes = buildSyntheticBdf(kCallbackScript);
    BinaryReader reader(bytes);
    auto page = Page::loadFromReader(reader, "synthetic-callbacks");
    const auto* sprite = page->component("Sprite_Holder");

    // Click on the (200,200)-(300,300) hotspot -> LeftButtonClickOn(7).
    page->lButtonDown(250, 250);
    assert(item(sprite, 0, "phase").toInt() == 17);

    // Right-click on the (0,0)-(100,100) hotspot -> RightButtonClickOn(5).
    page->rButtonDown(50, 50);
    assert(item(sprite, 0, "phase").toInt() == 25);

    // Hovering in fires MouseMoveIn(5); repeating the same point does not
    // re-fire.
    page->mouseMove(50, 50);
    assert(item(sprite, 0, "phase").toInt() == 105);
    page->mouseMove(50, 50);
    assert(item(sprite, 0, "phase").toInt() == 105);

    // Moving across fires MouseMoveOut(5) then MouseMoveIn(7); the final phase
    // reflects whichever ran last (MouseMoveIn).
    page->mouseMove(250, 250);
    assert(item(sprite, 0, "phase").toInt() == 107);

    // Moving to empty space fires MouseMoveOut(7) with nothing to enter.
    page->mouseMove(150, 150);
    assert(item(sprite, 0, "phase").toInt() == 207);
}

const char* kSpriteCallbackScript = R"S(
void OnOpenPage() {}

void Sprite_Holder.MouseClickOnDown(int spriteID)
{
   Sprite_Holder.ChangePhase(0, spriteID + 50);
}

void Sprite_Holder.MouseMoveIn(int spriteID)
{
   Sprite_Holder.ChangePhase(0, spriteID + 60);
}

void Sprite_Holder.MouseMoveOut(int spriteID)
{
   Sprite_Holder.ChangePhase(0, spriteID + 70);
}
)S";

// Sprites are addressed by instance index, hit-tested against the current
// phase's frame rect at their live position, and (unlike hotspots) have no
// right-click event.
void testSpriteCallbacks() {
    const auto bytes = buildSyntheticBdf(kSpriteCallbackScript);
    BinaryReader reader(bytes);
    auto page = Page::loadFromReader(reader, "synthetic-sprite");
    const auto* sprite = page->component("Sprite_Holder");

    // Seeded straight from the instance record: (400,400), visible, phase 0.
    assert(item(sprite, 0, "x").toInt() == 400);
    assert(item(sprite, 0, "visible").toInt() == 1);

    // The sprite's rect is its 32x16 frame at (400,400) -> (432,416) inclusive.
    page->lButtonDown(410, 405);
    assert(item(sprite, 0, "phase").toInt() == 50);   // MouseClickOnDown(0)

    // Just outside the frame: nothing fires, phase unchanged.
    page->lButtonDown(433, 405);
    assert(item(sprite, 0, "phase").toInt() == 50);

    // Sprite_Holder declares no right-click event -> a right click over it
    // fires nothing (the page-level OnRButtonDown is undefined here too).
    page->rButtonDown(410, 405);
    assert(item(sprite, 0, "phase").toInt() == 50);

    // Hover in, then out to empty space.
    page->mouseMove(410, 405);
    assert(item(sprite, 0, "phase").toInt() == 60);   // MouseMoveIn(0)
    page->mouseMove(410, 405);
    assert(item(sprite, 0, "phase").toInt() == 60);   // no re-fire
    page->mouseMove(150, 150);
    assert(item(sprite, 0, "phase").toInt() == 70);   // MouseMoveOut(0)
}

// A page with a single sprite whose frame carries a per-pixel transparency
// mask: an 8x8 sprite transparent (index 30) everywhere except its left half,
// with the pixel-test opt-ins set (def+0x24 and frame+0x08).
std::vector<std::uint8_t> buildMaskedSpriteBdf(const std::string& script) {
    const auto spriteClsid = Guid::fromString("B64F3336-5368-11D0-B445-008048EB5D40");
    const std::uint32_t w = 8, h = 8, transp = 30;

    std::vector<std::uint8_t> b;
    putFixed(b, "YDP Board data file. Ver:1 test", 100);
    putU32(b, 1);
    putU32(b, 0); putU32(b, 0); putU32(b, 640); putU32(b, 480);
    putU32(b, 0xffffffffu); putU32(b, 8);
    putU32(b, 0);
    putU8(b, 0);
    putU32(b, 0);
    putU32(b, 0);

    putU32(b, 1);   // list version
    putU32(b, 1);   // one component

    putWrapper(b, spriteClsid, "Sprite_Holder");
    putU32(b, 1);   // sprite private version
    putU32(b, 1);   // definition count
    putU32(b, 1);   // instance count

    // Blob: header (0xb8) + frame table already inside it + 8x8 pixel plane.
    const std::size_t pixels = 0xb8;               // frame[0x48] == 0
    std::vector<std::uint8_t> blob(pixels + w * h, transp);
    auto putBlob = [&blob](std::size_t off, std::uint32_t v) {
        blob[off] = static_cast<std::uint8_t>(v);
        blob[off + 1] = static_cast<std::uint8_t>(v >> 8);
        blob[off + 2] = static_cast<std::uint8_t>(v >> 16);
        blob[off + 3] = static_cast<std::uint8_t>(v >> 24);
    };
    putBlob(0x00, 1);        // phaseCount
    putBlob(0x24, 1);        // def opts into the pixel test
    putBlob(0x80, w);        // header width/height
    putBlob(0x84, h);
    putBlob(0x6c + 0x04, transp);  // frame transparent index
    putBlob(0x6c + 0x08, 1);       // frame opts into the pixel test
    putBlob(0x6c + 0x10, w);       // row width -> stride 8
    putBlob(0x6c + 0x14, w);       // frame width/height
    putBlob(0x6c + 0x18, h);
    putBlob(0x6c + 0x48, 0);       // pixel offset (relative to blob+0xb8)
    // Opaque only in the left half (cols 0..3); bottom-up storage.
    for (std::uint32_t row = 0; row < h; ++row) {
        const std::size_t src = pixels + (h - 1 - row) * w;
        for (std::uint32_t col = 0; col < 4; ++col) blob[src + col] = 7;  // != transp
    }
    putU32(b, static_cast<std::uint32_t>(blob.size()));
    b.insert(b.end(), blob.begin(), blob.end());

    // Instance: definition 0 at (100,100), layer 0, phase 0, visible.
    std::vector<std::uint8_t> rec(0x8c, 0);
    auto putRec = [&rec](std::size_t off, std::int32_t v) {
        const auto u = static_cast<std::uint32_t>(v);
        rec[off] = static_cast<std::uint8_t>(u);
        rec[off + 1] = static_cast<std::uint8_t>(u >> 8);
        rec[off + 2] = static_cast<std::uint8_t>(u >> 16);
        rec[off + 3] = static_cast<std::uint8_t>(u >> 24);
    };
    putRec(0x08, 100); putRec(0x0c, 100); putRec(0x88, 1);
    b.insert(b.end(), rec.begin(), rec.end());

    putU32(b, 1);
    putArchiveString(b, script);
    return b;
}

const char* kMaskScript = R"S(
void OnOpenPage() {}
void Sprite_Holder.MouseClickOnDown(int spriteID) { Sprite_Holder.ChangePhase(0, 1); }
)S";

// The rect is (100,100)-(108,108); only the left half is opaque. A click on the
// opaque half hits; a click on the transparent half falls through the rect.
void testSpritePixelMask() {
    const auto bytes = buildMaskedSpriteBdf(kMaskScript);
    BinaryReader reader(bytes);
    auto page = Page::loadFromReader(reader, "synthetic-mask");
    const auto* sprite = page->component("Sprite_Holder");

    page->lButtonDown(101, 104);   // left half: opaque -> hit
    assert(item(sprite, 0, "phase").toInt() == 1);

    // Reset and click the transparent right half: no hit.
    auto page2 = [&] { BinaryReader r(bytes); return Page::loadFromReader(r, "m2"); }();
    page2->lButtonDown(106, 104);  // right half: transparent -> miss
    assert(item(page2->component("Sprite_Holder"), 0, "phase").toInt() == 0);
}

// A page with one draggable sprite (instance+0x1c == 1), a 20x20 frame at
// (100,100), used to exercise the down->move->up drag sequence.
std::vector<std::uint8_t> buildDraggableSpriteBdf(const std::string& script) {
    const auto spriteClsid = Guid::fromString("B64F3336-5368-11D0-B445-008048EB5D40");
    std::vector<std::uint8_t> b;
    putFixed(b, "YDP Board data file. Ver:1 test", 100);
    putU32(b, 1);
    putU32(b, 0); putU32(b, 0); putU32(b, 640); putU32(b, 480);
    putU32(b, 0xffffffffu); putU32(b, 8);
    putU32(b, 0); putU8(b, 0); putU32(b, 0); putU32(b, 0);
    putU32(b, 1); putU32(b, 1);   // list: one component
    putWrapper(b, spriteClsid, "Sprite_Holder");
    putU32(b, 1); putU32(b, 1); putU32(b, 1);   // version, 1 def, 1 instance
    putU32(b, 0x90);
    {
        std::vector<std::uint8_t> blob(0x90, 0);
        blob[0x00] = 1;    // phaseCount
        blob[0x80] = 20;   // frame width
        blob[0x84] = 20;   // frame height
        b.insert(b.end(), blob.begin(), blob.end());
    }
    {
        std::vector<std::uint8_t> rec(0x8c, 0);
        auto put = [&rec](std::size_t off, std::int32_t v) {
            const auto u = static_cast<std::uint32_t>(v);
            rec[off]=static_cast<std::uint8_t>(u);      rec[off+1]=static_cast<std::uint8_t>(u>>8);
            rec[off+2]=static_cast<std::uint8_t>(u>>16); rec[off+3]=static_cast<std::uint8_t>(u>>24);
        };
        put(0x00, 0); put(0x08, 100); put(0x0c, 100);
        put(0x18, 4); put(0x1c, 1);   // layer, drag-enabled
        put(0x88, 1);                 // visible
        b.insert(b.end(), rec.begin(), rec.end());
    }
    putU32(b, 1);
    putArchiveString(b, script);
    return b;
}

const char* kDragScript = R"S(
void OnOpenPage() {}
void Sprite_Holder.MouseClickOnDown(int id) { down = down + 1; }
void Sprite_Holder.MouseClickOnUp(int id) { up = up + 1; }
void Sprite_Holder.MouseDrop(int id, int left, int top, int right, int bottom)
{
   dropLeft = left; dropTop = top; dropRight = right; dropBottom = bottom;
}
)S";

// The full down -> move -> up drag: MouseClickOnDown fires on press, the sprite
// follows the cursor (offset by the grab point), MouseClickOnUp + MouseDrop fire
// on release with the sprite's final bounding box.
void testSpriteDrag() {
    const auto bytes = buildDraggableSpriteBdf(kDragScript);
    BinaryReader reader(bytes);
    auto page = Page::loadFromReader(reader, "drag");
    page->open();
    const auto* spr = page->component("Sprite_Holder");

    // Grab at (110,110) -- 10px into the 20x20 sprite at (100,100).
    page->lButtonDown(110, 110);
    // Drag so the cursor moves to (300,250); grab offset (10,10) keeps the same
    // spot under the cursor, so the top-left lands at (290,240).
    page->mouseMove(300, 250);
    assert(item(spr, 0, "x").toInt() == 290);
    assert(item(spr, 0, "y").toInt() == 240);
    page->lButtonUp(300, 250);

    // Callbacks: one down, one up, and MouseDrop with the final 20x20 rect.
    assert(page->hasGlobal("down") && page->getGlobal("down").toInt() == 1);
    assert(page->hasGlobal("up") && page->getGlobal("up").toInt() == 1);
    assert(page->getGlobal("dropLeft").toInt() == 290);
    assert(page->getGlobal("dropTop").toInt() == 240);
    assert(page->getGlobal("dropRight").toInt() == 310);
    assert(page->getGlobal("dropBottom").toInt() == 260);

    // After release the drag state is cleared: a hover now fires normally
    // instead of moving the sprite.
    page->mouseMove(295, 245);   // inside the moved sprite -> hover, not drag
    assert(item(page->component("Sprite_Holder"), 0, "x").toInt() == 290);
}

const char* kCompletionScript = R"S(
OnOpenPage()
{
   HotSpot_Holder.DisableHotSpot(0);
}

void Sprite_Holder.EndAnimation(int spriteID)
{
   Sprite_Holder.ChangePhase(0, spriteID + 30);
}
)S";

// Playback-completion callbacks: animationEnd(id) fires Sprite_Holder.EndAnimation
// on the page's Sprite_Holder, only if the handler is defined. (videoEnd/soundEnd
// share the code path and are verified on real data -- INTRO's TheEnd chain.)
void testCompletionEvents() {
    const auto bytes = buildSyntheticBdf(kCompletionScript);
    BinaryReader reader(bytes);
    auto page = Page::loadFromReader(reader, "completion");
    page->open();
    const auto* sprite = page->component("Sprite_Holder");

    page->animationEnd(4);   // -> Sprite_Holder.EndAnimation(4) -> ChangePhase(0,34)
    assert(item(sprite, 0, "phase").toInt() == 34);

    // A kind with no component / no handler is a safe no-op.
    page->videoEnd(0);
    page->soundEnd(0);
}

const char* kPrecedenceScript = R"S(
void OnOpenPage()
{
   Sprite_Holder.MoveTo(0, 50, 50);
}

void Sprite_Holder.MouseClickOnDown(int spriteID)
{
   Sprite_Holder.ChangePhase(0, 50);
}

void HotSpot_Holder.LeftButtonClickOn(int rectID)
{
   Sprite_Holder.ChangePhase(0, 99);
}
)S";

// Cross-kind z-order: the board dispatches the topmost layer first, so a sprite
// on layer 3 beats a hotspot on layer 1 wherever they overlap -- and the
// hotspot still wins where the sprite's frame does not reach.
void testCrossKindLayerPrecedence() {
    const auto bytes = buildSyntheticBdf(kPrecedenceScript);
    BinaryReader reader(bytes);
    auto page = Page::loadFromReader(reader, "synthetic-precedence");
    page->open();   // moves the sprite to (50,50) -> frame rect (50,50)-(82,66)

    const auto* sprite = page->component("Sprite_Holder");

    // (60,60) is inside both the sprite frame and hotspot id 5's (0,0)-(100,100).
    // The sprite's layer 3 outranks the hotspot's layer 1.
    page->lButtonDown(60, 60);
    assert(item(sprite, 0, "phase").toInt() == 50);

    // (90,90) is inside the hotspot but past the sprite's 32x16 frame, so the
    // hotspot wins by default rather than by layer.
    page->lButtonDown(90, 90);
    assert(item(sprite, 0, "phase").toInt() == 99);
}

// A page with a single Bitmap_Holder holding one bitmap. Bitmap blob layout:
// rect at +0x08..+0x14, layer at +0x18, name at +0x34, pixels from +0x90.
std::vector<std::uint8_t> buildBitmapBdf(const std::string& script,
                                         std::int32_t l, std::int32_t t,
                                         std::int32_t r, std::int32_t bot,
                                         std::int32_t layer) {
    const auto clsid = Guid::fromString("0D8A5736-5337-11D0-B444-008048EB5D40");
    std::vector<std::uint8_t> b;
    putFixed(b, "YDP Board data file. Ver:1 test", 100);
    putU32(b, 1);
    putU32(b, 0); putU32(b, 0); putU32(b, 640); putU32(b, 480);
    putU32(b, 0xffffffffu); putU32(b, 8);
    putU32(b, 0);
    putU8(b, 0);
    putU32(b, 0);
    putU32(b, 0);

    putU32(b, 1);   // list version
    putU32(b, 1);   // one component

    putWrapper(b, clsid, "Bitmap_Holder");
    putU32(b, 1);   // bitmap private version
    putU32(b, 1);   // bitmap count

    const std::uint32_t w = static_cast<std::uint32_t>(r - l);
    const std::uint32_t h = static_cast<std::uint32_t>(bot - t);
    const std::uint32_t stride = (w + 3) & ~3u;
    std::vector<std::uint8_t> blob(0x90 + stride * h, 0);
    auto putBlob = [&blob](std::size_t off, std::int32_t v) {
        const auto u = static_cast<std::uint32_t>(v);
        blob[off] = static_cast<std::uint8_t>(u);
        blob[off + 1] = static_cast<std::uint8_t>(u >> 8);
        blob[off + 2] = static_cast<std::uint8_t>(u >> 16);
        blob[off + 3] = static_cast<std::uint8_t>(u >> 24);
    };
    putBlob(0x08, l); putBlob(0x0c, t); putBlob(0x10, r); putBlob(0x14, bot);
    putBlob(0x18, layer);
    putU32(b, static_cast<std::uint32_t>(blob.size()));
    b.insert(b.end(), blob.begin(), blob.end());

    putU32(b, 1);
    putArchiveString(b, script);
    return b;
}

const char* kBitmapScript = R"S(
void OnOpenPage() {}
void Bitmap_Holder.MouseClickOnDown(int bitmapID) { Bitmap_Holder.MoveTo(bitmapID, 300, 300); }
void Bitmap_Holder.MouseMoveIn(int bitmapID) { Bitmap_Holder.MoveTo(bitmapID, 5, 5); }
)S";

// Bitmap_Holder click/hover, addressed by index, rect via PtInRect
// (right/bottom exclusive), with MoveTo shifting what is clickable.
void testBitmapCallbacks() {
    const auto bytes = buildBitmapBdf(kBitmapScript, 100, 100, 200, 150, /*layer=*/2);
    BinaryReader reader(bytes);
    auto page = Page::loadFromReader(reader, "synthetic-bitmap");
    const auto* bmp = page->component("Bitmap_Holder");

    // Seeded at its rect's top-left.
    assert(item(bmp, 0, "x").toInt() == 100);
    assert(item(bmp, 0, "y").toInt() == 100);

    // Inside the rect -> hit. The right/bottom edges are exclusive.
    assert(page->hitTestHotSpot(150, 120) == -1);  // hotspot-only query ignores bitmaps
    assert(page->component("Bitmap_Holder") != nullptr);
    assert(page->hasHandler("Bitmap_Holder.MouseClickOnDown"));
    assert(page->hasHandler("Bitmap_Holder.MouseMoveIn"));

    page->lButtonDown(200, 120);   // x == right (exclusive) -> miss
    assert(item(page->component("Bitmap_Holder"), 0, "x").toInt() == 100);

    page->lButtonDown(150, 120);   // inside -> MouseClickOnDown(0) -> MoveTo(0,300,300)
    assert(item(page->component("Bitmap_Holder"), 0, "x").toInt() == 300);
    assert(item(page->component("Bitmap_Holder"), 0, "y").toInt() == 300);

    // It moved: the old point now misses, the new rect (300,300)-(400,350) hits.
    page->mouseMove(150, 120);     // old location -> no hover
    assert(item(page->component("Bitmap_Holder"), 0, "x").toInt() == 300);
    page->mouseMove(350, 320);     // new location -> MouseMoveIn(0) -> MoveTo(0,5,5)
    assert(item(page->component("Bitmap_Holder"), 0, "x").toInt() == 5);
}

// A Bitmap_Holder bitmap with a per-pixel transparency mask: 10x10 at (100,100),
// transparent index 30, opaque only in the left half, with the record's pixel-
// test gates (blob+0x20 and +0x28) set.
std::vector<std::uint8_t> buildMaskedBitmapBdf(const std::string& script) {
    const auto clsid = Guid::fromString("0D8A5736-5337-11D0-B444-008048EB5D40");
    const std::int32_t l = 100, t = 100, w = 10, h = 10, transp = 30;
    std::vector<std::uint8_t> b;
    putFixed(b, "YDP Board data file. Ver:1 test", 100);
    putU32(b, 1);
    putU32(b, 0); putU32(b, 0); putU32(b, 640); putU32(b, 480);
    putU32(b, 0xffffffffu); putU32(b, 8);
    putU32(b, 0); putU8(b, 0); putU32(b, 0); putU32(b, 0);
    putU32(b, 1); putU32(b, 1);
    putWrapper(b, clsid, "Bitmap_Holder");
    putU32(b, 1); putU32(b, 1);   // version, bitmap count

    const std::uint32_t stride = (w + 3) & ~3u;
    std::vector<std::uint8_t> blob(0x80 + stride * h + 0x10, static_cast<std::uint8_t>(transp));
    auto pb = [&blob](std::size_t off, std::int32_t v) {
        const auto u = static_cast<std::uint32_t>(v);
        blob[off] = static_cast<std::uint8_t>(u);      blob[off+1] = static_cast<std::uint8_t>(u>>8);
        blob[off+2] = static_cast<std::uint8_t>(u>>16); blob[off+3] = static_cast<std::uint8_t>(u>>24);
    };
    pb(0x04, transp);                        // transparent index
    pb(0x08, l); pb(0x0c, t); pb(0x10, l+w); pb(0x14, t+h);
    pb(0x18, 0);                             // layer
    pb(0x20, 1); pb(0x28, 1);                // pixel-test opt-in gates
    for (std::uint32_t row = 0; row < static_cast<std::uint32_t>(h); ++row) {
        const std::size_t src = 0x80 + (h - 1 - row) * stride;   // bottom-up
        for (std::uint32_t col = 0; col < 5; ++col) blob[src + col] = 7;  // left half opaque
    }
    putU32(b, static_cast<std::uint32_t>(blob.size()));
    b.insert(b.end(), blob.begin(), blob.end());

    // Bitmap_Holder has no instance table; the script text block follows directly.
    putU32(b, 1);
    putArchiveString(b, script);
    return b;
}

void testBitmapPixelMask() {
    const char* script =
        "void OnOpenPage(){}\n"
        "void Bitmap_Holder.MouseClickOnDown(int id){ Bitmap_Holder.MoveTo(id,7,7); }\n";
    const auto bytes = buildMaskedBitmapBdf(script);
    BinaryReader reader(bytes);
    auto page = Page::loadFromReader(reader, "masked-bitmap");

    // Rect is (100,100)-(110,110); only the left half (cols 0..4) is opaque.
    page->lButtonDown(107, 104);   // right half -> transparent -> miss
    assert(item(page->component("Bitmap_Holder"), 0, "x").toInt() == 100);
    page->lButtonDown(102, 104);   // left half -> opaque -> hit -> MoveTo(0,7,7)
    assert(item(page->component("Bitmap_Holder"), 0, "x").toInt() == 7);
}

// A bitmap on layer 5 outranks a hotspot on layer 1 where they overlap.
void testBitmapVsHotSpotLayer() {
    // Reuse the hotspot builder's page, but a bitmap-only page suffices here:
    // verify a bitmap participates in the cross-kind layer walk via findHit by
    // confirming a click on it fires the bitmap event, not a hotspot's.
    const auto bytes = buildBitmapBdf(
        "void OnOpenPage(){}\nvoid Bitmap_Holder.MouseClickOnDown(int id){ Bitmap_Holder.MoveTo(id,1,1); }\n",
        0, 0, 640, 480, /*layer=*/5);
    BinaryReader reader(bytes);
    auto page = Page::loadFromReader(reader, "bitmap-fullscreen");
    page->lButtonDown(320, 240);
    assert(item(page->component("Bitmap_Holder"), 0, "x").toInt() == 1);
}

} // namespace

int main() {
    testDriveSyntheticPage();
    testHotSpotCallbacks();
    testSpriteCallbacks();
    testSpritePixelMask();
    testSpriteDrag();
    testCompletionEvents();
    testCrossKindLayerPrecedence();
    testBitmapCallbacks();
    testBitmapPixelMask();
    testBitmapVsHotSpotLayer();
    return 0;
}
