# web_port runtime notes — reconciling with the recovered engine

These notes exist so the browser runtime can be checked against the
reverse-engineered ground truth without re-deriving it. The authoritative
recovery lives in the C++ port; this file summarizes the parts that bear on
`runtime.js` / `app.js` and flags where the two currently diverge.

**Source of truth (do not guess event names/args — look here):**

- `../cpp_port/docs/component_interfaces.md` — the complete, verified interface
  vocabulary for all 12 holder types, both directions:
  - **incoming** (`script → component`, the methods a page script *calls*),
    recovered from each DLL's embedded MSFT type library, and
  - **outgoing** (`component → script`, the events a component *fires* back),
    recovered from the host-cached event typeinfo in the `.BDF` wrapper, with
    exact argument signatures.
- `../cpp_port/docs/runtime_recovery_notes.md` — the engine lifecycle, the
  script-language shape, and the per-item status of the runtime port.
- `../cpp_port/src/runtime/page.cpp` — a verified reference implementation of the
  **component → script callback direction** for HotSpot_Holder, Sprite_Holder and
  Bitmap_Holder: click, right-click, hover, pixel-accurate hit-testing, sprite
  drag, and playback-completion (`Page::lButtonDown`/`lButtonUp`/`mouseMove`/
  `drag`/`videoEnd`/`soundEnd`/`animationEnd`). Unit-tested in
  `../cpp_port/tests/runtime_page.cpp` and regression-checked against real game
  files by `../cpp_port/tools/verify_scenes.ps1`. Drive it with
  `gbinspect PAGE.BDF --click X,Y | --drag X1,Y1,X2,Y2 | --video-end N | …`.

## The two-step input model (this is what the original board does)

A raw mouse/key event runs **two** things, in order:

1. The **page-level** script handler always runs if defined:
   `OnLButtonDown(x, y)`, `OnRButtonDown(x, y)`, `OnKeyDown(key)`, `OnTimer(id)`,
   `OnOpenPage()`, `OnClosePage()`. (Frequencies over the 191 scripted pages:
   `OnOpenPage`/`OnClosePage` 191, `OnTimer` 184, `OnLButtonDown` 184,
   `OnRButtonDown` 181, `OnKeyDown` 134.)
2. Then, as the board delivers the raw input to each component, the **hit
   component's own callback** fires — but only if the page script defines a
   handler for it.

The web_port already splits this correctly: `app.js` dispatches the page-level
handlers on the stage (`OnLButtonDown`/`OnRButtonDown`/`OnOpenPage`), and
`runtime.js` `bindRuntimeEvents` dispatches the per-component callbacks from DOM
events. Keep that split; the notes below are only about matching the exact
event names and firing rules of step 2.

## HotSpot_Holder callback direction (verified)

Events (from `component_interfaces.md`), all taking `(int rectID)` unless noted:

```
LeftButtonClickOn(int rectID)
RightButtonClickOn(int rectID)
DBLClickOn(int rectID)
LeftButtonClickOnUp(int rectID)
MouseMoveIn(int rectID)
MouseMoveOut(int rectID)
```

Firing rules the C++ reference implements (all read out of `HotSpotHolder.dll`
with Ghidra; see "How the board routes raw input" in `component_interfaces.md`):

- **`rectID` is the hotspot's stored id, not its array index.** The engine reads
  it from the record's `+0x18` field. RZECZKA.BDF stores ids `0,1,3,4,6,5,7,8`
  for array indices `0..7` — id `2` does not exist and `5`/`6` are transposed.
  This applies in both directions: the callback receives the id, and
  `EnableHotSpot(id)` / `DisableHotSpot(id)` match on it.

  > **Fixed in web_port v41.** `graphboard_export_scene.py` now emits the stored
  > `+0x18` id. RZECZKA exports `0,1,3,4,6,5,7,8`, matching the original
  > callback and enable/disable addressing.
