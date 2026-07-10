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
- `../cpp_port/src/runtime/page.cpp` — a small, verified reference
  implementation of the **component → script callback direction** for
  `HotSpot_Holder` (see `Page::lButtonDown` / `rButtonDown` / `mouseMove`).
  Unit-tested in `../cpp_port/tests/runtime_page.cpp` (`testHotSpotCallbacks`)
  and regression-checked against real game files by
  `../cpp_port/tools/verify_scenes.ps1`.

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

Firing rules the C++ reference implements:

- **Hit-test rule:** a point hits a hotspot rect when
  `left <= x <= right && top <= y < bottom` (note: **inclusive** on left/right
  and top, **exclusive** on bottom), the hotspot is **enabled**, and among all
  containing enabled hotspots the **highest layer wins**.
- Left click over a hit hotspot → `HotSpot_Holder.LeftButtonClickOn(rectID)`.
- Right click over a hit hotspot → `HotSpot_Holder.RightButtonClickOn(rectID)`.
- Hover: `MouseMoveIn`/`MouseMoveOut` are **edge-triggered** — fire once when
  the pointer crosses into a hotspot and once when it leaves; do **not** re-fire
  while the pointer stays over the same item. (DOM `mouseenter`/`mouseleave`
  give this for free, which is why the web_port's approach is fine here.)
- Every callback fires **only if** the page script defines that top-level
  handler.

## Divergences worth checking in `runtime.js`

Flagged from a read of `bindRuntimeEvents` (~L219–252) and
`dispatchComponentEvent` (~L477); confirm before changing anything.

1. **`MouseClickOnDown` fired for every type on `mousedown` (L229).**
   That event name is real for `Sprite_Holder`, `Transparent_Video_Holder`,
   `MultiBitmap`, `Bitmap_Holder` — but **`HotSpot_Holder` has no
   `MouseClickOnDown` event**. Its click event is `LeftButtonClickOn` (correctly
   fired on `click` at L249). Firing `MouseClickOnDown` on a hotspot is a
   no-op only because no such handler is ever defined; it's still the wrong
   name. Consider gating the `mousedown → MouseClickOnDown` dispatch to the
   holder kinds that actually declare it.

2. **`MultiBitmap.MouseClickOnDown` has a 4-arg signature.**
   `MouseClickOnDown(int bitmapID, int x, int y, int deep)` — not the bare
   `(id)` the generic path passes. If any scene handles it, the extra args
   matter.

3. **Right-click component callbacks.** `app.js` dispatches the page-level
   `OnRButtonDown`, but `bindRuntimeEvents` has no `contextmenu`/right-button
   path, so `HotSpot_Holder.RightButtonClickOn` (and the sprite/video
   right-click analogs, where they exist) never fire from a component. Add a
   right-button branch if a scene needs it.

4. **Unimplemented-but-real events.** These are in the recovered vocabulary and
   may be referenced by scene scripts; grep the scenes before assuming they're
   unused: HotSpot `DBLClickOn` / `LeftButtonClickOnUp`; Sprite `MouseDrop`
   (5 args), `InPlace(id,x,y)`, `Crash(id,x,y,angle)`, `EndAnimation(id)`;
   Text `PlayMark(id,markNo)`, `EndOfSynchroText(id)`. Signatures are all in
   `component_interfaces.md`.

## Not yet wired in the C++ reference either (so no ground-truth impl to copy)

The C++ port only wires the HotSpot direction so far. For the others the
**names/signatures are recovered** (`component_interfaces.md`) but the firing
geometry is not yet settled:

- Sprite/Bitmap/MultiBitmap click & hover callbacks need per-item click rects
  and a verified **cross-kind z-order** rule (nothing yet establishes whether a
  hotspot or an overlapping sprite wins a click). The web_port sidesteps this
  with DOM stacking + `z-index`, which is a reasonable browser approximation but
  is not the recovered rule.
- Playback-completion events (`Transparent_Video_Holder.TheEnd(videoID)`,
  `Sound_Holder.EndPlaySound(soundID)`, `Text_Holder.EndOfSynchroText`) are
  timer/duration-driven, not hit-tested. The web_port approximates these with
  `setTimeout` (e.g. L1388/L1444); the real durations come from the asset
  headers (board-video `frameDurationMs`/`frameCount`, WAV length) if exact
  timing is ever needed.
