// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EOSVoiceChatUser.h"

#if WITH_EOS_RTC

class FAndroidEOSVoiceChatUser : public FEOSVoiceChatUser
{
public:
	FAndroidEOSVoiceChatUser(FEOSVoiceChat& InEOSVoiceChat) : FEOSVoiceChatUser(InEOSVoiceChat) {}
	virtual ~FAndroidEOSVoiceChatUser() = default;

	// ~Begin IVoiceChatUser Interface
	virtual void SetSetting(const FString& Name, const FString& Value) override;
	virtual FString GetSetting(const FString& Name) override;
	// ~End IVoiceChatUser Interface

private:
	bool bEnableBluetoothMicrophone = false;
};

#endif // WITH_EOS_RTC