//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "EIKCommands.h"
#include "EIKStyle.h"
#include "OnlineSubsystemEIKModule.h"
#include "OnlineSubsystemEOSPrivate.h"
#include "OnlineSubsystemModule.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEOSTypes.h"
#include "..\Public\EIKSettings.h"

#include "Features/IModularFeature.h"
#include "Features/IModularFeatures.h"

#include "Misc/CoreDelegates.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/LazySingleton.h"
#include "Misc/MessageDialog.h"

#if WITH_EDITOR
	#include "ISettingsModule.h"
	#include "ISettingsSection.h"
#endif

#define LOCTEXT_NAMESPACE "EIK"

IMPLEMENT_MODULE(FOnlineSubsystemEIKModule, OnlineSubsystemEIK);

/**
 * Class responsible for creating instance(s) of the subsystem
 */
class FOnlineFactoryEOS :
	public IOnlineFactory
{
public:

	FOnlineFactoryEOS() {}
	virtual ~FOnlineFactoryEOS() {}

	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName)
	{
		FOnlineSubsystemEOSPtr OnlineSub = MakeShared<FOnlineSubsystemEOS, ESPMode::ThreadSafe>(InstanceName);
		if (!OnlineSub->Init())
		{
			UE_LOG_ONLINE(Warning, TEXT("EOS API failed to initialize!"));
			OnlineSub->Shutdown();
			OnlineSub = nullptr;
		}

		return OnlineSub;
	}
};

void FOnlineSubsystemEIKModule::StartupModule()
{
	if (IsRunningCommandlet())
	{
		return;
	}

	EOSFactory = new FOnlineFactoryEOS();

	// Create and register our singleton factory with the main online subsystem for easy access
	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.RegisterPlatformService("EIK", EOSFactory);

#if WITH_EOS_SDK
	// Have to call this as early as possible in order to hook the rendering device
	FOnlineSubsystemEOS::ModuleInit();
#endif

	FEIKStyle::Initialize();
	FEIKStyle::ReloadTextures();

	FEIKCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEIKCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FOnlineSubsystemEIKModule::PluginButtonClicked),
		FCanExecuteAction());

#if WITH_EDITOR
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FOnlineSubsystemEIKModule::RegisterMenus));
#endif
#if WITH_EDITOR
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FOnlineSubsystemEIKModule::OnPostEngineInit);
	FCoreDelegates::OnPreExit.AddRaw(this, &FOnlineSubsystemEIKModule::OnPreExit);
#endif
}

#if WITH_EDITOR
void FOnlineSubsystemEIKModule::OnPostEngineInit()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Game", "Online Subsystem EIK",
			LOCTEXT("OSSEOSSettingsName", "EOS Integration Kit"),
			LOCTEXT("OSSEOSSettingsDescription", "Take control of your EOS Integration Kit settings here. Explore the options available or refer to the documentation for further information on how to customize your settings."),
			GetMutableDefault<UEIKSettings>());
	}
}
#endif

#if WITH_EDITOR
void FOnlineSubsystemEIKModule::OnPreExit()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Game", "Online Subsystem EIK");
	}
}
#endif

void FOnlineSubsystemEIKModule::ShutdownModule()
{
	if (IsRunningCommandlet())
	{
		return;
	}

	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
#if WITH_EDITOR

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FEIKStyle::Shutdown();

	FEIKCommands::Unregister();
#endif
#if WITH_EDITOR
	FCoreDelegates::OnPostEngineInit.RemoveAll(this);
	FCoreDelegates::OnPreExit.RemoveAll(this);
#endif

#if WITH_EOS_SDK
	FOnlineSubsystemEOS::ModuleShutdown();
#endif

	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.UnregisterPlatformService("EIK");

	delete EOSFactory;
	EOSFactory = nullptr;

	TLazySingleton<FUniqueNetIdEOSRegistry>::TearDown();
}

void FOnlineSubsystemEIKModule::PluginButtonClicked()
{
#if WITH_EDITOR
	FEOSSettings EOSSettings = UEIKSettings::GetSettings();
	UEIKSettings* UeikSettings = GetMutableDefault<UEIKSettings>();
	FString ProductName = UeikSettings->ProductName;
	FString OrganizationId = UeikSettings->OrganizationName;

	if (!ProductName.IsEmpty() && !OrganizationId.IsEmpty())
	{
		// Open URL with product name and organization ID as parameters
		FString URL = FString::Printf(TEXT("https://dev.epicgames.com/portal/en-US/%s/products/%s/"), *OrganizationId, *ProductName);
		FPlatformProcess::LaunchURL(*URL, nullptr, nullptr);
	}
	else
	{
		// Print error message if either ProductName or OrganizationId is empty
		FString ErrorMessage;
		if (ProductName.IsEmpty())
		{
			ErrorMessage = TEXT("Product name");
		}
		else if (OrganizationId.IsEmpty())
		{
			ErrorMessage = TEXT("Organization Name");
		}
		else if(OrganizationId.IsEmpty() && ProductName.IsEmpty() )
		{
			ErrorMessage = TEXT("Organization Name and Product Name");
		}

		// Show error message to the user
		FString SettingsText = FString::Printf(TEXT("Make sure to set the value of %s in the project settings."), *ErrorMessage);
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(SettingsText));
	}
#endif
}

void FOnlineSubsystemEIKModule::RegisterMenus()
{
#if WITH_EDITOR
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEIKCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FEIKCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
#endif
}

#undef LOCTEXT_NAMESPACE
