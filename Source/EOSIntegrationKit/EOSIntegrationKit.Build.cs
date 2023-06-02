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
			}
		);
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"SocketSubsystemEIK",
				"OnlineSubsystemEIK"
			}
		);
	}
}