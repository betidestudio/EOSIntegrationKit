#include "EOSIntegrationKit.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/MessageDialog.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMisc.h"
#include "Runtime/Launch/Resources/Version.h"
#define LOCTEXT_NAMESPACE "FEOSIntegrationKitModule"


DEFINE_LOG_CATEGORY(LogEOSIntegrationKit);
void FEOSIntegrationKitModule::StartupModule()
{
    ConfigureOnlineSubsystemEIK();
#if WITH_EDITOR && ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
    //Special love and respect to Redpoint Dev but this had to be added as a warning and you know why 🙏
    if(IPluginManager::Get().FindPlugin("EOSCore") != nullptr && IPluginManager::Get().FindPlugin("EOSCore")->IsEnabled())
    {
        const FText Message = LOCTEXT("EOSIntegrationKitError","EOS Integration Kit Error");
        FMessageDialog::Open(EAppMsgCategory::Warning,EAppMsgType::Ok,LOCTEXT("EOS Integration Kit Error","EOSCore Plugin is enabled which will not allow EOS Integration Kit to work properly. Please disable EOSCore Plugin and restart the editor. The basic reasoning behind this is that the EOS SDK is already included in our plugin and you cannot include the SDK twice."));
    }
    if(IPluginManager::Get().FindPlugin("OnlineSubsystemRedpoint") != nullptr && IPluginManager::Get().FindPlugin("OnlineSubsystemRedpoint")->IsEnabled())
    {
        const FText Message = LOCTEXT("EOSIntegrationKitError","EOS Integration Kit Error");
        FMessageDialog::Open(EAppMsgCategory::Warning,EAppMsgType::Ok,LOCTEXT("EOS Integration Kit Error","OnlineSubsystemRedpoint Plugin is enabled which will not allow EOS Integration Kit to work properly. Please disable OnlineSubsystemRedpoint Plugin and restart the editor. The basic reasoning behind this is that the EOS SDK is already included in our plugin and you cannot include the SDK twice."));
    }
#endif
}

