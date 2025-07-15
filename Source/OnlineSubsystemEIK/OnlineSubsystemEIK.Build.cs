// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
#if !UE_5_0_OR_LATER
using Tools.DotNETCommon;
#else
using EpicGames.Core;
#endif
public class OnlineSubsystemEIK : ModuleRules
{
	public OnlineSubsystemEIK(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDefinitions.Add("ONLINESUBSYSTEMEOS_PACKAGE=1");
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

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
				"EIKSDK",
				"EIKShared",
				"EIKVoiceChat",
				"Json",
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
				"EOSIntegrationKit",
			}
		);
		#if UE_5_0_OR_LATER
		PrivateDependencyModuleNames.AddRange(new string[] { "CoreOnline", "Core", "Sockets" , "OnlineBase"});
		#endif

		
		PrivateDefinitions.Add("USE_XBL_XSTS_TOKEN=" + (bUseXblXstsToken ? "1" : "0"));
		PrivateDefinitions.Add("USE_PSN_ID_TOKEN=" + (bUsePsnIdToken ? "1" : "0"));
		PrivateDefinitions.Add("ADD_USER_LOGIN_INFO=" + (bAddUserLoginInfo ? "1" : "0"));
		bool bSupportOculusPlatform = false;
		ConfigHierarchy PlatformGameConfig = ConfigCache.ReadHierarchy(ConfigHierarchyType.Engine, DirectoryReference.FromFile(Target.ProjectFile), UnrealTargetPlatform.Android);
		if (!PlatformGameConfig.GetBool("OnlineSubsystemOculus", "bEnabled", out bSupportOculusPlatform))
		{
			bSupportOculusPlatform = false;
		}
		if (bSupportOculusPlatform)
		{
			PublicDefinitions.Add("SUPPORTOCULUSPLATFORM=1");
		}
		else
		{
			PublicDefinitions.Add("SUPPORTOCULUSPLATFORM=0");
		}

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
				}
			);
		}
	}

	protected virtual bool bUseXblXstsToken { get { return false; } }
	protected virtual bool bUsePsnIdToken { get { return false; } }
	protected virtual bool bAddUserLoginInfo { get { return false; } }
}
