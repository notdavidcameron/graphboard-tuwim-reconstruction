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
2. [TODO] `runtime/Value` — the variant record + coercion table from the
   recovered compare/assign functions.
3. [TODO] `runtime/ScriptInterpreter` — statement driver mirroring
   `InterpretStatements`/`ExecuteStatementOrBuiltin`; use the parsed
   switch-block cache exactly like the original (`ExecuteSwitchStatement`).
4. [PARTIAL] `runtime/script` — `discoverHandlers` finds every top-level
   function (page events, dotted component callbacks, user functions) with
   params and body spans; `collectCalls` finds every builtin/component call
   site. These give the interpreter its handler table and API surface. Still
   TODO: fold in components + engine state into an executable `Page` and a
   component registry keyed by displayName.
5. [TODO] Component behavior stubs — start with HotSpot (hit test is already
   recovered: `left<=x<=right, top<=y<bottom, enabled, layer`), Sound
   (embedded WAVs), MultiBitmap/Bitmap (blit), TVH (frame streams already
   decodable per the RLE reconstruction).
6. [TODO] Drive it headless first: load START.PRJ → run global setup → open
   intro.bdf → run OnOpenPage → feed synthetic clicks/timers; assert against
   script-visible state. Rendering can come later.

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

- `Recorder.dll`: locate/name the private-state serializer. cpp_port treats
  the block as `u32 version + 0x68 raw bytes` (empirically constant across
  ABEC_R/CUDA_R/DYZIO_R/FIG_R); confirming field semantics would firm it up.
- `MultiBmp.dll`: locate/name the serializer backing the (already verified)
  MultiBitmap layout.
- `VideoHolder.dll`, `PanoramaHolder.dll`, `Panorama.dll`: serializers — the
  last 10 unwalkable pages stop on these three holders.
- Script engine field semantics: engine `+0x30/+0x34/+0x38/+0x3c` (serialized
  parserState) and `+0x44..+0x58` (schema 2-4 scalar fields; RZECZKA values
  19598/0/19730/0 look like text offsets).
- Map `GraphBrdComponentCallback_InvokeScriptSlotXX` slot numbers to component
  event names (which slot is TVH end-of-playback, HotSpot click, etc.).
- Tuwim.exe builtin dispatch table used by `MatchTokenInTable`/
  `InvokeIndexedCall`: the token table address + kind enumeration would give
  the authoritative builtin list for the C++ runtime.
