# First Windows Development package

Date: 16 September 2026
Branch: codex/package-interaction-fix
Source identity: fb3d5c5 plus the four reviewed runtime-class-flag substitutions in TVInteractionComponent.cpp. No binary assets changed.

## Failure and correction

Initial UAT log Saved/BuildLogs/Package-Development-20260916.log: exit 6. Game compilation failed at four ClassGeneratedBy references because that UClass field is editor-only. Editor-only compilation had not exposed the defect.

Correction: replace ClassGeneratedBy == nullptr with !HasAnyClassFlags(CLASS_CompiledFromBlueprint) at the four native-dispatch gates. Native dispatch and Blueprint Execute_* dispatch remain separate as before. Independent source review approved with no findings. No synthetic class-flag test was added; real game-target compilation is the regression evidence for the missing runtime field.

## Build, cook and archive

Command: docs/tasks/early-windows-package.md (same command, corrected source).

Corrected log: Saved/BuildLogs/Package-Development-dispatch-fix-20260916.log.

- Game and editor targets compiled and linked.
- Cook commandlet reported success, zero errors and zero warnings.
- Staging and archive completed; BuildCookRun 159.92 seconds; UAT exit 0.
- Compiler 14.51.36256 / toolchain 14.51.36231 was an explicit diagnostic override, not approval of a new baseline.
- Overall build warnings remain: non-preferred compiler, missing Visual Studio SDK editor integration and engine-header deprecations. The entire package process is not claimed warning-free.

Archive: Saved/Packages/Development-20260916/Windows. The directory contains the bootstrap executable, game executable, .pak/.utoc/.ucas containers and runtime dependencies. Copy the complete directory when testing; the executable alone is insufficient.

SHA-256 identities:

- TheVeil/Binaries/Win64/TheVeil.exe: 500F8CF290523F084BB3D08DEC790C675DBCCF062D083EADEB92CB7F7DFD725D
- TheVeil/Content/Paks/TheVeil-Windows.ucas: B7C65AD68177AD7FF02E507E922D4669F9D5533EE9585485EDB8003E882B41DA

## Runtime probes and regression tests

Non-rendered package arguments: -nullrhi -unattended -nosound -seconds=15, with absolute log path Saved/BuildLogs/Package-Development-smoke-20260916.log. Process exited 0. Log confirms L_Dev_Sandbox, BP_TheVeilGameMode_C, spawned door/object and orderly engine shutdown. This is not a controls or camera test.

Rendered package arguments: -windowed -ResX=1280 -ResY=720 -unattended -nosound -seconds=15 and quoted -ExecCmds="HighResShot 1". Launched hidden to avoid disrupting the desktop. Log: Saved/BuildLogs/Package-Development-render-20260916.log. Process exited 0; D3D12 selected NVIDIA GeForce RTX 4060 Laptop GPU with 7956 MB dedicated video memory. This is a short startup probe, not performance acceptance or a played encounter.

Actual 1280x720 capture: Saved/Packages/Development-20260916/Windows/TheVeil/Saved/Screenshots/Windows/HighresScreenshot00000.png. Captured near startup (frame 6); current default settings, not a controlled final-quality comparison. No audio or animation capture was assessed.

Fresh full automation: Saved/AutomationReports/20260916-225256-016-a2727ab4/index.json. Hardened runner validated all 21 expected tests, zero test warnings/errors/failures/skips; exit 0. Independent reviewer inspected the UAT result, individual test results and smoke log.

## Visual baseline, not acceptance

The actual captured player remains an ellipsoid; NPCs are composed primitive shapes and ground uses checkerboard surfaces. The estate, realistic cast, final materials and required gameplay spaces are not delivered by this capture. This fails the visual rubric's conspicuous-placeholder gate and provides no basis for an 8/10 claim.

The current player constructor does not create a humanoid mesh, and the saved-content setup manifest still specifies the ellipsoid. Earlier 0.7a wording about replacing the player visual must not be treated as current rendered evidence. A later bounded character/content task must reconcile the visual contract through the authorised asset workflow.

Lead review, provisional visible sample only: environment 1/10, materials 1/10, lighting 4/10, characters 1/10, presentation coherence 2/10. Animation/contact and camera temporal stability are unassessed. No weighted overall score is assigned because required routes, cast, motion and conditions are missing. These low sample scores describe the greybox baseline, not finished art quality.

Independent reviewer assessed the same sample without receiving the lead scores: environment 2/10, materials 2/10, lighting 4/10, characters 2/10, presentation coherence 2/10; animation/contact and camera/image stability unassessed. Retain these differences rather than averaging. Both reviews agree that conspicuous placeholders prevent visual acceptance and that no overall score is supported.

## Remaining gates

Real keyboard/controller interaction, Blueprint override execution, a full round, Shipping package, clean-machine installation, performance, audio, visual realism and Chad's acceptance remain open. Do not infer any of these from successful package startup. All generated packages, captures and raw logs remain under ignored Saved/.
