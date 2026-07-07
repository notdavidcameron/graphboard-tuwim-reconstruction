param(
    [Parameter(Mandatory=$true)]
    [string]$Program,
    [string]$BaseUrl = "http://127.0.0.1:8089",
    [string]$OutDir = "",
    [string]$NamePattern = "FUN_",
    [int]$PageSize = 100,
    [int]$StartOffset = 0,
    [int]$MaxFunctions = 0,
    [int]$SettleMilliseconds = 750,
    [int]$DecompileTimeoutSec = 90,
    [switch]$OpenProgramFirst,
    [switch]$PerFunctionFiles,
    [switch]$EnhancedSearch,
    [switch]$Regex,
    [switch]$Resume
)

$ErrorActionPreference = "Stop"

if (-not $OutDir) {
    $OutDir = Join-Path (Resolve-Path (Join-Path $PSScriptRoot "..")).Path "generated"
}

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

function Invoke-GhidraGet {
    param(
        [string]$Path,
        [hashtable]$Query = @{},
        [int]$TimeoutSec = 120
    )

    $pairs = foreach ($key in $Query.Keys) {
        "$([uri]::EscapeDataString($key))=$([uri]::EscapeDataString([string]$Query[$key]))"
    }
    $uri = if ($pairs.Count) {
        "${BaseUrl}${Path}?$($pairs -join '&')"
    } else {
        "${BaseUrl}${Path}"
    }

    Invoke-RestMethod -Uri $uri -TimeoutSec $TimeoutSec
}

function ConvertTo-SafeFileStem {
    param([string]$Value)
    $stem = $Value.Trim("/")
    $stem = $stem -replace "[\\/:*?`"<>| ]+", "_"
    if (-not $stem) {
        return "program"
    }
    return $stem.ToLowerInvariant()
}

function ConvertTo-SafeFunctionStem {
    param(
        [string]$Address,
        [string]$Name
    )
    $safeName = if ($Name) { $Name } else { "function" }
    $safeName = $safeName -replace "[\\/:*?`"<>| ]+", "_"
    return "$Address`_$safeName"
}

function Add-JsonLineWithRetry {
    param(
        [string]$Path,
        [object]$Value,
        [int]$Retries = 10
    )

    $line = ($Value | ConvertTo-Json -Compress -Depth 8) + [Environment]::NewLine
    for ($attempt = 1; $attempt -le $Retries; $attempt++) {
        try {
            [System.IO.File]::AppendAllText($Path, $line, [System.Text.UTF8Encoding]::new($false))
            return
        } catch {
            if ($attempt -eq $Retries) {
                throw
            }
            Start-Sleep -Milliseconds (100 * $attempt)
        }
    }
}

function Get-FunctionItems {
    $offset = $StartOffset
    $items = @()

    while ($true) {
        Write-Host "Listing functions at offset $offset"
        if ($EnhancedSearch) {
            $page = Invoke-GhidraGet -Path "/search_functions_enhanced" -Query @{
                program = $Program
                name_pattern = $NamePattern
                regex = if ($Regex) { "true" } else { "false" }
                offset = $offset
                limit = $PageSize
            } -TimeoutSec 120
        } else {
            $page = Invoke-GhidraGet -Path "/search_functions" -Query @{
                program = $Program
                name_pattern = $NamePattern
                offset = $offset
                limit = $PageSize
            } -TimeoutSec 120
        }

        $pageItems = @()
        if ($page -is [string]) {
            $pageItems = @($page -split "`r?`n" | Where-Object { $_.Trim() })
        } elseif ($page.functions) {
            $pageItems = @($page.functions)
        } elseif ($page.results) {
            $pageItems = @($page.results)
        } elseif ($page -is [array]) {
            $pageItems = @($page)
        }

        if ($pageItems.Count -eq 0) {
            break
        }

        foreach ($pageItem in $pageItems) {
            $items += $pageItem
            if ($MaxFunctions -gt 0 -and $items.Count -ge $MaxFunctions) {
                return $items
            }
        }
        if ($pageItems.Count -lt $PageSize) {
            break
        }
        $offset += $PageSize
        Start-Sleep -Milliseconds $SettleMilliseconds
    }

    return $items
}

function Get-FunctionName {
    param($Item)
    if ($Item -is [string]) {
        if ($Item -match "^\s*(.*?)\s+@\s+[0-9a-fA-F]+") {
            return $Matches[1]
        }
        return $Item.Trim()
    }
    foreach ($property in "name", "function", "function_name", "symbol") {
        if ($Item.PSObject.Properties[$property] -and $Item.$property) {
            return [string]$Item.$property
        }
    }
    return ""
}

function Get-FunctionAddress {
    param($Item)
    if ($Item -is [string]) {
        if ($Item -match "([0-9a-fA-F]{8,})") {
            return $Matches[1]
        }
        return ""
    }
    foreach ($property in "address", "entry", "entry_point", "entryPoint") {
        if ($Item.PSObject.Properties[$property] -and $Item.$property) {
            return ([string]$Item.$property) -replace "^0x", ""
        }
    }
    return ""
}

if ($OpenProgramFirst) {
    Write-Host "Opening $Program"
    Invoke-GhidraGet -Path "/open_program" -Query @{
        path = $Program
        auto_analyze = "false"
    } -TimeoutSec 180 | ConvertTo-Json -Depth 4
    Start-Sleep -Seconds 8
}

