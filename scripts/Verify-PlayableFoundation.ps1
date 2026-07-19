[CmdletBinding()]
param(
    [switch]$SkipBuild,
    [switch]$SkipTests,
    [switch]$AllowIncompleteEditorIntegration,
    [string]$BaseRef = 'origin/main'
)

$ErrorActionPreference = 'Stop'
$repositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path

Push-Location $repositoryRoot
try {
    $verifiedRoot = (git rev-parse --show-toplevel).Trim().Replace('/', '\')
    if ($verifiedRoot -ne $repositoryRoot) {
        throw "Repository preflight failed. Expected '$repositoryRoot', found '$verifiedRoot'."
    }

    if (-not $SkipBuild) {
        & powershell -ExecutionPolicy Bypass -File '.\scripts\Build-TheVeil.ps1'
        if ($LASTEXITCODE -ne 0) {
            throw "TheVeilEditor build failed with exit code $LASTEXITCODE."
        }
    }

    if (-not $SkipTests) {
        $pythonCommand = Get-Command python -ErrorAction Stop
        & $pythonCommand.Source '.\scripts\PlayableFoundation\test_playable_foundation_contract.py'
        if ($LASTEXITCODE -ne 0) {
            throw "Playable Foundation manifest-contract tests failed with exit code $LASTEXITCODE."
        }

        & powershell -ExecutionPolicy Bypass -File '.\scripts\Run-TheVeilTests.ps1'
        if ($LASTEXITCODE -ne 0) {
            throw "TheVeil automation failed with exit code $LASTEXITCODE."
        }
    }

    $validationArguments = @(
        '-ExecutionPolicy', 'Bypass',
        '-File', '.\scripts\Validate-PlayableFoundationEditor.ps1'
    )
    if ($AllowIncompleteEditorIntegration) {
        $validationArguments += '-AllowIncomplete'
    }
    & powershell @validationArguments
    if ($LASTEXITCODE -ne 0) {
        throw "Playable Foundation Editor validation failed with exit code $LASTEXITCODE."
    }

    git diff --check
    if ($LASTEXITCODE -ne 0) {
        throw 'git diff --check failed for the working tree.'
    }

    git diff --cached --check
    if ($LASTEXITCODE -ne 0) {
        throw 'git diff --cached --check failed for the index.'
    }

    git rev-parse --verify --quiet $BaseRef | Out-Null
    if ($LASTEXITCODE -ne 0) {
        throw "Verification base ref '$BaseRef' does not exist."
    }

    git diff --check "$BaseRef...HEAD"
    if ($LASTEXITCODE -ne 0) {
        throw "git diff --check failed for $BaseRef...HEAD."
    }

    Write-Host 'Playable Foundation verification completed.'
}
finally {
    Pop-Location
}
