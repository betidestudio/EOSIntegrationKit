// Copyright Epic Games, Inc. All Rights Reserved.

#include "AndroidEOSVoiceChat.h"

#if WITH_EOS_RTC

#include "AndroidEOSVoiceChatUser.h"

FAndroidEOSVoiceChat::FAndroidEOSVoiceChat(IEOSSDKManager& InSDKManager, const IEIKPlatformHandlePtr& InPlatformHandle)
	: FEOSVoiceChat(InSDKManager, InPlatformHandle)
{
}

IVoiceChatUser* FAndroidEOSVoiceChat::CreateUser()
{
	const FEOSVoiceChatUserRef& User = VoiceChatUsers.Emplace_GetRef(MakeShared<FAndroidEOSVoiceChatUser, ESPMode::ThreadSafe>(*this));
	return &User.Get();
}

#endif // WITH_EOS_RTC