using UnrealBuildTool;
using System.Collections.Generic;

public class TheVeilTarget : TargetRules
{
    public TheVeilTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("TheVeil");
    }
}
