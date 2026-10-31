# SixEngine branch build script
# Fix: Editor/Sandbox PostBuildEvent only xcopy assets, never Engine.dll
# => incremental build leaves stale Engine.dll in Editor\out / Sandbox\out
# This script force-copies Engine.dll to both consumers after every build.

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$cfg  = 'Debug'
$plat = 'x64'
$msb  = 'G:\Visual\MSBuild\Current\Bin\MSBuild.exe'

Write-Host "=== 1. kill running ===" -ForegroundColor Cyan
Get-Process Editor, Sandbox -ErrorAction SilentlyContinue | Stop-Process -Force

Write-Host "=== 2. build solution ===" -ForegroundColor Cyan
& $msb "$root\SixEngine.slnx" /p:Configuration=$cfg /p:Platform=$plat /v:minimal /m 2>&1 |
    Tee-Object -Variable msbOut |
    Select-String -Pattern 'error|Build succeeded|Build FAILED' |
    Select-Object -Last 15

if ($LASTEXITCODE -ne 0 -or ($msbOut -join "`n") -match ': error ') {
    Write-Host "`n=== BUILD FAILED (exit=$LASTEXITCODE) ===" -ForegroundColor Red
    exit 1
}

$engineDll = "$root\Engine\out\$plat\$cfg\Engine.dll"
if (-not (Test-Path $engineDll)) {
    Write-Error "Engine.dll not produced: $engineDll"
    exit 1
}

Write-Host "=== 3. sync Engine.dll to consumers ===" -ForegroundColor Cyan
foreach ($proj in 'Editor', 'Sandbox') {
    $dstDir = "$root\$proj\out\$plat\$cfg"
    if (Test-Path $dstDir) {
        Copy-Item $engineDll $dstDir -Force
        Copy-Item "$root\Engine\out\$plat\$cfg\Engine.pdb" $dstDir -Force -ErrorAction SilentlyContinue
        $ts = (Get-Item "$dstDir\Engine.dll").LastWriteTime
        Write-Host "  $proj : Engine.dll synced ($ts)"
    } else {
        Write-Host "  $proj : out dir not found, skipped"
    }
}

Write-Host "`n=== build done ===" -ForegroundColor Green
Write-Host "Engine.dll : $engineDll ($((Get-Item $engineDll).LastWriteTime))"
Write-Host "Editor.exe : $(Test-Path "$root\Editor\out\$plat\$cfg\Editor.exe")"
Write-Host "Sandbox.exe: $(Test-Path "$root\Sandbox\out\$plat\$cfg\Sandbox.exe")"