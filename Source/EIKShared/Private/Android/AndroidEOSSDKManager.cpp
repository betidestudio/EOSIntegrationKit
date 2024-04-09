// Copyright Epic Games, Inc. All Rights Reserved.

#include "AndroidEOSSDKManager.h"

#include "Android/eos_android.h"
#include "HAL/FileManager.h"

#if WITH_EOS_SDK


EOS_EResult FAndroidEOSSDKManager::EOSInitialize(EOS_InitializeOptions& Options)
{
	EOS_Android_InitializeOptions SystemInitializeOptions = { 0 };
	SystemInitializeOptions.ApiVersion = EOS_ANDROID_INITIALIZEOPTIONS_API_LATEST;
	static_assert(EOS_ANDROID_INITIALIZEOPTIONS_API_LATEST == 2, "EOS_Android_InitializeOptions updated, check new fields");
	SystemInitializeOptions.Reserved = nullptr;
	SystemInitializeOptions.OptionalInternalDirectory = nullptr;
	SystemInitializeOptions.OptionalExternalDirectory = nullptr;

	Options.SystemInitializeOptions = &SystemInitializeOptions;

	return FEIKSDKManager::EOSInitialize(Options);
}

FString FAndroidEOSSDKManager::GetCacheDirBase() const
{
	FString BaseCacheDirBase = FEIKSDKManager::GetCacheDirBase();
	return IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(*BaseCacheDirBase);
};

#endif // WITH_EOS_SDK