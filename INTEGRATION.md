# Integration Guide — The Veil 0.5 Native Playable Foundation

This repository is cumulative. Regenerate project files when required and build `TheVeilEditor` before editing dependent Blueprints. The preferred build, test and launch commands are under `scripts/`.

## Verified environment

Verified on 19 July 2026 unless otherwise noted:

- Unreal Engine 5.8
- Windows x64
- MSVC toolchain `14.50.35717`
- Windows SDK `10.0.22621.0`
- successful `TheVeilEditor` Development/Win64 build
- twelve passing `TheVeil.*` automation tests
- strict structural validation of the saved Playable Foundation assets and map integration with zero failures
- successful final `L_Dev_Sandbox` Editor/PIE acceptance on 19 July 2026, including physical input, camera feel/collision, focus-loss recovery and Product Owner visual approval

Build:

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Build-TheVeil.ps1"
```

Test:

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Run-TheVeilTests.ps1"
```

Launch:

```powershell
powershell -ExecutionPolicy Bypass -File ".\scripts\Launch-TheVeil.ps1"
```

## Playable Foundation 0.5 native classes

The native playable shell consists of:

- `ATheVeilCharacter` in `Source/TheVeil/Characters/`: character movement, camera-relative movement, look, held sprint, toggle crouch, normal Character Movement jump, transient-state reset, camera boom and follow camera.
- `ATheVeilPlayerController` in `Source/TheVeil/Core/`: Enhanced Input context ownership, input bindings, forwarding to the possessed The Veil character, duplicate-binding protection, possession cleanup and focus-loss cleanup.
- `ATheVeilGameMode` in `Source/TheVeil/Core/`: native defaults for `ATheVeilCharacter`, `ATheVeilPlayerController` and the existing `ATheVeilGameState`. It uses standard Unreal spawning and `RestartPlayer`; do not add Level Blueprint or Blueprint duplicate-spawn logic.

Native starting values are walk `500 cm/s`, sprint `720 cm/s`, crouch `300 cm/s`, acceleration `2200 cm/s²`, walking braking `2000 cm/s²`, yaw rotation `600 degrees/s`, ground friction `8`, air control `0.35`, spring-arm length `340 cm`, collision probe size `12 cm` and camera lag speed `18`. The tuning values are exposed under `The Veil|Movement|Tuning` and `The Veil|Camera|Tuning` on derived character defaults.

## Enhanced Input asset contract

The checked setup manifest creates these assets at the exact paths below. The tables remain the authoritative manual fallback; do not replace them with legacy action or axis mappings.

| Asset path | Asset type | Value type |
|---|---|---|
| `/Game/TheVeil/Input/IA_Move` | Input Action | Axis2D (Vector2D) |
| `/Game/TheVeil/Input/IA_Look` | Input Action | Axis2D (Vector2D) |
| `/Game/TheVeil/Input/IA_Jump` | Input Action | Digital (bool) |
| `/Game/TheVeil/Input/IA_Sprint` | Input Action | Digital (bool) |
| `/Game/TheVeil/Input/IA_Crouch` | Input Action | Digital (bool) |
| `/Game/TheVeil/Input/IMC_Player` | Input Mapping Context | Not applicable |

Leave action-level triggers and modifiers empty. Configure `IMC_Player` as follows; modifier order is significant where more than one modifier is listed.

| Action | Key | Mapping modifiers |
|---|---|---|
| `IA_Move` | `W` | Swizzle Input Axis Values: `YXZ` (produces `+Y`) |
| `IA_Move` | `S` | Negate, then Swizzle Input Axis Values: `YXZ` (produces `-Y`) |
| `IA_Move` | `A` | Negate (produces `-X`) |
| `IA_Move` | `D` | None (produces `+X`) |
| `IA_Move` | Gamepad Left Thumbstick 2D-Axis | Dead Zone: Radial, lower threshold `0.20`, upper threshold `1.00` |
| `IA_Look` | Mouse XY 2D-Axis | None |
| `IA_Look` | Gamepad Right Thumbstick 2D-Axis | Dead Zone: Radial, lower threshold `0.20`, upper threshold `1.00`; then Scalar `X=1.25`, `Y=1.00`, `Z=1.00` |
| `IA_Jump` | Space Bar | None |
| `IA_Jump` | Gamepad Face Button Bottom | None |
| `IA_Sprint` | Left Shift | None |
| `IA_Sprint` | Gamepad Left Thumbstick Button | None |
| `IA_Crouch` | Left Ctrl | None |
| `IA_Crouch` | Gamepad Face Button Right | None |

