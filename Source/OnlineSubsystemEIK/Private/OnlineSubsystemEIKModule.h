// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

/**
 * Online subsystem module class  (EOS Implementation)
 * Code related to the loading of the EOS module
 */
class FOnlineSubsystemEIKModule : public IModuleInterface
{
private:

	/** Class responsible for creating instance(s) of the subsystem */
	class FOnlineFactoryEOS* EOSFactory;

public:

	FOnlineSubsystemEIKModule() :
		EOSFactory(NULL)
	{}

	virtual ~FOnlineSubsystemEIKModule() {}

#if WITH_EDITOR
	void OnPostEngineInit();
	void OnPreExit();
#endif

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
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	void ConfigureOnlineSubsystemEIK();
};