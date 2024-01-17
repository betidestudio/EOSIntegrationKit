// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if WITH_EOS_SDK
#if defined(EOS_PLATFORM_BASE_FILE_NAME)
#include EOS_PLATFORM_BASE_FILE_NAME
#endif

#include "eos_common.h"
#endif

class SOCKETSUBSYSTEMEIK_API ISocketSubsystemEOSUtils
{
public:
	virtual ~ISocketSubsystemEOSUtils() {};

#if WITH_EOS_SDK
	virtual EOS_ProductUserId GetLocalUserId() = 0;
#endif
	virtual FString GetSessionId() = 0;

	virtual FName GetSubsystemInstanceName() = 0;
};
typedef TSharedPtr<ISocketSubsystemEOSUtils, ESPMode::ThreadSafe> ISocketSubsystemEOSUtilsPtr;