Sprint intentionally has no toggle trigger: the controller handles `Started`, `Completed` and `Canceled`, making it a held action. Crouch is toggled from `Started`. `DefaultInput.ini` already uses `EnhancedPlayerInput`, `EnhancedInputComponent` and `bShouldFlushPressedKeysOnViewportFocusLost=True`; no text config edit is required.

## Blueprint and map setup

1. Compile the native editor target before creating derived Blueprints.
2. Create `/Game/TheVeil/Core/BP_TheVeilPlayerController` with parent `ATheVeilPlayerController`.
3. In its Class Defaults under `The Veil|Input`, assign `Player Mapping Context = IMC_Player` and assign all five matching Input Actions to `Move Action`, `Look Action`, `Jump Action`, `Sprint Action` and `Crouch Action`.
4. Create `/Game/TheVeil/Characters/BP_TheVeilCharacter` with parent `ATheVeilCharacter`.
5. Set `Auto Possess Player` to `Disabled`. Do not bind input or add movement logic in this Blueprint.
6. For a safe temporary visual, add a `StaticMeshComponent` named `PlaceholderVisual`, attach it to the inherited Capsule Component, use `/Engine/BasicShapes/Sphere.Sphere`, keep relative location and rotation at zero, set Collision Preset to `NoCollision`, disable `Can Ever Affect Navigation`, and scale it to `0.84, 0.84, 1.92`. This produces a deliberately simple ellipsoid inside the character bounds. A project-owned skeletal placeholder may replace it later, but it must contain no gameplay logic and must have no collision authority.
7. Create `/Game/TheVeil/Core/BP_TheVeilGameMode` with parent `ATheVeilGameMode`.
8. In its Class Defaults, set Default Pawn Class to `BP_TheVeilCharacter`, Player Controller Class to `BP_TheVeilPlayerController`, and confirm Game State Class remains `ATheVeilGameState`. Do not add custom spawn events.
9. Open `/Game/Maps/L_Dev_Sandbox` and set World Settings -> GameMode Override to `BP_TheVeilGameMode`.
10. Remove duplicate PlayerStarts and any placed player pawn configured for auto possession. Place exactly one PlayerStart on walkable ground with full capsule clearance; its icon must not show `BADsize`.

Do not set Auto Possess Player on a placed pawn. Standard GameMode spawning at the one PlayerStart is the sole player-spawn path and should produce exactly one controlled pawn.

## Preferred Playable Foundation automation route

The manual settings above remain the authoritative fallback. The preferred route uses `scripts/PlayableFoundation/playable-foundation-manifest.json` so input, Blueprint defaults, the temporary visual and the sandbox GameMode override are reproducible.

1. With Unreal Editor closed, preview the exact asset operations:

   ```powershell
   powershell -ExecutionPolicy Bypass -File ".\scripts\Prepare-PlayableFoundationEditor.ps1"
   ```

2. Review `Saved/PlayableFoundation/setup-report.json`. The dry run must report `map_mutated: false`; it previews the five Input Actions, `IMC_Player`, three derived Blueprints and collision-free temporary visual. The map remains untouched unless the separate map guard is supplied.
3. Apply the reviewed manifest and the exact sandbox GameMode override:

   ```powershell
   powershell -ExecutionPolicy Bypass -File ".\scripts\Prepare-PlayableFoundationEditor.ps1" -Apply -ApplyMap
   ```

   Apply replaces the mappings inside the manifest-owned `IMC_Player` with the exact declared set, creates or repairs the manifest-owned visual, and saves only the declared map override. Keep unrelated input in a different mapping context. The command refuses to run beside an open Editor. `-ApplyMap` has a separate guard and is bounded to `/Game/Maps/L_Dev_Sandbox`.
