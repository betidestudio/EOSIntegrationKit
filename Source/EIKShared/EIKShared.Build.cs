//Copyright (c) 2023 Betide Studio. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class EIKShared : ModuleRules
{
	public EIKShared(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.CPlusPlus;

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
