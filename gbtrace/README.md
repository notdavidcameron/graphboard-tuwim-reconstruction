# GraphBoard Runtime Trace Helper

`gbtrace` is developer-only tooling for comparing the C++ reconstruction with
the original 32-bit GraphBoard host. It injects `gbtrace.dll` into a locally
owned `Tuwim.exe` and records configured function-entry breakpoints.

Build from an x86 Visual C++ environment (or let `build.bat` locate one):

```bat
build.bat
```

Run with explicit paths; no machine-specific target is compiled in:

```bat
gblaunch.exe "C:\path\to\Tuwim.exe" "%CD%\gbtrace.dll" "C:\path\to\game"
```

Breakpoint addresses and argument counts live in `gbtrace.ini`. Generated
binaries, objects, and `gbtrace.log` are ignored and must not be copied into the
`gbgame` distribution. Use this only with the archival executable in this
workspace; it is not a general-purpose injector or an end-user dependency.
