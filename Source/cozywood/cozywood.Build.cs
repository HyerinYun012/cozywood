using UnrealBuildTool;

public class cozywood : ModuleRules
{
    public cozywood(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

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

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] {
            "Final_test",
            "Final_test/Variant_Platforming",
            "Final_test/Variant_Platforming/Animation",
            "Final_test/Variant_Combat",
            "Final_test/Variant_Combat/AI",
            "Final_test/Variant_Combat/Animation",
            "Final_test/Variant_Combat/Gameplay",
            "Final_test/Variant_Combat/Interfaces",
            "Final_test/Variant_Combat/UI",
            "Final_test/Variant_SideScrolling",
            "Final_test/Variant_SideScrolling/AI",
            "Final_test/Variant_SideScrolling/Gameplay",
            "Final_test/Variant_SideScrolling/Interfaces",
            "Final_test/Variant_SideScrolling/UI"
        });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    }
}