# Verify the first Windows package

Date: 16 September 2026
Status: Development package and startup verification passed; rendered controls and wider delivery gates remain open

## Objective and value

Build, cook, stage and archive the existing sandbox as a Windows Development application. Detect deployment defects before expanding the playable loop. This is a prototype delivery check, not acceptance of the complete level or graphics.

## Read first

AGENTS.md, docs/tasks/playable-level-delivery.md, docs/context/AGENT_HANDOFF.md, docs/technical/TECHNICAL_ARCHITECTURE.md, docs/evidence/2026-09-16-delivery-baseline.md, project targets/config and current GameMode.

## Current and required behaviour

Editor compilation and 21 automation tests pass with an explicit installed-compiler override. Packaged operation has not been verified. Produce a runnable package with the sandbox, required Blueprint/input assets and runtime-spawned development actors. Inspect actual build/cook/stage output and any warnings. If packaging fails, retain the first causal error before proposing a bounded correction.

## Scope and non-goals

Run Unreal Automation Tool against the current source/content. Generated output stays under ignored Saved/. Document the command and actual evidence. No map, binary asset, engine or gameplay edits in this verification pass. No final-art, complete-round, clean-machine or Shipping acceptance claims from a Development package.

## Acceptance criteria

1. UAT returns success for build, cook, stage and archive.
2. The archive contains the executable and cooked map/content containers.
3. The packaged application starts and loads the intended map without fatal errors; label unattended/non-rendered checks accurately.
4. Rendered interaction, controls, performance and clean-machine playthrough remain separate human/runtime gates.
5. Record exact source commit, compiler override, command, outputs and limitations.

## Verification command

```powershell
& 'C:/Program Files/Epic Games/UE_5.8/Engine/Build/BatchFiles/RunUAT.bat' BuildCookRun '-project=C:/devprojects/game-dev/TheVeil.uproject' -noP4 -platform=Win64 -clientconfig=Development -build -cook '-map=/Game/Maps/L_Dev_Sandbox' -stage -pak -archive '-archivedirectory=C:/devprojects/game-dev/Saved/Packages/Development-20260916' '-ubtargs=-CompilerVersion=14.51.36231 -MaxParallelActions=1' -unattended -utf8output
```

Log: Saved/BuildLogs/Package-Development-20260916.log. The compiler override is diagnostic and does not replace the approved 14.50 baseline. UAT's installed ProjectParams.cs supports ubtargs forwarding. Before rerunning, check whether the original process is still live; never restart merely because a polling interval elapsed.

## Risks and rollback

Cooking may expose missing references or editor-only dependencies. Packaging may take substantial time; preserve live-process handles and report progress. Do not delete shared caches or source assets to fix a cook. No tracked production content changes are planned; review any unexpected Git changes before proceeding.

## Demonstrated blocker and scope amendment

The initial UAT attempt returned exit 6 before cooking. Game-target compilation failed at four interaction dispatch sites because UClass::ClassGeneratedBy is editor-only data. A separate bounded correction is tracked in docs/tasks/packaged-interaction-dispatch.md; source changes in that task must be included in the package identity and reviewed. The failed baseline log above remains intact. Subsequent attempts use distinct logs.

## Routing

Current Codex lead handles this bounded verification with local UAT access. A separate reviewer checks evidence/limitations after completion. Chad retains rendered gameplay, visual acceptance and merge authority.
