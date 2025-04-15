using System;
using System.IO;
using UnrealBuildTool;

public class EIKLoginMethods : ModuleRules
{
    public EIKLoginMethods(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "Launch" });

            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            Console.WriteLine("PluginPath: " + PluginPath);
            //AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "GoogleOneTap_UPL.xml"));
        }
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "OnlineSubsystemEIK",
                "GoogleOneTapLibrary",
                "GooglePlayGamesLibrary",
                "Json",
                "JsonUtilities"
            }
        );
    }
}