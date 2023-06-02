//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#if WITH_EOS_SDK

#include "EOSSDKManager.h"

class FWindowsEOSSDKManager : public FEIKSDKManager
{
protected:
	virtual IEOSPlatformHandlePtr CreatePlatform(const FEOSSDKPlatformConfig& PlatformConfig, EOS_Platform_Options& PlatformOptions) override;
};

using FPlatformEOSSDKManager = FWindowsEOSSDKManager;

#endif // WITH_EOS_SDK