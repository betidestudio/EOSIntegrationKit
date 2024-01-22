// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_EOS_SDK

#include "EOSSDKManager.h"

class FIOSEOSSDKManager : public FEIKSDKManager
{
	virtual FString GetCacheDirBase() const override;
};

using FPlatformEOSSDKManager = FIOSEOSSDKManager;

#endif // WITH_EOS_SDK