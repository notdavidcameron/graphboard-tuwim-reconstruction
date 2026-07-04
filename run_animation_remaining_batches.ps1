$ErrorActionPreference = "Stop"

$Workspace = "C:\Users\Administrator\Documents\GRAPHBOARD"
$SourceRoot = Get-ChildItem "C:\Users\Administrator\Desktop" -Directory |
    Where-Object { $_.Name -like "Multimedialny*Juliana*Tuwima PL" } |
    Select-Object -First 1
if ($null -eq $SourceRoot) {
    throw "Could not find the Tuwim source folder on the Desktop."
}
$Source = Join-Path $SourceRoot.FullName "Tuwim\DATA"
$Output = Join-Path $Workspace "extracted_assets"
$LogDir = Join-Path $Output "logs"
New-Item -ItemType Directory -Force -Path $LogDir | Out-Null

$Stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$LogPath = Join-Path $LogDir "animation_remaining_batches_$Stamp.log"

Set-Location $Workspace
"GraphBoard remaining animation refresh log" | Tee-Object -FilePath $LogPath
"Started: $(Get-Date -Format o)" | Tee-Object -FilePath $LogPath -Append
"Source: $Source" | Tee-Object -FilePath $LogPath -Append
"Output: $Output" | Tee-Object -FilePath $LogPath -Append
"" | Tee-Object -FilePath $LogPath -Append

foreach ($Start in 25,50,75,100,125,150,175,200) {
    "=== Batch start=$Start size=25 ===" | Tee-Object -FilePath $LogPath -Append
    python .\extract_animation_batch.py $Source -o $Output --batch-size 25 --start $Start 2>&1 |
        Tee-Object -FilePath $LogPath -Append
    "" | Tee-Object -FilePath $LogPath -Append
}

"Finished: $(Get-Date -Format o)" | Tee-Object -FilePath $LogPath -Append
"Log: $LogPath" | Tee-Object -FilePath $LogPath -Append
