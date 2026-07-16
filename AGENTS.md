# AGENTS.md

## Project Snapshot

Repository: `notdavidcameron/graphboard-tuwim-reconstruction`

Workspace root:

```text
C:\Users\Administrator\Documents\GRAPHBOARD
```

This project reconstructs GraphBoard `.BDF` / `.GRP` files from *Multimedialny swiat Juliana Tuwima* and provides native and WebAssembly players powered by the shared C++ runtime.

## Active Web Port

`web_port_v2/` is the active browser implementation. It runs the shared C++
runtime through WebAssembly. The former JavaScript-only `web_port/` has moved to
`archive/web_port_legacy/` and must be treated as read-only recovery reference.
Any later references in this handoff to the old static port describe historical
state, not the current implementation target.

The GitHub repository was created as private and pushed on `main`.

Current pushed commit:

```text
64798c0 Initial GraphBoard reconstruction and web port
```

## Important Files

- `README.md` - user-facing overview and local run instructions.
- `graphboard_file_formats.md` - recovered file-format notes, including Ghidra findings.
- `graphboard_extract_assets.py` - asset extraction pipeline.
- `cpp_port/tools/gbweb.cpp` - active WebAssembly bridge.
- `web_port_v2/index.html` - active browser player entry point.
- `web_port_v2/src/` - browser audio, video, input, loading, and canvas integration.
- `graphboard_export_scene.py` - legacy exporter retained for the archived static viewer.
- `archive/web_port_legacy/` - obsolete JavaScript-only viewer, kept as recovery reference.
- `extracted_assets/` - canonical recovered assets used by the viewer.
- `ghidra_import/TransparentVideoHolder_decomp.txt` - decompiled TVH notes used for geometry recovery.
- `cpp_port/reconstructed/GraphBoardDocument_reconstructed.cpp` - source-style reconstruction of the `Tuwim.exe` host serializers (`START.PRJ`, `.BDF`, `.GRP`, script-text block, component wrapper, script engine state).
- `cpp_port/reconstructed/ComponentHolders_reconstructed.cpp` - source-style reconstruction of the `SpriteHolder.dll` and `HotSpotHolder.dll` component-private serializers.

Ignored duplicate/generated collection folders:

```text
extracted_assets/all_images/
extracted_assets/all_images_jpeg/
```

Canonical assets remain committed under:

```text
extracted_assets/bdf/
extracted_assets/direct/
extracted_assets/grp/
```

## Archived HTML Port Historical State

The commands and implementation notes in this section describe the archived
JavaScript-only viewer. Do not use them as the active browser implementation.

Run locally:

```powershell
python -m http.server 8765
```

Open:

```text
http://127.0.0.1:8765/web_port/index.html
```

Example:

```text
http://127.0.0.1:8765/web_port/index.html?scene=RADIO
```

Current cache-bust in `web_port/index.html` is:

```text
styles.css?v=11
runtime.js?v=11
app.js?v=11
```

## Major Findings Already Implemented

`TransparentVideoHolder.dll` was inspected through the Ghidra MCP HTTP server at:

```text
http://127.0.0.1:8089
```

The useful manually-defined function was:

```text
TransparentVideoHolder.dll:10007af0
TVH_SerializeOrDeserialize
```

TransparentVideoHolder private block layout:

```text
u32 version
u32 entryCount
repeat entryCount:
  u32 boardVideoByteSize
  u8 boardVideoBytes[boardVideoByteSize]
  u8 holderEntry[0x568]
  u8 stillFrame[((width * 8 + 31) >> 3 & ~3) * height]
```

Important holder-entry offsets:

```text
+0x080 u32 width
+0x084 u32 height
+0x508 i32 originalX
+0x50c i32 originalY
+0x510 i32 originalZ
+0x54c i32 current/stage X
+0x550 i32 current/stage Y
+0x554 i32 current/stage Z
+0x558 i32 backing video start offset
+0x55c i32 backing video end offset
```

This is implemented in `graphboard_export_scene.py` as per-GIF rect/z recovery.

Current recovered TVH geometry count from the last full export:

