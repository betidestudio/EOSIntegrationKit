// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_EOS_SDK

#if defined(EOS_PLATFORM_BASE_FILE_NAME)
#include EOS_PLATFORM_BASE_FILE_NAME
#endif

#include "eos_auth_types.h"
#include "eos_types.h"
#include "Interfaces/OnlineExternalUIInterface.h"

class FOnlineSubsystemEOS;
using IEOSPlatformHandlePtr = TSharedPtr<class IEOSPlatformHandle, ESPMode::ThreadSafe>;

class FEOSHelpers
{
public:
	virtual ~FEOSHelpers() = default;

	virtual void PlatformAuthCredentials(EOS_Auth_Credentials &Credentials);
	virtual void PlatformTriggerLoginUI(FOnlineSubsystemEOS* EOSSubsystem, const int ControllerIndex, bool bShowOnlineOnly, bool bShowSkipButton, const FOnLoginUIClosedDelegate& Delegate);
	virtual IEOSPlatformHandlePtr CreatePlatform(EOS_Platform_Options& PlatformOptions);
};

#endif // WITH_EOS_SDK