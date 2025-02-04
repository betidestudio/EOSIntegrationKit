// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class EIKShared : ModuleRules
{
	public EIKShared(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "EIKSDK" });
		Type = ModuleType.CPlusPlus;
		PrivatePCHHeaderFile = "Private/EIKSharedModule.h";

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"EIKSDK",
				"Projects",
			}
		);
	}
}
