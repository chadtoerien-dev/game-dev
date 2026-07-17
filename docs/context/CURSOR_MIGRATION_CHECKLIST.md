# ChatGPT-to-Cursor Migration Checklist

Last updated: 17 July 2026

## Goal

Finish with one authoritative repository in which:

- all project prompts, decisions, design documents and history are versioned
- the verified UE5.8 source fixes are on `main`
- the sandbox map and all World Partition companion files are on `main`
- Cursor and other agents read durable repository instructions instead of relying on chat history
- local `main` and `origin/main` are identical

## Safety warning: use the correct repository

Project Git commands must run from:

```text
C:\devprojects\game-dev
```

They must not run from:

```text
C:\Users\chadt
```

The home directory is a different Git repository on branch `design`. Its large untracked-file list is unrelated to The Veil. Do not add, clean, reset or delete files there as part of this migration.

## Step 1 — Enter and verify the project repository

Paste only this block into PowerShell:

```powershell
Set-Location "C:\devprojects\game-dev"

$ExpectedRoot = (Resolve-Path "C:\devprojects\game-dev").Path
$ActualRoot = (git rev-parse --show-toplevel).Trim()
$Branch = (git branch --show-current).Trim()

Write-Host "Expected root: $ExpectedRoot"
Write-Host "Actual root:   $ActualRoot"
Write-Host "Branch:        $Branch"

if ((Resolve-Path $ActualRoot).Path -ne $ExpectedRoot) {
    throw "STOP: Git is not operating in the The Veil repository."
}
```

The actual root must resolve to `C:\devprojects\game-dev`.

## Step 2 — Confirm the remote

```powershell
git remote -v
git status --short
```

The `origin` remote must point to:

```text
https://github.com/chadtoerien-dev/game-dev.git
```

Do not continue if it points anywhere else.

## Step 3 — Confirm or switch to local main

```powershell
git branch --show-current
```

When the working tree contains the expected The Veil changes, switch only if safe:

```powershell
git switch main
```

Do not use `git reset --hard` and do not clean untracked content.

## Step 4 — Commit the local sandbox and project defaults

Review the expected files:

```powershell
git status --short
```

Expected local additions or modifications include:

- `.gitignore`
- `.vsconfig`
- `Config/DefaultEngine.ini`
- `Config/DefaultInput.ini`
- `Content/Maps/L_Dev_Sandbox.umap`
- `Content/Maps/L_Dev_Sandbox_HLOD0_Instancing.uasset`
- `Content/__ExternalActors__/Maps/L_Dev_Sandbox/`
- `Content/__ExternalObjects__/Maps/L_Dev_Sandbox/`

Stage only these paths:

```powershell
git add -- ".gitignore" ".vsconfig" "Config/DefaultEngine.ini" "Config/DefaultInput.ini" "Content"
```

Inspect the staged file list:

```powershell
git diff --cached --name-status
```

Do not continue if the staged list contains files from `C:\Users\chadt`, generated Unreal folders, secrets, API keys, `.slnx`, or unrelated projects.

Commit:

```powershell
git commit -m "Add development sandbox map and project defaults"
```

## Step 5 — Reconcile with the merged handover work

After the Cursor handover pull request has merged to `main`:

```powershell
git fetch origin
git pull --rebase origin main
```

This replays the local UE5.8 repair and sandbox commits on top of the current remote documentation history.

When a conflict occurs, stop and inspect it. Do not choose `ours` or `theirs` blindly. The expected migration documentation should coexist with the local source and content changes.

## Step 6 — Re-run verification after the rebase

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Build-TheVeil.ps1"
```

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Run-TheVeilTests.ps1"
```

Launch and run a short PIE smoke test:

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Launch-TheVeil.ps1"
```

Success means:

- editor opens
- `L_Dev_Sandbox` loads
- PIE begins
- PIE exits with `Esc`
- no crash or fatal error
- all five automation tests pass

## Step 7 — Push local main

```powershell
git push origin main
```

If Git rejects the push, do not force-push. Fetch and inspect the difference:

```powershell
git fetch origin
git log --oneline --decorate --graph --all -20
git rev-list --left-right --count main...origin/main
```

Resolve by a normal rebase or merge review. Never use `--force` on `main` for this migration.

## Step 8 — Prove local and remote are identical

```powershell
git fetch origin
$Counts = git rev-list --left-right --count main...origin/main
Write-Host "Local-only / remote-only commits: $Counts"
git status --short
```

Completion requires:

```text
0    0
```

and no unexpected working-tree output.

## Step 9 — Verify required files on main

```powershell
git ls-tree -r --name-only origin/main | Select-String -Pattern @(
    '^AGENTS.md$',
    '^\.cursor/rules/00-the-veil-core\.mdc$',
    '^docs/context/AGENT_HANDOFF\.md$',
    '^docs/context/CURSOR_MIGRATION_CHECKLIST\.md$',
    '^docs/production/ROADMAP\.md$',
    '^docs/production/AI_AGENT_WORKFLOW\.md$',
    '^docs/production/TASK_BRIEF_TEMPLATE\.md$',
    '^scripts/Build-TheVeil\.ps1$',
    '^scripts/Launch-TheVeil\.ps1$',
    '^scripts/Run-TheVeilTests\.ps1$',
    '^Content/Maps/L_Dev_Sandbox\.umap$'
)
```

Also confirm the source-repair commit or rebased successor is visible:

```powershell
git log --oneline --all --grep="Fix UE5.8 build"
```

## Step 10 — Open the repository in Cursor

From PowerShell:

```powershell
Set-Location "C:\devprojects\game-dev"
cursor .
```

If the `cursor` shell command is not installed, open Cursor manually and choose `File -> Open Folder`, then select `C:\devprojects\game-dev`.

Allow codebase indexing to complete before delegating architecture-wide work.

## Step 11 — First Cursor session

Create a fresh Agent thread and paste:

```text
Read AGENTS.md, docs/context/AGENT_HANDOFF.md, docs/production/ROADMAP.md, docs/production/AI_AGENT_WORKFLOW.md, docs/production/TASK_BRIEF_TEMPLATE.md, docs/technical/TECHNICAL_ARCHITECTURE.md and INTEGRATION.md.

Do not edit files yet.

Confirm:
1. the current verified project state;
2. the authority order;
3. the build, test and launch commands;
4. the immediate milestone;
5. the systems and files that are out of scope;
6. any contradictions or missing repository evidence.

Then draft a task brief for Playable Foundation 0.5: third-person character, camera and input.
```

Review the resulting task brief before allowing implementation.

## Migration completion criteria

The migration is complete only when all are true:

- [ ] Cursor handover PR merged to `main`
- [ ] UE5.8 source-repair commit on `main`
- [ ] sandbox map and every World Partition companion asset on `main`
- [ ] config and `.vsconfig` on `main`
- [ ] generated `.slnx` files ignored
- [ ] local `main` equals `origin/main`
- [ ] UE editor target builds after reconciliation
- [ ] five automation tests pass after reconciliation
- [ ] sandbox PIE smoke test passes after reconciliation
- [ ] Cursor reads `AGENTS.md` and the project rule
- [ ] first 0.5 task brief approved by the human owner
