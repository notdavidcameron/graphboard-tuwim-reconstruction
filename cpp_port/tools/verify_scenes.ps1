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
# Six components; four parse today, walk stops at Text_Holder (see
# cpp_port/docs/holder_recovery_notes.md for the recovered Text/Sound layouts).
$rz = Get-Scene "RZECZKA.BDF"
Assert-Eq $rz.header.componentListOffset 1165 "RZECZKA componentListOffset"
Assert-Eq ($rz.header.pageRect -join ',') "0,0,640,480" "RZECZKA pageRect"
Assert-Eq $rz.componentList.count 6 "RZECZKA component count"
Assert-Eq $rz.componentList.componentsParsed 4 "RZECZKA componentsParsed"
Assert-Eq (($rz.componentList.components | ForEach-Object holderKind) -join ',') `
    "HotSpot_Holder,Transparent_Video_Holder,Sprite_Holder,Text_Holder" "RZECZKA component order"
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

# --------------------------------------------------------------- GRZESIU.BDF
# TVH-first ordering, cp1250 sprite names, stops at Bitmap_Holder.
$gr = Get-Scene "GRZESIU.BDF"
Assert-Eq $gr.header.componentListOffset 309349 "GRZESIU componentListOffset"
Assert-Eq $gr.componentList.count 6 "GRZESIU component count"
Assert-Eq $gr.componentList.componentsParsed 4 "GRZESIU componentsParsed"
Assert-Eq (($gr.componentList.components | ForEach-Object holderKind) -join ',') `
    "Transparent_Video_Holder,HotSpot_Holder,Sprite_Holder,Bitmap_Holder" "GRZESIU component order"
Assert-Eq $gr.componentList.components[0].privateState.entryCount 15 "GRZESIU TVH entries"
Assert-Eq $gr.componentList.components[1].privateState.hotspotCount 14 "GRZESIU hotspot count"
$gsp = $gr.componentList.components[2].privateState
Assert-Eq $gsp.definitionCount 2 "GRZESIU sprite definitions"
Assert-Eq $gsp.definitions[1].name ([char]::ConvertFromUtf32(0x75) + [char]::ConvertFromUtf32(0x15b) + "miech ") `
    "GRZESIU sprite1 cp1250 name (uśmiech)"

# ----------------------------------------------------------------- INTRO.BDF
# The startup scene. All three components enumerate; the walk stops AT the
# last one (Sound_Holder), exercising the stop-at-final-component edge.
$in = Get-Scene "INTRO.BDF"
Assert-Eq $in.componentList.count 3 "INTRO component count"
Assert-Eq $in.componentList.componentsParsed 3 "INTRO componentsParsed"
Assert-Eq (($in.componentList.components | ForEach-Object holderKind) -join ',') `
    "Sprite_Holder,Transparent_Video_Holder,Sound_Holder" "INTRO component order"
Assert-Eq $in.componentList.components[2].privateStateParsed $false "INTRO Sound_Holder unparsed"
Assert-Eq $in.componentList.components[0].privateState.definitions[0].name "slow" "INTRO sprite0 name"
Assert-Eq $in.componentList.components[1].privateState.entryCount 10 "INTRO TVH entries"
Assert-Eq ($in.componentList.components[1].privateState.entries.frameCount -join ',') `
    "10,20,40,35,16,17,10,30,15,33" "INTRO TVH frame counts"

# ------------------------------------------------------------------- summary
Write-Output ""
if ($script:failures -eq 0) {
    Write-Output "verify_scenes: all $script:checks checks passed (START.PRJ, RZECZKA, GRZESIU, INTRO)"
    exit 0
} else {
    Write-Output "verify_scenes: $script:failures of $script:checks checks FAILED"
    exit 1
}
