// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "eos_init.h"

#if WITH_EOS_SDK

#include "EOSSDKManager.h"

class FAndroidEOSSDKManager : public FEIKSDKManager
{
public:
	virtual EOS_EResult EOSInitialize(EOS_InitializeOptions& Options) override;
	virtual FString GetCacheDirBase() const override;
};

using FPlatformEOSSDKManager = FAndroidEOSSDKManager;

#endif // WITH_EOS_SDK