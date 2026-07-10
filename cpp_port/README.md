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
- A holder registry (`holders`) mapping the six confirmed wrapper CLSIDs to their
  identity, implementing DLL, and serializer anchor, with a note on which
  component-private payloads this port can currently parse.
- Component-private state parsers, each byte-exact vs `RZECZKA.BDF`:
  `HotSpot_Holder`, `Sprite_Holder`, `MultiBitmap`, `Transparent_Video_Holder`.
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

`Text_Holder` (plus its trailing FontControl font block) and `Sound_Holder` have
fully recovered, real-file-verified layouts and a step-by-step implementation
plan in `docs/holder_recovery_notes.md`. Six more holder CLSIDs found across the
DATA folder (`Bitmap_Holder`, `Video_Holder`, `Panorama_Holder`, `Panorama`,
`Puzzle`, `Recorder`) are registered but their private layouts are unrecovered.
Because a component's private block can only be skipped by fully parsing it, the
walker stops at the first component whose private state is not yet implemented
and reports how far it got.

## Real-File Regression Check

`tools/verify_scenes.ps1` runs `gbinspect` against the original game DATA and
asserts recovered ground truth for four files: `START.PRJ`, `RZECZKA.BDF`,
`GRZESIU.BDF` (TVH-first ordering, cp1250 sprite names), and `INTRO.BDF`
(stop-at-final-component edge). Run it after any parser change:

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
ctest --test-dir cpp_port/build
```

Inspect files:

```powershell
.\cpp_port\build\gbinspect.exe "C:\path\to\START.PRJ"
.\cpp_port\build\gbinspect.exe "C:\path\to\MURZYNEK.BDF"
```

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
