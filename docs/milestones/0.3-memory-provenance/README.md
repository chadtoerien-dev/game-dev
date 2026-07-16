# The Veil — Memory and Information Provenance 0.3

This is a complete replacement package for Round and Phase Foundation 0.2.
It retains the previous phase, checkpoint, opportunity, access, routine,
evidence and audio-state foundations and adds the first NPC knowledge pipeline.

## Proven causal path

`Gameplay action -> typed event -> individual witness memory -> report or rumour -> directed relationship change -> checkpoint -> Dawn public memory`

## New systems

- `UTVGameplayEventSubsystem`
- `UTVMemoryComponent`
- `UTVWitnessComponent`
- `UTVInformationRules`
- `UTVInformationSubsystem`
- `UTVRelationshipSubsystem`
- `UTVSocialInterpretationProfile`
- persistent event, memory and relationship snapshots
- public Dawn evidence events that do not reveal hidden objective culprits
- deterministic report, rumour and deliberate-lie transformation
- two new Unreal Automation tests

## Important design boundary

The event log contains objective simulation facts. NPCs do not query it directly.
They act on their own `FTVMemoryRecord` collections. Public evidence uses
`AttributedContestantId`, not the hidden `SourceContestantId`, so a clean,
ambiguous or framed operation does not leak the answer through AI state.

## Installation

1. Close Unreal Editor.
2. Replace the matching `Source/TheVeil/` files with this package.
3. Copy `Config/Tags/TheVeil_PhaseTags.ini` into the project's `Config/Tags/`.
4. Confirm `GameplayTags` and `AudioMixer` are in `TheVeil.Build.cs`.
5. Regenerate project files.
6. Build `TheVeilEditor` using Development Editor / Win64.
7. Reopen Unreal and follow `INTEGRATION.md`.

## Save compatibility

Save version is now `2`. Version 1 saves can load; their event, memory and
relationship arrays will be empty until a new checkpoint is captured. A newer
save version is rejected.

## Verification status

The package has been structurally inspected, checked for balanced source
braces, checked for Unreal generated-header placement and validated as a ZIP.
It could not be compiled against your local UE project from this environment.
Resolve the first compiler or UHT error before later cascading errors.
