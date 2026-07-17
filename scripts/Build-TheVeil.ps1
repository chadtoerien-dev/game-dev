param(
    [string]$EngineRoot = "C:\Program Files\Epic Games\UE_5.8",
    [string]$ToolchainVersion = "14.50.35717",
    [ValidateSet("Development", "DebugGame", "Shipping")]
    [string]$Configuration = "Development"
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$Project = Join-Path $RepoRoot "TheVeil.uproject"
$BuildBat = Join-Path $EngineRoot "Engine\Build\BatchFiles\Build.bat"
$LogDirectory = Join-Path $RepoRoot "Saved\BuildLogs"
$Timestamp = Get-Date -Format "yyyyMMdd-HHmmss"
$LogPath = Join-Path $LogDirectory "TheVeilEditor-$Configuration-$Timestamp.log"

if (-not (Test-Path $Project -PathType Leaf)) {
    throw "The Veil project was not found at: $Project"
}

if (-not (Test-Path $BuildBat -PathType Leaf)) {
    throw "Unreal Build Tool entry point was not found at: $BuildBat"
}

New-Item -ItemType Directory -Path $LogDirectory -Force | Out-Null

$BuildArguments = @(
    "TheVeilEditor"
    "Win64"
    $Configuration
    "-Project=$Project"
    "-WaitMutex"
    "-2026"
    "-VCToolchainVersion=$ToolchainVersion"
    "-architecture=x64"
)

Write-Host "Repository: $RepoRoot" -ForegroundColor Cyan
Write-Host "Project:    $Project" -ForegroundColor Cyan
Write-Host "Engine:     $EngineRoot" -ForegroundColor Cyan
Write-Host "Toolchain:  $ToolchainVersion" -ForegroundColor Cyan
Write-Host "Log:        $LogPath" -ForegroundColor Cyan
Write-Host "Building TheVeilEditor..." -ForegroundColor Green

& $BuildBat @BuildArguments 2>&1 | Tee-Object -FilePath $LogPath
$ExitCode = $LASTEXITCODE

if ($ExitCode -ne 0) {
    Write-Error "TheVeilEditor build failed with exit code $ExitCode. See: $LogPath"
    exit $ExitCode
}

Write-Host "TheVeilEditor build succeeded." -ForegroundColor Green
Write-Host "Build log: $LogPath"
exit 0
