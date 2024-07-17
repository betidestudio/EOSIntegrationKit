using UnrealBuildTool;

public class EIKEditor : ModuleRules
{
    public EIKEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "MainFrame", "EditorStyle",
            }
        );
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "UnrealEd",
                "LevelEditor",
                "ToolMenus",
                "Projects",
                "UnrealEd", 
                "LauncherServices",
                "EOSIntegrationKit",
            }
        );
    }
}