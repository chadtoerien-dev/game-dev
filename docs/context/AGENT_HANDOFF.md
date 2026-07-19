# Agent and Contributor Handover

Last updated: 19 July 2026

## Mission

Continue designing and implementing **The Veil**, an original third-person social-deduction action RPG in Unreal Engine 5.8, towards a polished 25–30 minute PC vertical slice.

The intended experience combines:

- responsive third-person action-adventure movement
- social access, persuasion, charisma and deception
- observer-specific memories and rumours
- public claims and contradictions
- explainable voting and exile
- active night operations for both factions
- cinematic but original stealth and social-suspense presentation

## Executive state

The native social-simulation foundation has reached milestone **0.4**. The native and saved structural integration for milestone **0.5** are implemented and verified locally; subjective Editor/PIE acceptance and independent review remain.

Verified on 17 July 2026:

- `TheVeilEditor` Development/Win64 compiled and linked successfully
- UnrealHeaderTool completed successfully
- five `TheVeil.*` automation tests passed
- result: 5 tests, 0 failures, 0 skips
- Unreal Editor opened the project successfully
- `L_Dev_Sandbox` loaded
- Play-in-Editor started and exited without a crash or fatal error

Verified on 19 July 2026:

- `TheVeilEditor` Development/Win64 build succeeded
- all eleven `TheVeil.*` automation tests passed with zero failures and zero skips, including movement-basis, binding idempotence and controller cleanup coverage
- manifest-driven Editor setup persisted five Input Actions, one Input Mapping Context and three derived Blueprints
- `BP_TheVeilCharacter` contains one collision-free, navigation-neutral temporary ellipsoid visual attached to the native capsule
- `L_Dev_Sandbox` persists `BP_TheVeilGameMode`, one loaded PlayerStart and zero placed auto-possessed pawns
- strict read-only structural validation passed all 60 automated checks and dirtied neither map nor content
- compilation/link evidence: `Saved/BuildLogs/TheVeilEditor-Development-20260719-025121.log`
- final automation evidence: `Saved/AutomationReports/20260719-030032` (11 succeeded, 0 failed, 0 skipped)
- durable PR evidence: `docs/evidence/0.5-playable-foundation-automated-verification.md`
- structural evidence: `Saved/PlayableFoundation/validation-report.json` (`automated_status: pass`, 0 failures)
- King’s College, Cambridge is the approved one-to-one-scale, maximum-source-fidelity architectural basis for the bounded vertical-slice footprint

The saved playable shell now exists. The next gate is a human Editor/PIE acceptance session for physical controller parity, camera feel/collision, focus-loss recovery, spawn clearance, the two-minute soak and capture evidence.

## Repository and local environment

- GitHub: `https://github.com/chadtoerien-dev/game-dev`
- Default branch: `main`
- Local repository: `C:\devprojects\game-dev`
- Project: `C:\devprojects\game-dev\TheVeil.uproject`
- Engine: `C:\Program Files\Epic Games\UE_5.8`
- Platform: Windows x64
- Visual Studio: Community 2026 at `C:\Program Files\Microsoft Visual Studio\18\Community`
- Selected MSVC folder: `14.50.35717`
- Compiler reported by Unreal: Visual Studio `14.50.35737`
- Windows SDK: `10.0.22621.0`

Before any Git command that changes state, run:

```powershell
git rev-parse --show-toplevel
git branch --show-current
```

The top-level path must be `C:/devprojects/game-dev` and the intended branch must be explicit.

### Important Git incident

A commit attempt was accidentally run from `C:\Users\chadt`, which is a different Git repository on branch `design`. That produced permission warnings and listed the entire Windows profile as untracked. No project files were committed there.

Do not run project Git commands from the home directory. Do not add, clean or delete files in that unrelated repository while working on The Veil.

## Authority order

When documents conflict, use this order:

