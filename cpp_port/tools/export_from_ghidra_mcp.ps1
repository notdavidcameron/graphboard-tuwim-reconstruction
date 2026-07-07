param(
    [string]$BaseUrl = "http://127.0.0.1:8089",
    [string]$OutDir = "",
    [int]$SettleSeconds = 8,
    [int]$OpenTimeoutSec = 180,
    [string[]]$ProgramPath = @(),
    [string]$OpenLogPath = "",
    [switch]$OpenAllPrograms,
    [switch]$OpenOnly,
    [switch]$ResumeOpenLog,
    [switch]$SkipKnownBadOpen,
    [switch]$DismissAnalyzePrompt
)

$ErrorActionPreference = "Stop"

if (-not $OutDir) {
    $OutDir = Join-Path (Resolve-Path (Join-Path $PSScriptRoot "..")).Path "generated"
}

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

if (-not $OpenLogPath) {
    $OpenLogPath = Join-Path $OutDir "open_programs.jsonl"
}

function New-GhidraUri {
    param(
        [string]$Path,
        [hashtable]$Query = @{}
    )

    $pairs = foreach ($key in $Query.Keys) {
        "$([uri]::EscapeDataString($key))=$([uri]::EscapeDataString([string]$Query[$key]))"
    }
    if ($pairs.Count) {
        return "${BaseUrl}${Path}?$($pairs -join '&')"
    } else {
        return "${BaseUrl}${Path}"
    }
}

function Invoke-GhidraGet {
    param(
        [string]$Path,
        [hashtable]$Query = @{},
        [int]$TimeoutSec = 120
    )

    $uri = New-GhidraUri -Path $Path -Query $Query
    Invoke-RestMethod -Uri $uri -TimeoutSec $TimeoutSec
}

function Invoke-GhidraGetWithPromptWatcher {
    param(
        [string]$Path,
        [hashtable]$Query = @{},
        [int]$TimeoutSec = 120
    )

    if (-not $DismissAnalyzePrompt) {
        return Invoke-GhidraGet -Path $Path -Query $Query -TimeoutSec $TimeoutSec
    }

    $uri = New-GhidraUri -Path $Path -Query $Query
    $job = Start-Job -ScriptBlock {
        param($JobUri, $JobTimeoutSec)
        Invoke-RestMethod -Uri $JobUri -TimeoutSec $JobTimeoutSec
    } -ArgumentList $uri, $TimeoutSec

    $deadline = (Get-Date).AddSeconds($TimeoutSec + 5)
    while ($job.State -eq "Running") {
        Dismiss-AnalyzePrompt
        if ((Get-Date) -gt $deadline) {
            Stop-Job -Job $job -Force -ErrorAction SilentlyContinue
            Remove-Job -Job $job -Force -ErrorAction SilentlyContinue
            throw "Timed out while waiting for $uri"
        }
        Start-Sleep -Milliseconds 500
    }

    try {
        Receive-Job -Job $job -ErrorAction Stop
    } finally {
        Remove-Job -Job $job -Force -ErrorAction SilentlyContinue
    }
}

function Open-ProgramSlowly {
    param([string]$ProgramPath)

    Write-Host "Opening $ProgramPath"
    $started = (Get-Date).ToString("o")
    try {
        $result = Invoke-GhidraGetWithPromptWatcher -Path "/open_program" -Query @{
            path = $ProgramPath
            auto_analyze = "false"
        } -TimeoutSec $OpenTimeoutSec
        if ($DismissAnalyzePrompt) {
            Dismiss-AnalyzePrompt
        }
        $status = [pscustomobject]@{
            timestamp = $started
            program = $ProgramPath
            success = $true
            result = $result
        }
        $status | ConvertTo-Json -Depth 8 -Compress | Add-Content -Encoding UTF8 -Path $OpenLogPath
        $result | ConvertTo-Json -Depth 4
        Write-Host "Settling for $SettleSeconds seconds"
        Start-Sleep -Seconds $SettleSeconds
    } catch {
        $status = [pscustomobject]@{
            timestamp = $started
            program = $ProgramPath
            success = $false
            error = $_.Exception.Message
        }
        $status | ConvertTo-Json -Depth 8 -Compress | Add-Content -Encoding UTF8 -Path $OpenLogPath
        throw
    }
}

