# Real-file regression check for Multimedialny Swiat Jana Brzechwy.
#
# Usage:
#   .\cpp_port\tools\verify_brzechwa.ps1 [-DataDir <Brzechwa DATA>] [-GbInspect <gbinspect.exe>]

param(
    [string]$DataDir = "",
    [string]$GbInspect = ""
)

$ErrorActionPreference = "Stop"
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
try { [Console]::OutputEncoding = [System.Text.Encoding]::UTF8 } catch {}

$extraPath = @(
    "C:\ProgramData\mingw64\mingw64\bin",
    "G:\Tools\mingw-build\mingw64\bin"
) | Where-Object { Test-Path $_ }
if ($extraPath.Count -gt 0) {
    $env:PATH = ($extraPath -join ";") + ";" + $env:PATH
}

if (-not $DataDir) {
    $resolved = Resolve-Path "C:\Users\Administrator\Desktop\Multimedialny Swiat Jana Brzechwy\DATA" -ErrorAction SilentlyContinue |
        Select-Object -First 1
    if ($resolved) { $DataDir = $resolved.Path }
}
if (-not $GbInspect) {
    $candidates = @(
        (Join-Path $scriptDir "..\build-mingw\gbinspect.exe"),
        (Join-Path $scriptDir "..\build\cmake\gbinspect.exe")
    )
    $GbInspect = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
}
if (-not $GbInspect -or -not (Test-Path $GbInspect)) {
    throw "gbinspect not found - build cpp_port first or pass -GbInspect"
}
if (-not $DataDir -or -not (Test-Path $DataDir)) {
    throw "Brzechwa DATA folder not found - pass -DataDir"
}

$script:checks = 0
$script:failures = 0

function Assert-Eq($actual, $expected, $label) {
    $script:checks++
    if ("$actual" -ne "$expected") {
        Write-Output "FAIL  $label`: expected '$expected', got '$actual'"
        $script:failures++
    }
}

function Get-Json([string]$name, [string[]]$arguments = @()) {
    $raw = & $GbInspect (Join-Path $DataDir $name) @arguments 2>&1
    if ($LASTEXITCODE -ne 0) { throw "gbinspect failed on $name`: $raw" }
    return $raw | ConvertFrom-Json
}

# Version-0 project layout: no signature CString, then the global script block.
$project = Get-Json "START.PRJ"
Assert-Eq $project.version 0 "START.PRJ version"
Assert-Eq $project.startupPage "intro.bdf" "START.PRJ startup page"
Assert-Eq $project.pageCount 79 "START.PRJ page count"
Assert-Eq $project.groupCount 0 "START.PRJ group count"
Assert-Eq $project.decodedSignature "" "START.PRJ absent signature"
Assert-Eq $project.globalScriptVersion 1 "START.PRJ global script version"
Assert-Eq $project.globalScriptByteCount 375 "START.PRJ global script bytes"
Assert-Eq ($project.globalScript -match "CString mHistory") $true "START.PRJ mHistory global"
Assert-Eq ($project.globalScript -match "int doTanca=0") $true "START.PRJ doTanca global"
Assert-Eq ($project.pages -contains "WARZYWA.BDF") $false "WARZYWA is not a project page"
$startup = Get-Json "START.PRJ" @("--page", "intro.bdf", "--follow", "0")
Assert-Eq $startup.projectGlobals.mHistory "" "START.PRJ mHistory initial value"
Assert-Eq $startup.projectGlobals.doTanca 0 "START.PRJ doTanca initial value"

# Parse every genuine board resource. WARZYWA.BDF is a known non-YDP file with
# a misleading extension and is deliberately reported rather than parsed.
$resources = @(Get-ChildItem -LiteralPath $DataDir -File | Where-Object {
    $_.Extension -ieq ".BDF" -or $_.Extension -ieq ".GRP"
})
$parsed = 0
$knownNonBoard = 0
foreach ($resource in $resources) {
    if ($resource.Name -ieq "WARZYWA.BDF") {
        $knownNonBoard++
        continue
    }
    & $GbInspect $resource.FullName *> $null
    if ($LASTEXITCODE -ne 0) {
        Write-Output "FAIL  parse $($resource.Name)"
        $script:failures++
    } else {
        $parsed++
    }
    $script:checks++
}
Assert-Eq $parsed 176 "genuine BDF/GRP resources parsed"
Assert-Eq $knownNonBoard 1 "known WARZYWA.BDF exception"