```text
221 TransparentVideoHolder asset rects across 22 pages
```

RADIO specifically:

```text
53 TransparentVideoHolder GIF layers
53 serialized rects recovered
```

## Current Runtime Coverage

`web_port/runtime.js` supports a partial C-like script runtime:

- function body extraction and handler execution
- `if` / `else`
- `while`
- `switch`
- scalar and simple array variables such as `parki[1]`
- basic expressions/comparisons
- `LoadGroup`, `LoadPage`, `SetCursor`, `SetTimer`, `FadeScreen`
- `Sound_Holder.PlayDSound`, `Stop`, `StopAll`
- `Transparent_Video_Holder.Play`, `Stop`, `ResetVideo`, `ShowFirsLastVideoFrame`, `HideFirsLastVideoFrame`
- video end timers that call `Transparent_Video_Holder.TheEnd(id)`
- GIF restart/freeze-to-still behavior for played TVH layers
- `MultiBitmap.ShowBitmap` / `HideBitmap`
- `Sprite_Holder` movement/show/hide/timer stubs
- `HotSpot_Holder` enable/disable/click aliases
- `Text_Holder` show/hide/synchro stubs

Verified in the in-app browser before this handoff:

- RADIO starts with 27 visible base layers, not all 53.
- Clicking RADIO video `1` ran `Transparent_Video_Holder.MouseClickOnDown(1)`.
- `Transparent_Video_Holder.Play(1)` scheduled and fired `Transparent_Video_Holder.TheEnd(1)`.

Browser wrapper caveat:

- The in-app browser control could inspect and click via locator API.
- It did not expose a working reload/navigation helper.
- User may need to manually refresh after cache-bust changes.

## Known Limitations / Next Work

1. Group runtime is not reconstructed yet.
   - Scripts call `LoadGroup("Cursors.grp")`, `Group.Sprite_Holder.*`, `Group.HotSpot_Holder.*`, and `Group.Sound_Holder.*`.
   - Current runtime logs or stubs many group operations.
   - `extracted_assets/grp/*/metadata.json` exists, but only cursor-like GRP data is currently parsed.

2. TextHolder geometry/content is still weak.
   - `Text_Holder` calls are mostly runtime stubs.
   - Text display, synchronized text/audio, and scroll behavior are not visually reconstructed.

3. SpriteHolder private serialization has not been recovered.
   - Ghidra has `SpriteHolder.dll` available in `ghidra_import/`.
   - Useful next target if toolbar/group UI reconstruction matters.

4. HotSpotHolder private serialization has not been recovered.
   - Current hitboxes are only best-effort component boxes.
   - Real hotspot rects would improve click areas.

5. Some script parser features are still incomplete.
   - It is intentionally small and permissive, not a full C interpreter.
   - If a page behaves oddly, inspect the script and add only the needed language feature.

6. GIF reconstruction is usable but not perfect.
   - BoardVideo RLE decoding works well enough for many pages.
   - Some animations may still show artifacts or imperfect transparency.

## Useful Commands

Regenerate scenes:

```powershell
python .\graphboard_export_scene.py --extracted .\extracted_assets --output .\web_port --project "C:\Users\Administrator\Desktop\Multimedialny świat Juliana Tuwima PL\Tuwim\DATA\START.PRJ"
```

Validate syntax:

```powershell
node --check .\web_port\app.js
node --check .\web_port\runtime.js
python -m py_compile .\graphboard_export_scene.py
```

Git status:

```powershell
git status --short --branch
```

Push:

```powershell
git push
```

## Guidance For The Next Agent

- Keep `Tuwim.exe` and the component DLLs as reverse-engineering sources, but do not broadly explore DLLs unless needed.
- `Tuwim.exe` remains the host serializer authority.
- Use Ghidra MCP only for targeted confirmation of component-private structures.
- Prefer improving exported scene JSON and the static runtime incrementally.
- Be careful with asset folders: keep canonical extracted assets; avoid committing duplicate converted collection folders.
- Do not make the repo public without explicit user confirmation because it contains recovered commercial/archival assets.
