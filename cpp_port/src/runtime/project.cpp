#include "graphboard/runtime/project.hpp"

#include "graphboard/binary_reader.hpp"
#include "graphboard/runtime/interpreter.hpp"

#include <cctype>
#include <string>

namespace graphboard::runtime {

namespace {

// The global setup block is a list of declarations; it makes no host calls in
// the shipped title. Anything it does call is accepted and ignored, so a stray
// builtin cannot abort project load.
class NullHost : public Host {
public:
    Value callBuiltin(const std::string& name, const std::vector<Value>&) override {
        return name == "IsProject" ? Value::integer(1) : Value();
    }
    Value callComponent(const std::string&, const std::string&,
                        const std::vector<Value>&) override {
        return Value();
    }
};

std::string toLower(std::string s) {
    for (char& c : s) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return s;
}

} // namespace

std::unique_ptr<Project> Project::loadFromFile(const std::filesystem::path& startPrj) {
    auto reader = BinaryReader::fromFile(startPrj);
    std::unique_ptr<Project> project(new Project());
    project->manifest_ = parseProjectManifest(reader);
    project->directory_ = startPrj.parent_path();

    // Run the trailing global block once; its declarations are the project-wide
    // variables every page shares.
    NullHost host;
    Interpreter setup(project->manifest_.globalScript, host);
    setup.runGlobalSetup();
    project->globals_ = setup.globals();

    return project;
}

bool Project::hasGlobal(const std::string& name) const {
    return globals_.find(name) != globals_.end();
}

Value Project::getGlobal(const std::string& name) const {
    const auto it = globals_.find(name);
    return it != globals_.end() ? it->second : Value();
}

std::filesystem::path Project::resolvePagePath(const std::string& pageName) const {
    // Scripts and the manifest name pages in lower case ("intro.bdf") while the
    // shipped DATA folder is upper case ("INTRO.BDF").
    const std::filesystem::path direct = directory_ / pageName;
    if (std::filesystem::exists(direct)) {
        return direct;
    }
    const std::string wanted = toLower(pageName);
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(directory_, ec)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        if (toLower(entry.path().filename().string()) == wanted) {
            return entry.path();
        }
    }
    throw ParseError("Project::openPage: page not found: " + pageName);
}

void Project::seedGlobals(Page& page) const {
    for (const auto& [name, value] : globals_) {
        page.setGlobal(name, value);
    }
}

void Project::harvestGlobals(const Page& page) {
    // Only the project-declared names carry across pages; a page's own
    // OnOpenPage variables stay page-scoped.
    for (auto& [name, value] : globals_) {
        if (page.hasGlobal(name)) {
            value = page.getGlobal(name);
        }
    }
}

Page& Project::openPage(const std::string& pageName) {
    const auto path = resolvePagePath(pageName);

    // Carry the outgoing page's writes to the project globals before replacing it.
    if (current_) {
        harvestGlobals(*current_);
    }

    auto page = Page::loadFromFile(path);
    seedGlobals(*page);   // must precede OnOpenPage: its guards read these
    page->open();

    current_ = std::move(page);
    currentPageName_ = pageName;

    // OnOpenPage may already have written project globals.
    harvestGlobals(*current_);
    return *current_;
}

Page& Project::openStartupPage() {
    return openPage(manifest_.startupPage);
}

bool Project::followPendingPage() {
    if (!current_) {
        return false;
    }
    const std::string next = current_->pendingPage();
    if (next.empty()) {
        return false;
    }
    openPage(next);
    return true;
}

} // namespace graphboard::runtime
