// Copyright Epic Games, Inc. All Rights Reserved.

#include "EOSVoiceChatFactory.h"

#if WITH_EOS_RTC

#include "Misc/Parse.h"
#include "IEOSSDKManager.h"
#include "EOSVoiceChatLog.h"

#include COMPILED_PLATFORM_HEADER(EOSVoiceChat.h)

IVoiceChatPtr FEOSVoiceChatFactory::CreateInstance()
{
	IEOSSDKManager* EOSSDKManager = IEOSSDKManager::Get();
	check(EOSSDKManager);

	FEOSVoiceChatPtr VoiceChat = MakeShared<FPlatformEOSVoiceChat, ESPMode::ThreadSafe>(*EOSSDKManager, nullptr);
	Instances.Emplace(VoiceChat);
	return VoiceChat;
}

IVoiceChatPtr FEOSVoiceChatFactory::CreateInstanceWithPlatform(const IEIKPlatformHandlePtr& PlatformHandle)
{
	IEOSSDKManager* EOSSDKManager = IEOSSDKManager::Get();
	check(EOSSDKManager);

	FEOSVoiceChatPtr VoiceChat = MakeShared<FPlatformEOSVoiceChat, ESPMode::ThreadSafe>(*EOSSDKManager, PlatformHandle);
	if (VoiceChat->Initialize())
	{
		TOptional<bool> bConnectResult;
		VoiceChat->Connect(FOnVoiceChatConnectCompleteDelegate::CreateLambda([&bConnectResult](const FVoiceChatResult& VoiceChatResult)
		{
			bConnectResult = VoiceChatResult.IsSuccess();
		}));

		while (!bConnectResult.IsSet())
		{
			PlatformHandle->Tick();
		}

		if (!bConnectResult.GetValue())
		{
			UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChatFactory::CreateInstance Connect failed"));
			VoiceChat.Reset();
		}
	}
	else
	{
		UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChatFactory::CreateInstance Initialize failed"));
		VoiceChat.Reset();
	}

	if(VoiceChat)
	{
		Instances.Emplace(VoiceChat);
	}
	return VoiceChat;
}

bool FEOSVoiceChatFactory::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	// When list is requested, the factory is responsible for returning true when all instances have reported their state.
	bool bListRequested = false;

	const TCHAR* CmdCopy = Cmd;
	if (FParse::Command(&CmdCopy, TEXT("EOSVOICECHAT")))
	{
		if (FParse::Command(&CmdCopy, TEXT("LIST")))
		{
			bListRequested = true;
		}
	}

	for (TArray<IVoiceChatWeakPtr>::TIterator Iter = Instances.CreateIterator(); Iter; ++Iter)
	{
		if (IVoiceChatPtr StrongPtr = Iter->Pin())
		{
			FEOSVoiceChat& EosVoiceChat = static_cast<FEOSVoiceChat&>(*StrongPtr);
			const bool bExecResult = EosVoiceChat.Exec(InWorld, Cmd, Ar);
			if (!bListRequested && bExecResult)
			{
				return true;
			}
		}
		else
		{
			Iter.RemoveCurrent();
		}
	}

	return bListRequested;
}

#endif // WITH_EOS_RTC