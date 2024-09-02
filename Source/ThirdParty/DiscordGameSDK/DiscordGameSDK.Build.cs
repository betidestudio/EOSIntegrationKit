// Copyright (c) 2024 xist.gg

using System;
using System.IO;
using UnrealBuildTool;

public class DiscordGameSDK : ModuleRules
{
	public DiscordGameSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		string libPath = string.Empty;
		string dllPath = string.Empty;
		string pluginDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			libPath = Path.Combine(ModuleDirectory, "x86_64", "discord_game_sdk.dll.lib");
			dllPath = Path.Combine(pluginDir, "ThirdParty\\DiscordGameSDK\\x86_64\\discord_game_sdk.dll");
			if (File.Exists(libPath) && File.Exists(dllPath))
			{
				PublicDefinitions.Add("EIKDISCORDACTIVE=1");
				
				// Add the import library
				PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x86_64", "discord_game_sdk.dll.lib"));

				// Delay-load the DLL, so we can load it from the right place first
				PublicDelayLoadDLLs.Add("discord_game_sdk.dll");

				// Ensure that the DLL is staged along with the executable
				RuntimeDependencies.Add("$(TargetOutputDir)/discord_game_sdk.dll", "$(PluginDir)/Source/ThirdParty/DiscordGameSDK/x86_64/discord_game_sdk.dll");
			}
			else
			{
				PublicDefinitions.Add("EIKDISCORDACTIVE=0");
			}
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
	        string ArchDir = (Target.Architecture.ToString() == "x86_64") ? "x86_64" : "aarch64";
	        dllPath = Path.Combine(ModuleDirectory, ArchDir, "discord_game_sdk.dylib");
	        if (File.Exists(dllPath))
	        {
		        PublicDefinitions.Add("EIKDISCORDACTIVE=1");
		        PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, ArchDir, "discord_game_sdk.dylib"));
		        RuntimeDependencies.Add("$(TargetOutputDir)/discord_game_sdk.dylib", Path.Combine("$(PluginDir)/Source/ThirdParty/DiscordGameSDK", ArchDir, "discord_game_sdk.dylib"));
	        }
	        else
	        {
		        PublicDefinitions.Add("EIKDISCORDACTIVE=0");
	        }
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			dllPath = "$(PluginDir)/Source/ThirdParty/DiscordGameSDK/x86_64/discord_game_sdk.so";
			string dllPathCheck = Path.Combine(pluginDir, "Source/ThirdParty/DiscordGameSDK/x86_64/discord_game_sdk.so");
			if (File.Exists(dllPathCheck))
			{
				Console.WriteLine("DiscordGameSDK: " + dllPath);
				PublicDefinitions.Add("EIKDISCORDACTIVE=1");
				PublicAdditionalLibraries.Add(dllPath);
				PublicDelayLoadDLLs.Add(dllPath);
				RuntimeDependencies.Add("$(TargetOutputDir)/discord_game_sdk.so", dllPath);
			}
			else
			{
				PublicDefinitions.Add("EIKDISCORDACTIVE=0");
			}
		}
        else
        {
	        PublicDefinitions.Add("EIKDISCORDACTIVE=0");
        }
	}
}
