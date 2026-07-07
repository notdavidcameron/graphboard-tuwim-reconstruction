# GraphBoard High-Level C++ Port

This branch is a readable C++ reconstruction of GraphBoard/Tuwim behavior. It is
not a direct paste of Ghidra decompiler output. Decompiled code and recovered
offsets are used as references, but the port is organized around domain concepts:
archives, projects, pages, components, assets, and runtime actions.

Dependency-free code is not a goal by itself. Prefer proven libraries when they
reduce risk or keep the port focused on recovered behavior instead of commodity
plumbing.

## Current Scope

- MFC `CArchive` string decoding.
- `START.PRJ` project manifest parsing.
- Top-level `.BDF` page header parsing.
- A small `gbinspect` command-line tool that prints parsed summaries as JSON.

Component-private payloads such as TransparentVideoHolder, BitmapHolder,
SpriteHolder, and SoundHolder should be added as focused modules as their layouts
become stable.

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
