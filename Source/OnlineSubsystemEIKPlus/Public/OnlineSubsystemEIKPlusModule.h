//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

/**
 * OSS that aggregates EOS plus the platform OSS
 */
class FOnlineSubsystemEIKPlusModule : public IModuleInterface
{
public:

	FOnlineSubsystemEIKPlusModule() :
		PlusFactory(nullptr)
	{}

	virtual ~FOnlineSubsystemEIKPlusModule() {}


// IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}

	virtual bool SupportsAutomaticShutdown() override
	{
		return false;
	}
// ~IModuleInterface

private:
	void LoadBaseOSS();

	/** Class responsible for creating instance(s) of the subsystem */
	class FOnlineFactoryEOSPlus* PlusFactory;
};
