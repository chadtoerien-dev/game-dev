# The Veil — Causal Spine 0.1

This package contains the native foundation for the first integrated gameplay chain:

1. A social action raises trust.
2. An opportunity becomes available.
3. The target's routine changes.
4. Access is evaluated from invitation, relationship and context.
5. A night action generates evidence.
6. A central audio director resolves a gameplay music state.

## Files

Copy the folders under `Source/TheVeil/` into the matching folders in your project.

## Build.cs

Add `GameplayTags` and `AudioMixer` to `PublicDependencyModuleNames` or `PrivateDependencyModuleNames` as appropriate.

Example:

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "Core",
    "CoreUObject",
    "Engine",
    "InputCore",
    "EnhancedInput",
    "GameplayTags",
    "AudioMixer"
});
```

## Important limitation

The audio subsystem in this package resolves the winning music state but intentionally does not yet schedule MetaSound stems. Bind its `OnResolvedMusicStateChanged` delegate in `BP_TVAudioDirectorProxy` and use placeholder audio first. Add Quartz scheduling only after the causal gameplay chain is working.

## Compile

1. Close Unreal Editor.
2. Regenerate project files if your IDE does not detect the new classes.
3. Build `TheVeilEditor` in Development Editor / Win64.
4. Reopen the project.
5. Fix redirects if you changed any class paths.
