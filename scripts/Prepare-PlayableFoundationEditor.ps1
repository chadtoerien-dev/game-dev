[CmdletBinding()]
param(
    [switch]$Apply,
    [switch]$ApplyMap
)

$ErrorActionPreference = 'Stop'

if ($ApplyMap -and -not $Apply) {
    throw '-ApplyMap requires -Apply because the map override references the generated Blueprints.'
}

$repositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$verifiedRoot = (git -C $repositoryRoot rev-parse --show-toplevel).Trim().Replace('/', '\')
if ($verifiedRoot -ne $repositoryRoot) {
    throw "Repository preflight failed. Expected '$repositoryRoot', found '$verifiedRoot'."
}

$engineExecutable = 'C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe'
$projectFile = Join-Path $repositoryRoot 'TheVeil.uproject'
$manifestFile = Join-Path $repositoryRoot 'scripts\PlayableFoundation\playable-foundation-manifest.json'
$pythonScript = Join-Path $repositoryRoot 'scripts\PlayableFoundation\setup_playable_foundation.py'
$evidenceDirectory = Join-Path $repositoryRoot 'Saved\PlayableFoundation'
$reportFile = Join-Path $evidenceDirectory 'setup-report.json'
$logFile = Join-Path $evidenceDirectory 'setup-commandlet.log'

foreach ($requiredFile in @($engineExecutable, $projectFile, $manifestFile, $pythonScript)) {
    if (-not (Test-Path -LiteralPath $requiredFile)) {
        throw "Required file not found: $requiredFile"
    }
}

if ($Apply) {
    $runningEditor = Get-Process -Name 'UnrealEditor', 'UnrealEditor-Cmd' -ErrorAction SilentlyContinue
    if ($runningEditor) {
        throw 'Close every Unreal Editor and UnrealEditor-Cmd process before applying the asset manifest.'
    }
}

New-Item -ItemType Directory -Force -Path $evidenceDirectory | Out-Null
$env:TV_PLAYABLE_MODE = if ($Apply) { 'apply' } else { 'dry-run' }
$env:TV_PLAYABLE_MANIFEST = $manifestFile
$env:TV_PLAYABLE_REPORT = $reportFile
$env:TV_PLAYABLE_APPLY_MAP = if ($ApplyMap) { '1' } else { '0' }
if ($Apply) {
    $env:TV_PLAYABLE_CONFIRM = 'CREATE_OR_REPAIR_PLAYABLE_FOUNDATION'
}
if ($ApplyMap) {
    $env:TV_PLAYABLE_MAP_CONFIRM = 'SET_PLAYABLE_FOUNDATION_SANDBOX_GAMEMODE'
}

try {
    $commandOutput = & $engineExecutable $projectFile `
        -run=PythonScript `
        "-script=$pythonScript" `
        -EnablePlugins=PythonScriptPlugin `
        -unattended `
        -nop4 `
        -NullRHI `
        -nosplash `
        -NoSound 2>&1
    $commandExitCode = $LASTEXITCODE
    $commandOutput | Out-File -LiteralPath $logFile -Encoding utf8
}
finally {
    Remove-Item Env:TV_PLAYABLE_MODE -ErrorAction SilentlyContinue
    Remove-Item Env:TV_PLAYABLE_MANIFEST -ErrorAction SilentlyContinue
    Remove-Item Env:TV_PLAYABLE_REPORT -ErrorAction SilentlyContinue
    Remove-Item Env:TV_PLAYABLE_APPLY_MAP -ErrorAction SilentlyContinue
    Remove-Item Env:TV_PLAYABLE_CONFIRM -ErrorAction SilentlyContinue
    Remove-Item Env:TV_PLAYABLE_MAP_CONFIRM -ErrorAction SilentlyContinue
}

if ($commandExitCode -ne 0) {
    Get-Content -LiteralPath $logFile | Select-Object -Last 80
    throw "Playable Foundation setup failed with exit code $commandExitCode. See $logFile"
}
if (-not (Test-Path -LiteralPath $reportFile)) {
    throw "Playable Foundation setup did not produce $reportFile"
}

$report = Get-Content -Raw -LiteralPath $reportFile | ConvertFrom-Json
$report.operations | Format-Table operation, asset, detail -AutoSize
Write-Host "Setup status: $($report.status)"
Write-Host "Map mutated by setup: $($report.map_mutated)"
Write-Host "Map persisted by setup: $($report.map_persisted)"
Write-Host "Report: $reportFile"

if ($Apply) {
    Write-Warning 'Binary assets were created or repaired. Review the exact Content file list before staging.'
} else {
    Write-Host 'Dry-run only. Re-run with -Apply after reviewing the operations above.'
}
