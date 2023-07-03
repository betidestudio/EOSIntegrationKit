//Copyright (c) 2023 Betide Studio. All Rights Reserved.


using System;
using System.IO;
using UnrealBuildTool;

public class EIKSDK : ModuleRules
{
	public string SDKBaseDir
	{
		get { return ModuleDirectory; }
	}

	public virtual string SDKBinariesDir
	{
		get
		{
			if (Target.Platform == UnrealTargetPlatform.Android)
			{
				return Path.Combine(SDKBaseDir, "Bin", "Android");
			}
			else if (Target.Platform == UnrealTargetPlatform.IOS)
			{
				return Path.Combine(SDKBaseDir, "Bin", "IOS");
			}

			return Path.Combine(SDKBaseDir, "Bin");
		}
	}

	public virtual string SDKLibsDir
	{
		get { return Path.Combine(SDKBaseDir, "Lib"); }
	}

	public virtual string SDKIncludesDir
	{
		get
		{
			if (Target.Platform == UnrealTargetPlatform.Android)
			{
				return Path.Combine(SDKBinariesDir, "include");
			}
			else if (Target.Platform == UnrealTargetPlatform.IOS)
			{
				return Path.Combine(SDKBinariesDir, "EOSSDK.framework", "Headers");
			}


			return Path.Combine(SDKBaseDir, "Include");
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
		Console.WriteLine(RuntimeLibraryFileName);
		PublicIncludePaths.Add(SDKIncludesDir);
		PublicSystemIncludePaths.Add(SDKIncludesDir);
		PrivateIncludePaths.Add(SDKIncludesDir);
		PublicDefinitions.Add("EOSSDK_USE_PROJECT_BINARY=1");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			string PluginDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
			string DLLFileName = "EOSSDK-Win64-Shipping.dll";
			string DLLSourcePath = Path.Combine(PluginDir, "ThirdParty", "EIKSDK", "Bin", DLLFileName);
			string DLLTargetPath = "$(BinaryOutputDir)/" + DLLFileName;

			string LIBFileName = "EOSSDK-Win64-Shipping.lib";
			string LIBSourcePath = Path.Combine(ModuleDirectory, "Bin", LIBFileName);
			string LIBTargetPath = "$(BinaryOutputDir)/" + LIBFileName;

			if (File.Exists(DLLSourcePath))
			{
				Console.WriteLine("EOS Integration Kit: DLL file exists at the specified source path.");
			}
			else
			{
				Console.WriteLine("EOS Integration Kit: DLL file does not exist at the specified source path.");
			}

			if (File.Exists(LIBSourcePath))
			{
				Console.WriteLine("EOS Integration Kit: LIB file exists at the specified source path.");
			}
			else
			{
				Console.WriteLine("EOS Integration Kit: LIB file does not exist at the specified source path.");
			}

			// Add the import library
			PublicAdditionalLibraries.Add(LIBSourcePath);

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add(DLLFileName);

			// Ensure that the DLL and LIB are staged along with the executable
			RuntimeDependencies.Add(DLLTargetPath, DLLSourcePath, StagedFileType.NonUFS);
			RuntimeDependencies.Add(LIBTargetPath, LIBSourcePath, StagedFileType.NonUFS);
		}

		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			PrivateRuntimeLibraryPaths.Add(SDKBinariesDir);
			PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "libEOSSDK-Linux-Shipping.so"));
			RuntimeDependencies.Add(Path.Combine("$(TargetOutputDir)", "libEOSSDK-Linux-Shipping.so"), Path.Combine(SDKBinariesDir, "libEOSSDK-Linux-Shipping.so"));
			RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "libEOSSDK-Linux-Shipping.so"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			PublicDelayLoadDLLs.Add(Path.Combine(SDKBinariesDir, "libEOSSDK-Mac-Shipping.dylib"));
			RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "libEOSSDK-Mac-Shipping.dylib"));
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PublicAdditionalFrameworks.Add(new Framework("EOSSDK", SDKBinariesDir, "", true));
			AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(SDKBaseDir, "LibEIK_UPL.xml"));
		} 
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "arm64-v8a", "libEOSSDK.so"));
			//PublicAdditionalLibraries.Add(Path.Combine(SDKBinariesDir, "armeabi-v7a", "libEOSSDK.so"));

			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(SDKBaseDir, "LibEIK_APL.xml"));
		}
	}
}
