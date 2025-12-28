# build_windows.ps1
# This script automates the build process for Kitchen Knight on Windows

$VS_PATH = "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools"
$VCVARS = "$VS_PATH\VC\Auxiliary\Build\vcvars64.bat"

if (-not (Test-Path $VCVARS)) {
    Write-Error "Visual Studio 2022 Build Tools not found at $VS_PATH. Please check the path."
    exit 1
}

Write-Host "Initializing MSVC environment..." -ForegroundColor Cyan
# Run vcvars64.bat and capture environment variables
$env_file = New-TemporaryFile
cmd /c " `"$VCVARS`" && set > `"$env_file`" "
Get-Content $env_file | ForEach-Object {
    if ($_ -match "^(.*?)=(.*)$") {
        Set-Item -Path "Env:\$($Matches[1])" -Value $Matches[2]
    }
}
Remove-Item $env_file

Write-Host "Configuring build directory..." -ForegroundColor Cyan
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}
cd build

Write-Host "Running CMake configure..." -ForegroundColor Cyan
cmake .. -DCUSTOMIZE_BUILD=OFF

Write-Host "Building project..." -ForegroundColor Cyan
cmake --build . --config Release

Write-Host "Build complete! You can run the game with: .\Release\kitchen_knight.exe" -ForegroundColor Green
