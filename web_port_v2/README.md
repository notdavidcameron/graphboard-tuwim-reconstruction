# GraphBoard WebAssembly player

This is the active browser port. `cpp_port/tools/gbweb.cpp` compiles the shared
GraphBoard runtime to WebAssembly; the modules in `src/` provide browser
devices for canvas rendering, WebAudio, external VideoHolder media, input,
text, microphone recording, and lazy page loading. The animation clock follows
`requestAnimationFrame`; synchronized poem narration scrolls and highlights the
active line, while the original DirectSound attenuation calls drive WebAudio
gain (including the song-page cog control).

## Build

Configure `cpp_port/build-wasm` with Emscripten, then run:

```powershell
cmake --build cpp_port\build-wasm --target gbweb --parallel
Copy-Item cpp_port\build-wasm\gbweb.js web_port_v2\engine\gbweb.js -Force
Copy-Item cpp_port\build-wasm\gbweb.wasm web_port_v2\engine\gbweb.wasm -Force
```

Package the original game files. FFmpeg needs AVI demuxing plus Indeo 3 decode,
H.264 encode, and AAC encode support. The packager also includes the `.EXS`
RIFF/WAVE files used by synchronized poem narration. Tuwim and Brzechwa use
separate generated data roots while sharing the same browser shell and WASM
engine:

```powershell
python tools\build_web_data.py "C:\path\to\Tuwim\DATA" web_port_v2\data\tuwim --link --ffmpeg "C:\path\to\ffmpeg.exe"
python tools\build_web_data.py "C:\path\to\Brzechwa\DATA" web_port_v2\data\brzechwa --link --ffmpeg "C:\path\to\ffmpeg.exe"
```

Serve the repository root and open `/web_port_v2/index.html` for Tuwim or
`/web_port_v2/brzechwa.html` for Brzechwa. Use `?page=PAGE_NAME` on either URL
to start a specific page during QA.

Recorder (`*_R`) pages ask for microphone permission only when the user presses
their record control. Recordings are kept for the lifetime of the open page and
can be stopped, replayed, or cleared through the original GraphBoard controls.

Generated `engine/` and `data/` files are intentionally ignored by Git.
