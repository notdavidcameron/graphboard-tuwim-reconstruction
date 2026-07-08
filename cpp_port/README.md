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
- The `HotSpot_Holder` component-private state parser (byte-exact vs `RZECZKA.BDF`).
- A `gbinspect` command-line tool that prints parsed summaries as JSON, walking a
  page's components as far as the implemented private-state parsers allow.

The remaining component-private payloads (TransparentVideoHolder, SpriteHolder,
Text_Holder, Sound_Holder, MultiBitmap) are documented in
`../graphboard_file_formats.md` and registered in `holders`; each should be added
as a focused parser module. Because a component's private block can only be
skipped by fully parsing it, the walker stops at the first component whose private
state is not yet implemented and reports how far it got.

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

## Porting Policy

- Prefer clear types and checked parsing over raw pointer arithmetic.
- Use existing working libraries for solved problems: JSON, image/audio codecs,
  windowing, compression, and tests are all fair game.
- Keep source offsets in parsed structures when they help compare against Ghidra.
- Treat serialized pointer-looking fields as file artifacts unless proven live.
- Add tests with synthetic byte streams before wiring larger recovered assets.
