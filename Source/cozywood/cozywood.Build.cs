using UnrealBuildTool;

public class cozywood : ModuleRules
{
    public cozywood(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // CS0618 fix: UE5.6에서 UnsafeTypeCastWarningLevel 접근 방식 변경
        CppCompileWarningSettings.UnsafeTypeCastWarningLevel = WarningLevel.Warning;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "HTTP",
            "Json",
            "JsonUtilities",
            "UMG",
            "Slate",
            "SlateCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    }
}