#include "graphboard/runtime/project.hpp"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
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

std::string encodeSignature(const std::string& value) {
    std::string out;
    for (const unsigned char c : value) out.push_back(static_cast<char>(c + 0x21));
    return out;
}

// START.PRJ: manifest + trailing global-setup script.
std::vector<std::uint8_t> buildProject(const std::string& globalScript) {
    std::vector<std::uint8_t> b;
    putU32(b, 1);                            // version
    putArchiveString(b, "intro.bdf");        // startupPage
    putArchiveString(b, "");                 // currentPageOrGroupState
    putU32(b, 0);                            // audioPresetIndex
    putU32(b, 2);                            // pageCount
    putArchiveString(b, "intro.bdf");
    putArchiveString(b, "second.bdf");
    putU32(b, 0);                            // groupCount
    putArchiveString(b, encodeSignature("Julian Tuwim"));
    putU32(b, 1);                            // globalScriptVersion
    putArchiveString(b, globalScript);
    return b;
}

// A component-less .BDF carrying only a page script.
std::vector<std::uint8_t> buildPage(const std::string& script) {
    std::vector<std::uint8_t> b;
    putFixed(b, "YDP Board data file. Ver:1 test", 100);
    putU32(b, 1);                                                 // version
    putU32(b, 0); putU32(b, 0); putU32(b, 640); putU32(b, 480);   // page rect
    putU32(b, 0xffffffffu); putU32(b, 8);                         // layer range
    putU32(b, 0);                                                 // background flag
    putU8(b, 0);                                                  // background color
    putU32(b, 0);                                                 // palette bytes
    putU32(b, 0);                                                 // dib bytes
    putU32(b, 1);                                                 // list version
    putU32(b, 0);                                                 // component count
    putU32(b, 1);                                                 // script version
    putArchiveString(b, script);
    return b;
}

void writeFile(const std::filesystem::path& path, const std::vector<std::uint8_t>& bytes) {
    std::ofstream out(path, std::ios::binary);
    out.write(reinterpret_cast<const char*>(bytes.data()),
              static_cast<std::streamsize>(bytes.size()));
}

const char* kGlobalScript =
    "//Global variables\n"
    "   int global1=0;\n"
    "   int global2=0;\n"
    "   int global3=0;\n"
    "   CString parentWnd;\n";

const char* kIntroScript = R"S(
void OnOpenPage()
{
   global1 = global1 + 5;   // reads the seeded project global (0)
   int pageLocal = 99;      // page-scoped: must not become a project global
}

void OnLButtonDown(int x,int y)
{
   global2 = 42;
   LoadPage("second.bdf");
}
)S";

const char* kSecondScript = R"S(
void OnOpenPage()
{
   global3 = global1 + global2;   // 5 + 42 = 47, carried across the page change
}
)S";

// The setup block runs once at project load and its declarations become the
// project-wide globals; pages then share them across a LoadPage navigation.
void testProjectGlobalsAcrossPages() {
    const auto dir = std::filesystem::temp_directory_path() / "gb_project_test";
    std::filesystem::remove_all(dir);
    std::filesystem::create_directories(dir);

    writeFile(dir / "START.PRJ", buildProject(kGlobalScript));
    // Upper-case on disk, lower-case in the manifest: exercises case-insensitive
    // resolution, as in the shipped DATA folder.
    writeFile(dir / "INTRO.BDF", buildPage(kIntroScript));
    writeFile(dir / "SECOND.BDF", buildPage(kSecondScript));

    auto project = Project::loadFromFile(dir / "START.PRJ");
    assert(project->manifest().startupPage == "intro.bdf");

    // The setup block declared the project globals.
    assert(project->hasGlobal("global1"));
    assert(project->hasGlobal("parentWnd"));
    assert(project->getGlobal("global1").toInt() == 0);

    // Opening the startup page seeds them, runs OnOpenPage, harvests writes.
    Page& intro = project->openStartupPage();
    assert(project->currentPageName() == "intro.bdf");
    assert(project->getGlobal("global1").toInt() == 5);
    // OnOpenPage's own variable is page-scoped, not project-wide.
    assert(intro.hasGlobal("pageLocal"));
    assert(!project->hasGlobal("pageLocal"));

    // A click writes another global and requests the next page.
    intro.lButtonDown(10, 10);
    assert(intro.pendingPage() == "second.bdf");

    // Following it carries global1 and global2 into the new page's scope.
    assert(project->followPendingPage());
    assert(project->currentPageName() == "second.bdf");
    assert(project->getGlobal("global2").toInt() == 42);
    assert(project->getGlobal("global3").toInt() == 47);

    // Nothing further pending.
    assert(!project->followPendingPage());

    std::filesystem::remove_all(dir);
}

} // namespace

int main() {
    testProjectGlobalsAcrossPages();
    return 0;
}
