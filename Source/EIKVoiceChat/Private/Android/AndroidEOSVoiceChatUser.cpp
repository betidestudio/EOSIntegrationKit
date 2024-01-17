// Copyright Epic Games, Inc. All Rights Reserved.

#include "AndroidEOSVoiceChatUser.h"

#if WITH_EOS_RTC

void FAndroidEOSVoiceChatUser::SetSetting(const FString& Name, const FString& Value)
{
	if (Name == TEXT("BluetoothMicrophone"))
	{
		bEnableBluetoothMicrophone = FCString::ToBool(*Value);
		// TODO
	}
	else
	{
		FEOSVoiceChatUser::SetSetting(Name, Value);
	}
}

FString FAndroidEOSVoiceChatUser::GetSetting(const FString& Name)
{
	if (Name == TEXT("BluetoothMicrophone"))
	{
		return LexToString(bEnableBluetoothMicrophone);
	}
	else
	{
		return FEOSVoiceChatUser::GetSetting(Name);
	}
}

#endif // WITH_EOS_RTC