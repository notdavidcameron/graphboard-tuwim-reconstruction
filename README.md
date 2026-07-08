# GraphBoard / Tuwim Reconstruction

Tools, notes, extracted assets, and a static HTML scene viewer for recovered GraphBoard `.BDF` / `.GRP` content from *Multimedialny swiat Juliana Tuwima*.

## What Is Here

- `graphboard_extract_assets.py` extracts backgrounds, scripts, audio, MultiBitmap frames, and custom TransparentVideoHolder board-video streams.
- `graphboard_export_scene.py` exports static browser scene JSON under `web_port/scenes`.
- `web_port/` is a no-build HTML/CSS/JS scene viewer with a partial GraphBoard runtime.
- `graphboard_file_formats.md` documents the recovered file-format structures and Ghidra findings.
- `extracted_assets/` contains the recovered visual/audio assets used by the viewer.
- `ghidra_import/` contains component DLL references/decompilation notes used during reverse engineering.
- `cpp_port/` contains the C++ decompilation/reconstruction work: `reconstructed/` holds the compile-checkable source-style serializer reconstructions.

## Current Runtime Coverage

The viewer reconstructs page backgrounds, image/GIF layers, recovered TransparentVideoHolder geometry, compact audio lists, script handler discovery, and a partial script runtime. The runtime currently supports common page calls such as:

- `LoadGroup`, `LoadPage`, `SetCursor`, `SetTimer`, `FadeScreen`
- `Sound_Holder.PlayDSound`, `Stop`, `StopAll`
- `Transparent_Video_Holder.Play`, `Stop`, `ResetVideo`, first/last frame show/hide, end callbacks
- `MultiBitmap.ShowBitmap` / `HideBitmap`
- `Sprite_Holder` movement/show/hide/timer stubs
- `HotSpot_Holder` enable/disable/click aliases
- `Text_Holder` show/hide/synchro stubs

This is a visual/runtime port, not yet a full faithful reimplementation of the original engine.

## Run Locally

From the repository root:

```powershell
python -m http.server 8765
```

Then open:

```text
http://127.0.0.1:8765/web_port/index.html
```

Example:

```text
http://127.0.0.1:8765/web_port/index.html?scene=RADIO
```

## Regenerate Scenes

```powershell
python .\graphboard_export_scene.py --extracted .\extracted_assets --output .\web_port --project "C:\Users\Administrator\Desktop\Multimedialny świat Juliana Tuwima PL\Tuwim\DATA\START.PRJ"
```

## Compile Reconstruction Notes

The source-style C++ reconstructions under `cpp_port/reconstructed/` are kept compile-checkable as object files. With Chocolatey-installed `mingw`, `cmake`, and `ninja`, run:

```powershell
.\tools\compile_reconstructions.ps1
```

## Notes

The repository includes recovered assets for local preservation/research. Keep repository visibility and sharing choices appropriate for the source material.
