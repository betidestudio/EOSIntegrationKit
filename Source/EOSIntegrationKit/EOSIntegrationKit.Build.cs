//Copyright (c) 2023 Betide Studio. All Rights Reserved.
using System;
using System.Linq;
using System.Reflection;
using UnrealBuildTool;

public class EOSIntegrationKit : ModuleRules
{
	public EOSIntegrationKit(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"OnlineSubsystem",
				"OnlineSubsystemUtils",
				"OnlineSubsystemEOS",
				"EOSVoiceChat",
				"Json",
				"JsonUtilities",
				"HTTP",

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		if (false)
		{
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
									"PlayFab",
                    				"PlayFabCpp",
                    				"PlayFabCommon"
				}
			);
			PublicDefinitions.Add("PLAYFAB_PLUGIN_INSTALLED=1");
		}
	}
}
