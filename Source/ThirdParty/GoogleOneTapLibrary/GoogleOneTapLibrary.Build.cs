// Fill out your copyright notice in the Description page of Project Settings.

using System;
using System.IO;
using UnrealBuildTool;

public class GoogleOneTapLibrary : ModuleRules
{
	public GoogleOneTapLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			if (IsGoogleOneTapEnabled(Target))
			{
				PublicDefinitions.Add("GOOGLE_ONETAP_ENABLED=1");
				PublicDependencyModuleNames.AddRange(new string[] { "Launch" });

				string ModulePath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
				AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModulePath, "GoogleOneTap_APL.xml"));
			}
			else
			{
				PublicDefinitions.Add("GOOGLE_ONETAP_ENABLED=0");
			}
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicDefinitions.Add("GOOGLE_ONETAP_ENABLED=1");
			PublicAdditionalFrameworks.Add(
				new Framework(
					"GoogleSignIn",
					"IOS/Frameworks/GoogleSignIn.embeddedframework.zip",
					"GoogleSignIn.framework/GoogleSignIn.bundle"
				)
			);
			PublicAdditionalFrameworks.Add(
				new Framework(
					"AppAuth",
					"IOS/Frameworks/AppAuth.embeddedframework.zip"
				)
			);
			PublicAdditionalFrameworks.Add(
				new Framework(
					"GTMAppAuth",
					"IOS/Frameworks/GTMAppAuth.embeddedframework.zip"
				)
			);
			PublicAdditionalFrameworks.Add(
				new Framework(
					"GTMSessionFetcher",
					"IOS/Frameworks/GTMSessionFetcher.embeddedframework.zip"
				)
			);

			PublicDefinitions.Add("TARGET_TV_OS=0");
			PublicDefinitions.Add("DEBUG=0");
			
			PublicFrameworks.AddRange(
				new string[]
				{
					"Accelerate",
					"SafariServices",
					"WebKit",
					"VideoToolbox"
				}
			);
			
			string SDKROOT = Utils.RunLocalProcessAndReturnStdOut("/usr/bin/xcrun", "--sdk iphoneos --show-sdk-path"); // iphoneos / iphonesimulator
			PublicSystemLibraryPaths.Add(SDKROOT + "/usr/lib/swift");
			PublicSystemLibraryPaths.Add(SDKROOT + "../../../../../../Toolchains/XcodeDefault.xctoolchain/usr/lib/swift/iphoneos");
			PublicSystemLibraryPaths.Add(SDKROOT + "../../../../../../Toolchains/XcodeDefault.xctoolchain/usr/lib/swift-5.0/iphoneos");
			PublicSystemLibraryPaths.Add(SDKROOT + "../../../../../../Toolchains/XcodeDefault.xctoolchain/usr/lib/swift-5.5/iphoneos");
			
			PublicSystemLibraries.Add("sqlite3");
			PublicSystemLibraries.Add("xml2");
			
			string ModulePath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(ModulePath, "GoogleOneTap_UPL.xml"));
		}
		else
		{
			PublicDefinitions.Add("GOOGLE_ONETAP_ENABLED=0");
		}
	}
	
	private bool IsGoogleOneTapEnabled(ReadOnlyTargetRules Target)
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
					// For example: bEnableGoogleOneTap=true
					if (trimmedLine.StartsWith("bEnableGoogleOneTap", StringComparison.InvariantCultureIgnoreCase))
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
