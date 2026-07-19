[CmdletBinding()]
param(
    [switch]$SkipBuild,
    [switch]$SkipTests,
    [switch]$AllowIncompleteEditorIntegration
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
        throw 'git diff --check failed.'
    }

    Write-Host 'Playable Foundation verification completed.'
}
finally {
    Pop-Location
}