function Dismiss-AnalyzePrompt {
    $typeName = "GhidraCppPortWinInput"
    if (-not ($typeName -as [type])) {
        Add-Type -TypeDefinition @"
using System;
using System.Text;
using System.Runtime.InteropServices;
public class GhidraCppPortWinInput {
  public delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);
  [DllImport("user32.dll")] public static extern bool EnumWindows(EnumWindowsProc lpEnumFunc, IntPtr lParam);
  [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr hWnd);
  [DllImport("user32.dll")] public static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr hWnd);
  [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
}
"@
    }
    Add-Type -AssemblyName System.Windows.Forms -ErrorAction SilentlyContinue

    $script:analyzeWindows = @()
    [GhidraCppPortWinInput]::EnumWindows({
        param($hWnd, $lParam)
        if ([GhidraCppPortWinInput]::IsWindowVisible($hWnd)) {
            $titleBuilder = [System.Text.StringBuilder]::new(512)
            [void][GhidraCppPortWinInput]::GetWindowText($hWnd, $titleBuilder, 512)
            if ($titleBuilder.ToString() -eq "Analyze?") {
                $script:analyzeWindows += $hWnd
            }
        }
        return $true
    }, [IntPtr]::Zero) | Out-Null

    foreach ($window in $script:analyzeWindows) {
        Write-Host "Dismissing Ghidra Analyze? prompt"
        [void][GhidraCppPortWinInput]::ShowWindow($window, 5)
        [void][GhidraCppPortWinInput]::SetForegroundWindow($window)
        Start-Sleep -Milliseconds 300
        [System.Windows.Forms.SendKeys]::SendWait("{ESC}")
        Start-Sleep -Milliseconds 700
    }
}

