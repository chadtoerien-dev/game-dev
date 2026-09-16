# Preserve interaction dispatch in packaged builds

Date: 16 September 2026

Verification update: the bounded correction passed independent source review, game/editor compilation, Development build/cook/stage/archive and all 21 automation tests. Package startup probes exited 0. See docs/evidence/2026-09-16-first-windows-package.md. Genuine Blueprint override execution, controls and merge acceptance remain open.
Status: bounded correction verified and independently reviewed; human merge acceptance pending

## Objective and production value

Remove an editor-only dependency that prevents the Windows game target compiling, while retaining native interaction and Blueprint overrides. This unblocks the early package verification; it does not accept the playable level.

## Current and required behaviour

The first package attempt reports four C2039 errors in TVInteractionComponent.cpp: UClass::ClassGeneratedBy is excluded by WITH_EDITORONLY_DATA in the game target. The existing native/reflected dispatch split was introduced after an observed native dispatch mismatch and independent review of Blueprint overrides (0.6 evidence). Preserve that split using runtime class flags.

## Read first

- AGENTS.md and the production workflow/delegation protocol
- docs/tasks/early-windows-package.md
- docs/evidence/0.6-interaction-camera-feel-automated-verification.md
- docs/context/AGENT_HANDOFF.md and docs/technical/TECHNICAL_ARCHITECTURE.md
- TVInteractionComponent, TVInteractable, TVInteractionTestActors and TVInteractionAutomationTests

## Scope, constraints and expected files

Change only Source/TheVeil/Interaction/TVInteractionComponent.cpp and this brief. Replace the four editor-only class checks with HasAnyClassFlags(CLASS_CompiledFromBlueprint), preserving all dispatch and validity branches. No interface, content, save, input, trace, social or engine changes. British English; no binary edits; dedicated codex/package-interaction-fix branch. No commits or merges by the implementer.

## Acceptance criteria

1. Windows Development game target compiles without editor-only class data.
2. Native prompt, eligibility, activation and result-feedback behaviour remains intact.
3. Blueprint-generated actors continue through Execute_* so Blueprint overrides remain eligible for reflected dispatch.
4. Invalid and non-interactable actors retain their existing rejection paths.
5. Editor build and existing automation pass; package results and manual limitations are recorded accurately.

## Verification

The parent agent runs scripts/Build-TheVeil.ps1 and scripts/Run-TheVeilTests.ps1 with the documented installed-compiler override, then the exact UAT command in early-windows-package.md. Run git diff --check. Existing FocusPromptAndActivation covers native prompt, eligibility, activation count and feedback. A game-target build is the regression check for the excluded field; editor tests alone cannot detect it. No artificial flag-mutation test is added because it would not exercise a genuine cooked Blueprint override. Blueprint override behaviour still needs an editor-created test actor or suitable existing asset and rendered/manual evidence.

## Source rationale

Installed UE 5.8 CoreUObject/Public/UObject/Class.h places ClassGeneratedBy inside WITH_EDITORONLY_DATA, while ClassFlags is outside that guard. ObjectMacros.h describes CLASS_CompiledFromBlueprint as identifying classes created from Blueprint source. The installed runtime BlueprintGeneratedClass.cpp also uses HasAnyClassFlags(CLASS_CompiledFromBlueprint). Generated TVInteractable Execute_* wrappers route through FindFunction/ProcessEvent before native fallback; those wrappers remain unchanged.

## Risks and rollback

An editor-only test run cannot establish packaged success. This change preserves the earlier native dispatch workaround rather than redesigning reflection behaviour. Revert only the four predicate replacements to roll back; doing so restores the known packaging failure. Do not remove generated assets or shared caches.

## Routing and human checkpoint

Planner: parent Codex agent. Implementer: bounded Codex Unreal C++ agent with installed engine-source access. Independent reviewer: separate parent-assigned agent before acceptance. Chad owns rendered interaction acceptance and merge approval. Builds/tests are delegated to the parent to avoid simultaneous Unreal processes.
