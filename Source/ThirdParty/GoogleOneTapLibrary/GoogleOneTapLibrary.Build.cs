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
            PublicDependencyModuleNames.AddRange(new string[] { "Launch" });

            string ModulePath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModulePath, "GoogleOneTap_APL.xml"));
        }
		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
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
	}
}
