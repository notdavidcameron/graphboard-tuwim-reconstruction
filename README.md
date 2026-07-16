# GraphBoard / Tuwim reconstruction

This repository reconstructs the GraphBoard runtime and the `.BDF` / `.GRP`
content from *Multimedialny świat Juliana Tuwima*. The main implementation is
the shared C++ runtime in [`cpp_port/`](cpp_port/), used by both the native
player and the active WebAssembly browser port.

The project is a preservation and research effort. The recovered game assets
are kept in the repository for local reconstruction; treat them appropriately
when sharing or redistributing the project.

## Current browser player

[`web_port_v2/`](web_port_v2/) is the active browser implementation. It loads
the C++ runtime through WebAssembly and provides:

- the 640×480 canvas renderer and pixel-art scaling;
- reconstructed page scripts, hotspots, sprites, groups, and animations;
- WebAudio playback, volume control, and external video playback;
- synchronized poem narration from the original `.EXS` / `.EXS_` RIFF/WAVE
  files, including scrolling text and line highlighting;
- recorder pages using the browser microphone API;
- lazy loading of page data so the initial page can start quickly.

The former JavaScript-only viewer is archived under
[`archive/web_port_legacy/`](archive/web_port_legacy/) and is read-only
recovery reference. It is not the implementation target.

## Run locally

The browser must be served over HTTP. Opening `index.html` directly as
`file://` prevents the browser from loading the WASM module and data files.

From the repository root:

```powershell
python -m http.server 8765
```

Then open:

```text
http://127.0.0.1:8765/web_port_v2/index.html
```

For QA, start on a particular page with a `?page=` query, for example:

```text
http://127.0.0.1:8765/web_port_v2/index.html?page=SLON
```

The browser player requires generated files under `web_port_v2/engine/` and
`web_port_v2/data/`. These directories are intentionally ignored by Git.

## Build the WebAssembly engine

An Emscripten-enabled CMake toolchain is required to rebuild the browser
engine:

```powershell
cmake --build cpp_port\build-wasm --target gbweb --parallel
Copy-Item cpp_port\build-wasm\gbweb.js web_port_v2\engine\gbweb.js -Force
Copy-Item cpp_port\build-wasm\gbweb.wasm web_port_v2\engine\gbweb.wasm -Force
```

The checked-in browser source is in `web_port_v2/src/`; the generated engine
files are local build artifacts.

## Package the original game data

To regenerate the browser data package from the original `DATA` directory, use
the packager below. FFmpeg is needed for the legacy Indeo AVI files and their
browser-compatible audio/video output.

```powershell
python .\tools\build_web_data.py `
  "C:\path\to\Tuwim\DATA" `
  .\web_port_v2\data\tuwim `
  --link `
  --ffmpeg "C:\path\to\ffmpeg.exe"
```

The packager includes external `.EXS` / `.EXS_` narration files. Although the
extension is not `.wav`, these files contain RIFF/WAVE PCM audio. Their
full-scale synchronization samples are consumed by the browser player for
line timing and concealed before playback to avoid audible marker pops.

## Native runtime and tests

Configure and build the native C++ targets:

```powershell
cmake -S cpp_port -B cpp_port\build
cmake --build cpp_port\build --config Release --parallel
ctest --test-dir cpp_port\build -C Release --output-on-failure
```

Useful tools include:

```powershell
.\cpp_port\build\Release\gbinspect.exe "C:\path\to\DATA\SLON.BDF"
.\tools\compile_reconstructions.ps1
```

`gbinspect` walks recovered component-private serializers and prints the
parsed page, component, text, sound, and script state. See
[`cpp_port/README.md`](cpp_port/README.md) for native player and headless-drive
details.

## Repository layout

- `cpp_port/` — parsers, runtime, renderer, native tools, tests, and the
  `gbweb` WebAssembly bridge.
- `web_port_v2/` — active browser entry point, device adapters, and local
  generated engine/data directories.
- `archive/web_port_legacy/` — obsolete JavaScript-only viewer kept for
  recovery comparison.
- `extracted_assets/` — canonical recovered images, audio, metadata, and
  embedded component assets.
- `graphboard_file_formats.md` — recovered `.PRJ`, `.BDF`, `.GRP`, component,
  and script-engine format notes.
- `cpp_port/reconstructed/` — readable source-style reconstructions based on
  the original executable and component DLLs.
- `ghidra_import/` — reference binaries, decompiler output, and targeted
  reverse-engineering notes.
- `gbtrace/` — tracing helpers used to compare behavior with the original
  Windows executable.

## Known limitations

This is an incremental reconstruction rather than a complete replacement for
the original Windows runtime. Some component-private formats and group
behaviors remain best-effort, and a few old indexed-animation transparency
cases can still vary by browser. The recorder and some rarely used pages also
depend on browser permissions and media support.

For implementation status and recovered serializer details, consult the
project notes in `AGENTS.md`, `graphboard_file_formats.md`, and
`cpp_port/docs/`.

## Source repository

The project is hosted at
[github.com/notdavidcameron/graphboard-tuwim-reconstruction](https://github.com/notdavidcameron/graphboard-tuwim-reconstruction).
