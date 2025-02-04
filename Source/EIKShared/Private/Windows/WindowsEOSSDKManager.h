// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_EOS_SDK

#include "EOSSDKManager.h"

class FWindowsEOSSDKManager : public FEIKSDKManager
{
protected:
	virtual IEIKPlatformHandlePtr CreatePlatform(const FEOSSDKPlatformConfig& PlatformConfig, EOS_Platform_Options& PlatformOptions) override;
};

using FPlatformEOSSDKManager = FWindowsEOSSDKManager;

#endif // WITH_EOS_SDK