4. Review the exact binary list in Git. The expected persisted set is the nine manifest-owned assets plus `Content/Maps/L_Dev_Sandbox.umap`; no PlayerStart or World Partition external actor is changed by this automation.
5. Run strict structural validation:

   ```powershell
   powershell -ExecutionPolicy Bypass -File ".\scripts\Validate-PlayableFoundationEditor.ps1"
   ```

6. Run the complete non-interactive gate:

   ```powershell
   powershell -ExecutionPolicy Bypass -File ".\scripts\Verify-PlayableFoundation.ps1"
   ```

7. Open Unreal Editor for the human checks: inspect the existing PlayerStart for ground/capsule clearance and `BADsize`, judge the temporary visual, and run the manual PIE checklist below. Structural validation cannot prove camera feel, collision judder, physical controller input, focus-loss recovery or soak stability. This gate was completed and accepted on 19 July 2026.

Generated reports and commandlet logs are under ignored `Saved/PlayableFoundation/`. The checked scripts invoke the installed UE 5.8 Python Editor plugin only for the commandlet; they do not add a permanent project plugin or config dependency.

## World Partition save expectations

The automated integration saves the five Input Actions, one Input Mapping Context, three Blueprints and `Content/Maps/L_Dev_Sandbox.umap`. It does not move the PlayerStart, so it should not create or modify a World Partition external actor. If the human later moves the PlayerStart, use `Save All` and review any legitimate external actor/object changes. Preserve opaque files under `Content/__ExternalActors__/`, `Content/__ExternalObjects__/` and the map HLOD assets; never delete or hand-edit them to make the diff look smaller.

## Playable Foundation manual PIE checklist

1. Open `L_Dev_Sandbox`; confirm World Settings shows `BP_TheVeilGameMode` and its resolved classes are the three intended The Veil classes.
2. Confirm there is exactly one PlayerStart, no `BADsize`, and no placed auto-possessed player pawn.
3. Play in Selected Viewport and confirm exactly one controlled pawn spawns on walkable ground without intersecting geometry or falling through terrain.
4. Confirm `WASD` movement is relative to camera yaw, stops responsively, and rotates the character towards movement.
5. Confirm left-stick movement matches keyboard direction and speed, including clean response around the radial dead zone.
6. Confirm mouse look orbits the camera and right-stick look has functional parity without drift.
7. Hold and release Left Shift and the gamepad left-thumbstick button; sprint must start and stop reliably.
8. Toggle crouch with Left Ctrl and Gamepad Face Button Right; entering crouch must cancel sprint and sprint must not activate while crouched.
9. Jump with Space and Gamepad Face Button Bottom; normal Character Movement rules must prevent invalid repeated jumps.
10. Walk beside walls and under obstructions; confirm the spring arm retracts, the camera does not visibly clip, and normal camera motion does not judder.
11. Hold sprint, Alt-Tab or otherwise remove viewport focus, release sprint while unfocused, return, and confirm sprint and jump are not stuck and crouch has been cleared.
12. Run PIE for at least two minutes, then press `Esc`; confirm clean shutdown with no crash or fatal error.
13. Re-run all `TheVeil.*` automation tests and confirm the existing social simulation remains unaffected.
14. Record a short capture showing keyboard and controller movement, both look inputs, sprint, crouch, jump, camera collision and the Alt-Tab recovery check for independent visual review.

## Playable Foundation rollback

If the Editor integration fails, close PIE, revert the human-owned binary changes through source control, and restore the previous map GameMode override and PlayerStart state in Unreal Editor. Remove newly created assets through the Content Browser or revert them through source control; do not delete World Partition companion files manually. The native implementation can be rolled back by reverting its bounded feature commit. Rebuild and run the pre-existing automation suite to confirm the 0.4 baseline is restored.

