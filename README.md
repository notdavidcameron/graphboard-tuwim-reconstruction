# GraphBoard / Tuwim Reconstruction

Reverse-engineering and reconstruction of the GraphBoard engine and its
`.BDF` / `.GRP` content from *Multimedialny świat Juliana Tuwima*. The primary
deliverable is a native C++ port (`cpp_port/`) that parses the real project
files and plays them back; an earlier static HTML scene viewer (`web_port/`)
remains as a secondary, browser-based reference.

## What Is Here

- **`cpp_port/`** — the active C++ reconstruction: a `gbinspect` CLI that
  parses and headlessly drives `START.PRJ` / `.BDF` / `.GRP` files, and
  `gbgame`, a native Windows player with GDI rendering, embedded BoardVideo
  decoding, audio, input, timers, page navigation, and the sliding
  `CURSORS.GRP` toolbar. See [`cpp_port/README.md`](cpp_port/README.md) for
  build instructions, current scope, and the real-file regression check.
- **`ghidra_import/`** — component DLL references/decompilation notes used
  during reverse engineering.
- **`graphboard_file_formats.md`** — recovered file-format structures and
  Ghidra findings that back both ports.
- **`extracted_assets/`** — recovered visual/audio assets, extracted by
  `graphboard_extract_assets.py`, used by the web viewer and by asset-adjacent
  tooling.
- **`gbtrace/`** — an INT3/VEH DLL injector for live-tracing the real
  `Tuwim.exe`, used to confirm serializer call sites and timing (e.g. glide
  speeds) against ground truth.
- **`web_port/`** — a no-build HTML/CSS/JS archive viewer for every exported
  BDF scene, GRP library, embedded asset, cursor, script, and loose WAV/AVI
  file. It includes a partial GraphBoard script runtime and is exported from
  `extracted_assets/` via `graphboard_export_scene.py`.

## C++ Port (Primary)

Build and test:

```powershell
cmake -S cpp_port -B cpp_port/build
cmake --build cpp_port/build
ctest --test-dir cpp_port/build -C Debug --output-on-failure
```

Run the real-file regression check (123 recovered facts against the original
game data):

```powershell
.\cpp_port\tools\verify_scenes.ps1
```

Inspect or headlessly drive a file, or launch the native player:

```powershell
.\cpp_port\build\gbinspect.exe "C:\path\to\START.PRJ"
.\cpp_port\build-mingw\gbgame.exe "C:\path\to\DATA\START.PRJ"
```

See [`cpp_port/README.md`](cpp_port/README.md) for full build options
(including the portable MinGW release build), current scope, known gaps, and
the headless-drive flags (`--click`, `--move`, `--follow`, etc.).

## Web Viewer (Secondary)

From the repository root:

```powershell
python -m http.server 8765
```

Then open `http://127.0.0.1:8765/web_port/index.html`. The archive browser
supports searchable BDF/GRP/media collections, per-component and per-asset
inspection, phase/frame previews, geometry overlays, zoom controls, audio
playback, raw JSON, and deep links such as `?scene=RADIO`, `?group=CURSORS`,
or `?media=audio-RADIO`. The BDF runtime supports common page calls
(`LoadGroup`, `LoadPage`, `SetCursor`, `SetTimer`, and
sound/video/bitmap/sprite/hotspot/text holder basics), but it is not a
faithful reimplementation — treat runtime behavior as a visual sanity check,
not ground truth.

Regenerate scenes after extracting new assets:

```powershell
python .\graphboard_export_scene.py --extracted .\extracted_assets --output .\web_port --project "C:\Users\Administrator\Desktop\Multimedialny świat Juliana Tuwima PL\Tuwim\DATA\START.PRJ"
```

## Compile Reconstruction Notes

The source-style C++ reconstructions under `cpp_port/reconstructed/` (direct,
readable translations of specific Ghidra decompiler output, distinct from the
higher-level `cpp_port` port above) are kept compile-checkable as object
files via the root `CMakeLists.txt`. With Chocolatey-installed `mingw`,
`cmake`, and `ninja`, run:

```powershell
.\tools\compile_reconstructions.ps1
```

## Notes

The repository includes recovered assets for local preservation/research.
Keep repository visibility and sharing choices appropriate for the source
material.
