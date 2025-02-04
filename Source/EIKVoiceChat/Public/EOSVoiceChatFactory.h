// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EOSShared.h"

#if WITH_EOS_RTC

#include "Features/IModularFeatures.h"
#include "Misc/CoreMisc.h"
#include "Templates/SharedPointer.h"
#include "IEOSSDKManager.h"

using IVoiceChatPtr = TSharedPtr<class IVoiceChat, ESPMode::ThreadSafe>;
using IVoiceChatWeakPtr = TWeakPtr<class IVoiceChat, ESPMode::ThreadSafe>;

class EIKVOICECHAT_API FEOSVoiceChatFactory : public IModularFeature, public FSelfRegisteringExec
{
public:
	static FEOSVoiceChatFactory* Get()
	{
		if (IModularFeatures::Get().IsModularFeatureAvailable(GetModularFeatureName()))
		{
			return &IModularFeatures::Get().GetModularFeature<FEOSVoiceChatFactory>(GetModularFeatureName());
		}
		return nullptr;
	}

	static FName GetModularFeatureName()
	{
		static const FName FeatureName = TEXT("EOSVoiceChatFactory");
		return FeatureName;
	}

	virtual ~FEOSVoiceChatFactory() = default;

	/** Create an instance with its own EOS platform. */
	IVoiceChatPtr CreateInstance();
	/** Create an instance sharing an existing EOS platform. Used to enable interaction with e.g. lobby voice channels via an IVoiceChat interface. */
	IVoiceChatPtr CreateInstanceWithPlatform(const IEIKPlatformHandlePtr& PlatformHandle);

	// ~Begin FSelfRegisteringExec
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	// ~End FSelfRegisteringExec

private:
	TArray<IVoiceChatWeakPtr> Instances;
};

#endif // WITH_EOS_RTC