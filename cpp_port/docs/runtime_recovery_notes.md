# GraphBoard Runtime Recovery Notes

Date: 2026-07-08. Read-only survey of `Tuwim.exe` in the Ghidra project (no
annotations were modified — the naming below is the renaming agent's work,
cited by address so either side can continue). Goal: recreate the GraphBoard
runtime in `cpp_port`.

## Big picture

The original runtime is an MFC doc/view app whose document owns a set of
helper objects (offsets recovered in `GraphBrdDoc_Construct`, 00405780):

```text
doc+0x9c  page script editor dialog     (script TEXT lives here)
doc+0xa0  group/global script editor    (START.PRJ global script)
doc+0xa4  page component list           (the wrappers cpp_port already parses)
doc+0xa8  group component list          (.GRP)
doc+0xb4  script engine                 (interpreter + parse cache)
doc+0xbc  audio manager                 (DirectSound; owns audioPresetIndex)
```

**The script engine is a direct tree-walking interpreter over the script
TEXT** — there is no bytecode. That is why every serialized "parse cache"
value (switch blocks, builtin call records) is a character offset into the
page script text. The engine re-parses statements as it executes; the cache
only accelerates `switch` dispatch and pre-resolves builtin call sites.
This is great news for the port: a faithful runtime is a text interpreter,
and `gbinspect` already extracts the exact script text of all 191 walkable
pages plus the global script.

## Script engine lifecycle (function names @ Tuwim.exe addresses)

1. Page load: `GraphBrdDoc_SerializePageData` (004066f0) reads components,
   script text, engine state; if the parsed flag (doc-owned) is clear it runs
   `GraphBrdScriptEngine_ParsePageScript` (00426300), which drives
   `GraphBrdScript_IndexSwitchCaseBlocks` (00426480),
   `GraphBrdScript_IndexBuiltInCalls` (00427dd0),
   `GraphBrdScript_ResolveComponentMemberReferences` (004269e0), and
   `GraphBrdScriptEngine_ValidatePageEventHandlers` (00427450).
2. Project load: the START.PRJ trailing script runs through
   `GraphBrdScript_RunGlobalSetupBlock` (004281b0) — global variables and
   shared handlers.
3. Event pump: window/timer events dispatch to named handlers found in the
   script text by name:
   - `GraphBrdScript_RunOnOpenPageHandler`     0041a610
   - `GraphBrdScript_RunOnClosePageHandler`    004102a0
   - `GraphBrdScript_RunOnTimerHandler`        004101d0
   - `GraphBrdScript_RunOnKeyDownHandler`      004102e0 / KeyUp 00410490
   - `GraphBrdScript_RunOnLButtonDownHandler`  00410640 / Up 00410840
   - `GraphBrdScript_RunOnRButtonDownHandler`  00410a40 / Up 00410c40
   - `GraphBrdScript_RunOnMouseMoveStopHandler` 00410270
     (signature checked by `GraphBrdScript_ValidateMouseMoveHandlerSignature`
     004271e0)
4. Statement execution core:
   - `GraphBrdScript_InterpretStatements`      00417640 (block driver)
   - `GraphBrdScript_ExecuteStatementOrBuiltin` 00410e40 (statement head)
   - `GraphBrdScript_ExecuteIfElseStatement`   00419160
   - `GraphBrdScript_ExecuteSwitchStatement`   00418380 (uses the cached index)
   - `GraphBrdScript_ExecuteWhileLoop`         00418bf0
   - `GraphBrdScript_HandleKeywordStatement`   00418800
   - `GraphBrdScript_TryDeclareVariableStatement` 00418130 /
     `GraphBrdScript_TryAssignmentStatement` 0041a5d0
   - `GraphBrdScript_CallUserFunction`         00416300 /
     `GraphBrdScript_RunScriptBlock` 004162b0
   - lexer helpers: `GraphBrdScript_SkipTriviaAndMeasureToken` 00417480,
     `GraphBrdScript_SkipLineOrBlockComment` 0040ffc0,
     `GraphBrdScript_MatchTokenInTable` 0040fa00
5. Value model: variant-like value records
   (`GraphBrdScript_CreateEmptyValueRecord` 00411fd0 /
   `GraphBrdScript_FreeValueRecord` 0040f860), coercing operators
   (`CompareEqualCoerced` 004209f0 and friends, `ApplyAssignmentOperator`
   0041d6e0, `AssignCoercedValue` 0041ea90), arrays
   (`GetArrayElementValuePtr` 0041e5c0), locals
   (`DeclareLocalVariable` 004169f0), expressions
   (`EvaluateExpression` 0041b330, `ParseAtomOrLiteralValue` 0041c7c0).
   Built-in value classes with methods:
   - `CString`-like:  `InvokeStringValueMember`  00414cc0
   - `CRect`-like:    `InvokeRectValueMember`    004151a0
   - storage object:  `InvokeStorageValueMember` 004143f0 — persistent
     variables backed by `GraphBrdScriptStorage_*` (0042c260..0042cd40:
     open/close backing file, create/delete section, read/write variable)
   - generic class:   `InvokeClassValueMember`   00413bc0
