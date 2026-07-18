# GraphBoard reconstruction

This repository is a preservation and research reconstruction of the GraphBoard
runtime and the `.BDF` / `.GRP` content used by *Multimedialny świat Juliana
Tuwima*. Its shared C++ runtime powers a native Windows player, authoring tools,
and the active WebAssembly browser player.

Recovered game assets are included for local research and reconstruction. Treat
them appropriately when sharing or redistributing this project; do not make the
repository public without permission from the rights holders.

## What is here

- A C++ parser and runtime for GraphBoard projects, pages, groups, components,
  scripts, embedded BoardVideo animations, audio, input, timers, and navigation.
- `gbgame`, a native Windows player for original `START.PRJ` projects.
- GraphBoard Studio (`gbeditor`) and `gbauthor`, which import, inspect, edit,
  validate, and export compatible GraphBoard projects.
- A WebAssembly player that runs the same C++ runtime in a browser.
- Recovered format notes, source-style serializer reconstructions, regression
  checks, and canonical extracted assets.

The browser runtime also supports *Multimedialny świat Jana Brzechwy* with a
separate locally generated data package.

## Run the browser player

The active browser implementation is [`web_port_v2/`](web_port_v2/). It uses
the C++ runtime compiled to WebAssembly and provides canvas rendering, page and
group behavior, audio/video playback, synchronized poem narration, recorder
pages, and lazy page loading.

Serve the repository over HTTP; opening the HTML files directly with `file://`
prevents the browser from loading the WASM module and game data:

```powershell
python -m http.server 8765
```

Open one of these URLs:

- Tuwim: <http://127.0.0.1:8765/web_port_v2/index.html>
- Brzechwa: <http://127.0.0.1:8765/web_port_v2/brzechwa.html>

For QA, begin on a page by adding `?page=PAGE_NAME`, for example:

```text
http://127.0.0.1:8765/web_port_v2/index.html?page=SLON
```

The generated `web_port_v2/engine/` and `web_port_v2/data/` directories are
local build artifacts and are intentionally ignored by Git. See
[`web_port_v2/README.md`](web_port_v2/README.md) for browser-specific details.

### Build the WebAssembly engine

Configure `cpp_port/build-wasm` with an Emscripten CMake toolchain, then run:

```powershell
cmake --build cpp_port\build-wasm --target gbweb --parallel
Copy-Item cpp_port\build-wasm\gbweb.js web_port_v2\engine\gbweb.js -Force
Copy-Item cpp_port\build-wasm\gbweb.wasm web_port_v2\engine\gbweb.wasm -Force
```

### Package game data

Use the packager to build a browser data root from an original `DATA` directory.
FFmpeg is required to convert the legacy Indeo AVI media for browsers.

```powershell
python .\tools\build_web_data.py `
  "C:\path\to\Tuwim\DATA" `
  .\web_port_v2\data\tuwim `
  --link `
  --ffmpeg "C:\path\to\ffmpeg.exe"
```

Package Brzechwa separately while reusing the same engine:

```powershell
python .\tools\build_web_data.py `
  "C:\path\to\Brzechwa\DATA" `
  .\web_port_v2\data\brzechwa `
  --link `
  --ffmpeg "C:\path\to\ffmpeg.exe"
```

The packager preserves the original `.EXS` / `.EXS_` narration resources. Their
extension is nonstandard, but they are RIFF/WAVE PCM files used for narration
and line synchronization.

## Build and use the native tools

Configure, build, and test the C++ port:

```powershell
cmake -S cpp_port -B cpp_port\build
cmake --build cpp_port\build --config Release --parallel
ctest --test-dir cpp_port\build -C Release --output-on-failure
```

Run an original project with the native player:

```powershell
.\cpp_port\build\Release\gbgame.exe "C:\path\to\DATA\START.PRJ"
```

Launch the editor, or use the headless authoring workflow:

```powershell
.\cpp_port\build\Release\gbeditor.exe
.\cpp_port\build\Release\gbauthor.exe import "C:\path\to\DATA\START.PRJ" .\my-board
.\cpp_port\build\Release\gbauthor.exe validate .\my-board
.\cpp_port\build\Release\gbauthor.exe export .\my-board .\legacy-output
```

Inspect legacy files or replay synthetic input from the command line:

```powershell
.\cpp_port\build\Release\gbinspect.exe "C:\path\to\DATA\SLON.BDF"
.\cpp_port\build\Release\gbinspect.exe "C:\path\to\DATA\START.PRJ" --click 320,240 --follow 3
```

See [`cpp_port/README.md`](cpp_port/README.md) for the full native runtime,
editor, compatibility, and headless-drive documentation.

## Repository layout

- `cpp_port/` — shared parsers, runtime, renderer, native applications, tests,
  and the `gbweb` WebAssembly bridge.
- `web_port_v2/` — active browser shell and device adapters.
- `archive/web_port_legacy/` — obsolete JavaScript-only viewer, retained as
  read-only recovery reference.
- `extracted_assets/` — canonical recovered image, audio, metadata, and
  embedded component assets.
- `graphboard_file_formats.md` — recovered `.PRJ`, `.BDF`, `.GRP`, component,
  and script-engine format notes.
- `cpp_port/reconstructed/` — readable source-style serializer
  reconstructions based on the original executable and component DLLs.
- `cpp_port/docs/` — runtime, editor, and verification documentation.
- `ghidra_import/` — reference binaries, decompiler output, and targeted
  reverse-engineering notes.
- `tools/` — data packaging and verification helpers.

## Verification

After parser or runtime work, run the real-file regression check against a
local original Tuwim `DATA` directory:

```powershell
.\cpp_port\tools\verify_scenes.ps1
```

For an external Brzechwa installation, use the parameterized compatibility
matrix:

```powershell
.\cpp_port\tools\verify_brzechwa.ps1 `
  -DataDir "C:\path\to\Multimedialny Swiat Jana Brzechwy\DATA"
```

## Current limitations

The reconstruction is incremental, not yet a drop-in replacement for every
behavior of the original Windows runtime. Some component-private formats,
palette fades, exact TextHolder synchronization, and rare indexed-animation
transparency cases remain best-effort. Recorder pages also depend on browser
microphone permissions and media support.

## Source repository

<https://github.com/notdavidcameron/graphboard-tuwim-reconstruction>
