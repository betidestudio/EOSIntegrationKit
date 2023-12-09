//Copyright (c) 2023 Betide Studio. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class OnlineSubsystemEIK : ModuleRules
{
	public OnlineSubsystemEIK(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDefinitions.Add("ONLINESUBSYSTEMEOS_PACKAGE=1");

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"OnlineSubsystemUtils"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreOnline",
				"CoreUObject",
				"Engine",
				"EIKSDK",
				"EIKShared",
				"EIKVoiceChat",
				"Json",
				"OnlineBase",
				"OnlineSubsystem",
				"Sockets",
				"VoiceChat",
				"NetCore", 
				"SocketSubsystemEIK",
				"InputCore",
				"Projects",
				"Slate",
				"SlateCore",
				"HTTP",
			}
		);

		
		PrivateDefinitions.Add("USE_XBL_XSTS_TOKEN=" + (bUseXblXstsToken ? "1" : "0"));
		PrivateDefinitions.Add("USE_PSN_ID_TOKEN=" + (bUsePsnIdToken ? "1" : "0"));
		PrivateDefinitions.Add("ADD_USER_LOGIN_INFO=" + (bAddUserLoginInfo ? "1" : "0"));
		
		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
		}
		
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

	protected virtual bool bUseXblXstsToken { get { return false; } }
	protected virtual bool bUsePsnIdToken { get { return false; } }
	protected virtual bool bAddUserLoginInfo { get { return false; } }
}
