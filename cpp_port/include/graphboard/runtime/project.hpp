#pragma once

#include "graphboard/format.hpp"
#include "graphboard/runtime/page.hpp"
#include "graphboard/runtime/value.hpp"

#include <filesystem>
#include <map>
#include <memory>
#include <string>

namespace graphboard::runtime {

// An executable GraphBoard project: START.PRJ plus the pages it names.
//
// At project load the host runs the manifest's trailing script — a block of
// global variable declarations (`int global1=0; CString parentWnd;`) — through
// GraphBrdScript_RunGlobalSetupBlock (Tuwim.exe:004281b0). Those variables are
// project-wide: 52 of the title's pages read or write them, so they must
// survive a page change.
//
// This models that lifetime. `globals()` holds the project-wide variables;
// openPage() seeds them into the page's script scope before running OnOpenPage,
// and harvests their values back when the page is left. Only names the setup
// block declared are carried across — a page's own OnOpenPage variables are
// page-scoped (see Interpreter::declareToGlobal_).
//
// Navigation follows the script: a page's LoadPage("next.bdf") sets its
// pendingPage(), and followPendingPage() opens it.
class Project {
public:
    // `startPrj` is the path to START.PRJ; pages are resolved next to it.
    static std::unique_ptr<Project> loadFromFile(const std::filesystem::path& startPrj);

    const ProjectManifest& manifest() const { return manifest_; }

    // Project-wide script globals, after the setup block ran (and after any
    // page that has been opened wrote to them).
    const std::map<std::string, Value>& globals() const { return globals_; }
    bool hasGlobal(const std::string& name) const;
    Value getGlobal(const std::string& name) const;

    // Open a page by the name the manifest/scripts use ("intro.bdf"); resolved
    // case-insensitively against the project directory. Seeds the project
    // globals, runs OnOpenPage, and becomes the current page. Throws ParseError
    // if the page file cannot be found or parsed.
    Page& openPage(const std::string& pageName);

    // Open manifest().startupPage.
    Page& openStartupPage();

    Page* currentPage() { return current_.get(); }
    const Page* currentPage() const { return current_.get(); }
    const std::string& currentPageName() const { return currentPageName_; }

    // If the current page requested a page change (LoadPage), open it and
    // return true. Harvests the outgoing page's project globals first.
    bool followPendingPage();

private:
    Project() = default;

    // Copy project globals into the page before it runs, and back out after.
    void seedGlobals(Page& page) const;
    void harvestGlobals(const Page& page);

    // Case-insensitive lookup of `pageName` in the project directory.
    std::filesystem::path resolvePagePath(const std::string& pageName) const;

    ProjectManifest manifest_;
    std::filesystem::path directory_;
    std::map<std::string, Value> globals_;
    std::unique_ptr<Page> current_;
    std::string currentPageName_;
};

} // namespace graphboard::runtime
