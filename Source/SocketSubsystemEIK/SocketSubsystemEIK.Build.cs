// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class SocketSubsystemEIK : ModuleRules
{
	public SocketSubsystemEIK(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.CPlusPlus;
		PrivatePCHHeaderFile = "Public/InternetAddrEIK.h";
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Engine",
				"EIKShared",
				"NetCore",
				"Sockets",
				"OnlineSubsystemUtils"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"EIKSDK"
			}
		);
	}
}
