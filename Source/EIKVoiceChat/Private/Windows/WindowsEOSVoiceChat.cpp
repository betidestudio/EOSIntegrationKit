// Copyright Epic Games, Inc. All Rights Reserved.

#include "WindowsEOSVoiceChat.h"

#if WITH_EOS_RTC

#include "HAL/FileManager.h"
#include "Misc/Paths.h"

#include "Windows/eos_Windows.h"

FWindowsEOSVoiceChat::FWindowsEOSVoiceChat(IEOSSDKManager& InSDKManager, const IEIKPlatformHandlePtr& InPlatformHandle)
	: FEOSVoiceChat(InSDKManager, InPlatformHandle)
{
}

IEIKPlatformHandlePtr FWindowsEOSVoiceChat::EOSPlatformCreate(EOS_Platform_Options& PlatformOptions)
{
	const FString XAudioPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::Combine(FPaths::EngineDir(), TEXT("Binaries/ThirdParty/Windows/XAudio2_9"), PLATFORM_64BITS ? TEXT("x64") : TEXT("x86"), TEXT("xaudio2_9redist.dll")));
	const FTCHARToUTF8 Utf8XAudioPath(*XAudioPath);

	EOS_Windows_RTCOptions WindowsRTCOptions = { 0 };
	WindowsRTCOptions.ApiVersion = EOS_WINDOWS_RTCOPTIONS_API_LATEST;
	static_assert(EOS_WINDOWS_RTCOPTIONS_API_LATEST == 1, "EOS_Windows_RTCOptions updated, check new fields");
	WindowsRTCOptions.XAudio29DllPath = Utf8XAudioPath.Get();

	const_cast<EOS_Platform_RTCOptions*>(PlatformOptions.RTCOptions)->PlatformSpecificOptions = &WindowsRTCOptions;

	return FEOSVoiceChat::EOSPlatformCreate(PlatformOptions);
}

#endif // WITH_EOS_RTC