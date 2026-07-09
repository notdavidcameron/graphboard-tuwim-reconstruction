#pragma once

#include "graphboard/binary_reader.hpp"
#include "graphboard/holders.hpp"
#include "graphboard/runtime/interpreter.hpp"
#include "graphboard/runtime/value.hpp"

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace graphboard::runtime {

// The live state of one component instance on a page. GraphBoard components are
// index-addressed (a Sprite_Holder holds many sprites, a HotSpot_Holder many
// hotspots), so mutating methods like MoveTo(id, x, y) update per-index item
// state; holder-wide effects (which sound is playing) go in `props`.
struct ComponentState {
    std::string displayName;               // e.g. "Sprite_Holder"
    HolderKind kind = HolderKind::Unknown;
    std::map<int, std::map<std::string, Value>> items;  // per-index item state
    std::map<std::string, Value> props;                 // holder-wide state
    std::vector<HotSpot> hotspots;         // HotSpot_Holder geometry (for hit tests)
};

// One host call made while executing script (builtin or component method).
struct HostCallRecord {
    bool builtin = true;
    std::string component;   // component method only
    std::string name;
    std::vector<Value> args;
};

// An executable GraphBoard page: parsed components wired to the script
// interpreter as a live Host. Running an event handler mutates component and
// page state exactly as the script directs, so a scene can be driven headless
// and its script-visible state asserted.
//
// This models the script->component direction (dispatch to stateful components)
// and page/host builtins. The component->script callback direction (a hotspot
// click or video-end firing Sprite_Holder.MouseClickOnDown, etc.) is exposed as
// a hit-test query for now; wiring those callbacks is the next step.
class Page : public Host {
public:
    static std::unique_ptr<Page> loadFromFile(const std::filesystem::path& path);
    static std::unique_ptr<Page> loadFromReader(BinaryReader& reader, std::string sourceLabel);

    // Event dispatch (only fires if the handler is defined in the page).
    bool hasHandler(const std::string& name) const;
    void runEvent(const std::string& name, const std::vector<Value>& args);
    void open() { runEvent("OnOpenPage", {}); }
    void close() { runEvent("OnClosePage", {}); }
    void timer() { runEvent("OnTimer", {Value::integer(0)}); }
    void lButtonDown(int x, int y) {
        runEvent("OnLButtonDown", {Value::integer(x), Value::integer(y)});
    }
    void rButtonDown(int x, int y) {
        runEvent("OnRButtonDown", {Value::integer(x), Value::integer(y)});
    }
    void keyDown(int key) { runEvent("OnKeyDown", {Value::integer(key)}); }

    // Host interface.
    Value callBuiltin(const std::string& name, const std::vector<Value>& args) override;
    Value callComponent(const std::string& component, const std::string& method,
                        const std::vector<Value>& args) override;

    // Queries.
    const ComponentState* component(const std::string& name) const;
    std::vector<std::string> componentNames() const;
    const std::string& scriptText() const { return script_; }
    const std::vector<HostCallRecord>& callLog() const { return callLog_; }

    // Page/host state set by builtins.
    const std::string& pendingPage() const { return pendingPage_; }
    const std::string& currentGroup() const { return currentGroup_; }
    int cursor() const { return cursor_; }
    int timerInterval() const { return timerInterval_; }
    bool exited() const { return exited_; }

    // Index of the top-most enabled HotSpot_Holder hotspot containing (x, y),
    // or -1. Uses the recovered hit-test rule (left<=x<=right, top<=y<bottom,
    // enabled, highest layer wins).
    int hitTestHotSpot(int x, int y) const;

private:
    Page() = default;
    void parse(BinaryReader& reader);
    ComponentState* resolve(const std::string& componentPath);

    std::string script_;
    std::vector<ComponentState> components_;
    std::map<std::string, std::size_t> byName_;
    std::unique_ptr<Interpreter> interpreter_;

    std::vector<HostCallRecord> callLog_;
    std::string pendingPage_;
    std::string currentGroup_;
    int cursor_ = -1;
    int timerInterval_ = 0;
    bool exited_ = false;
    std::uint32_t randomState_ = 0x12345678u;
};

} // namespace graphboard::runtime
