# GraphBoard / Tuwim Reconstruction

Reverse-engineering and reconstruction of the GraphBoard engine and its
`.BDF` / `.GRP` content from *Multimedialny świat Juliana Tuwima*.

The primary implementation is the C++ runtime in `cpp_port/`. It powers both
the native Windows player and the active WebAssembly browser player in
`web_port_v2/`. The earlier JavaScript-only viewer is preserved under
`archive/web_port_legacy/` as a recovery reference.

## What Is Here

- **`cpp_port/`** — the active reconstruction: parsers, runtime, renderer,
  authoring tools, tests, the native `gbgame` player, and the `gbweb`
  WebAssembly bridge.
- **`web_port_v2/`** — the active browser player. It supplies canvas,
  WebAudio, external video, text, input, and lazy data loading around the same
  C++ runtime.
- **`archive/web_port_legacy/`** — the obsolete static scene viewer and its
  partial JavaScript runtime, kept only for asset/recovery reference.
- **`ghidra_import/`** — component DLL references and decompilation notes.
- **`graphboard_file_formats.md`** — recovered file-format structures and
  Ghidra findings.
- **`extracted_assets/`** — canonical recovered visual/audio assets and
  metadata used by recovery tooling.
- **`gbtrace/`** — the live-tracing injector used to confirm behavior against
  the original executable.

## Native C++ Port

Build and test:

```powershell
cmake -S cpp_port -B cpp_port/build
cmake --build cpp_port/build
ctest --test-dir cpp_port/build -C Debug --output-on-failure
```

Run the real-file regression checks or launch the native player:

```powershell
.\cpp_port\tools\verify_scenes.ps1
.\cpp_port\build\Debug\gbinspect.exe "C:\path\to\START.PRJ"
.\cpp_port\build-mingw\gbgame.exe "C:\path\to\DATA\START.PRJ"
```

See [`cpp_port/README.md`](cpp_port/README.md) for build variants, scope, and
headless drive options.

## WebAssembly Port

Build the `gbweb` Emscripten target and copy its generated module:

```powershell
cmake --build cpp_port\build-wasm --target gbweb --parallel
Copy-Item cpp_port\build-wasm\gbweb.js web_port_v2\engine\gbweb.js -Force
Copy-Item cpp_port\build-wasm\gbweb.wasm web_port_v2\engine\gbweb.wasm -Force
```

Package the original DATA folder. FFmpeg is required to transcode the legacy
Indeo AVIs to browser video plus WebAudio tracks:

```powershell
python .\tools\build_web_data.py "C:\path\to\Tuwim\DATA" .\web_port_v2\data\tuwim --link --ffmpeg "C:\path\to\ffmpeg.exe"
```

Serve the repository root:

```powershell
python -m http.server 8765
```

Open `http://127.0.0.1:8765/web_port_v2/index.html`. For QA, open a specific
page with `?page=TANIEC_T`; the `.BDF` suffix is optional.

The archived viewer remains available at
`/archive/web_port_legacy/index.html`, but it is no longer an implementation
target.

## Compile Reconstruction Notes

The readable source-style reconstructions under `cpp_port/reconstructed/` are
kept compile-checkable through the root CMake project:

```powershell
.\tools\compile_reconstructions.ps1
```

## Notes

The repository includes recovered assets for local preservation and research.
Keep repository visibility and sharing choices appropriate for the source
material.
