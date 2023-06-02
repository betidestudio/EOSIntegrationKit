//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "..\Public\OnlineSubsystemEIKPlusModule.h"
#include "OnlineSubsystemModule.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystemEIKPlus.h"

#include "Features/IModularFeature.h"
#include "Features/IModularFeatures.h"
#include "Misc/ConfigCacheIni.h"

#define LOCTEXT_NAMESPACE "EOSPlus"

IMPLEMENT_MODULE(FOnlineSubsystemEIKPlusModule, OnlineSubsystemEIKPlus);

/**
 * Class responsible for creating instance(s) of the subsystem
 */
class FOnlineFactoryEOSPlus :
	public IOnlineFactory
{
public:

	FOnlineFactoryEOSPlus() {}
	virtual ~FOnlineFactoryEOSPlus() {}

	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName)
	{
		FOnlineSubsystemEOSPlusPtr OnlineSub = MakeShared<FOnlineSubsystemEOSPlus, ESPMode::ThreadSafe>(InstanceName);
		if (!OnlineSub->Init())
		{
			UE_LOG_ONLINE(Warning, TEXT("EOSPlus failed to initialize!"));
			OnlineSub->Shutdown();
			OnlineSub = nullptr;
		}

		return OnlineSub;
	}
};

void FOnlineSubsystemEIKPlusModule::StartupModule()
{
	if (IsRunningCommandlet())
	{
		return;
	}

	// Force loading of the EOS OSS module in case the plugin manager failed to get the dependencies right
	FModuleManager::LoadModuleChecked<FOnlineSubsystemModule>(TEXT("OnlineSubsystemEIK"));
	// Make sure the base OSS has a chance to register its OSS factory
	LoadBaseOSS();

	PlusFactory = new FOnlineFactoryEOSPlus();

	// Create and register our singleton factory with the main online subsystem for easy access
	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.RegisterPlatformService(EOSPLUS_SUBSYSTEM, PlusFactory);
}

void FOnlineSubsystemEIKPlusModule::ShutdownModule()
{
	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.UnregisterPlatformService(EOSPLUS_SUBSYSTEM);

	delete PlusFactory;
	PlusFactory = nullptr;
}

void FOnlineSubsystemEIKPlusModule::LoadBaseOSS()
{
	FString BaseOSSName;
	GConfig->GetString(TEXT("[OnlineSubsystemEIKPlus]"), TEXT("BaseOSSName"), BaseOSSName, GEngineIni);
	if (BaseOSSName.IsEmpty())
	{
		// Load the native platform OSS name
		GConfig->GetString(TEXT("OnlineSubsystem"), TEXT("NativePlatformService"), BaseOSSName, GEngineIni);
	}
	if (BaseOSSName.IsEmpty())
	{
		UE_LOG_ONLINE(Verbose, TEXT("EOSPlus failed to find the native OSS!"));
		return;
	}
	// Load the module for the base OSS
	FString ModuleName(TEXT("OnlineSubsystem") + BaseOSSName);
	FModuleManager::LoadModuleChecked<FOnlineSubsystemModule>(*ModuleName);
}

#undef LOCTEXT_NAMESPACE
