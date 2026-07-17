param(
    [string]$EngineRoot = "C:\Program Files\Epic Games\UE_5.8",
    [switch]$Wait
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$Project = Join-Path $RepoRoot "TheVeil.uproject"
$Editor = Join-Path $EngineRoot "Engine\Binaries\Win64\UnrealEditor.exe"

if (-not (Test-Path $Project -PathType Leaf)) {
    throw "The Veil project was not found at: $Project"
}

if (-not (Test-Path $Editor -PathType Leaf)) {
    throw "Unreal Editor was not found at: $Editor"
}

Write-Host "Launching The Veil..." -ForegroundColor Green
Write-Host "Project: $Project" -ForegroundColor Cyan
Write-Host "Editor:  $Editor" -ForegroundColor Cyan

$Process = Start-Process `
    -FilePath $Editor `
    -ArgumentList "`"$Project`"" `
    -WorkingDirectory $RepoRoot `
    -PassThru

Write-Host "Unreal Editor process ID: $($Process.Id)"

if ($Wait) {
    $Process.WaitForExit()
    exit $Process.ExitCode
}

exit 0
