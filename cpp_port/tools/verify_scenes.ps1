# Real-file regression check for the cpp_port GraphBoard parsers.
#
# Runs gbinspect against the original Tuwim game data and asserts recovered
# ground-truth values for several scenes. Extend the scene table whenever a new
# private-state parser lands or a new page's values are verified by hand.
#
# Usage:
#   .\cpp_port\tools\verify_scenes.ps1 [-DataDir <path to Tuwim DATA>] [-GbInspect <path to gbinspect.exe>]

param(
    [string]$DataDir = "",
    [string]$GbInspect = ""
)

$ErrorActionPreference = "Stop"
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# gbinspect emits UTF-8 (cp1250 source is converted). Decode its stdout as UTF-8
# so cp1250-derived names (e.g. "uśmiech") round-trip through ConvertFrom-Json
# regardless of the host console codepage.
try { [Console]::OutputEncoding = [System.Text.Encoding]::UTF8 } catch {}

# gbinspect is built with MinGW and needs its runtime DLLs on PATH.
$extraPath = @(
    "C:\ProgramData\mingw64\mingw64\bin",
    "G:\Tools\mingw-build\mingw64\bin"
) | Where-Object { Test-Path $_ }
if ($extraPath.Count -gt 0) {
    $env:PATH = ($extraPath -join ";") + ";" + $env:PATH
}

# Default DATA location: resolved by wildcard so this script stays ASCII-only
# (the real folder name contains a Polish 's with acute').
if (-not $DataDir) {
    $resolved = Resolve-Path "C:\Users\Administrator\Desktop\Multimedialny *wiat Juliana Tuwima PL\Tuwim\DATA" -ErrorAction SilentlyContinue |
        Select-Object -First 1
    if ($resolved) { $DataDir = $resolved.Path }
}
if (-not $GbInspect) {
    $GbInspect = Join-Path $scriptDir "..\build\cmake\gbinspect.exe"
}
if (-not (Test-Path $GbInspect)) {
    throw "gbinspect not found at $GbInspect - build cpp_port first (cmake --build cpp_port/build/cmake)"
}
if (-not (Test-Path $DataDir)) {
    throw "DATA folder not found at $DataDir"
}

$script:failures = 0
$script:checks = 0

function Assert-Eq($actual, $expected, $label) {
    $script:checks++
    $a = "$actual"; $e = "$expected"
    if ($a -ne $e) {
        Write-Output "FAIL  $label`: expected '$e', got '$a'"
        $script:failures++
    }
}

function Get-Scene($name) {
    $raw = & $GbInspect (Join-Path $DataDir $name) 2>&1
    if ($LASTEXITCODE -ne 0) { throw "gbinspect failed on $name`: $raw" }
    return $raw | ConvertFrom-Json
}

function Invoke-Drive($name, $handler) {
    $raw = & $GbInspect (Join-Path $DataDir $name) "--drive" $handler 2>&1
    if ($LASTEXITCODE -ne 0) { throw "gbinspect --drive failed on $name/$handler`: $raw" }
    return $raw | ConvertFrom-Json
}

# ---------------------------------------------------------------- START.PRJ
$prj = Get-Scene "START.PRJ"
Assert-Eq $prj.version 1 "START.PRJ version"
Assert-Eq $prj.startupPage "intro.bdf" "START.PRJ startupPage"
Assert-Eq $prj.pageCount 34 "START.PRJ pageCount"
Assert-Eq $prj.groupCount 6 "START.PRJ groupCount"
Assert-Eq $prj.decodedSignature "Julian Tuwim" "START.PRJ signature"
Assert-Eq $prj.globalScriptVersion 1 "START.PRJ globalScriptVersion"
Assert-Eq $prj.globalScriptByteCount 332 "START.PRJ globalScript bytes"