1. `MASTER_PROMPT.md`
2. `REFERENCE_MUSIC_ADDENDUM.md`
3. `DECISION_LOG.md`
4. this handover
5. `docs/production/ROADMAP.md`
6. `docs/technical/TECHNICAL_ARCHITECTURE.md`
7. current code, tests, config and `INTEGRATION.md`
8. historical chat and milestone archives

For multi-model work, also read:

- root `AGENTS.md`
- `docs/production/AI_AGENT_WORKFLOW.md`
- `docs/production/TASK_BRIEF_TEMPLATE.md`

## Build repairs completed locally

The successful local source repair was committed as:

```text
9a87fc1 Fix UE5.8 build and validate milestone 0.4
```

A successor commit may replace that hash after rebasing, so verify behaviour rather than relying only on the identifier.

Repairs included:

1. Updated both target files from `BuildSettingsVersion.V6` to `V7`.
2. Added the module directory as a public include root because the source tree uses includes such as `Core/TVGameplayTypes.h` and `Voting/TVVotingProfile.h`.
3. Restored the previously truncated `TVVotingProfile.h` definition.
4. Replaced the invalid `Engine/PrimaryDataAsset.h` include with `Engine/DataAsset.h`.
5. Replaced invalid runtime conditional verbosity inside `UE_LOG` with explicit success and failure log branches.
6. Used MSVC toolchain `14.50.35717`, the UE5.8-compatible installed toolchain.

The earlier compiler errors are resolved.

## Build command

Preferred:

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Build-TheVeil.ps1"
```

Underlying command:

```powershell
$UE = "C:\Program Files\Epic Games\UE_5.8"
$Project = "C:\devprojects\game-dev\TheVeil.uproject"

& "$UE\Engine\Build\BatchFiles\Build.bat" TheVeilEditor Win64 Development "-Project=$Project" -WaitMutex -2026 "-VCToolchainVersion=14.50.35717" "-architecture=x64"
```

Do not split the toolchain value across lines or PowerShell may pass an invalid version.

## Launch command

Preferred:

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Launch-TheVeil.ps1"
```

The project should open directly in Unreal Editor 5.8.

## Automation tests

