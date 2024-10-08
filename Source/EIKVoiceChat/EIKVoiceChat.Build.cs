// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class EIKVoiceChat : ModuleRules
	{
		public EIKVoiceChat(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Json",
					"AudioExtensions"
				}
			);

			PublicIncludePathModuleNames.AddRange(
				new string[]
				{
					"VoiceChat",
				}
			);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    "Core",
                    "CoreUObject",
                    "Projects",
                    "EIKShared",
                    "EIKSDK",
					"Engine",
                    "HTTP", 
                    "EOSIntegrationKit",
					"Synthesis",
					"SignalProcessing",
					"AudioMixer",
				}
			);

			if(Target.Platform == UnrealTargetPlatform.IOS)
			{
				PrivateDependencyModuleNames.AddRange(
					new string[]
					{
						"ApplicationCore"
					}
				);
			}
		}
	}
}