# --------------------------------------------------------------- RZECZKA.BDF
# All six components parse; the walk continues into the page script text and
# the script-engine parse cache, ending 4 bytes before EOF (COleDocument tail).
$rz = Get-Scene "RZECZKA.BDF"
Assert-Eq $rz.header.componentListOffset 1165 "RZECZKA componentListOffset"
Assert-Eq ($rz.header.pageRect -join ',') "0,0,640,480" "RZECZKA pageRect"
Assert-Eq $rz.componentList.count 6 "RZECZKA component count"
Assert-Eq $rz.componentList.componentsParsed 6 "RZECZKA componentsParsed"
Assert-Eq (($rz.componentList.components | ForEach-Object holderKind) -join ',') `
    "HotSpot_Holder,Transparent_Video_Holder,Sprite_Holder,Text_Holder,Sound_Holder,MultiBitmap" "RZECZKA component order"
$hs = $rz.componentList.components[0].privateState
Assert-Eq $hs.hotspotCount 8 "RZECZKA hotspot count"
Assert-Eq ($hs.hotspots[0].rect -join ',') "589,1,639,476" "RZECZKA hotspot0 rect"
$tvh = $rz.componentList.components[1].privateState
Assert-Eq $tvh.entryCount 8 "RZECZKA TVH entries"
Assert-Eq ($tvh.entries.frameCount -join ',') "24,50,23,11,80,77,86,73" "RZECZKA TVH frame counts"
Assert-Eq (($tvh.entries.audioSampleRate | Select-Object -Unique) -join ',') "22050" "RZECZKA TVH audio rate"
Assert-Eq $tvh.entries[0].streamOffset 4291 "RZECZKA TVH stream0 offset (0x10c3)"
$sp = $rz.componentList.components[2].privateState
Assert-Eq $sp.definitionCount 5 "RZECZKA sprite definitions"
Assert-Eq $sp.instanceCount 5 "RZECZKA sprite instances"
Assert-Eq $sp.definitions[0].name "rzeczka" "RZECZKA sprite0 name"
Assert-Eq $sp.definitions[0].width 1280 "RZECZKA sprite0 width"
Assert-Eq $sp.definitions[3].name "migotanie" "RZECZKA sprite3 name"
Assert-Eq (($sp.instances | ForEach-Object definitionIndex) -join ',') "0,1,2,3,4" "RZECZKA sprite instance indices"
$tx = $rz.componentList.components[3].privateState
Assert-Eq $tx.version 2 "RZECZKA Text version"
Assert-Eq ($tx.colors -join ',') "23,187,55,0" "RZECZKA Text colors"
Assert-Eq $tx.entries[0].secondaryText "rzeczka.exs" "RZECZKA Text synchro ref"
Assert-Eq $tx.entries[0].lineCount 13 "RZECZKA Text line count"
Assert-Eq (($tx.fontSlots | ForEach-Object height) -join ',') "19,21" "RZECZKA font slot heights"
$snd = $rz.componentList.components[4].privateState
Assert-Eq $snd.soundCount 14 "RZECZKA sound count"
Assert-Eq $snd.sounds[0].name "rzeczka -" "RZECZKA sound0 name"
Assert-Eq (($snd.sounds.looksLikeRiff | Select-Object -Unique) -join ',') "True" "RZECZKA sounds all RIFF"
$mb = $rz.componentList.components[5].privateState
Assert-Eq $mb.recordCount 20 "RZECZKA MultiBitmap records"
# page script + engine state (offsets are the documented 0xdf6715 / 0xdfb493)
Assert-Eq $rz.pageScript.offset 14640917 "RZECZKA pageScript offset"
Assert-Eq $rz.pageScript.textByteCount 19831 "RZECZKA pageScript bytes"
Assert-Eq $rz.scriptEngineState.offset 14660755 "RZECZKA engine offset"
Assert-Eq $rz.scriptEngineState.schemaVersion 2 "RZECZKA engine schema"
Assert-Eq $rz.scriptEngineState.switchBlockCount 11 "RZECZKA engine switch blocks"
Assert-Eq $rz.scriptEngineState.builtinCallCount 1 "RZECZKA engine builtin calls"
Assert-Eq $rz.trailingByteCount 4 "RZECZKA trailing bytes"

# --------------------------------------------------------------- GRZESIU.BDF
# TVH-first ordering, cp1250 sprite names, Bitmap_Holder with one bitmap.
$gr = Get-Scene "GRZESIU.BDF"
Assert-Eq $gr.header.componentListOffset 309349 "GRZESIU componentListOffset"
Assert-Eq $gr.componentList.count 6 "GRZESIU component count"
Assert-Eq $gr.componentList.componentsParsed 6 "GRZESIU componentsParsed"
Assert-Eq (($gr.componentList.components | ForEach-Object holderKind) -join ',') `
    "Transparent_Video_Holder,HotSpot_Holder,Sprite_Holder,Bitmap_Holder,Sound_Holder,Text_Holder" "GRZESIU component order"