$programOpenOrder = @(
    "/Tuwim.exe",
    "/GraphBoardComponents/TransparentVideoHolder.dll",
    "/GraphBoardComponents/MultiBmp.dll",
    "/MultiBmp.dll",
    "/Sound.dll",
    "/extracted/JT_Programs/Tuwim.exe",
    "/extracted/GraphBoard 1.00/MultiBmp.dll",
    "/extracted/GraphBoard 1.00/TransparentVideoHolder.dll",
    "/extracted/GraphBoard 1.00/SpriteHolder.dll",
    "/extracted/GraphBoard 1.00/HotSpotHolder.dll",
    "/extracted/GraphBoard 1.00/TextHolder.dll",
    "/extracted/GraphBoard 1.00/Sound.dll",
    "/extracted/GraphBoard 1.00/BitmapHolder.dll",
    "/extracted/GraphBoard 1.00/RTFText.dll",
    "/extracted/GraphBoard 1.00/VideoHolder.dll",
    "/extracted/GraphBoard 1.00/Recorder.dll",
    "/extracted/GraphBoard 1.00/EditBox1.dll",
    "/extracted/GraphBoard 1.00/RegionHolder.dll",
    "/extracted/GraphBoard 1.00/EditBox.dll",
    "/extracted/GraphBoard 1.00/VirtualRealityHolder.dll",
    "/extracted/GraphBoard 1.00/Panorama.dll",
    "/extracted/GraphBoard 1.00/Puzzle.dll",
    "/extracted/GraphBoard 1.00/PanoramaHolder.dll",
    "/extracted/GraphBoard 1.00/ColorFill.dll",
    "/extracted/GraphBoard 1.00/FontControl.dll",
    "/extracted/GraphBoard 1.00/AudioHolder.dll",
    "/extracted/GraphBoard 1.00/SetColor.dll",
    "/extracted/GraphBoard 1.00/SynchroText.dll",
    "/extracted/GraphBoard 1.00/Sound.dll.0",
    "/extracted/GraphBoard 1.00/SpriteHolder.dll.0",
    "/extracted/GraphBoard 1.00/BitmapHolder.dll.0",
    "/extracted/GraphBoard 1.00/HotSpotHolder.dll.0",
    "/extracted/GraphBoard 1.00/Recorder.dll.0",
    "/extracted/GraphBoard 1.00/VideoHolder.dll.0",
    "/extracted/GraphBoard 1.00/PanoramaHolder.dll.0",
    "/extracted/GraphBoard 1.00/RegionHolder.dll.0",
    "/extracted/GraphBoard 1.00/AudioHolder.dll.0",
    "/extracted/GraphBoard 1.00/EditBox.dll.0",
    "/extracted/GraphBoard 1.00/VirtualRealityHolder.dll.0",
    "/extracted/GraphBoard 1.00/TransparentVideoHolder.dll.0",
    "/extracted/GraphBoard 1.00/MultiBmp.dll.0",
    "/extracted/GraphBoard 1.00/FontControl.dll.0",
    "/extracted/GraphBoard 1.00/EditBox1.dll.0",
    "/extracted/GraphBoard 1.00/Panorama.dll.0",
    "/extracted/GraphBoard 1.00/ColorFill.dll.0",
    "/extracted/GraphBoard 1.00/SynchroText.dll.0",
    "/extracted/GraphBoard 1.00/Puzzle.dll.0",
    "/extracted/GraphBoard 1.00/TextHolder.dll.0",
    "/extracted/GraphBoard 1.00/RTFText.dll.0",
    "/extracted/GraphBoard 1.00/SetColor.dll.0",
    "/iscript.dll"
)

$knownBadOpen = @(
    "/iscript.dll"
)

$openTargets = if ($ProgramPath.Count -gt 0) {
    $ProgramPath
} elseif ($OpenAllPrograms) {
    $programOpenOrder
} else {
    @()
}

if ($SkipKnownBadOpen) {
    $openTargets = @($openTargets | Where-Object { $knownBadOpen -notcontains $_ })
}

$alreadyOpened = @{}
if ($ResumeOpenLog -and (Test-Path $OpenLogPath)) {
    Get-Content $OpenLogPath | Where-Object { $_.Trim() } | ForEach-Object {
        try {
            $entry = $_ | ConvertFrom-Json
            if ($entry.success -eq $true) {
                $alreadyOpened[[string]$entry.program] = $true
            }
        } catch {
            Write-Warning "Ignoring malformed open log line: $_"
        }
    }
}

if ($openTargets.Count -gt 0) {
    foreach ($programToOpen in $openTargets) {
        if ($ResumeOpenLog -and $alreadyOpened.ContainsKey($programToOpen)) {
            Write-Host "Skipping already-opened target from log: $programToOpen"
            continue
        }
        Open-ProgramSlowly -ProgramPath $programToOpen
    }

    if ($OpenOnly) {
        return
    }
}