- **Hit-test rule (click path):** a point hits when
  `left <= x <= right && top <= y <= bottom` — **inclusive on every edge** — and
  the hotspot is **enabled**. (The engine's separate *query* entry point,
  `IsYou`, uses a bottom-exclusive rect; don't mix them up.)
- **Layering:** the board dispatches mouse input one layer at a time, topmost
  layer first, and a holder only considers records on the layer being dispatched;
  within a layer, the **last** matching record wins an overlap. The net effect is
  "highest layer wins, later record breaks ties" — but it is a consequence of the
  board's per-layer walk, not a sort inside the holder.
- Left click over a hit hotspot → `HotSpot_Holder.LeftButtonClickOn(rectID)`.
- Right click over a hit hotspot → `HotSpot_Holder.RightButtonClickOn(rectID)`.
- Hover: `MouseMoveIn`/`MouseMoveOut` are **edge-triggered** — fire once when
  the pointer crosses into a hotspot and once when it leaves; do **not** re-fire
  while the pointer stays over the same item. (DOM `mouseenter`/`mouseleave`
  give this for free, which is why the web_port's approach is fine here.)
- Every callback fires **only if** the page script defines that top-level
  handler.

## Current web-port checkpoint (v48)

- The exporter now consumes the recovered SpriteHolder definition/instance
  split. It emits 919 placed instances across the corpus with their saved
  definition, position, layer, phase, and visibility. `MoveTo`/`GotoXY`
  preserves that visibility; `ShowSprite`/`HideSprite` remains authoritative.
- The recovered HotSpotHolder serializer now supplies 255 real hit rectangles,
  stored ids, layers, names, and enabled flags. Stored ids are intentionally
  not normalized or deduplicated: PIEKARZ, for example, has two records with
  id 0 and both are addressed by that script-visible id.
- Five Panorama/PanoramaHolder bitmap assets are exported. The browser can open,
  close, position, edge-pan, and run the recovered movement callbacks. Zoom and
  embedded panorama sprites remain visual stubs, and motion speed is still an
  approximation rather than recovered timing.
- Asset-less holder fallbacks use item id 0 rather than the wrapper/component
  index. Full-component fallback hitboxes are no longer added on top of real
  asset layers.
- Script handler indexing accepts GraphBoard's untyped functions while
  excluding nested control-flow blocks (`if`, `while`, `switch`, and friends).

## Remaining divergences in `runtime.js`

The generic event-routing issues formerly listed here were fixed in v41:
events are gated by holder type, MultiBitmap receives four arguments, and
hotspot left/up/right/double-click callbacks use their recovered names.

1. **Unimplemented-but-real events.** These are in the recovered vocabulary and
   may be referenced by scene scripts; grep the scenes before assuming they're
   unused: HotSpot `DBLClickOn` / `LeftButtonClickOnUp`; Sprite `MouseDrop`
   (5 args), `InPlace(id,x,y)`, `Crash(id,x,y,angle)`, `EndAnimation(id)`;
   Text `PlayMark(id,markNo)`, `EndOfSynchroText(id)`. Signatures are all in
   `component_interfaces.md`.

2. **Sprite animation timing.** v42 recovers each placed sprite's saved phase,
   layer and visibility, so hidden sprites no longer leak onto the page. The
   internal timing that advances an animated phase and fires `EndAnimation`
   is still not reconstructed (the C++ port doesn't model it either — see below).

## Now wired in the C++ reference (ground-truth impls to copy)

Everything below was recovered from the DLLs and implemented + verified in
`cpp_port/src/runtime/page.cpp` since these notes were first written. The
**cross-kind z-order question is answered**: the board dispatches raw mouse one
layer at a time, topmost first, each holder testing only items on that layer and
stopping at the first component that handles it. `Page::findHit` reproduces it by
scanning all holders and keeping the strictly-highest layer (earliest holder wins
a tie). Full per-holder table in `component_interfaces.md` ("How the board routes
raw input"). Key points for `runtime.js`:

- **Sprite_Holder**: click fires `MouseClickOnDown(index)` (instance index, no
  stored id), hover `MouseMoveIn/Out`, all-edges-inclusive rect. **No right-click
  event.**
- **Bitmap_Holder**: click fires `MouseClickOnDown(index)`, hover in/out; rect is
  `PtInRect` (**right/bottom exclusive**); **no visibility gate** and no
  right-click event.
- **Pixel-accurate hit-testing.** Irregular sprites/bitmaps refine a rect hit
  with a per-pixel transparency test — a click on a transparent pixel misses
  (confirmed in-game: transparent backgrounds do nothing). Sprite pixels:
  `blob+0xb8 + frame[0x48]`. **Bitmap pixels are at `blob+0x80`, NOT `+0x90`** —
  reading `+0x90` shears every row 16 bytes. The v48 asset extractor now uses
  the verified `+0x80` base (see `component_interfaces.md`).
- **Sprite drag** (`instance+0x1c == 1` → draggable; CUDA butterflies yes, DYZIO
  food no): down fires `MouseClickOnDown`, move follows at the grab offset with
  hover suppressed, up fires `MouseClickOnUp(id)` then
  `MouseDrop(id, left, top, right, bottom)` with the final frame rect.
- **Playback completion**: `Transparent_Video_Holder.TheEnd(id)`,
  `Sound_Holder.EndPlaySound(id)`, `Sprite_Holder.EndAnimation(id)` fire on the
  matching component when a clip ends. The C++ port takes the *event* explicitly
  and does **not** model the *timing* (frame duration / WAV length / phase count)
  — the web_port's `setTimeout` approximation (L1388/L1444) is still the only
  timing model on either side. Durations if needed: board-video
  `frameDurationMs`×`frameCount`, WAV length.
- **The hover caption** (bottom-center poem title) is just
  `Text_Holder.SetText(0, "<title>")` from the hover handler — e.g. WYBORW's
  `HotSpot_Holder.MouseMoveIn` switches rectID to a title string.

## Still unwired on both sides

- MultiBitmap click callbacks (only 1 page uses them): 4-arg
  `MouseClickOnDown(id, x, y, deep)` where x/y are the bitmap's own `left`/`top`,
  recovered in `MultiBmp_LButtonDown_HitTestAndFireClick` (@ 100047a5).
- Autonomous animation timing (the point above): what advances a phase and
  decides when a clip ends.