## Contestant knowledge components

Add `TVContestantStateComponent`, `TVMemoryComponent` and `TVWitnessComponent` to Eleanor, Marcus and Beatrice. Use matching contestant IDs. Assign a `TVSocialInterpretationProfile` to each memory component.

Initial witness settings:

- visual range: 1800
- hearing range: 1400
- require line of sight: true
- occluded hearing confidence multiplier: 0.65
- minimum salience: 0.05

## Directed relationships

Create a development bootstrap actor rather than using the Level Blueprint. Initialise observer-to-subject records through `TVRelationshipSubsystem`. The matrix is directional: `Eleanor -> Player` is not the same as `Player -> Eleanor`.

Use the relationship subsystem as the authority for trust, suspicion, affinity, respect, fear and credibility. Compatibility values on `TVContestantStateComponent` should not drive new social logic.

## Event and memory pipeline

Operations emit one `FTVGameplayEventRecord`. Witness components independently decide whether they saw, heard or received the event. Do not add memories directly from operation Blueprints.

For archive sabotage, use:

- event: `Event.Operation.ArchiveSabotage`
- instigator: `Player`
- target: `Eleanor`
- location: `Location.Archive`
- audience: local senses
- channel: mixed
- loudness: 0.8
- reliability: 0.95
- salience: 0.9
- reveal instigator on visual witness: true
- context: `Context.Event.Illegal`, `Context.Event.Private`

The evidence record's `SourceContestantId` is hidden simulation truth. Only `AttributedContestantId` may become public knowledge.

Expected behaviour:

- Eleanor sees the action and forms a first-hand memory about the player.
- Marcus hears the action without automatically identifying the player.
- Reporting the memory to Marcus lowers confidence and records Eleanor as immediate source.
- Spreading it as a rumour to Beatrice increases transmission depth and distortion.
- A deliberate lie can change the alleged subject without revealing its falsity to the recipient.

## Checkpoint behaviour

The run save persists phase, opportunities, evidence, events, contestant state, routines, memories, directed relationships, claims, contradictions, vote commitments and vote intentions. Historical events restore without re-broadcasting, preventing duplicate witness memories.

## Claim, contradiction and voting integration

Create a `TVVotingProfile` Data Asset named `DA_Voting_Default`. Start with the supplied defaults.

Configure `BP_TVRoundDirector`:

- assign `DA_Voting_Default`
- populate active `Voting Contestant Ids`
- populate eligible `Voting Candidate Ids`
- disable the short prototype loop when testing Tribunal and Vote

Create public or private statements with `TVClaimSubsystem -> Create Claim From Memory`. Always pass the contestants who actually heard the claim. Public claims should receive the explicit active audience; an empty audience is not omniscient global knowledge.

The prototype detects:

- assertion versus denial of the same topic
- incompatible locations for the same subject and topic
- incompatible phases for the same subject and topic
- explicit retraction by the same speaker

A contradiction affects only contestants who know both claims.

For each voter, expose a development-only score breakdown:

- suspicion
- distrust
- low affinity
- fear
- perceived threat
- known claims
- known contradictions
- commitments
- group pressure
- deterministic tie-breaker

Do not display exact values in the release UI.

## Required playable social test

1. Eleanor witnesses archive sabotage.
2. Eleanor publicly accuses the player.
3. The player denies being in the archive.
4. Marcus hears both claims.
5. A critical contradiction is recorded.
6. Marcus's opinion of the speaker judged at fault loses credibility.
7. Marcus's vote breakdown changes.
8. A trusted ally privately defends the player.
9. Only voters who heard the defence respond.
10. Enter Vote and confirm every NPC locks exactly one candidate.
11. Save and restore; claims, contradictions and locked votes remain stable.

This scenario is not yet embodied as a complete player-facing level sequence. It remains the target integration test for the Tribunal milestone.

## Automation tests

Run from `Tools -> Session Frontend -> Automation`, or use the command-line test script.

