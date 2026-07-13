# GraphBoard High-Level C++ Port

This branch is a readable C++ reconstruction of GraphBoard/Tuwim behavior. It is
not a direct paste of Ghidra decompiler output. Decompiled code and recovered
offsets are used as references, but the port is organized around domain concepts:
archives, projects, pages, components, assets, and runtime actions.

Dependency-free code is not a goal by itself. Prefer proven libraries when they
reduce risk or keep the port focused on recovered behavior instead of commodity
plumbing.

## Current Scope

- MFC `CArchive` string decoding (`binary_reader`).
- `START.PRJ` project manifest parsing, including the trailing global-script block
  (`format`). Verified byte-exact against the real `START.PRJ`.
- Top-level `.BDF` page header parsing (`format`).
- OLE `CLSID`/GUID handling (`guid`).
- Page/group component-list walking (`component`): the container framing
  (per-item leading CLSID) plus the reflected `GraphBrdCntrItem` wrapper record
  (functions/properties). Verified against `RZECZKA.BDF`.
- A holder registry (`holders`) mapping the confirmed wrapper CLSIDs to their
  identity, implementing DLL, and serializer anchor, with a note on which
  component-private payloads this port can currently parse.
- Component-private state parsers for every holder used by the shipped title,
  including text/font, sound, bitmap, video, puzzle, recorder, and panorama.
- Page script text + script-engine parse cache parsing (`format`), so a full
  `.BDF` is read end-to-end: header, components, script, engine state, trailer.
- A runtime front-end (`runtime/lexer`, `runtime/script`): a cp1250-safe token
  scanner, plus `discoverHandlers` (page events, dotted component callbacks,
  user functions with params + body spans) and `collectCalls` (the builtin /
  component-method API surface). This is the first stage of the runtime; see
  `docs/runtime_recovery_notes.md` for the recovered engine internals and plan.
- An executable `Project` (`runtime/project`): runs the `START.PRJ` global-setup
  block (project-wide script variables), opens pages by name with those globals
  seeded and harvested back, and follows script-driven `LoadPage` navigation.
- A `gbinspect` command-line tool that prints parsed summaries as JSON, walking a
  page's components as far as the implemented private-state parsers allow and
  reporting each page's script handlers and API usage. It can also drive a page
  or a whole project headless with synthetic input (see Build, below).
- `.GRP` cursor/component parsing and a live second component namespace:
  `Group.*` methods, callbacks, hotspots, sprites, glides, sounds, rendering,
  and `CloseGroup` remain isolated from the page's own holders.
- A native Windows player, `gbgame`, with GDI scene output, embedded BoardVideo
  decoding, overlapping audio, input, timers, navigation, and the sliding
  `CURSORS.GRP` toolbar.
- A native Windows creator/editor, `gbeditor`, plus the headless `gbauthor`
  workflow. It creates `.gbproj` directories, imports legacy projects/boards,
  authors the recovered core holders, previews through the real reconstructed
  parser/runtime, validates scripts and compatibility constraints, and exports
  staged original-compatible `START.PRJ`, `.BDF`, and `.GRP` files. See
  `docs/editor_format_recovery.md` for the Ghidra-backed writer layouts.

GraphBoard Studio includes page/group dependency trees, shared and page-local
palette editing, WIC image import, retained RGBA artwork, WAV and Media
Foundation AVI preview, cursor and all recovered core-holder inspectors,
grid/snap, box selection, hotspot drawing/resize handles, item/component order,
visibility locks, undo/redo, strict diagnostics/navigation, autosave recovery,
and reconstructed-runtime Play mode. Play stages the complete current document,
so `OnOpenPage`, `.GRP` loading, project globals, input callbacks, and
`LoadPage` navigation use the same parsers/runtime as `gbgame`. See
`docs/editor_acceptance.md` for the synthetic and real-file verification matrix.

TextHolder content rendering/synchronization, exact palette fades, custom cursor
artwork, and several less-used component behaviors remain outside this native
checkpoint. Parsed TextHolder state is retained for later work.