6. Built-in page/host calls (examples): `GraphBrdScript_BuiltinOpenGroup`
   00412950, `BuiltinPrintView` 00412310, `BuiltinRunCommand` 00412600,
   `BuiltinCallViewTwoOrThreeIntCommand` 00412010; the serialized builtin
   call records feed `GraphBrdScript_InvokeIndexedCall` 0040fa60.

## Component integration (both directions)

- **Script → component**: the reflected member records that cpp_port already
  parses from every wrapper (function/property tables) are the dispatch
  surface. `GraphBrdScript_InvokeComponentDispatchMethod` (00419950) resolves
  `Component_Name.Method(args)` script calls through those cached records to
  the component's IDispatch (`InvokeHelper` 0042e9d4). So the wrapper records
  in the .BDF are not just metadata — they ARE the runtime's linking table,
  and cpp_port already has them per page.
- **Component → script**: components fire events back into the page script
  via `GraphBrdCntrItem_GetCallbackInterface` (00429310) and the ~40
  `GraphBrdComponentCallback_InvokeScriptSlotXX` thunks (00429520..0042a630+),
  e.g. a TransparentVideoHolder end-of-playback callback or a HotSpot click
  runs the matching script handler. Slot semantics per component still need
  mapping (see "wanted next").

## Script language shape (from extracted real scripts)

C-like, cp1250 text: `//` comments, `int`/`CString` declarations, `if/else`,
`while`, `switch/case`, user functions, member calls like
`Sound_Holder.PlayDSound(3)`, page/host builtins like `LoadPage("radio.bdf")`,
`SetTimer`, `SetCursor`, `FadeScreen`. The web_port `runtime.js` implements a
partial JS interpretation of these builtins — its behavior notes transfer
directly to the C++ runtime, but the C++ interpreter should follow the
recovered engine functions, not the JS approximation.

## Proposed cpp_port runtime plan (status)

1. [DONE] `runtime/lexer` — token scanner (`tokenize`), cp1250-safe, `//` and
   `/* */`, two-char operators, tolerant of unterminated strings/comments.
   Faithful in output to `SkipTriviaAndMeasureToken`/`SkipLineOrBlockComment`;
   it is a plain scanner, not yet the offset-measuring variant the engine uses.
2. [DONE] `runtime/value` — an int/CString variant with the coercion rules the
   recovered compare/assign operators imply ('+' concatenates if either side is
   a string; comparisons are numeric only when both sides are ints; truthiness
   is `toInt()!=0`). CRect/storage/class value objects and floats are not yet
   modeled.
3. [DONE] `runtime/interpreter` — a tree-walking `Interpreter` over the tokens:
   declarations, assignment + compound assign, `++`/`--` (pre/postfix),
   `if/else`, `while`, `switch/case/default/break`, `return`, user-function
   calls, dotted (and chained `A.B.C`) component calls to the Host, undotted
   host builtins, and precedence-climbing expressions. A `Host` interface
   supplies `callBuiltin`/`callComponent`. Undotted name resolution is:
   user function if defined in this page, else host builtin (verified sound
   over all 191 pages). Known simplifications, each guarded/noted in code:
   no `&&`/`||` short-circuit, arrays are parsed-but-not-stored, switch runs
   without the offset cache. Loop guard 2M iterations, call-depth guard 256.
4. [DONE] `runtime/page` — an executable `Page` folds the parsed components +
   script into a live scene. It loads a `.BDF` (header → components → script),
   builds a component registry keyed by displayName, and implements the `Host`
   itself: `callComponent` dispatches to stateful `ComponentState` objects
   (index-addressed items + holder props) and `callBuiltin` updates page state
   (pendingPage, cursor, group, timer). Event entry points (`open`, `timer`,
   `lButtonDown`, ...) run the matching handler. Verified: all 201 scripted
   pages drive `OnOpenPage` through `Page` with no hang; `gbinspect --drive`
   dumps the resulting component/page state (INTRO: cursor 1, group cursors.grp,
   TVH+Sound playing clip 0).