# Original cursor artwork is carried by dynamically loaded groups.
$group = Get-Json "WYBOR.GRP"
Assert-Eq $group.cursorCount 6 "WYBOR.GRP cursor count"
Assert-Eq $group.cursors[0].name "action1" "WYBOR.GRP first cursor"
Assert-Eq ($group.cursors[0].size -join "x") "27x31" "WYBOR.GRP cursor dimensions"
Assert-Eq $group.cursors[0].transparentIndex 215 "WYBOR.GRP transparent index"

# WYBOR intentionally cycles one ambient preview at a time, at the CRect
# coordinates stored in its script.
$selection = Get-Json "WYBOR.BDF" @("--advance", "3100")
$moves = @($selection.calls | Where-Object {
    $_.component -eq "Transparent_Video_Holder" -and $_.name -eq "MoveTo"
})
$first = $moves | Where-Object { $_.args[0] -eq 0 -and $_.args[1] -eq 438 -and $_.args[2] -eq 134 }
$second = $moves | Where-Object { $_.args[0] -eq 1 -and $_.args[1] -eq 341 -and $_.args[2] -eq 129 }
Assert-Eq ([bool]$first) $true "WYBOR preview 0 position"
Assert-Eq ([bool]$second) $true "WYBOR preview 1 position"
$selectionVideo = $selection.components | Where-Object name -eq "Transparent_Video_Holder"
$playing = @($selectionVideo.items.PSObject.Properties.Value | Where-Object { $_.playing -eq 1 })
Assert-Eq $playing.Count 1 "WYBOR one ambient preview playing"
Assert-Eq $selectionVideo.items.'1'.playing 1 "WYBOR preview 1 active after first completion"

# Hovering a poem selection changes its highlight/cursor. It must not launch a
# second preview; the only Play here is the ambient clip started by OnOpenPage.
$hover = Get-Json "WYBOR.BDF" @("--move", "50,45")
$hoverPlays = @($hover.calls | Where-Object {
    $_.component -eq "Transparent_Video_Holder" -and $_.name -eq "Play"
})
$hoverHighlight = $hover.calls | Where-Object {
    $_.component -eq "Bitmap_Holder" -and $_.name -eq "ShowBitmap" -and $_.args[0] -eq 0
}
Assert-Eq $hover.cursor 0 "WYBOR selection hover cursor"
Assert-Eq ([bool]$hoverHighlight) $true "WYBOR selection hover highlight"
Assert-Eq $hoverPlays.Count 1 "WYBOR selection hover does not start preview"

# The right-edge reveal is the authored toolbar path.
$toolbar = Get-Json "WYBOR.BDF" @("--move", "620,430", "--advance", "1200",
                                   "--click", "620,430", "--advance", "12000")
$toolbarPlay = $toolbar.calls | Where-Object {
    $_.component -eq "Transparent_Video_Holder" -and $_.name -eq "Play" -and $_.args[0] -eq 25
}
$toolbarSprite = $toolbar.components | Where-Object name -eq "Sprite_Holder"
Assert-Eq ([bool]$toolbarPlay) $true "WYBOR toolbar opening clip"
Assert-Eq $toolbarSprite.items.'1'.x 80 "WYBOR toolbar final x"

# Equal-layer group dispatch: the revealed finger sprite must receive the click
# instead of the hotspot beneath it.
$left = Get-Json "PYTAL.BDF" @("--move", "20,20", "--advance", "1200",
                                "--click", "20,20", "--advance", "6000")
$leftPlay = $left.calls | Where-Object {
    $_.component -eq "Group.Transparent_Video_Holder" -and $_.name -eq "Play" -and $_.args[0] -eq 7
}
Assert-Eq ([bool]$leftPlay) $true "PYTAL left finger plays clip 7"
Assert-Eq $left.pendingPage "lato.bdf" "PYTAL left finger destination"

$right = Get-Json "PYTAL.BDF" @("--move", "620,20", "--advance", "1200",
                                 "--click", "620,20", "--advance", "6000")
$rightPlay = $right.calls | Where-Object {
    $_.component -eq "Group.Transparent_Video_Holder" -and $_.name -eq "Play" -and $_.args[0] -eq 8
}
Assert-Eq ([bool]$rightPlay) $true "PYTAL right finger plays clip 8"
Assert-Eq $right.pendingPage "samo.bdf" "PYTAL right finger destination"

Write-Output ""
if ($script:failures -eq 0) {
    Write-Output "verify_brzechwa: all $script:checks checks passed ($parsed board resources; WARZYWA.BDF reported as non-board)"
    exit 0
}
Write-Output "verify_brzechwa: $script:failures of $script:checks checks FAILED"
exit 1
