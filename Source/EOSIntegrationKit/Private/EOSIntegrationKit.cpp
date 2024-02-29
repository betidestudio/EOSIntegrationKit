#include "EOSIntegrationKit.h"

#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Misc/MessageDialog.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMisc.h"
#include "Runtime/Launch/Resources/Version.h"
#define LOCTEXT_NAMESPACE "FEOSIntegrationKitModule"

void FEOSIntegrationKitModule::StartupModule()
{
    ConfigureOnlineSubsystemEIK();
#if WITH_EDITOR && ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
    if(IPluginManager::Get().FindPlugin("OnlineSubsystemEOS") != nullptr && IPluginManager::Get().FindPlugin("OnlineSubsystemEOS")->IsEnabled())
    {
        const FText Message = LOCTEXT("EOSIntegrationKitError","EOS Integration Kit Error");
        FMessageDialog::Open(EAppMsgCategory::Warning,EAppMsgType::Ok,LOCTEXT("EOS Integration Kit Error","OnlineSubsystemEOS Plugin is enabled which will not allow EOS Integration Kit to work properly. Please disable OnlineSubsystemEOS Plugin and restart the editor. The basic reasoning behind this is that the EOS SDK is already included in our plugin and you cannot include the SDK twice."));
    }
    else
    {
        if(IPluginManager::Get().FindPlugin("EOSShared")->IsEnabled() || IPluginManager::Get().FindPlugin("EOSVoiceChat")->IsEnabled() || IPluginManager::Get().FindPlugin("SocketSubsystemEOS")->IsEnabled())
        {
            const FText Message = LOCTEXT("EOSIntegrationKitError","EOS Integration Kit Error");
            FMessageDialog::Open(EAppMsgCategory::Warning,EAppMsgType::Ok,LOCTEXT("EOS Integration Kit Error","One of the following plugins is enabled: EOSShared, EOSVoiceChat, SocketSubsystemEOS. Please disable these plugins and restart the editor."));
        }
    }
    //Special love and respect to eelDev and Redpoint Devs but this had to be added as a warning and you know why 🙏
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
    ConfigureDedicatedServerConfigEIK();
    FString EngineIniPath = FPaths::ProjectConfigDir() / TEXT("DefaultEngine.ini");
    FString EngineIniText;
    if (FFileHelper::LoadFileToString(EngineIniText, *EngineIniPath))
    {
        if (EngineIniText.Contains(TEXT("bAutomaticallySetupEIK=False")))
        {
            return;
        }
        bool bConfigChanged = false;

        // Check if [OnlineSubsystemEIK] section exists and add it if not
        if (!EngineIniText.Contains(TEXT("[OnlineSubsystemEIK]")))
        {
            EngineIniText += TEXT("\n[OnlineSubsystemEIK]\nbEnabled=true\n");
            bConfigChanged = true;
        }

        // Update [OnlineSubsystem] section
        if (!EngineIniText.Contains(TEXT("[OnlineSubsystem]")))
        {
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

        // Update [/Script/Engine.GameEngine] section
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

        // Save the modified text back to the DefaultEngine.ini file if any changes were made
        if (bConfigChanged)
        {
            if (FFileHelper::SaveStringToFile(EngineIniText, *EngineIniPath))
            {
                UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystemEIK configuration added/updated in DefaultEngine.ini"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to save modified DefaultEngine.ini"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystemEIK configuration already exists in DefaultEngine.ini"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load DefaultEngine.ini"));
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