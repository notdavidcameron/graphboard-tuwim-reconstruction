# GraphBoard direct C++ port

This branch is a seed for a direct C++ port of the Ghidra decompiler output.
It intentionally preserves the decompiler's structure, names, pointer arithmetic,
and MFC/Win32 call shapes before any higher-level cleanup.

The original program is C++ rather than plain C: the recovered code uses MFC
classes such as `CString`, `CArchive`, `COleDocument`, `CDialog`, vtables, and
component DLL classes. For that reason this branch uses `.cpp`.

## Layout

- `generated/tuwim_host_serializers.cpp` - direct Ghidra MCP export for the known
  `Tuwim.exe` document/script/container serializer spine.
- `generated/transparent_video_holder.cpp` - direct Ghidra MCP export for the
  currently recovered `TransparentVideoHolder.dll` functions, including
  `TVH_SerializeOrDeserialize`.
- `generated/extracted_graphboard_1.00_setcolor.dll.offset*.cpp` and
  `generated/extracted_graphboard_1.00_setcolor.dll/functions/*.cpp` - chunked
  direct export for `SetColor.dll`.
- `generated/extracted_graphboard_1.00_recorder.dll.offset*.cpp` and
  `generated/extracted_graphboard_1.00_recorder.dll/functions/*.cpp` - chunked
  direct export for `Recorder.dll`.
- `generated/extracted_graphboard_1.00_audioholder.dll.offset*.cpp` and
  `generated/extracted_graphboard_1.00_audioholder.dll/functions/*.cpp` -
  chunked direct export for `AudioHolder.dll`.
- `generated/*.manifest.json` - source program, address, and function metadata
  captured during export.
- `include/ghidra_compat.hpp` - minimal type shims for decompiler-style code.
- `tools/export_from_ghidra_mcp.ps1` - slow one-at-a-time exporter for the local
  bethington/ghidra-mcp HTTP server.
- `tools/export_program_functions_from_ghidra_mcp.ps1` - full per-program
  function exporter with checkpoints.

## Current Ghidra status

The focused exports for `Tuwim.exe`, `TransparentVideoHolder.dll`, `SetColor.dll`,
`Recorder.dll`, and `AudioHolder.dll` were captured from the local MCP server.
Broad open attempts previously hung on `/iscript.dll`, and the user later opened
the useful component programs manually. Prefer querying already-open programs and
exporting in small chunks with pauses between runs.

## Re-export

From the repository root:

```powershell
.\cpp_port\tools\export_from_ghidra_mcp.ps1 -OpenAllPrograms -OpenOnly -ResumeOpenLog -SkipKnownBadOpen -DismissAnalyzePrompt
```

The script opens one program at a time with `auto_analyze=false`, sleeps between
opens, logs to `generated/open_programs.jsonl`, and stops on the first timeout so
the MCP server is not flooded while it is stuck. `-DismissAnalyzePrompt` sends
Escape to Ghidra's `Analyze?` modal after each open; without that, the MCP thread
can block inside the auto-analysis prompt. Drop `-SkipKnownBadOpen` if you want
to try `/iscript.dll`; keep it last.

Export one whole program after it opens cleanly:

```powershell
.\cpp_port\tools\export_program_functions_from_ghidra_mcp.ps1 -Program "/GraphBoardComponents/TransparentVideoHolder.dll" -Resume
```

Export an already-open program in safe enhanced-search chunks:

```powershell
.\cpp_port\tools\export_program_functions_from_ghidra_mcp.ps1 -Program "/extracted/GraphBoard 1.00/SetColor.dll" -EnhancedSearch -Regex -NamePattern ".*" -StartOffset 0 -MaxFunctions 50 -PerFunctionFiles
```

For a focused serializer export:

```powershell
.\cpp_port\tools\export_from_ghidra_mcp.ps1
```
