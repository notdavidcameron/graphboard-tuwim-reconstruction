$ErrorActionPreference = "Stop"

$Workspace = "C:\Users\Administrator\Documents\GRAPHBOARD"
$Python = "C:\Users\Administrator\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe"
$OutputRoot = Join-Path $Workspace "extracted_assets"
$LogDir = Join-Path $OutputRoot "logs"
New-Item -ItemType Directory -Force -Path $LogDir | Out-Null

$Stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$LogPath = Join-Path $LogDir "convert_all_images_to_jpeg_$Stamp.log"

Set-Location $Workspace
"GraphBoard convert all images to JPEG log" | Tee-Object -FilePath $LogPath
"Started: $(Get-Date -Format o)" | Tee-Object -FilePath $LogPath -Append
"" | Tee-Object -FilePath $LogPath -Append

$InputImages = Join-Path $OutputRoot "all_images"
$OutputImages = Join-Path $OutputRoot "all_images_jpeg"

& $Python .\convert_all_images_to_jpeg.py `
    --input $InputImages `
    --output $OutputImages `
    --quality 95 `
    --clean 2>&1 |
    Tee-Object -FilePath $LogPath -Append

"" | Tee-Object -FilePath $LogPath -Append
"Finished: $(Get-Date -Format o)" | Tee-Object -FilePath $LogPath -Append
"Log: $LogPath" | Tee-Object -FilePath $LogPath -Append
