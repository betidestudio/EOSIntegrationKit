// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnrealBuildTool;
using EpicGames.Core;

public class EIKSDK : ModuleRules
{
	public virtual string SDKBaseDir
	{
		get
		{
			// Overridden by platform extensions to point at the PE module directory
			return Path.Combine(ModuleDirectory, "SDK");
		}
	}

	public virtual string SDKIncludesDir
	{
		get
		{
			if (Target.Platform == UnrealTargetPlatform.Android)
			{
				return Path.Combine(SDKBinariesDir, "include");
			}
			else if(Target.Platform == UnrealTargetPlatform.IOS)
            {
				return Path.Combine(SDKBinariesDir, "EIKSDK.framework", "Headers");
			}
			return Path.Combine(SDKBaseDir, "Include");
		}
	}

	public virtual string SDKLibsDir
	{
		get
		{
			return Path.Combine(SDKBaseDir, "Lib");
		}
	}

	public virtual string SDKBinariesDir
	{
		get
		{
			if (Target.Platform == UnrealTargetPlatform.IOS)
			{
				return Path.Combine(SDKBaseDir, "Bin", "IOS");
			}
			else if (Target.Platform == UnrealTargetPlatform.Android)
            {
				return Path.Combine(SDKBaseDir, "Bin", "Android");
            }
			return Path.Combine(SDKBaseDir, "Bin");
		}
	}

	public virtual string EngineBinariesDir
	{
		get
		{
			return Path.Combine(EngineDirectory, "Binaries", Target.Platform.ToString());
		}
	}

	public virtual string LibraryLinkNameBase
	{
		get
		{
			if(Target.Platform == UnrealTargetPlatform.Android)
            {
				return "EOSSDK";
            }
			
			return String.Format("EOSSDK-{0}-Shipping", Target.Platform.ToString());
		}
	}

	public virtual string LibraryLinkName
	{
		get
		{
			if (Target.Platform == UnrealTargetPlatform.Mac)
			{
				return Path.Combine(SDKBinariesDir, "lib" + LibraryLinkNameBase + ".dylib");
			}
			else if(Target.Platform == UnrealTargetPlatform.Linux)
			{
				return Path.Combine(SDKBinariesDir, "lib" + LibraryLinkNameBase + ".so");
			}
			else if (Target.Platform.IsInGroup(UnrealPlatformGroup.Microsoft))
			{
				return Path.Combine(SDKLibsDir, LibraryLinkNameBase + ".lib");
			}
			// Android has one .so per architecture, so just deal with that below.
			// Other platforms will override this property.

			throw new BuildException("Unsupported platform");
		}
	}

	public virtual string RuntimeLibraryFileName
	{
		get
		{
			if (Target.Platform == UnrealTargetPlatform.Mac)
			{
				return "lib" + LibraryLinkNameBase + ".dylib";
			}
			else if (Target.Platform == UnrealTargetPlatform.IOS)
			{
				return LibraryLinkNameBase + ".framework";
			}
			else if (Target.Platform == UnrealTargetPlatform.Android ||
				Target.Platform == UnrealTargetPlatform.Linux)
			{
				return "lib" + LibraryLinkNameBase + ".so";
			}
			else if (Target.Platform.IsInGroup(UnrealPlatformGroup.Microsoft))
			{
				return LibraryLinkNameBase + ".dll";
			}
			// Other platforms will override this property.

			throw new BuildException("Unsupported platform");
		}
	}

	public virtual bool bRequiresRuntimeLoad
	{
		get
		{
			return Target.Platform.IsInGroup(UnrealPlatformGroup.Windows) || Target.Platform == UnrealTargetPlatform.Mac;
			// Other platforms may override this property.
		}
	}

	public EIKSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PublicDefinitions.Add("WITH_EOS_SDK=1");

		PublicDefinitions.Add(String.Format("EOSSDK_RUNTIME_LOAD_REQUIRED={0}", bRequiresRuntimeLoad ? 1 : 0));
		PublicDefinitions.Add(String.Format("EOSSDK_RUNTIME_LIBRARY_NAME=\"{0}\"", RuntimeLibraryFileName));
		PublicIncludePaths.Add(SDKIncludesDir);
		PublicSystemIncludePaths.Add(SDKIncludesDir);
		PrivateIncludePaths.Add(SDKIncludesDir);
		PublicDefinitions.Add("EOSSDK_USE_PROJECT_BINARY=1");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "EOSSDK-Win64-Shipping.lib"));
			PublicDelayLoadDLLs.Add("EOSSDK-Win64-Shipping.dll");
			RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "EOSSDK-Win64-Shipping.dll"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			PrivateRuntimeLibraryPaths.Add(SDKBinariesDir);
			PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "libEOSSDK-Linux-Shipping.so"));
			RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", "libEOSSDK-Linux-Shipping.so"), Path.Combine(SDKBinariesDir, "libEOSSDK-Linux-Shipping.so"));
			RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "libEOSSDK-Linux-Shipping.so"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "arm64-v8a", "libEOSSDK.so"));
			PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "armeabi-v7a", "libEOSSDK.so"));

			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(SDKBaseDir, "EIKSDK_UPL.xml"));
		}
		return;

		PublicIncludePaths.Add(SDKIncludesDir);


		bool bUseProjectBinary = Target.LinkType == TargetLinkType.Monolithic && Target.GlobalDefinitions.Contains("EOSSDK_USE_PROJECT_BINARY=1");

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicIncludePaths.Add(Path.Combine(SDKIncludesDir, "Android"));

			if (!bUseProjectBinary)
			{
				PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "static-stdc++", "libs", "armeabi-v7a", RuntimeLibraryFileName));
				PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "static-stdc++", "libs", "arm64-v8a", RuntimeLibraryFileName));

				string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
				AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "EIKSDK_UPL.xml"));
			}
        }
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			if (!bUseProjectBinary)
			{
				PublicAdditionalFrameworks.Add(new Framework("EOSSDK", SDKBinariesDir, "", true));
			}
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			string DylibPath = Path.Combine(Target.UEThirdPartyBinariesDirectory, "EOSSDK", "Mac", RuntimeLibraryFileName);
			PublicDelayLoadDLLs.Add(DylibPath);
			RuntimeDependencies.Add(DylibPath);
		}
		else
		{
			// Allow projects to provide their own EOSSDK binaries. We will still compile against our own headers, because EOSSDK makes guarantees about forward compat. Note this global definition is only valid for monolithic targets.
			if(!bUseProjectBinary)
            {
				PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, LibraryLinkName));
				RuntimeDependencies.Add(Path.Combine(EngineBinariesDir, RuntimeLibraryFileName), Path.Combine(SDKBinariesDir, RuntimeLibraryFileName));

				// needed for linux to find the .so
				PublicRuntimeLibraryPaths.Add(EngineBinariesDir);
				
				if (bRequiresRuntimeLoad)
				{
					PublicDelayLoadDLLs.Add(RuntimeLibraryFileName);
				}
			}
		}
		
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "EOSSDK-Win64-Shipping.lib"));
			PublicDelayLoadDLLs.Add("EOSSDK-Win64-Shipping.dll");
			RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "EOSSDK-Win64-Shipping.dll"));
		}
	}
}
