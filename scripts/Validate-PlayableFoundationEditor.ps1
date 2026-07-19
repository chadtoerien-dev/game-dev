[CmdletBinding()]
param(
    [switch]$AllowIncomplete
)

$ErrorActionPreference = 'Stop'

$repositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$verifiedRoot = (git -C $repositoryRoot rev-parse --show-toplevel).Trim().Replace('/', '\')
if ($verifiedRoot -ne $repositoryRoot) {
    throw "Repository preflight failed. Expected '$repositoryRoot', found '$verifiedRoot'."
}

$engineExecutable = 'C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe'
$projectFile = Join-Path $repositoryRoot 'TheVeil.uproject'
$manifestFile = Join-Path $repositoryRoot 'scripts\PlayableFoundation\playable-foundation-manifest.json'
$pythonScript = Join-Path $repositoryRoot 'scripts\PlayableFoundation\validate_playable_foundation.py'
$evidenceDirectory = Join-Path $repositoryRoot 'Saved\PlayableFoundation'
$reportFile = Join-Path $evidenceDirectory 'validation-report.json'
$logFile = Join-Path $evidenceDirectory 'validation-commandlet.log'

foreach ($requiredFile in @($engineExecutable, $projectFile, $manifestFile, $pythonScript)) {
    if (-not (Test-Path -LiteralPath $requiredFile)) {
        throw "Required file not found: $requiredFile"
    }
}

New-Item -ItemType Directory -Force -Path $evidenceDirectory | Out-Null
$env:TV_PLAYABLE_MANIFEST = $manifestFile
$env:TV_PLAYABLE_REPORT = $reportFile
$env:TV_PLAYABLE_ALLOW_INCOMPLETE = if ($AllowIncomplete) { '1' } else { '0' }

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
    Remove-Item Env:TV_PLAYABLE_MANIFEST -ErrorAction SilentlyContinue
    Remove-Item Env:TV_PLAYABLE_REPORT -ErrorAction SilentlyContinue
    Remove-Item Env:TV_PLAYABLE_ALLOW_INCOMPLETE -ErrorAction SilentlyContinue
}

if (-not (Test-Path -LiteralPath $reportFile)) {
    Get-Content -LiteralPath $logFile | Select-Object -Last 80
    throw "Playable Foundation validation did not produce $reportFile"
}

$report = Get-Content -Raw -LiteralPath $reportFile | ConvertFrom-Json
$report.checks | Format-Table status, id, expected, actual -AutoSize
Write-Host "Automated status: $($report.automated_status)"
Write-Host "Acceptance status: $($report.acceptance_status)"
Write-Host "Failures: $($report.failures.Count)"
Write-Host "Map mutated by validation: $($report.map_mutated)"
Write-Host "Content mutated by validation: $($report.content_mutated)"
Write-Host "Report: $reportFile"

if ($commandExitCode -ne 0) {
    Get-Content -LiteralPath $logFile | Select-Object -Last 80
    throw "Playable Foundation validation failed with exit code $commandExitCode. See $logFile"
}
