using System.IO;
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
				}
			);
		}

		if (!Target.bBuildEditor && Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicDefinitions.Add("ANDROIDX_ENABLED=1");
			PublicDependencyModuleNames.AddRange(new string[] { "Launch" });

			string modulePath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(modulePath, "EOSIntegrationKit_Android.xml"));
		}
	}
}