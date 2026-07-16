using UnrealBuildTool;
using System.Collections.Generic;

public class TheVeilEditorTarget : TargetRules
{
    public TheVeilEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("TheVeil");
    }
}