## Real-File Regression Check

`tools/verify_scenes.ps1` runs `gbinspect` against the original game DATA and
asserts 123 recovered facts across `START.PRJ`, all seven shipped `.GRP` files,
and representative pages. Run it after parser or runtime changes:

```powershell
.\cpp_port\tools\verify_scenes.ps1
```

All GraphBoard strings are Windows-1250; `include/graphboard/text.hpp` converts
them to UTF-8 at the JSON boundary. `gbinspect` refuses files whose banner is
not `"YDP Board data file."` — the DATA folder contains misnamed non-BDF files
(e.g. `KOTEK2.BDF` is a 16-bit NE executable).

## Build

```powershell
cmake -S cpp_port -B cpp_port/build
cmake --build cpp_port/build
ctest --test-dir cpp_port/build -C Debug --output-on-failure
```

Launch GraphBoard Studio from a matching configuration directory:

```powershell
.\cpp_port\build\Release\gbeditor.exe
```

The editable project is a directory containing `project.gbproj`, content-
addressed `assets/`, and content-addressed `opaque/` preserved private/source
blocks. The GUI has File >
Import legacy and File > Export binaries. Equivalent headless commands are:

```powershell
.\cpp_port\build\Release\gbauthor.exe import "C:\path\to\START.PRJ" .\my-board
.\cpp_port\build\Release\gbauthor.exe validate .\my-board
.\cpp_port\build\Release\gbauthor.exe export .\my-board .\legacy-output
.\cpp_port\build\Release\gbauthor.exe acceptance .\acceptance-output
.\cpp_port\build\Release\gbauthor.exe verify-import "C:\path\to\START.PRJ"
```

Portable MinGW release build:

```powershell
cmake -S cpp_port -B cpp_port/build-mingw -G Ninja -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_CXX_COMPILER=C:/ProgramData/mingw64/mingw64/bin/g++.exe
cmake --build cpp_port/build-mingw
ctest --test-dir cpp_port/build-mingw --output-on-failure
```

Inspect files:

```powershell
.\cpp_port\build\gbinspect.exe "C:\path\to\START.PRJ"
.\cpp_port\build\gbinspect.exe "C:\path\to\MURZYNEK.BDF"
.\cpp_port\build\gbinspect.exe "C:\path\to\CURSORS.GRP"
```

Run the native player with an explicit manifest, or without arguments to choose
`START.PRJ` in a file dialog:

```powershell
.\cpp_port\build-mingw\gbgame.exe "C:\path\to\DATA\START.PRJ"
```

Pages and groups resolve case-insensitively beside the manifest. The executable
does not bundle or modify the original DATA assets.

Drive a page headless (opens `OnOpenPage`, then replays synthetic input and
prints the resulting state + host call log). Input events apply in order:

```powershell
.\cpp_port\build\gbinspect.exe "C:\path\to\RZECZKA.BDF" --click 481,280
.\cpp_port\build\gbinspect.exe "C:\path\to\RZECZKA.BDF" --move 100,100 --timer --key 27
```

Drive the whole project: runs the `START.PRJ` global-setup block, opens the
startup page (or `--page NAME`) with the project globals seeded, applies input,
and follows up to N `LoadPage` navigations:

```powershell
.\cpp_port\build\gbinspect.exe "C:\path\to\START.PRJ" --click 320,240 --follow 3
.\cpp_port\build\gbinspect.exe "C:\path\to\START.PRJ" --page michalp1.bdf --follow 0
```

## Porting Policy

- Prefer clear types and checked parsing over raw pointer arithmetic.
- Use existing working libraries for solved problems: JSON, image/audio codecs,
  windowing, compression, and tests are all fair game.
- Keep source offsets in parsed structures when they help compare against Ghidra.
- Treat serialized pointer-looking fields as file artifacts unless proven live.
- Add tests with synthetic byte streams before wiring larger recovered assets.
