//Copyright (c) 2023 Betide Studio. All Rights Reserved.

using UnrealBuildTool;

public class OnlineSubsystemEIKPlus : ModuleRules
{
	public OnlineSubsystemEIKPlus(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDefinitions.Add("ONLINESUBSYSTEMEOSPLUS_PACKAGE=1");

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"OnlineSubsystemUtils"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"Sockets",
				"OnlineSubsystem",
				"Json",
				"OnlineSubsystemEIK"
			}
		);

		PrivateDefinitions.Add("CREATE_MIRROR_PLATFORM_SESSION=" + (bCreateMirrorPlatformSession ? "1" : "0"));
	}

	protected virtual bool bCreateMirrorPlatformSession { get { return true; } }
}
