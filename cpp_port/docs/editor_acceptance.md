# GraphBoard Studio acceptance evidence

The editor/writer acceptance path is deliberately independent of recovered
commercial artwork. `gbauthor acceptance <directory>` creates a synthetic
two-page project containing:

- shared `CURSORS.GRP` cursor group;
- indexed background artwork with retained RGBA sources;
- a two-frame sprite, hotspot, bitmap, and multibitmap;
- embedded RIFF/WAVE sound and an external AVI reference;
- global/page scripts, group loading, click callback, sound completion callback,
  and `LoadPage` navigation.

Run the complete headless workflow:

```powershell
cmake --build cpp_port/build --config Release
$out = Join-Path $env:TEMP graphboard-studio-acceptance
.\cpp_port\build\Release\gbauthor.exe acceptance $out
.\cpp_port\build\Release\gbauthor.exe validate (Join-Path $out project)
.\cpp_port\build\Release\gbinspect.exe `
  (Join-Path $out legacy\START.PRJ) --click 5,5 --follow 1
.\cpp_port\build\Release\gbgame.exe (Join-Path $out legacy\START.PRJ)
```

`graphboard_authoring_test` executes the same behavioral assertions in-process:
the startup page loads the cursor group, the hotspot dispatches
`Sound_Holder.PlayDSound`, `Sound_Holder.EndPlaySound` reaches the page callback,
and navigation opens `SECOND.BDF` while carrying the project global.

## Writer and preservation gates

The authoring test keeps assertions enabled in every CMake configuration and
covers:

- MFC `CString` length boundaries, GUID byte order, checked patch/fixed fields,
  strict CP1250 conversion, and Central-European case folding;
- deterministic quantization, transparency/alpha cutoff, bottom-up stride-4
  payloads, and retained RGBA project assets;
- every authorable holder, wrapper callback offsets, nested switch/case and
  builtin-call cache limits, and CP1250 byte offsets;
- schema-2 Sound absolute-offset relocation and Transparent Video
  `+0x558/+0x55c` relocation;
- manifest/BDF/GRP framing, original layer bounds, and zero document trailer;
- immutable unknown-CLSID passthrough, duplicate-holder locking, unchanged raw
  private payloads, content-addressed `assets/` and `opaque/`, and atomic save;
- project validation, palette conflicts, callback signatures, AVI packaging,
  full staged export, runtime group/click/sound/navigation behavior, and
  byte-identical unchanged files.

On the available original Tuwim DATA directory, the non-mutating checks are:

```powershell
.\cpp_port\build\Release\gbauthor.exe verify-import C:\path\to\DATA\START.PRJ
.\cpp_port\build\Release\gbauthor.exe validate-import C:\path\to\DATA\START.PRJ
.\cpp_port\tools\verify_scenes.ps1 `
  -GbInspect .\cpp_port\build\Release\gbinspect.exe
```

The July 2026 verification imported and preserved 209 valid PRJ/BDF/GRP files,
staged 237 files including external AVIs, reopened every generated board, and
matched all 237 source/output SHA-256 pairs. The existing real-scene verifier
passed all 123 assertions. Legacy diagnostics consist only of the known absent
`test.avi`, one missing page dependency, and four genuine cross-palette group
uses; they are warnings for unchanged passthrough content.

The remaining external acceptance boundary is a manual smoke test in the
original commercial executable. No such executable is available in the
workspace, so parser/runtime reopening, byte equality, and `gbgame` are the
automated compatibility authorities until it becomes available.