$gbm = $gr.componentList.components[3].privateState
Assert-Eq $gbm.bitmapCount 1 "GRZESIU Bitmap_Holder count"
Assert-Eq $gbm.bitmaps[0].name "autobus09" "GRZESIU bitmap name"
Assert-Eq ($gbm.bitmaps[0].rect -join ',') "144,29,596,388" "GRZESIU bitmap rect"
Assert-Eq $gbm.bitmaps[0].pixelSizeConsistent $true "GRZESIU bitmap pixel size"
Assert-Eq $gr.pageScript.textByteCount 16933 "GRZESIU pageScript bytes"
Assert-Eq $gr.scriptEngineState.switchBlockCount 7 "GRZESIU engine switch blocks"
Assert-Eq $gr.trailingByteCount 4 "GRZESIU trailing bytes"
Assert-Eq $gr.componentList.components[0].privateState.entryCount 15 "GRZESIU TVH entries"
Assert-Eq $gr.componentList.components[1].privateState.hotspotCount 14 "GRZESIU hotspot count"
$gsp = $gr.componentList.components[2].privateState
Assert-Eq $gsp.definitionCount 2 "GRZESIU sprite definitions"
Assert-Eq $gsp.definitions[1].name ([char]::ConvertFromUtf32(0x75) + [char]::ConvertFromUtf32(0x15b) + "miech ") `
    "GRZESIU sprite1 cp1250 name (uśmiech)"

# ----------------------------------------------------------------- INTRO.BDF
# The startup scene; smallest full walk (923-byte script, no switch blocks).
$in = Get-Scene "INTRO.BDF"
Assert-Eq $in.componentList.count 3 "INTRO component count"
Assert-Eq $in.componentList.componentsParsed 3 "INTRO componentsParsed"
Assert-Eq (($in.componentList.components | ForEach-Object holderKind) -join ',') `
    "Sprite_Holder,Transparent_Video_Holder,Sound_Holder" "INTRO component order"
Assert-Eq $in.componentList.components[2].privateStateParsed $true "INTRO Sound_Holder parsed"
Assert-Eq $in.pageScript.textByteCount 923 "INTRO pageScript bytes"
Assert-Eq $in.trailingByteCount 4 "INTRO trailing bytes"
Assert-Eq $in.componentList.components[0].privateState.definitions[0].name "slow" "INTRO sprite0 name"
Assert-Eq $in.componentList.components[1].privateState.entryCount 10 "INTRO TVH entries"
Assert-Eq ($in.componentList.components[1].privateState.entries.frameCount -join ',') `
    "10,20,40,35,16,17,10,30,15,33" "INTRO TVH frame counts"
# Runtime front-end: handler discovery + API inventory over the page script.
$hnames = @($in.pageScript.handlers | ForEach-Object { $_.name })
Assert-Eq ($hnames -join ',') "OnOpenPage,OnClosePage,Transparent_Video_Holder.TheEnd,OnLButtonDown" `
    "INTRO handler names"
$cb = $in.pageScript.handlers | Where-Object { $_.kind -eq 'componentCallback' }
Assert-Eq $cb.component "Transparent_Video_Holder" "INTRO callback component"
Assert-Eq $cb.method "TheEnd" "INTRO callback method"
Assert-Eq $cb.parameters[0].name "videoID" "INTRO callback param"
Assert-Eq (($in.pageScript.handlers | Where-Object { $_.kind -eq 'pageEvent' }).Count) 3 "INTRO page events"
Assert-Eq (($in.pageScript.builtinsUsed | Sort-Object) -join ',') "FadeScreen,LoadGroup,LoadPage,SetCursor" `
    "INTRO builtins used"
Assert-Eq ($in.pageScript.componentMethodsUsed -contains "Sound_Holder.PlayDSound") $true `
    "INTRO uses Sound_Holder.PlayDSound"