5. [PARTIAL] Component behavior — the common script->component mutators are
   modeled with real state effects: Sprite/Bitmap show/hide/move/phase/anim,
   HotSpot enable/disable (+ the recovered hit test
   `left<=x<=right, top<=y<bottom, enabled, highest layer`, exposed as
   `Page::hitTestHotSpot`), Sound/TVH play/stop. Still TODO: the
   component->script callback direction (a hotspot click or video-end firing
   `Sprite_Holder.MouseClickOnDown`, `Transparent_Video_Holder.TheEnd`, ...),
   which needs the callback-slot → event-name mapping; and real asset effects
   (blitting frames, playing WAVs) for a rendered runtime.
6. [DONE (headless drive)] Two drive modes: `gbinspect --run <handler>` prints
   a raw call trace (recording Host); `gbinspect --drive <handler>` runs through
   `Page` and prints live component + page state. Every scripted page drives
   `OnOpenPage` with no hang (busiest ABECADLO = 89 calls: nested `while` loops
   with `++` counters, a user function, `if/else`, `Random`, arrays as no-ops).
   Next: feed synthetic clicks/timers that route through hotspot hit-testing to
   component callbacks (needs the callback-slot mapping), and START.PRJ
   global-setup before opening the first page.

**IMPORTANT (workflow):** always run `--run` under `timeout` and never in the
background — a script bug that stalls a loop counter makes the recording Host
grow without bound (a hung run reached 10 GB). See the
`gbinspect-run-timeout-hazard` memory.

## Recovered API surface (from `collectCalls` over all 191 scripted pages)

The interpreter must implement these. Frequencies are pages-that-use-each.

