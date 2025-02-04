// Copyright Epic Games, Inc. All Rights Reserved.

#if WITH_EOS_SDK

#include "WindowsEOSSDKManager.h"

#include "HAL/FileManager.h"
#include "Misc/Paths.h"

#include "Windows/eos_Windows.h"

IEIKPlatformHandlePtr FWindowsEOSSDKManager::CreatePlatform(const FEOSSDKPlatformConfig& PlatformConfig, EOS_Platform_Options& PlatformOptions)
{
	if (PlatformConfig.bWindowsEnableOverlayD3D9) PlatformOptions.Flags |= EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D9;
	if (PlatformConfig.bWindowsEnableOverlayD3D10) PlatformOptions.Flags |= EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D10;
	if (PlatformConfig.bWindowsEnableOverlayOpenGL) PlatformOptions.Flags |= EOS_PF_WINDOWS_ENABLE_OVERLAY_OPENGL;

	if (PlatformConfig.bEnableRTC)
	{
		static const FString XAudioPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::Combine(FPaths::EngineDir(), TEXT("Binaries/ThirdParty/Windows/XAudio2_9"), PLATFORM_64BITS ? TEXT("x64") : TEXT("x86"), TEXT("xaudio2_9redist.dll")));
		static const FTCHARToUTF8 Utf8XAudioPath(*XAudioPath);

		static_assert(EOS_WINDOWS_RTCOPTIONS_API_LATEST == 1, "EOS_Windows_RTCOptions updated");
		static EOS_Windows_RTCOptions WindowsRTCOptions = {};
		WindowsRTCOptions.ApiVersion = EOS_WINDOWS_RTCOPTIONS_API_LATEST;
		WindowsRTCOptions.XAudio29DllPath = Utf8XAudioPath.Get();

		const_cast<EOS_Platform_RTCOptions*>(PlatformOptions.RTCOptions)->PlatformSpecificOptions = &WindowsRTCOptions;
	}

	return FEIKSDKManager::CreatePlatform(PlatformConfig, PlatformOptions);
}

#endif // WITH_EOS_SDK