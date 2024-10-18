// Copyright (c) 2024 xist.gg

#include "DiscordGame.h"
#include "Misc/Paths.h"
#include "Windows/WindowsPlatformProcess.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"  // IWYU pragma: keep

DEFINE_LOG_CATEGORY(LogDiscord);

const FName FDiscordGameModule::ModuleName ("EOSIntegrationKit");

void FDiscordGameModule::StartupModule()
{
#if EIKDISCORDACTIVE
	// Determine the path to the Discord GameSDK DLL to load for the current platform and environment
	const FString LibraryPath = GetPathToDLL();

	// Make sure we got something valid
	if ensureAlwaysMsgf(!LibraryPath.IsEmpty(), TEXT("Expect LibraryPath to not be empty"))
	{
		// Try to load this DLL
		DiscordGameSDKHandle = FPlatformProcess::GetDllHandle(*LibraryPath);

		// Log a message indicating success or failure for the DLL load attempt
		if ensureAlwaysMsgf(DiscordGameSDKHandle, TEXT("Expect to load Discord SDK at path [%s]"), *LibraryPath)
		{
			UE_LOG(LogDiscord, Log, TEXT("Loaded Discord GameSDK DLL [%s]"), *LibraryPath);
			if(DiscordGameSDKHandle)
			{
				bDiscordSDKLoaded = true;
			}
			else
			{
				UE_LOG(LogDiscord, Error, TEXT("Failed to load Discord GameSDK DLL [%s]"), *LibraryPath);
			}
		}
		else
		{
			UE_LOG(LogDiscord, Error, TEXT("Failed to load Discord GameSDK DLL [%s]"), *LibraryPath);
		}
	}
	else
	{
		UE_LOG(LogDiscord, Error, TEXT("Failed to determine path to Discord GameSDK DLL"));
	}
#else
	UE_LOG(LogDiscord, Log, TEXT("EIK: DiscordGame is disabled, skipping DiscordGame SDK load"));
#endif
}

void FDiscordGameModule::ShutdownModule()
{
	if (DiscordGameSDKHandle)
	{
		// Free the dll handle
		FPlatformProcess::FreeDllHandle(DiscordGameSDKHandle);
		DiscordGameSDKHandle = nullptr;
	}
}

FString FDiscordGameModule::GetPathToDLL() const
{
	// Add on the relative location of the third party dll and load it
	FString LibraryPath;

#if WITH_EDITOR && (PLATFORM_WINDOWS || PLATFORM_LINUX)

	// When compiling as Editor, the SDK files DO NOT get copied to the Binaries directory,
	// so we need to load them directly from the Source location.

	FString BaseDir = IPluginManager::Get().FindPlugin(ModuleName.ToString())->GetBaseDir();
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/DiscordGameSDK"));

#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*LibraryPath, TEXT("x86_64/discord_game_sdk.dll"));
#elif PLATFORM_LINUX
    LibraryPath = FPaths::Combine(*LibraryPath, TEXT("x86_64/discord_game_sdk.so"));
#else
#error Unsupported platform
#endif

#else

	// When compiling as Game, the build process copies the SDK DLLs to the Binaries directory,
	// and from here they are packaged for distribution.
	//
	// Macs work a bit differently, they always package the DLLs in this way, even when
	// compiling as Editor.

	// Load DLLs from the distribution location.

	FString BaseDir = FPaths::ProjectDir();
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries"));

#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*LibraryPath, TEXT("Win64/discord_game_sdk.dll"));
#elif PLATFORM_MAC
	LibraryPath = FPaths::Combine(*LibraryPath, TEXT("Mac/discord_game_sdk.dylib"));
#elif PLATFORM_LINUX
	LibraryPath = FPaths::Combine(*LibraryPath, TEXT("Linux/discord_game_sdk.so"));
#else
#error Unsupported platform
#endif

#endif

	const FString FullPath (FPaths::ConvertRelativePathToFull(*LibraryPath));
	return FullPath;
}

IMPLEMENT_MODULE(FDiscordGameModule, DiscordGame)