Page event handlers the host dispatches (verified present as top-level defs):
`OnOpenPage` (191), `OnClosePage` (191), `OnTimer` (184), `OnLButtonDown`
(184), `OnRButtonDown` (181), `OnKeyDown` (134), `OnKeyUp` (6),
`OnLButtonUp` (1). (`OnRButtonUp`, `OnMouseMove*`, `OnChar` are in the
host's lookup set but unused by this title.)

Host builtins actually called (English names; 17 of them):
`SetCursor` (191), `FadeScreen` (191), `LoadGroup` (191), `LoadPage` (190),
`SetTimer` (184), `DisableScreen`/`ShowBuffer`/`EnableScreen` (143 — the
double-buffer flush trio), `IsProject` (49), `CloseGroup` (31),
`SetTextMode` (30), `SetRect` (26), `Random` (7), `Debug` (3), `Exit` (1),
`SetHotspots` (1), `MoveSprite` (1).

CAVEAT — `collectCalls` cannot tell a host builtin from a page-local user
function; both are undotted calls. The Polish-named "builtins" in the raw
survey (`PobierzAktywnoscPrzycisku`, `UstawAktywnoscPanelu`,
`ZliczNagraneZwrotki`, `UstawAktywnoscStrony`, `Muzyka`, `MuzykaStop`,
`RozlozLitery`, `ResetMotyle`) are user functions — each also appears as a
top-level definition in the same page. The interpreter resolves an undotted
call as a user function first (if a matching `discoverHandlers` entry exists),
else a host builtin. That rule cleanly separates the two sets here.

Component methods called: 69 distinct across the title. Highest-frequency by
component (these are the IDispatch members to implement per holder):
- `Sprite_Holder`: `ShowBitmap`/`HideBitmap`, `MoveSprite`, `StartAnimation`/
  `StopAnimation`, plus the callbacks it fires (`MouseClickOnDown/Up`,
  `MouseMoveIn/Out`, `InPlace`, `EndAnimation`).
- `Puzzle`: `MouseStartDrag`/`MouseDrop`/`MouseMoveIn/Out`, `GameOver`.
- `Text_Holder`, `HotSpot_Holder` (`Enable`/`Disable`, `MouseMoveIn`),
  `Sound_Holder` (`PlayDSound`, `Stop`), `Transparent_Video_Holder`
  (`Play`, `TheEnd`), `Recorder`, `Bitmap_Holder`.
Note the direction split: methods like `Sprite_Holder.ShowBitmap` are
script→component (dispatch through the wrapper's reflected members, which
cpp_port already parses); methods like `Sprite_Holder.MouseClickOnDown`
appear as top-level *definitions* — those are component→script callbacks.

`web_port/runtime.js` already implements JS approximations of many builtins
(`LoadGroup`, `LoadPage`, `SetCursor`, `SetTimer`, `FadeScreen`,
`Sound_Holder.PlayDSound`, `Transparent_Video_Holder.Play`, MultiBitmap
show/hide) — useful behavior reference, but defer to the recovered engine
functions for exact semantics.

## Wanted from the Ghidra renaming agent (notes, no rush)

- [DONE 2026-07-09] `Recorder.dll`: private-state serializer named
  `Recorder_SerializePrivateState` at 10003ef0. It writes schema `1`,
  serializes `holder+0x34`, then transfers a 100-byte state block through
  `CArchive`. cpp_port still treats shipped title data as
  `u32 version + 0x68 raw bytes` until the old/new layout skew is reconciled.
- [DONE 2026-07-09] `MultiBmp.dll`: serializer was already named
  `MultiBmp_SerializePrivateState` at 10004353. It writes schema/count, each
  backing byte range, and a `0xc0` metadata record per bitmap.
- [DONE 2026-07-09] `VideoHolder.dll`: serializer named
  `VideoHolder_SerializePrivateState` at 10003e20. It writes schema `1`,
  holder entry count at `+0x118`, then each `0x6c` record plus `CString`.
- [DONE 2026-07-09] `PanoramaHolder.dll`: serializer named
  `PanoramaHolder_SerializePrivateState` at 10005e20. It writes schema `0`,
  scene count at `+0x40`, each `0x224` scene record, DIB payload bytes,
  variable subimage blocks, and `0x48` region/animation records. `Panorama.dll`
  already had `PanoramaState_Serialize` at 10004b6a.
- [DONE 2026-07-09] Script-engine event handler offsets at `+0x30..+0x58`
  are all page-script text offsets, confirmed through
  `GraphBrdScriptEngine_ValidatePageEventHandlers` (00427450),
  `GraphBrdScriptEngine_Serialize` (0041aad0), and the event runners:

  ```text
  +0x30 OnClosePage
  +0x34 OnOpenPage
  +0x38 OnTimer
  +0x3c OnKeyDown
  +0x40 OnKeyUp
  +0x44 OnLButtonDown
  +0x48 OnLButtonUp
  +0x4c OnRButtonDown
  +0x50 OnRButtonUp
  +0x54 OnMouseMoveStart
  +0x58 OnMouseMoveStop
  ```

  Schema 1 serializes `+0x38`, `+0x3c`, `+0x30`, `+0x34` in that disk order.
  Schema 2 adds `+0x44..+0x50`, schema 3 adds `+0x54`, and schema 4 adds
  `+0x58`. Key handlers receive one temporary argument; button handlers
  receive two integer `x`/`y` arguments. `OnOpenPage` and `OnClosePage` are
  required by validation.
- [PARTIAL 2026-07-09] Map `GraphBrdComponentCallback_InvokeScriptSlotXX` slot
  numbers to component event names. The host thunks at 00429520..0042aa90 are
  mechanically generated wrappers for slots 7..0x38; each only preserves MFC
  module state and calls `GraphBrdComponentCallback_RunScriptEvent(owner,
  slotId, rawArgs)`. `RunScriptEvent` resolves the callback record from
  `CntrItem + 0x20 + slotId*4`; that record provides the script offset and
  argument type bytes. Exact event names therefore need per-component callback
  table/typeinfo evidence rather than a host-side rename.
- [DONE 2026-07-09] Tuwim.exe builtin dispatch tables used by
  `MatchTokenInTable` / `InvokeIndexedCall` were labeled. Host builtin pointer
  tables exist twice with the same 24-entry order:
  `GraphBrdScript_HostBuiltinNameTable` at 0043b62c for
  `ExecuteStatementOrBuiltin`, and
  `GraphBrdScript_IndexedHostBuiltinNameTable` at 0043c470 for
  `IndexBuiltInCalls`. The order is `MessageBox`, `Random`, `FadeScreen`,
  `LoadPage`, `Exit`, `SetTimer`, `SetCursor`, `CreateDirectSound`,
  `ReleaseDirectSound`, `EnableScreen`, `DisableScreen`, `ShowBuffer`,
  `IsProject`, `Debug`, `LoadGroup`, `CloseGroup`, `SetDisplayMode`,
  `PrintBuffer`, `Execute`, `GetProgramPath`, `GetCommandLine`, `DeleteFile`,
  `ShellExecute`, `GetResourcePath`. Page-event lookup table
  `GraphBrdScript_PageEventNameTable` at 0043c444 has `OnTimer`, `OnKeyDown`,
  `OnKeyUp`, `OnLButtonDown`, `OnLButtonUp`, `OnRButtonDown`, `OnRButtonUp`,
  `OnClosePage`, `OnOpenPage`, `OnMouseMoveStart`, `OnMouseMoveStop`.
  `GraphBrdScript_ValueTypeNameTable` at 0043c3a0 has `char`, `int`, `long`,
  `float`, `unsigned`, `UINT`, `BYTE`, `BOOL`, `void`, `CString`, `CRect`,
  `CVarStorage`.