$exports = @(
    @{
        program = "/Tuwim.exe"
        output = "tuwim_host_serializers.cpp"
        functions = @(
            @{ name = "GraphBrdDoc_Construct"; address = "00405780" },
            @{ name = "GraphBrdDoc_SerializeProjectState"; address = "00406020" },
            @{ name = "GraphBrdDoc_SerializeCursorAndGroupState"; address = "004064c0" },
            @{ name = "GraphBrdDoc_SerializePageData"; address = "004066f0" },
            @{ name = "GraphBrdScriptEngine_Serialize"; address = "0041aad0" },
            @{ name = "GraphBrdCntrItem_Serialize"; address = "0042b920" }
        )
    },
    @{
        program = "/GraphBoardComponents/TransparentVideoHolder.dll"
        output = "transparent_video_holder.cpp"
        functions = @(
            @{ name = "TVH_ComputeZRange"; address = "100073b0" },
            @{ name = "TVH_ResetVideo"; address = "10007880" },
            @{ name = "TVH_WriteArchiveChunked"; address = "10007990" },
            @{ name = "TVH_ReadArchiveChunked"; address = "10007a40" },
            @{ name = "TVH_SerializeOrDeserialize"; address = "10007af0" },
            @{ name = "TVH_SeekEntryVideo"; address = "10008120" },
            @{ name = "TVH_HideVideoRect"; address = "10008200" },
            @{ name = "TVH_FreeEntryBuffers"; address = "10009550" },
            @{ name = "TVH_OpenAudioDevice"; address = "10009690" },
            @{ name = "TVH_CloseAudioDevice"; address = "10009800" },
            @{ name = "TVH_StopAudioPlayback"; address = "10009820" },
            @{ name = "TVH_SetPlaybackRect"; address = "10009930" },
            @{ name = "TVH_OpenEntryPlayback"; address = "10009970" },
            @{ name = "TVH_DrawStillFrame"; address = "10009a00" },
            @{ name = "TVH_ProcessVideoChunks"; address = "10009aa0" },
            @{ name = "TVH_StopEntryPlayback"; address = "1000a180" },
            @{ name = "TVH_IsEntryPlaying"; address = "1000a1b0" },
            @{ name = "TVH_ReadFirstChunk"; address = "1000a1c0" },
            @{ name = "TVH_StartEntryPlayback"; address = "1000a3d0" },
            @{ name = "TVH_NotifyPlaybackEnd"; address = "1000a420" }
        )
    }
)

foreach ($group in $exports) {
    $program = $group.program
    $outputPath = Join-Path $OutDir $group.output
    $manifestPath = [System.IO.Path]::ChangeExtension($outputPath, ".manifest.json")
    $programEsc = $program
    $builder = [System.Text.StringBuilder]::new()
    [void]$builder.AppendLine("/*")
    [void]$builder.AppendLine("  Direct C++ port seed generated from Ghidra MCP localhost.")
    [void]$builder.AppendLine("  Source program: $program")
    [void]$builder.AppendLine("  Notes: raw Ghidra decompiler output with only file-level framing added.")
    [void]$builder.AppendLine("*/")
    [void]$builder.AppendLine('#include "../include/ghidra_compat.hpp"')
    [void]$builder.AppendLine("")
    [void]$builder.AppendLine("// clang-format off")

    $manifest = @()
    foreach ($function in $group.functions) {
        Write-Host "Decompiling $($function.name) @ $($function.address) from $program"
        $info = Invoke-GhidraGet -Path "/get_function_by_address" -Query @{
            program = $programEsc
            address = $function.address
        } -TimeoutSec 60
        Start-Sleep -Seconds 2
        $code = Invoke-GhidraGet -Path "/decompile_function" -Query @{
            program = $programEsc
            address = $function.address
            timeout = "90"
        } -TimeoutSec 140

        [void]$builder.AppendLine("")
        [void]$builder.AppendLine("// ============================================================================")
        foreach ($line in ([string]$info -split "`n")) {
            [void]$builder.AppendLine("// $line")
        }
        [void]$builder.AppendLine("// ============================================================================")
        [void]$builder.AppendLine(([string]$code).Trim())
        [void]$builder.AppendLine("")

        $manifest += [pscustomobject]@{
            program = $program
            function = $function.name
            address = $function.address
            info = $info
        }
        Start-Sleep -Seconds 3
    }

    [void]$builder.AppendLine("// clang-format on")
    [System.IO.File]::WriteAllText($outputPath, $builder.ToString(), [System.Text.UTF8Encoding]::new($false))
    $manifest | ConvertTo-Json -Depth 5 | Set-Content -Encoding UTF8 -Path $manifestPath
    Write-Host "Wrote $outputPath"
}
