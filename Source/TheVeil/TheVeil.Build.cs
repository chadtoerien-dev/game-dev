using UnrealBuildTool;

public class TheVeil : ModuleRules
{
    public TheVeil(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // The current source tree uses module-root include paths such as
        // "Core/TVGameplayTypes.h" and "Voting/TVVotingProfile.h".
        PublicIncludePaths.Add(ModuleDirectory);

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
    }
}
