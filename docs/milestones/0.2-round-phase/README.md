# The Veil — Round and Phase Foundation 0.2

This is a complete replacement package for Causal Spine 0.1. It includes the previous opportunity, access, routine, evidence and audio-state classes, plus:

- `ATheVeilGameState`
- `ATheVeilRoundDirector`
- `UTVPhaseSubsystem`
- `UTVRunSaveGame`
- `UTVContestantStateComponent`
- phase transition rules
- asynchronous checkpoint saves
- checkpoint restoration
- dawn evidence surfacing
- a basic Unreal Automation test

## Proven short loop

`SocialFreeRoam -> NightPlanning -> NightOperation -> DawnAssembly -> SocialFreeRoam`

At the Social-to-Night boundary, the system captures contestant state, access grants, routines, opportunities and evidence. Night Planning explicitly restores the checkpoint before the operation begins. At Dawn, new evidence is surfaced through the evidence subsystem and an `AtDawn` checkpoint is queued.

## Installation

1. Close Unreal Editor.
2. Copy `Source/TheVeil/` into your project's matching source folder.
3. Copy `Config/Tags/TheVeil_PhaseTags.ini` into your project's `Config/Tags/`.
4. Add `GameplayTags` and `AudioMixer` to `TheVeil.Build.cs`.
5. Regenerate project files.
6. Build `TheVeilEditor` in Development Editor / Win64.
7. Reopen Unreal.
8. Follow `INTEGRATION.md`.

## Important

The code was structurally checked in this package but could not be compiled against your local Unreal project. Resolve the first compiler error before later cascade errors.
