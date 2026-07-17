# Integration Guide — The Veil 0.4

This repository is cumulative. Regenerate project files when required and build `TheVeilEditor` before editing dependent Blueprints. The preferred build, test and launch commands are under `scripts/`.

## Verified environment

Verified on 17 July 2026:

- Unreal Engine 5.8
- Windows x64
- MSVC toolchain `14.50.35717`
- Windows SDK `10.0.22621.0`
- successful `TheVeilEditor` Development/Win64 build
- five passing `TheVeil.*` automation tests
- successful `L_Dev_Sandbox` PIE smoke test

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

All five passed locally on 17 July 2026 with zero failures and zero skips.

## Current playable boundary

`/Game/Maps/L_Dev_Sandbox` exists, loads and passes a PIE smoke test. It does not yet contain the production player character, movement, interaction, embodied contestant, Tribunal or night-operation content. The next integration target is Playable Foundation 0.5 as defined in `docs/production/ROADMAP.md`.
