// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"

#include "EOSSDKManager.h"

class FEOSSharedModule: public IModuleInterface
{
public:
	FEOSSharedModule() = default;
	~FEOSSharedModule() = default;

private:
	// ~Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// ~End IModuleInterface

#if WITH_EOS_SDK
	TUniquePtr<FEIKSDKManager> SDKManager;
#endif
};