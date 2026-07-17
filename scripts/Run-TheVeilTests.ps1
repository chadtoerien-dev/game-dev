param(
    [string]$EngineRoot = "C:\Program Files\Epic Games\UE_5.8",
    [string]$TestFilter = "TheVeil",
    [switch]$WithRendering
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$Project = Join-Path $RepoRoot "TheVeil.uproject"
$EditorCmd = Join-Path $EngineRoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
$Timestamp = Get-Date -Format "yyyyMMdd-HHmmss"
$ReportDirectory = Join-Path $RepoRoot "Saved\AutomationReports\$Timestamp"
$LogPath = Join-Path $ReportDirectory "TheVeil-Automation.log"

if (-not (Test-Path $Project -PathType Leaf)) {
    throw "The Veil project was not found at: $Project"
}

if (-not (Test-Path $EditorCmd -PathType Leaf)) {
    throw "UnrealEditor-Cmd.exe was not found at: $EditorCmd"
}

New-Item -ItemType Directory -Path $ReportDirectory -Force | Out-Null

$Arguments = @(
    $Project
    "-ExecCmds=Automation RunTest $TestFilter;Quit"
    "-ReportExportPath=$ReportDirectory"
    "-unattended"
    "-nop4"
    "-nosplash"
    "-stdout"
    "-FullStdOutLogOutput"
)

if (-not $WithRendering) {
    $Arguments += "-NullRHI"
}

Write-Host "Running Unreal automation tests..." -ForegroundColor Green
Write-Host "Filter:  $TestFilter" -ForegroundColor Cyan
Write-Host "Project: $Project" -ForegroundColor Cyan
Write-Host "Report:  $ReportDirectory" -ForegroundColor Cyan

& $EditorCmd @Arguments 2>&1 | Tee-Object -FilePath $LogPath
$ExitCode = $LASTEXITCODE

if ($ExitCode -ne 0) {
    Write-Error "Automation tests failed or the editor command exited abnormally with code $ExitCode. See: $LogPath"
    exit $ExitCode
}

Write-Host "Automation command completed successfully." -ForegroundColor Green
Write-Host "Review the report and log at: $ReportDirectory"
exit 0