$stem = ConvertTo-SafeFileStem -Value $Program
$chunkSuffix = if ($StartOffset -gt 0 -or $MaxFunctions -gt 0) {
    ".offset$StartOffset.count$MaxFunctions"
} else {
    ""
}
$outputPath = Join-Path $OutDir "$stem$chunkSuffix.cpp"
$manifestPath = Join-Path $OutDir "$stem$chunkSuffix.manifest.json"
$checkpointPath = Join-Path $OutDir "$stem.checkpoint.jsonl"
$functionDir = Join-Path $OutDir (Join-Path $stem "functions")
if ($PerFunctionFiles) {
    New-Item -ItemType Directory -Force -Path $functionDir | Out-Null
}

$completed = @{}
if ($Resume -and (Test-Path $checkpointPath)) {
    Get-Content $checkpointPath | Where-Object { $_.Trim() } | ForEach-Object {
        try {
            $entry = $_ | ConvertFrom-Json
            if ($entry.success -eq $true -and $entry.address) {
                $completed[[string]$entry.address] = $true
            }
        } catch {
            Write-Warning "Ignoring malformed checkpoint line: $_"
        }
    }
}

$functions = Get-FunctionItems

$builder = [System.Text.StringBuilder]::new()
[void]$builder.AppendLine("/*")
[void]$builder.AppendLine("  Direct C++ port generated from Ghidra MCP localhost.")
[void]$builder.AppendLine("  Source program: $Program")
[void]$builder.AppendLine("  Notes: raw Ghidra decompiler output with only file-level framing added.")
[void]$builder.AppendLine("*/")
[void]$builder.AppendLine('#include "../include/ghidra_compat.hpp"')
[void]$builder.AppendLine("")
[void]$builder.AppendLine("// clang-format off")

$manifest = @()
foreach ($item in $functions) {
    $name = Get-FunctionName -Item $item
    $address = Get-FunctionAddress -Item $item
    if (-not $address) {
        Write-Warning "Skipping function with no address: $($item | ConvertTo-Json -Compress -Depth 4)"
        continue
    }
    if ($Resume -and $completed.ContainsKey($address)) {
        Write-Host "Skipping checkpointed function $address"
        continue
    }

    Write-Host "Decompiling $name @ $address"
    try {
        $info = Invoke-GhidraGet -Path "/get_function_by_address" -Query @{
            program = $Program
            address = $address
        } -TimeoutSec 60
        Start-Sleep -Milliseconds $SettleMilliseconds
        $code = Invoke-GhidraGet -Path "/decompile_function" -Query @{
            program = $Program
            address = $address
            timeout = $DecompileTimeoutSec
        } -TimeoutSec ($DecompileTimeoutSec + 60)

        [void]$builder.AppendLine("")
        [void]$builder.AppendLine("// ============================================================================")
        foreach ($line in ([string]$info -split "`n")) {
            [void]$builder.AppendLine("// $line")
        }
        [void]$builder.AppendLine("// ============================================================================")
        [void]$builder.AppendLine(([string]$code).Trim())
        [void]$builder.AppendLine("")

        if ($PerFunctionFiles) {
            $functionStem = ConvertTo-SafeFunctionStem -Address $address -Name $name
            $functionPath = Join-Path $functionDir "$functionStem.cpp"
            $functionBuilder = [System.Text.StringBuilder]::new()
            [void]$functionBuilder.AppendLine("/*")
            [void]$functionBuilder.AppendLine("  Direct C++ port generated from Ghidra MCP localhost.")
            [void]$functionBuilder.AppendLine("  Source program: $Program")
            [void]$functionBuilder.AppendLine("  Source function: $name @ $address")
            [void]$functionBuilder.AppendLine("*/")
            [void]$functionBuilder.AppendLine('#include "../../../include/ghidra_compat.hpp"')
            [void]$functionBuilder.AppendLine("")
            [void]$functionBuilder.AppendLine("// clang-format off")
            [void]$functionBuilder.AppendLine("")
            [void]$functionBuilder.AppendLine("// ============================================================================")
            foreach ($line in ([string]$info -split "`n")) {
                [void]$functionBuilder.AppendLine("// $line")
            }
            [void]$functionBuilder.AppendLine("// ============================================================================")
            [void]$functionBuilder.AppendLine(([string]$code).Trim())
            [void]$functionBuilder.AppendLine("")
            [void]$functionBuilder.AppendLine("// clang-format on")
            [System.IO.File]::WriteAllText($functionPath, $functionBuilder.ToString(), [System.Text.UTF8Encoding]::new($false))
        }

        $manifest += [pscustomobject]@{
            program = $Program
            function = $name
            address = $address
            info = $info
        }
        Add-JsonLineWithRetry -Path $checkpointPath -Value ([pscustomobject]@{
            timestamp = (Get-Date).ToString("o")
            program = $Program
            function = $name
            address = $address
            success = $true
        })
        Start-Sleep -Milliseconds $SettleMilliseconds
    } catch {
        Add-JsonLineWithRetry -Path $checkpointPath -Value ([pscustomobject]@{
            timestamp = (Get-Date).ToString("o")
            program = $Program
            function = $name
            address = $address
            success = $false
            error = $_.Exception.Message
        })
        throw
    }
}

[void]$builder.AppendLine("// clang-format on")
[System.IO.File]::WriteAllText($outputPath, $builder.ToString(), [System.Text.UTF8Encoding]::new($false))
$manifest | ConvertTo-Json -Depth 5 | Set-Content -Encoding UTF8 -Path $manifestPath
Write-Host "Wrote $outputPath"
