@echo off
setlocal EnableDelayedExpansion
where cl >nul 2>nul
if errorlevel 1 (
  set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
  if not exist "!VSWHERE!" (
    echo build.bat: cl.exe is not configured and vswhere.exe was not found
    exit /b 1
  )
  for /f "usebackq tokens=*" %%i in (`"!VSWHERE!" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSROOT=%%i"
  if not defined VSROOT (
    echo build.bat: Visual Studio C++ build tools were not found
    exit /b 1
  )
  call "!VSROOT!\VC\Auxiliary\Build\vcvarsall.bat" x86
  if errorlevel 1 exit /b 1
)
cd /d "%~dp0"
cl /nologo /LD /O2 /W3 gbtrace.c /Fe:gbtrace.dll /link kernel32.lib user32.lib
if errorlevel 1 exit /b 1
cl /nologo /O2 /W3 gblaunch.c /Fe:gblaunch.exe /link kernel32.lib
if errorlevel 1 exit /b 1
echo BUILD OK