Expected tests:

- `TheVeil.Phase.ShortLoop`
- `TheVeil.Information.ProvenanceTransmission`
- `TheVeil.Information.InterpretationRuleSelection`
- `TheVeil.Claims.DirectContradiction`
- `TheVeil.Claims.LocationContradiction`
- `TheVeil.PlayableFoundation.CharacterDefaults`
- `TheVeil.PlayableFoundation.CharacterStateTransitions`
- `TheVeil.PlayableFoundation.ControllerBindings`
- `TheVeil.PlayableFoundation.ControllerMappingContextOwnership`
- `TheVeil.PlayableFoundation.ControllerTransientCleanup`
- `TheVeil.PlayableFoundation.GameModeDefaults`
- `TheVeil.PlayableFoundation.MovementBasis`

All twelve passed locally on 19 July 2026 with zero failures and zero skips.

## 19 July 2026 non-human verification evidence

- Guarded setup: `powershell -ExecutionPolicy Bypass -File ".\scripts\Prepare-PlayableFoundationEditor.ps1" -Apply -ApplyMap` passed preflight, recorded exactly nine saved content packages and verified the sandbox GameMode override. An idempotent re-run reported `content_persisted: true`, `map_mutated: false`, `map_persisted: true`, zero errors and zero commandlet warnings.
- Strict read-only validation: `Saved/PlayableFoundation/validation-report.json` reports 59 automated passes, one human PlayerStart-clearance check, `automated_status: pass`, zero failures, `map_mutated: false` and `content_mutated: false`. The automated checks include current compiler status for all three Blueprints, one loaded PlayerStart, zero placed auto-possessed pawns, all nine assets and the intended GameMode override.
- Build: `Saved/BuildLogs/TheVeilEditor-Development-20260719-032319.log` reports compiled and linked source with `Result: Succeeded`.
- Automation: `Saved/AutomationReports/20260719-032716/index.json` reports 12 succeeded, 0 succeeded with warnings, 0 failed and 0 not run.
- Durable evidence: `docs/evidence/0.5-playable-foundation-automated-verification.md` records the commands, extracts and SHA-256 checksums required by remote PR reviewers.
- Repository audit: working-tree, staged-index and complete `origin/main...HEAD` whitespace checks passed, with no World Partition companion or protected social-system changes.

Reports and logs under `Saved/` are local evidence and remain excluded from source control. Structural integration is complete; the final human acceptance is recorded in `docs/evidence/0.5-playable-foundation-human-acceptance.md`.

## 19 July 2026 human acceptance evidence

- `L_Dev_Sandbox` visibly resolved `BP_TheVeilGameMode` and the Outliner showed exactly one PlayerStart among 138 loaded actors.
- The selected PlayerStart had walkable-ground/capsule clearance and displayed no `BADsize`; PIE spawned exactly one controlled temporary pawn without intersection or fall-through.
- Keyboard movement, mouse look, sprint, crouch and jump were exercised in PIE, including a visible jump/land cycle and crouch/stand transition.
- A fresh continuous soak ran for 137 seconds and `Esc` returned cleanly to the Editor.
- Output Log searches returned no `Fatal` or `LogPIE: Error` entries for the session.
- Chad Toerien confirmed the remaining physical-controller parity, camera collision/feel and focus-loss recovery checks, accepted the temporary visual for this milestone and instructed the project to proceed.
- The live session was not retained as a video file. The Product Owner attestation is the durable acceptance record; no capture artefact is claimed.

## Current playable boundary

`/Game/Maps/L_Dev_Sandbox` now provides the accepted 0.5 playable shell. The saved map uses the intended GameMode, the nine manifest-owned input/Blueprint assets are persisted and wired, strict validation reports one loaded PlayerStart with zero placed auto-possessed pawns, all twelve automation tests pass, and the final Editor/PIE gate is accepted. Interaction, embodied contestants, the Tribunal, night-operation content and the maximum-fidelity King’s College environment remain future milestones.