# ---------------------------------------------------------------- ABEC_R.BDF
# Recorder-first page (fixed 0x68 record) plus Text/Bitmap/Sound.
$ar = Get-Scene "ABEC_R.BDF"
Assert-Eq $ar.componentList.count 4 "ABEC_R component count"
Assert-Eq $ar.componentList.componentsParsed 4 "ABEC_R componentsParsed"
Assert-Eq (($ar.componentList.components | ForEach-Object holderKind) -join ',') `
    "Recorder,Text_Holder,Bitmap_Holder,Sound_Holder" "ABEC_R component order"
Assert-Eq $ar.pageScript.textByteCount 22252 "ABEC_R pageScript bytes"
Assert-Eq $ar.scriptEngineState.switchBlockCount 12 "ABEC_R engine switch blocks"
Assert-Eq $ar.scriptEngineState.builtinCallCount 3 "ABEC_R engine builtin calls"
Assert-Eq $ar.trailingByteCount 4 "ABEC_R trailing bytes"

# ---------------------------------------------------------------- ABEC_T.BDF
# Video_Holder: 9 external .AVI clips; page walks fully to the script.
$at = Get-Scene "ABEC_T.BDF"
Assert-Eq $at.componentList.componentsParsed $at.componentList.count "ABEC_T fully parsed"
$vh = $at.componentList.components | Where-Object holderKind -eq 'Video_Holder'
Assert-Eq $vh.privateState.entryCount 9 "ABEC_T video entries"
Assert-Eq $vh.privateState.entries[0].name "tp_01ok.avi" "ABEC_T video0 name"
Assert-Eq ([bool]$at.pageScript) $true "ABEC_T script reached"

# --------------------------------------------------------------- OKULARY.BDF
# Panorama (non-holder): one 1224x480 panoramic scene with 19 layers.
$ok = Get-Scene "OKULARY.BDF"
Assert-Eq $ok.componentList.componentsParsed $ok.componentList.count "OKULARY fully parsed"
$pan = $ok.componentList.components | Where-Object holderKind -eq 'Panorama'
Assert-Eq $pan.privateState.sceneCount 1 "OKULARY panorama scenes"
Assert-Eq ($pan.privateState.scenes[0].size -join 'x') "1224x480" "OKULARY panorama size"
Assert-Eq $pan.privateState.scenes[0].subImageCount 19 "OKULARY panorama layers"

# ------------------------------------------------------------------ MROZ.BDF
# Panorama_Holder: length-prefixed scene block; 7-component page walks fully.
$mroz = Get-Scene "MROZ.BDF"
Assert-Eq $mroz.componentList.count 7 "MROZ component count"
Assert-Eq $mroz.componentList.componentsParsed 7 "MROZ fully parsed"
$panh = $mroz.componentList.components | Where-Object holderKind -eq 'Panorama_Holder'
Assert-Eq $panh.privateState.version 0 "MROZ panorama-holder version"
Assert-Eq $panh.privateState.sceneCount 1 "MROZ panorama-holder scenes"

# ----------------------------------------------- INTRO.BDF driven via the Page
# Run OnOpenPage through the executable Page model and assert real, live
# script-visible component + page state (not just a call trace).
$drive = Invoke-Drive "INTRO.BDF" "OnOpenPage"
Assert-Eq $drive.cursor 1 "INTRO drive: cursor set to 1"
Assert-Eq $drive.currentGroup "cursors.grp" "INTRO drive: group loaded"
$tvh = $drive.components | Where-Object name -eq 'Transparent_Video_Holder'
Assert-Eq $tvh.props.playing 0 "INTRO drive: TVH playing clip 0"
$snd = $drive.components | Where-Object name -eq 'Sound_Holder'
Assert-Eq $snd.props.playing 0 "INTRO drive: Sound playing clip 0"

# ------------------------------------------------------------------- summary
Write-Output ""
if ($script:failures -eq 0) {
    Write-Output "verify_scenes: all $script:checks checks passed (START.PRJ, RZECZKA, GRZESIU, INTRO, ABEC_R, ABEC_T, OKULARY, MROZ)"
    exit 0
} else {
    Write-Output "verify_scenes: $script:failures of $script:checks checks FAILED"
    exit 1
}
