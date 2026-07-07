param(
    [string]$BuildDir = "",
    [string]$Generator = "Ninja"
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptDir "..")

$extraPath = @(
    "C:\ProgramData\mingw64\mingw64\bin",
    "C:\Program Files\CMake\bin",
    "C:\ProgramData\chocolatey\bin",
    "G:\Tools\mingw-build\mingw64\bin"
) | Where-Object { Test-Path $_ }

if ($extraPath.Count -gt 0) {
    $env:PATH = ($extraPath -join ";") + ";" + $env:PATH
}

if (-not $BuildDir) {
    $BuildDir = Join-Path $repoRoot "build\reconstruction-check"
}

$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) {
    throw "cmake was not found. Install with: choco install cmake -y"
}

$compiler = Get-Command g++ -ErrorAction SilentlyContinue
if (-not $compiler) {
    throw "g++ was not found. Install with: choco install mingw -y"
}

if ($Generator -eq "Ninja" -and -not (Get-Command ninja -ErrorAction SilentlyContinue)) {
    throw "ninja was not found. Install with: choco install ninja -y"
}

& $cmake.Source -S $repoRoot -B $BuildDir -G $Generator -DCMAKE_CXX_COMPILER="$($compiler.Source)"
& $cmake.Source --build $BuildDir --target ghidra_reconstructions
