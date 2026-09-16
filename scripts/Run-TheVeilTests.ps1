param(
    [string]$EngineRoot = "C:\Program Files\Epic Games\UE_5.8",
    [string]$TestFilter = "TheVeil",
    [switch]$WithRendering,
    [string]$PythonExecutable = "python"
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$Project = Join-Path $RepoRoot "TheVeil.uproject"
$EditorCmd = Join-Path $EngineRoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
$Timestamp = (Get-Date -Format "yyyyMMdd-HHmmss-fff") + "-" + [Guid]::NewGuid().ToString("N").Substring(0, 8)
$ReportDirectory = Join-Path $RepoRoot "Saved\AutomationReports\$Timestamp"
$LogPath = Join-Path $ReportDirectory "TheVeil-Automation.log"

if (-not (Test-Path $Project -PathType Leaf)) {
    throw "The Veil project was not found at: $Project"
}

if (-not (Test-Path $EditorCmd -PathType Leaf)) {
    throw "UnrealEditor-Cmd.exe was not found at: $EditorCmd"
}

$Validator = Join-Path $PSScriptRoot "Automation\validate_report.py"
$SourceRoot = Join-Path $RepoRoot "Source"
& $PythonExecutable $Validator --source-root $SourceRoot --filter $TestFilter --discover-only
if ($LASTEXITCODE -ne 0) { throw "Requested test discovery failed; Unreal was not started." }

New-Item -ItemType Directory -Path $ReportDirectory | Out-Null
$StartedAt = [DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds() / 1000.0

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

& $PythonExecutable $Validator --source-root $SourceRoot --filter $TestFilter --report (Join-Path $ReportDirectory "index.json") --started-at $StartedAt.ToString([Globalization.CultureInfo]::InvariantCulture)
if ($LASTEXITCODE -ne 0) { throw "Automation report validation failed. See: $ReportDirectory" }

Write-Host "Automation report validated successfully." -ForegroundColor Green
Write-Host "Review the report and log at: $ReportDirectory"
exit 0
