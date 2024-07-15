using UnrealBuildTool;

public class EOSIntegrationKit : ModuleRules
{
	public EOSIntegrationKit(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"OnlineSubsystemUtils",
			}
		);
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreOnline",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Projects",
				"OnlineSubsystem",
				"Slate",
				"SlateCore",
			}
		);
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[] {
					"UnrealEd",
					"ToolMenus",
					"EditorFramework"
				}
			);
		}
	}
}