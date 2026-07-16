using UnrealBuildTool;

public class TheVeil : ModuleRules
{
    public TheVeil(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

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