void FEOSIntegrationKitModule::ConfigureOnlineSubsystemEIK() const
{
    FString EngineIniPath = FPaths::ProjectConfigDir() / TEXT("DefaultEngine.ini");
    FString EngineIniText;

    if (FFileHelper::LoadFileToString(EngineIniText, *EngineIniPath))
    {
        FString SectionName = TEXT("/Script/EOSIntegrationKit.EIKSettings");
        FString KeyName = TEXT("bAutomaticallySetupEIK");
        bool bAutomaticallySetupEIK = false;
        // Read the value from the ini file
        if (GConfig->GetBool(*SectionName, *KeyName, bAutomaticallySetupEIK, GEngineIni) && bAutomaticallySetupEIK)
        {
            UE_LOG(LogEOSIntegrationKit, Log, TEXT("Automatically setting up OnlineSubsystemEIK"));
            bool bConfigChanged = false;

            // Check if [OnlineSubsystemEIK] section exists and add it if not
            if (!EngineIniText.Contains(TEXT("[OnlineSubsystemEIK]")))
            {
                EngineIniText += TEXT("\n[OnlineSubsystemEIK]\nbEnabled=true\n");
                bConfigChanged = true;
            }

            // Update [OnlineSubsystem] section
            FString OnlineSubsystemSection = TEXT("[OnlineSubsystem]");
            if (EngineIniText.Contains(OnlineSubsystemSection))
            {
                // Find the existing DefaultPlatformService entry and update it if needed
                int32 SectionStart = EngineIniText.Find(OnlineSubsystemSection);
                int32 SectionEnd = EngineIniText.Find(TEXT("\n["), ESearchCase::IgnoreCase, ESearchDir::FromStart, SectionStart + 1);

                if (SectionEnd == INDEX_NONE)
                {
                    SectionEnd = EngineIniText.Len();
                }

                FString OnlineSubsystemContent = EngineIniText.Mid(SectionStart, SectionEnd - SectionStart);
                if (!OnlineSubsystemContent.Contains(TEXT("DefaultPlatformService=EIK")))
                {
                    int32 DefaultPlatformServiceIndex = OnlineSubsystemContent.Find(TEXT("DefaultPlatformService="));
                    if (DefaultPlatformServiceIndex != INDEX_NONE)
                    {
                        // Replace the existing DefaultPlatformService value
                        int32 LineEndIndex = OnlineSubsystemContent.Find(TEXT("\n"), ESearchCase::IgnoreCase, ESearchDir::FromStart, DefaultPlatformServiceIndex);
                        if (LineEndIndex == INDEX_NONE)
                        {
                            LineEndIndex = OnlineSubsystemContent.Len();
                        }

                        FString ExistingLine = OnlineSubsystemContent.Mid(DefaultPlatformServiceIndex, LineEndIndex - DefaultPlatformServiceIndex);
                        OnlineSubsystemContent.ReplaceInline(*ExistingLine, TEXT("DefaultPlatformService=EIK"));
                    }
                    else
                    {
                        // Add the DefaultPlatformService setting
                        OnlineSubsystemContent += TEXT("\nDefaultPlatformService=EIK");
                    }
                    EngineIniText.ReplaceInline(*EngineIniText.Mid(SectionStart, SectionEnd - SectionStart), *OnlineSubsystemContent);
                    bConfigChanged = true;
                }
            }
            else
            {
                // Add the [OnlineSubsystem] section
                EngineIniText += TEXT("\n[OnlineSubsystem]\nDefaultPlatformService=EIK\n");
                bConfigChanged = true;
            }

            // Update [/Script/OnlineSubsystemEIK.NetDriverEIK] section
            if (!EngineIniText.Contains(TEXT("[/Script/OnlineSubsystemEIK.NetDriverEIK]")))
            {
                FString Comment = TEXT("\n;EIK Comment: You do not need to worry about this setting as we dynamically set it in Travel URL depending upon if we are using Listen Server or Dedicated Server\n");
                EngineIniText += Comment;
                EngineIniText += TEXT("[/Script/OnlineSubsystemEIK.NetDriverEIK]\nbIsUsingP2PSockets=true\n");
                bConfigChanged = true;
            }

            // Update NetDriverDefinitions section (version-specific)
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 6
            // UE 5.6+ uses [/Script/Engine.Engine] and requires /Script/ prefix for class paths
            if (!EngineIniText.Contains(TEXT("[/Script/Engine.Engine]")))
            {
                EngineIniText += TEXT("\n[/Script/Engine.Engine]\n");

                // Update NetDriverDefinitions in [/Script/Engine.Engine] section
                FString NetDriverDefinitions = FString::Printf(
                    TEXT("!NetDriverDefinitions=ClearArray\n+NetDriverDefinitions=(DefName=\"GameNetDriver\",DriverClassName=\"/Script/OnlineSubsystemEIK.NetDriverEIK\",DriverClassNameFallback=\"OnlineSubsystemUtils.IpNetDriver\")\n+NetDriverDefinitions=(DefName=\"BeaconNetDriver\",DriverClassName=\"/Script/OnlineSubsystemEIK.NetDriverEIK\",DriverClassNameFallback=\"OnlineSubsystemUtils.IpNetDriver\")\n")
                );
                EngineIniText += NetDriverDefinitions;

                bConfigChanged = true;
            }
#else
            // UE 5.5 and below use [/Script/Engine.GameEngine] and old class path format
            if (!EngineIniText.Contains(TEXT("[/Script/Engine.GameEngine]")))
            {
                EngineIniText += TEXT("\n[/Script/Engine.GameEngine]\n");

                // Update NetDriverDefinitions in [/Script/Engine.GameEngine] section
                FString NetDriverDefinitions = FString::Printf(
                    TEXT("!NetDriverDefinitions=ClearArray\n+NetDriverDefinitions=(DefName=\"GameNetDriver\",DriverClassName=\"OnlineSubsystemEIK.NetDriverEIK\",DriverClassNameFallback=\"OnlineSubsystemUtils.IpNetDriver\")\n")
                );
                EngineIniText += NetDriverDefinitions;

                bConfigChanged = true;
            }
#endif

            // Save the modified text back to the DefaultEngine.ini file if any changes were made
            if (bConfigChanged)
            {
                if (FFileHelper::SaveStringToFile(EngineIniText, *EngineIniPath))
                {
                    UE_LOG(LogEOSIntegrationKit, Log, TEXT("OnlineSubsystemEIK configuration added/updated in DefaultEngine.ini"));
                }
                else
                {
                    UE_LOG(LogEOSIntegrationKit, Error, TEXT("Failed to save modified DefaultEngine.ini"));
                }
            }
            else
            {
                UE_LOG(LogEOSIntegrationKit, Log, TEXT("OnlineSubsystemEIK configuration already exists in DefaultEngine.ini"));
            }
        }
        else
        {
            UE_LOG(LogEOSIntegrationKit, Log, TEXT("Automatically setting up OnlineSubsystemEIK is disabled"));
        }
    }
    else
    {
        UE_LOG(LogEOSIntegrationKit, Log, TEXT("Failed to load DefaultEngine.ini"));
    }
}



void FEOSIntegrationKitModule::ConfigureDedicatedServerConfigEIK()
{
    FString EngineIniPath = FPaths::ProjectConfigDir() / TEXT("WindowsServer") / TEXT("Engine.ini");
}

void FEOSIntegrationKitModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FEOSIntegrationKitModule, EOSIntegrationKit)