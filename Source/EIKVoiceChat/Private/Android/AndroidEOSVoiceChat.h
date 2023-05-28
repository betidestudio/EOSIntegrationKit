// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EOSVoiceChat.h"

#if WITH_EOS_RTC

class FAndroidEOSVoiceChat : public FEOSVoiceChat
{
public:
	FAndroidEOSVoiceChat(IEOSSDKManager& InSDKManager, const IEOSPlatformHandlePtr& InPlatformHandle);
	virtual ~FAndroidEOSVoiceChat() = default;

	// ~Begin IVoiceChat Interface 
	virtual IVoiceChatUser* CreateUser() override;
	// ~End IVoiceChat Interface
};

using FPlatformEOSVoiceChat = FAndroidEOSVoiceChat;

#endif // WITH_EOS_RTC