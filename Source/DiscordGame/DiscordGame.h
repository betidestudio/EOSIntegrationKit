// Copyright (c) 2024 xist.gg

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDiscord, Log, All);

class FDiscordGameModule : public IModuleInterface
{
public:
	/** Name of this module */
	static const FName ModuleName;

	/**
	 * @return Pointer to FDiscordGameModule, if it has been loaded into the Engine, else nullptr
	 */
	static FDiscordGameModule* Get()
	{
		return static_cast<FDiscordGameModule*>(FModuleManager::Get().GetModule(ModuleName));
	}

	/**
	 * @return TRUE if we successfully loaded the Discord GameSDK DLL; else FALSE
	 */
	FORCEINLINE bool IsDiscordSDKLoaded() const { return bDiscordSDKLoaded; }
	//~IModuleInterface interface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//~End of IModuleInterface interface

protected:
	/**
	 * Get the path to the Discord GameSDK DLL for the current platform.
	 * @return Absolute path to the Discord GameSDK DLL we need to load
	 */
	FString GetPathToDLL() const;

private:
	/** Handle to the dll we will load */
	void* DiscordGameSDKHandle {nullptr};
	bool bDiscordSDKLoaded = false;

};