Preferred:

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Run-TheVeilTests.ps1"
```

Manual route:

1. Open Unreal Editor.
2. Open `Tools -> Session Frontend`.
3. Select the active editor session.
4. Open the Automation tab.
5. Search for `TheVeil`.
6. Run all tests under the parent group.

Expected tests:

- `TheVeil.Phase.ShortLoop`
- `TheVeil.Information.ProvenanceTransmission`
- `TheVeil.Information.InterpretationRuleSelection`
- `TheVeil.Claims.DirectContradiction`
- `TheVeil.Claims.LocationContradiction`
- `TheVeil.PlayableFoundation.CharacterDefaults`
- `TheVeil.PlayableFoundation.CharacterStateTransitions`
- `TheVeil.PlayableFoundation.ControllerBindings`
- `TheVeil.PlayableFoundation.ControllerTransientCleanup`
- `TheVeil.PlayableFoundation.GameModeDefaults`
- `TheVeil.PlayableFoundation.MovementBasis`

## Current content state

Development map:

```text
/Game/Maps/L_Dev_Sandbox
```

Local file:

```text
Content/Maps/L_Dev_Sandbox.umap
```

The map uses World Partition and includes required files under:

- `Content/__ExternalActors__/Maps/L_Dev_Sandbox/`
- `Content/__ExternalObjects__/Maps/L_Dev_Sandbox/`
- `Content/Maps/L_Dev_Sandbox_HLOD0_Instancing.uasset`

These files are part of the map. Never delete or hand-edit them because their names are opaque.

At the verification point, content comprised approximately 145 files and 31.24 MB. The largest individual asset was under 1 MB.

`DefaultEngine.ini` points both editor and game startup maps to:

```text
/Game/Maps/L_Dev_Sandbox.L_Dev_Sandbox
```

The sandbox can enter and exit the previously verified pre-0.5 PIE smoke test. The native player, movement, camera and GameMode classes exist; the saved map now references the manifest-generated Blueprint and Enhanced Input assets and passes strict structural validation. Physical-input, camera-feel and final visual acceptance remain pending.

## Native architecture implemented

Current causal pipeline:

```text
objective gameplay event
→ observer-specific memory
→ report, rumour or deliberate lie
→ audience-specific claim
→ contradiction
→ directed credibility and suspicion changes
→ explainable vote intention
→ locked vote
```

Implemented native foundations include:

- authoritative round and phase state
- checkpoint save and restore
- Opportunity Threads
- routine and contextual access evaluation
- evidence persistence and Dawn surfacing
- event, witness, memory and provenance systems
- reports, rumours and deliberate lies
- directed NPC-to-NPC relationships
- public and private claims
- contradiction detection
- credibility consequences
- data-driven vote scoring and deterministic vote locking
- adaptive-audio state arbitration foundation

## Critical simulation rules

- An NPC may only use information it has witnessed, received or inferred from allowed data.
- Objective culprit truth must never leak directly into NPC decisions.
- Public knowledge is not global knowledge; every claim has an audience.
- A contradiction affects only contestants who know both claims.
- Directed relationships are not symmetric.
- Voting must remain explainable in development tools.
- Release UI should not expose exact hidden score values.
- Save and restore must not rebroadcast historical events and create duplicate memories.

## Current GitHub sync boundary

The context archive, original prompts, design documents, milestone documents, source foundation and historical audit are already on GitHub `main`.

The following local work must be present on `main` before the migration is declared complete:

- the UE5.8 source-repair commit or its rebased successor
- `.gitignore` support for generated `.slnx` files
- `.vsconfig`
- `Config/DefaultEngine.ini`
- `Config/DefaultInput.ini`
- the complete `Content/` tree for `L_Dev_Sandbox`
- the Cursor handover, roadmap, model-routing and script changes from the migration PR

Use `docs/context/CURSOR_MIGRATION_CHECKLIST.md` for the final reconciliation procedure.

## Immediate next milestone

The next milestone is **Playable Foundation 0.5**, not final art and not a larger estate.

Required result:

```text
launch sandbox
→ spawn controlled third-person character
→ responsive camera-relative movement
→ sprint, crouch and jump
→ stable camera and collision
→ exit PIE cleanly
```

The reusable interaction trace and test-door loop belong to Milestone 0.6 after the 0.5 human acceptance gate.

Acceptance criteria are defined in the roadmap.

After that:

1. embody placeholder contestants with navigation and visible state
2. connect social actions to observable NPC behaviour
3. build an interactive Tribunal and Exile loop
4. build one active night operation
5. integrate one complete round
6. add presentation and performance polish

## Known limitations

- No polished player locomotion or animation stack
- No approved physical-controller or camera-feel evidence for the saved playable shell
- No human-approved PlayerStart clearance/`BADsize` evidence or two-minute PIE capture
- No embodied NPC StateTrees in the saved sandbox
- No playable conversation interface
- No interactive Tribunal UI
- No complete night-operation level
- No final audio, MetaSounds or original score assets
- No final manor art, cinematics or accessibility pass
- No packaged Development build yet
- No CI runner currently proves the Windows UE build on every PR

## Immediate human checkpoint

Use the preferred automation route in `INTEGRATION.md`:

1. review the checked binary list and strict structural report;
2. open the Editor and inspect the existing PlayerStart for walkable-ground clearance and absence of `BADsize` without moving it unless necessary;
3. approve or replace the collision-free temporary visual without adding gameplay or collision authority;
4. complete the keyboard/controller, camera collision, focus-loss and two-minute PIE checklist;
5. return the capture, Editor log and any legitimate post-inspection binary changes for independent review.

## Definition of done for every milestone

Every milestone must record:

- completed behaviour
- files and assets added or changed
- integration procedure
- exact build command and result
- automated test command and result
- manual editor or PIE test result
- screenshots or video where behaviour is visual
- known limitations
- next recommended task
