// Copyright Betide Studio 2025.
// Written by AvnishGameDev.

using System;
using System.IO;
using UnrealBuildTool;

public class GooglePlayBillingLibrary : ModuleRules
{
	public GooglePlayBillingLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			if (IsGooglePlayBillingEnabled(Target))
			{
				PublicDefinitions.Add("GOOGLE_PLAY_BILLING_ENABLED=1");
				PublicDependencyModuleNames.AddRange(new string[] { "Launch" });

				string ModulePath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
				AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModulePath, "GooglePlayBilling_APL.xml"));
			}
			else
			{
				PublicDefinitions.Add("GOOGLE_PLAY_BILLING_ENABLED=0");
			}
		}
		else
		{
			PublicDefinitions.Add("GOOGLE_PLAY_BILLING_ENABLED=0");
		}
	}
	
	private bool IsGooglePlayBillingEnabled(ReadOnlyTargetRules Target)
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
					if (trimmedLine.StartsWith("bEnableGooglePlayBilling", StringComparison.InvariantCultureIgnoreCase))
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
