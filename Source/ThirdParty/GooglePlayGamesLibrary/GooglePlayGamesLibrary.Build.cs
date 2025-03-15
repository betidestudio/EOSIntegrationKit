// Copyright Betide Studio 2025.
// Written by AvnishGameDev.

using System;
using System.IO;
using UnrealBuildTool;

public class GooglePlayGamesLibrary : ModuleRules
{
	public GooglePlayGamesLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			if (IsGooglePlayGamesEnabled(Target))
			{
				PublicDefinitions.Add("GOOGLE_PLAYGAMES_ENABLED=1");
				PublicDependencyModuleNames.AddRange(new string[] { "Launch" });

				string ModulePath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
				AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModulePath, "GooglePlayGames_APL.xml"));
			}
			else
			{
				PublicDefinitions.Add("GOOGLE_PLAYGAMES_ENABLED=0");
			}
		}
		else
		{
			PublicDefinitions.Add("GOOGLE_ONETAP_ENABLED=0");
		}
	}
	
	private bool IsGooglePlayGamesEnabled(ReadOnlyTargetRules Target)
	{
		// Assume the config file is located in the project's Config folder.
		if (Target.ProjectFile != null)
		{
			string projectDirectory = Path.GetDirectoryName(Target.ProjectFile.FullName);
			if (projectDirectory != null)
			{
				string configFilePath = Path.Combine(projectDirectory, "Config", "DefaultEngine.ini");
				if (!File.Exists(configFilePath))
				{
					Console.WriteLine("DefaultEngine.ini not found at: " + configFilePath);
					return false;
				}

				// Read each line from the ini file.
				foreach (string line in File.ReadAllLines(configFilePath))
				{
					string trimmedLine = line.Trim();

					// Skip blank lines and comments.
					if (string.IsNullOrEmpty(trimmedLine) || trimmedLine.StartsWith(";") || trimmedLine.StartsWith("#"))
					{
						continue;
					}

					// Look for the setting.
					// For example: bEnableGooglePlayGames=true
					if (trimmedLine.StartsWith("bEnableGooglePlayGames", StringComparison.InvariantCultureIgnoreCase))
					{
						string[] parts = trimmedLine.Split('=');
						if (parts.Length >= 2 && bool.TryParse(parts[1].Trim(), out bool isEnabled))
						{
							return isEnabled;
						}
					}
				}
			}
		}

		// Default to not enabled if the key wasn't found.
		return false;
	}
}
