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
        blob[0x80] = 32;             // width
        blob[0x84] = 16;             // height
        b.insert(b.end(), blob.begin(), blob.end());
    }
    {
        std::vector<std::uint8_t> rec(0x8c, 0);  // instance: def index 0 at +0x00
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

} // namespace

int main() {
    testDriveSyntheticPage();
    testHotSpotCallbacks();
    return 0;
}
