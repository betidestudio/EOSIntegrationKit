// Copyright Epic Games, Inc. All Rights Reserved.

#include "EOSSDKManager.h"
#include "Runtime/Launch/Resources/Version.h"
#if WITH_EOS_SDK

#include "Containers/Ticker.h"
#include "HAL/LowLevelMemTracker.h"
#include "Misc/App.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Misc/CoreMisc.h"
#include "Misc/CoreDelegates.h"
#include "Misc/NetworkVersion.h"
#include "Misc/Paths.h"
#include "Misc/ConfigCacheIni.h"
#include "ProfilingDebugging/CsvProfiler.h"
#include "Stats/Stats.h"
#include "Runtime/Projects/Public/Interfaces/IPluginManager.h"
#include "CoreGlobals.h"

#include "EOSShared.h"
#include "EOSSharedTypes.h"

#include "eos_auth.h"
#include "eos_connect.h"
#include "eos_friends.h"
#include "eos_init.h"
#include "eos_logging.h"
#include "eos_presence.h"
#include "eos_sdk.h"
#include "eos_userinfo.h"
#include "eos_version.h"

#ifndef EOS_TRACE_MALLOC
#define EOS_TRACE_MALLOC 1
#endif

namespace
{
	static void* EOS_MEMORY_CALL EosMalloc(size_t Bytes, size_t Alignment)
	{
		LLM_SCOPE(ELLMTag::RealTimeCommunications);

#if !EOS_TRACE_MALLOC
		CALLSTACK_TRACE_LIMIT_CALLSTACKRESOLVE_SCOPE();
#endif

		return FMemory::Malloc(Bytes, Alignment);
	}

	static void* EOS_MEMORY_CALL EosRealloc(void* Ptr, size_t Bytes, size_t Alignment)
	{
		LLM_SCOPE(ELLMTag::RealTimeCommunications);

#if !EOS_TRACE_MALLOC
		CALLSTACK_TRACE_LIMIT_CALLSTACKRESOLVE_SCOPE();
#endif

		return FMemory::Realloc(Ptr, Bytes, Alignment);
	}

	static void EOS_MEMORY_CALL EosFree(void* Ptr)
	{
		LLM_SCOPE(ELLMTag::RealTimeCommunications);

#if !EOS_TRACE_MALLOC
		CALLSTACK_TRACE_LIMIT_CALLSTACKRESOLVE_SCOPE();
#endif

		FMemory::Free(Ptr);
	}

#if !NO_LOGGING
	void EOS_CALL EOSLogMessageReceived(const EOS_LogMessage* Message)
	{
#define EOSLOG(Level) UE_LOG(LogEIKSDK, Level, TEXT("%s: %s"), UTF8_TO_TCHAR(Message->Category), *MessageStr)

		FString MessageStr(UTF8_TO_TCHAR(Message->Message));
		MessageStr.TrimStartAndEndInline();

		switch (Message->Level)
		{
		case EOS_ELogLevel::EOS_LOG_Fatal:			EOSLOG(Fatal); break;
		case EOS_ELogLevel::EOS_LOG_Error:			EOSLOG(Error); break;
		case EOS_ELogLevel::EOS_LOG_Warning:		EOSLOG(Warning); break;
		case EOS_ELogLevel::EOS_LOG_Info:			EOSLOG(Log); break;
		case EOS_ELogLevel::EOS_LOG_Verbose:		EOSLOG(Verbose); break;
		case EOS_ELogLevel::EOS_LOG_VeryVerbose:	EOSLOG(VeryVerbose); break;
		case EOS_ELogLevel::EOS_LOG_Off:
		default:
			// do nothing
			break;
		}
#undef EOSLOG
	}

	EOS_ELogLevel ConvertLogLevel(ELogVerbosity::Type LogLevel)
	{
		switch (LogLevel)
		{
		case ELogVerbosity::NoLogging:		return EOS_ELogLevel::EOS_LOG_Off;
		case ELogVerbosity::Fatal:			return EOS_ELogLevel::EOS_LOG_Fatal;
		case ELogVerbosity::Error:			return EOS_ELogLevel::EOS_LOG_Error;
		case ELogVerbosity::Warning:		return EOS_ELogLevel::EOS_LOG_Warning;
		default:							// Intentional fall through
		case ELogVerbosity::Display:		// Intentional fall through
		case ELogVerbosity::Log:			return EOS_ELogLevel::EOS_LOG_Info;
		case ELogVerbosity::Verbose:		return EOS_ELogLevel::EOS_LOG_Verbose;
		case ELogVerbosity::VeryVerbose:	return EOS_ELogLevel::EOS_LOG_VeryVerbose;
		}
	}
#endif // !NO_LOGGING

#if EOSSDK_RUNTIME_LOAD_REQUIRED
	static void* GetSdkDllHandle()
	{
		void* Result = nullptr;

		FString PluginModuleName = TEXT("EosIntegrationKit");

		// Get a reference to the plugin manager
		IPluginManager& PluginManager = IPluginManager::Get();

		// Get the plugin object by module name
		TSharedPtr<IPlugin> Plugin = PluginManager.FindPlugin(PluginModuleName);
		
		if (Plugin.IsValid())
		{
			// Get the plugin's directory
			FString PluginDirectory = Plugin->GetBaseDir();

			// Build the path to the binary
			FString BinaryPath = FPaths::Combine(*PluginDirectory, TEXT("Binaries"), FPlatformProcess::GetBinariesSubdirectory(), TEXT(EOSSDK_RUNTIME_LIBRARY_NAME));
			FString SecondBinaryPath = FPaths::Combine(*PluginDirectory, TEXT("Source"), TEXT("ThirdParty"), TEXT("EIKSDK"), TEXT("Bin"), TEXT(EOSSDK_RUNTIME_LIBRARY_NAME));
			// Check if the file exists before attempting to load
			if (FPaths::FileExists(BinaryPath))
			{
				UE_LOG(LogEIKSDK, Log, TEXT("Loading EOS SDK from plugin binaries: %s."), *BinaryPath);
				Result = FPlatformProcess::GetDllHandle(*BinaryPath);
			}
			else if (FPaths::FileExists(SecondBinaryPath))
			{
				UE_LOG(LogEIKSDK, Log, TEXT("Loading EOS SDK from main plugin binaries: %s."), *SecondBinaryPath);
				Result = FPlatformProcess::GetDllHandle(*SecondBinaryPath);
			}
			else
			{
				UE_LOG(LogEIKSDK, Log, TEXT("EOS SDK binary not found: %s. Now project binaries will be looked up."), *BinaryPath);
			}
		}
		else
		{
			UE_LOG(LogEIKSDK, Log, TEXT("Plugin not found: %s"), *PluginModuleName);
		}
		const FString ProjectBinaryPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::GetRelativePathToRoot(), TEXT("Binaries"), FPlatformProcess::GetBinariesSubdirectory(), TEXT(EOSSDK_RUNTIME_LIBRARY_NAME)));

		if(!Result)
		{
			if (FPaths::FileExists(ProjectBinaryPath))
			{
				UE_LOG(LogEIKSDK, Log, TEXT("Loading EOS SDK from project binaries: %s"), *ProjectBinaryPath);
				Result = FPlatformProcess::GetDllHandle(*ProjectBinaryPath);
			}
			else
			{
				UE_LOG(LogEIKSDK, Log, TEXT("Unable to find EOS SDK in project binaries: %s"), *ProjectBinaryPath);
			}
		}

		if (!Result)
		{
			UE_LOG(LogEIKSDK, Log, TEXT("Loading EOS SDK from engine binaries. This may not be good if you are using a older version of the SDK."));
			const FString EngineBinaryPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::EngineDir(), TEXT("Binaries"), FPlatformProcess::GetBinariesSubdirectory(), TEXT(EOSSDK_RUNTIME_LIBRARY_NAME)));
			if (FPaths::FileExists(EngineBinaryPath))
			{
				Result = FPlatformProcess::GetDllHandle(*EngineBinaryPath);
			}
		}

		if (!Result)
		{
			UE_LOG(LogEIKSDK, Warning, TEXT("Loading EOS SDK from system path"));
			Result = FPlatformProcess::GetDllHandle(TEXT(EOSSDK_RUNTIME_LIBRARY_NAME));
		}

		return Result;
	}
#endif
}


// if the platform wants to preload the EOS runtime library, then use the auto registration system to kick off
// the GetDllHandle call at a specified time. it is assumed that calling GetDllHandle here will not conflict
// with the call in FEIKSDKManager::FEIKSDKManager, and will only speed it up, even if it's not complete before
// the later one is called. if that's not the case, we will need an FEvent or similar to block the constructor
// until this is complete
#if EOSSDK_RUNTIME_LOAD_REQUIRED && defined(EOS_DLL_PRELOAD_PHASE)

#include "Misc/DelayedAutoRegister.h"
#include "Async/Async.h"

static FDelayedAutoRegisterHelper GKickoffDll(EDelayedRegisterRunPhase::EOS_DLL_PRELOAD_PHASE, []
{
	Async(EAsyncExecution::Thread, []
	{
		LLM_SCOPE(ELLMTag::RealTimeCommunications);
		SCOPED_BOOT_TIMING("Preloading EOS module");
		GetSdkDllHandle();
	});
});

#endif

FEIKSDKManager::FEIKSDKManager()
{
#if EOSSDK_RUNTIME_LOAD_REQUIRED
	LLM_SCOPE(ELLMTag::RealTimeCommunications);
	SDKHandle = GetSdkDllHandle();
	if (SDKHandle == nullptr)
	{
		UE_LOG(LogEIKSDK, Warning, TEXT("Unable to load EOSSDK dynamic library"));
		return;
	}
#endif
}

FEIKSDKManager::~FEIKSDKManager()
{
#if EOSSDK_RUNTIME_LOAD_REQUIRED
	if (SDKHandle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(SDKHandle);
	}
#endif
}

EOS_EResult FEIKSDKManager::Initialize()
{
#if EOSSDK_RUNTIME_LOAD_REQUIRED
	if (SDKHandle == nullptr)
	{
		UE_LOG(LogEIKSDK, Log, TEXT("FEIKSDKManager::Initialize failed, SDKHandle=nullptr"));
		return EOS_EResult::EOS_InvalidState;
	}
#endif

	if (IsInitialized())
	{
		return EOS_EResult::EOS_Success;
	}
	else
	{
		UE_LOG(LogEIKSDK, Log, TEXT("Initializing EOSSDK Version:%s"), UTF8_TO_TCHAR(EOS_GetVersion()));

		const FTCHARToUTF8 ProductName(*GetProductName());
		const FTCHARToUTF8 ProductVersion(*GetProductVersion());

		EOS_InitializeOptions InitializeOptions = {};
		InitializeOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
		static_assert(EOS_INITIALIZE_API_LATEST == 4, "EOS_InitializeOptions updated, check new fields");
		InitializeOptions.AllocateMemoryFunction = &EosMalloc;
		InitializeOptions.ReallocateMemoryFunction = &EosRealloc;
		InitializeOptions.ReleaseMemoryFunction = &EosFree;
		InitializeOptions.ProductName = ProductName.Get();
		InitializeOptions.ProductVersion = ProductVersion.Length() > 0 ? ProductVersion.Get() : nullptr;
		InitializeOptions.Reserved = nullptr;
		InitializeOptions.SystemInitializeOptions = nullptr;
		InitializeOptions.OverrideThreadAffinity = nullptr;

		EOS_EResult EosResult = EOSInitialize(InitializeOptions);

		if (EosResult == EOS_EResult::EOS_Success || EosResult == EOS_EResult::EOS_AlreadyConfigured)
		{
			bInitialized = true;
#if ENGINE_MAJOR_VERSION ==5 && ENGINE_MINOR_VERSION == 2 || ENGINE_MAJOR_VERSION ==5 && ENGINE_MINOR_VERSION == 3
			FCoreDelegates::TSOnConfigSectionsChanged().AddRaw(this, &FEIKSDKManager::OnConfigSectionsChanged);
#elif ENGINE_MAJOR_VERSION ==5 && ENGINE_MINOR_VERSION == 1
			FCoreDelegates::OnConfigSectionsChanged.AddRaw(this, &FEIKSDKManager::OnConfigSectionsChanged);
#endif

			LoadConfig();


#if !NO_LOGGING
			FCoreDelegates::OnLogVerbosityChanged.AddRaw(this, &FEIKSDKManager::OnLogVerbosityChanged);

			EosResult = EOS_Logging_SetCallback(&EOSLogMessageReceived);
			if (EosResult != EOS_EResult::EOS_Success)
			{
				UE_LOG(LogEIKSDK, Warning, TEXT("EOS_Logging_SetCallback failed error:%s"), *EIK_LexToString(EosResult));
			}

			EosResult = EOS_Logging_SetLogLevel(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, ConvertLogLevel(LogEIKSDK.GetVerbosity()));
			if (EosResult != EOS_EResult::EOS_Success)
			{
				UE_LOG(LogEIKSDK, Warning, TEXT("EOS_Logging_SetLogLevel failed Verbosity=%s error=[%s]"), ToString(LogEIKSDK.GetVerbosity()), *EIK_LexToString(EosResult));
			}
#endif // !NO_LOGGING
		}
		else
		{
			UE_LOG(LogEIKSDK, Warning, TEXT("EOS_Initialize failed error:%s"), *EIK_LexToString(EosResult));
		}

		return EosResult;
	}
}

const FEOSSDKPlatformConfig* FEIKSDKManager::GetPlatformConfig(const FString& PlatformConfigName, bool bLoadIfMissing)
{
	if (PlatformConfigName.IsEmpty())
	{
		UE_LOG(LogEIKSDK, Warning, TEXT("Platform name can't be empty"));
		return nullptr;
	}

	FEOSSDKPlatformConfig* PlatformConfig = PlatformConfigs.Find(PlatformConfigName);
	if (PlatformConfig || !bLoadIfMissing)
	{
		return PlatformConfig;
	}

	const FString SectionName(TEXT("EOSSDK.Platform.") + PlatformConfigName);
	if (!GConfig->DoesSectionExist(*SectionName, GEngineIni))
	{
		UE_LOG(LogEIKSDK, Warning, TEXT("Could not find platform config: %s"), *PlatformConfigName);
		return nullptr;
	}

	PlatformConfig = &PlatformConfigs.Emplace(PlatformConfigName);

	PlatformConfig->Name = PlatformConfigName;
	GConfig->GetString(*SectionName, TEXT("ProductId"), PlatformConfig->ProductId, GEngineIni);
	GConfig->GetString(*SectionName, TEXT("SandboxId"), PlatformConfig->SandboxId, GEngineIni);
	GConfig->GetString(*SectionName, TEXT("ClientId"), PlatformConfig->ClientId, GEngineIni);
	GConfig->GetString(*SectionName, TEXT("ClientSecret"), PlatformConfig->ClientSecret, GEngineIni);
	GConfig->GetString(*SectionName, TEXT("EncryptionKey"), PlatformConfig->EncryptionKey, GEngineIni);
	GConfig->GetString(*SectionName, TEXT("OverrideCountryCode"), PlatformConfig->OverrideCountryCode, GEngineIni);
	GConfig->GetString(*SectionName, TEXT("OverrideLocaleCode"), PlatformConfig->OverrideLocaleCode, GEngineIni);
	GConfig->GetString(*SectionName, TEXT("DeploymentId"), PlatformConfig->DeploymentId, GEngineIni);
	GConfig->GetString(*SectionName, TEXT("CacheDirectory"), PlatformConfig->CacheDirectory, GEngineIni);
	GConfig->GetBool(*SectionName, TEXT("bIsServer"), PlatformConfig->bIsServer, GEngineIni);
	GConfig->GetBool(*SectionName, TEXT("bLoadingInEditor"), PlatformConfig->bLoadingInEditor, GEngineIni);
	GConfig->GetBool(*SectionName, TEXT("bDisableOverlay"), PlatformConfig->bDisableOverlay, GEngineIni);
	GConfig->GetBool(*SectionName, TEXT("bDisableSocialOverlay"), PlatformConfig->bDisableSocialOverlay, GEngineIni);
	GConfig->GetBool(*SectionName, TEXT("bWindowsEnableOverlayD3D9"), PlatformConfig->bWindowsEnableOverlayD3D9, GEngineIni);
	GConfig->GetBool(*SectionName, TEXT("bWindowsEnableOverlayD3D10"), PlatformConfig->bWindowsEnableOverlayD3D10, GEngineIni);
	GConfig->GetBool(*SectionName, TEXT("bWindowsEnableOverlayOpenGL"), PlatformConfig->bWindowsEnableOverlayOpenGL, GEngineIni);
	GConfig->GetBool(*SectionName, TEXT("bEnableRTC"), PlatformConfig->bEnableRTC, GEngineIni);
	GConfig->GetInt(*SectionName, TEXT("TickBudgetInMilliseconds"), PlatformConfig->TickBudgetInMilliseconds, GEngineIni);
	GConfig->GetArray(*SectionName, TEXT("OptionalConfig"), PlatformConfig->OptionalConfig, GEngineIni);

	UE_LOG(LogEIKSDK, Verbose, TEXT("Loaded platform config: %s"), *PlatformConfigName);
	return PlatformConfig;
}

bool FEIKSDKManager::AddPlatformConfig(const FEOSSDKPlatformConfig& PlatformConfig)
{
	if (PlatformConfig.Name.IsEmpty())
	{
		UE_LOG(LogEIKSDK, Warning, TEXT("Platform name can't be empty"));
		return false;
	}

	if (PlatformConfigs.Find(PlatformConfig.Name))
	{
		UE_LOG(LogEIKSDK, Warning, TEXT("Platform config already exists: %s"), *PlatformConfig.Name);
		return false;
	}

	PlatformConfigs.Emplace(PlatformConfig.Name, PlatformConfig);
	UE_LOG(LogEIKSDK, Verbose, TEXT("Added platform config: %s"), *PlatformConfig.Name);
	return true;
}

const FString& FEIKSDKManager::GetDefaultPlatformConfigName()
{
	if (DefaultPlatformConfigName.IsEmpty())
	{
		FString PlatformConfigName;
		if (GConfig->GetString(TEXT("EOSSDK"), TEXT("DefaultPlatformConfigName"), PlatformConfigName, GEngineIni))
		{
			SetDefaultPlatformConfigName(PlatformConfigName);
		}
	}

	return DefaultPlatformConfigName;
}

void FEIKSDKManager::SetDefaultPlatformConfigName(const FString& PlatformConfigName)
{
	if (DefaultPlatformConfigName != PlatformConfigName)
	{
		UE_LOG(LogEIKSDK, Verbose, TEXT("Default platform name changed: New=%s Old=%s"), *PlatformConfigName, *DefaultPlatformConfigName);
		OnDefaultPlatformConfigNameChanged.Broadcast(PlatformConfigName, DefaultPlatformConfigName);
		DefaultPlatformConfigName = PlatformConfigName;
	}
}

IEIKPlatformHandlePtr FEIKSDKManager::CreatePlatform(const FString& PlatformConfigName, FName InstanceName)
{
	if (PlatformConfigName.IsEmpty())
	{
		UE_LOG(LogEIKSDK, Warning, TEXT("Platform name can't be empty"));
		return IEIKPlatformHandlePtr();
	}

	const FEOSSDKPlatformConfig* const PlatformConfig = GetPlatformConfig(PlatformConfigName, true);
	if (!PlatformConfig)
	{
		return IEIKPlatformHandlePtr();
	}

	TMap<FName, IEIKPlatformHandleWeakPtr>* PlatformMap = PlatformHandles.Find(PlatformConfigName);
	if (PlatformMap)
	{
		IEIKPlatformHandleWeakPtr* WeakPlatformHandle = PlatformMap->Find(InstanceName);
		if (WeakPlatformHandle)
		{
			if (IEIKPlatformHandlePtr Pinned = WeakPlatformHandle->Pin())
			{
				UE_LOG(LogEIKSDK, Verbose, TEXT("Found existing platform handle: PlatformConfigName=%s InstanceName=%s"), *PlatformConfigName, *InstanceName.ToString());
				return Pinned;
			}

			UE_LOG(LogEIKSDK, Verbose, TEXT("Removing stale platform handle pointer: PlatformConfigName=%s InstanceName=%s"), *PlatformConfigName, *InstanceName.ToString());
			PlatformMap->Remove(InstanceName);
		}
	}
	else
	{
		PlatformMap = &PlatformHandles.Emplace(PlatformConfigName);
	}

	const FTCHARToUTF8 Utf8ProductId(*PlatformConfig->ProductId);
	const FTCHARToUTF8 Utf8SandboxId(*PlatformConfig->SandboxId);
	const FTCHARToUTF8 Utf8ClientId(*PlatformConfig->ClientId);
	const FTCHARToUTF8 Utf8ClientSecret(*PlatformConfig->ClientSecret);
	const FTCHARToUTF8 Utf8EncryptionKey(*PlatformConfig->EncryptionKey);
	const FTCHARToUTF8 Utf8OverrideCountryCode(*PlatformConfig->OverrideCountryCode);
	const FTCHARToUTF8 Utf8OverrideLocaleCode(*PlatformConfig->OverrideLocaleCode);
	const FTCHARToUTF8 Utf8DeploymentId(*PlatformConfig->DeploymentId);
	const FTCHARToUTF8 Utf8CacheDirectory(*PlatformConfig->CacheDirectory);

	EOS_Platform_Options PlatformOptions = {};
	PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
	PlatformOptions.Reserved = nullptr;
	PlatformOptions.ProductId = Utf8ProductId.Length() ? Utf8ProductId.Get() : nullptr;
	PlatformOptions.SandboxId = Utf8SandboxId.Length() ? Utf8SandboxId.Get() : nullptr;
	PlatformOptions.ClientCredentials.ClientId = Utf8ClientId.Length() ? Utf8ClientId.Get() : nullptr;
	PlatformOptions.ClientCredentials.ClientSecret = Utf8ClientSecret.Length() ? Utf8ClientSecret.Get() : nullptr;
	PlatformOptions.bIsServer = PlatformConfig->bIsServer;
	PlatformOptions.EncryptionKey = Utf8EncryptionKey.Length() ? Utf8EncryptionKey.Get() : nullptr;
	PlatformOptions.OverrideCountryCode = Utf8OverrideCountryCode.Length() ? Utf8OverrideCountryCode.Get() : nullptr;
	PlatformOptions.OverrideLocaleCode = Utf8OverrideLocaleCode.Length() ? Utf8OverrideLocaleCode.Get() : nullptr;
	PlatformOptions.DeploymentId = Utf8DeploymentId.Length() ? Utf8DeploymentId.Get() : nullptr;

	PlatformOptions.Flags = 0;
	if (PlatformConfig->bLoadingInEditor) PlatformOptions.Flags |= EOS_PF_LOADING_IN_EDITOR;
	if (PlatformConfig->bDisableOverlay) PlatformOptions.Flags |= EOS_PF_DISABLE_OVERLAY;
	if (PlatformConfig->bDisableSocialOverlay) PlatformOptions.Flags |= EOS_PF_DISABLE_SOCIAL_OVERLAY;

	PlatformOptions.CacheDirectory = Utf8CacheDirectory.Length() ? Utf8DeploymentId.Get() : nullptr;
	PlatformOptions.TickBudgetInMilliseconds = PlatformConfig->TickBudgetInMilliseconds;

	EOS_Platform_RTCOptions PlatformRTCOptions = {};
	PlatformRTCOptions.ApiVersion = EOS_PLATFORM_RTCOPTIONS_API_LATEST;
	PlatformRTCOptions.PlatformSpecificOptions = nullptr;
	PlatformOptions.RTCOptions = PlatformConfig->bEnableRTC ? &PlatformRTCOptions : nullptr;

	PlatformOptions.IntegratedPlatformOptionsContainerHandle = nullptr;

	IEIKPlatformHandlePtr PlatformHandle = CreatePlatform(*PlatformConfig, PlatformOptions);
	if (PlatformHandle.IsValid())
	{
		UE_LOG(LogEIKSDK, Verbose, TEXT("Created platform handle: PlatformConfigName=%s InstanceName=%s"), *PlatformConfigName, *InstanceName.ToString());
		PlatformMap->Emplace(InstanceName, PlatformHandle);
	}

	return PlatformHandle;
}

IEIKPlatformHandlePtr FEIKSDKManager::CreatePlatform(const FEOSSDKPlatformConfig& PlatformConfig, EOS_Platform_Options& PlatformOptions)
{
	OnPreCreateNamedPlatform.Broadcast(PlatformConfig, PlatformOptions);
	return CreatePlatform(PlatformOptions);
}

IEIKPlatformHandlePtr FEIKSDKManager::CreatePlatform(EOS_Platform_Options& PlatformOptions)
{
	IEIKPlatformHandlePtr SharedPlatform;

	if (IsInitialized())
	{
		OnPreCreatePlatform.Broadcast(PlatformOptions);

		const EOS_HPlatform PlatformHandle = EOS_Platform_Create(&PlatformOptions);
		if (PlatformHandle)
		{
			ActivePlatforms.Emplace(PlatformHandle);
			SharedPlatform = MakeShared<FEIKPlatformHandle, ESPMode::ThreadSafe>(*this, PlatformHandle);
			SetupTicker();

			EOS_Platform_SetApplicationStatus(PlatformHandle, CachedApplicationStatus);
			EOS_Platform_SetNetworkStatus(PlatformHandle, CachedNetworkStatus);

			// Tick the platform once to work around EOSSDK error logging that occurs if you create then immediately destroy a platform.
			SharedPlatform->Tick();
		}
		else
		{
			UE_LOG(LogEIKSDK, Warning, TEXT("FEIKSDKManager::CreatePlatform failed. EosPlatformHandle=nullptr"));
		}
	}
	else
	{
		UE_LOG(LogEIKSDK, Warning, TEXT("FEIKSDKManager::CreatePlatform failed. SDK not initialized"));
	}

	return SharedPlatform;
}

void FEIKSDKManager::OnConfigSectionsChanged(const FString& IniFilename, const TSet<FString>& SectionNames)
{
	if (IniFilename == GEngineIni && SectionNames.Contains(TEXT("EOSSDK")))
	{
		LoadConfig();
	}
}

void FEIKSDKManager::LoadConfig()
{
	ConfigTickIntervalSeconds = 0.f;
	GConfig->GetDouble(TEXT("EOSSDK"), TEXT("TickIntervalSeconds"), ConfigTickIntervalSeconds, GEngineIni);

	SetupTicker();
}

void FEIKSDKManager::SetupTicker()
{
	if (TickerHandle.IsValid())
	{
#if ENGINE_MAJOR_VERSION == 5
		FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
#else
		FTicker::GetCoreTicker().RemoveTicker(TickerHandle);
#endif
		TickerHandle.Reset();
	}

	if (ActivePlatforms.Num() > 0)
	{
		const double TickIntervalSeconds = ConfigTickIntervalSeconds > SMALL_NUMBER ? ConfigTickIntervalSeconds / ActivePlatforms.Num() : 0.f;
#if ENGINE_MAJOR_VERSION == 5
		TickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FEIKSDKManager::Tick), TickIntervalSeconds);
#else
		TickerHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FEIKSDKManager::Tick), TickIntervalSeconds);
#endif
	}
}

bool FEIKSDKManager::Tick(float)
{
	ReleaseReleasedPlatforms();

	if (ActivePlatforms.Num())
	{
		TArray<EOS_HPlatform> PlatformsToTick;
		if (ConfigTickIntervalSeconds > SMALL_NUMBER)
		{
			PlatformTickIdx = (PlatformTickIdx + 1) % ActivePlatforms.Num();
			PlatformsToTick.Emplace(ActivePlatforms[PlatformTickIdx]);
		}
		else
		{
			PlatformsToTick = ActivePlatforms;
		}

		for (EOS_HPlatform PlatformHandle : PlatformsToTick)
		{
			LLM_SCOPE(ELLMTag::RealTimeCommunications); // TODO should really be ELLMTag::EOSSDK
			QUICK_SCOPE_CYCLE_COUNTER(FEIKSDKManager_Tick);
			CSV_SCOPED_TIMING_STAT_EXCLUSIVE(EOSSDK);
			EOS_Platform_Tick(PlatformHandle);
		}
	}

	return true;
}

void FEIKSDKManager::OnApplicationStatusChanged(EOS_EApplicationStatus ApplicationStatus)
{
	UE_LOG(LogEIKSDK, Log, TEXT("OnApplicationStatusChanged [%s] -> [%s]"), EIK_LexToString(CachedApplicationStatus), EIK_LexToString(ApplicationStatus));
	CachedApplicationStatus = ApplicationStatus;
	for (EOS_HPlatform PlatformHandle : ActivePlatforms)
	{
		EOS_Platform_SetApplicationStatus(PlatformHandle, ApplicationStatus);
	}
}

void FEIKSDKManager::OnNetworkStatusChanged(EOS_ENetworkStatus NetworkStatus)
{
	UE_LOG(LogEIKSDK, Log, TEXT("OnNetworkStatusChanged [%s] -> [%s]"), EIK_LexToString(CachedNetworkStatus), EIK_LexToString(NetworkStatus));
	CachedNetworkStatus = NetworkStatus;
	for (EOS_HPlatform PlatformHandle : ActivePlatforms)
	{
		EOS_Platform_SetNetworkStatus(PlatformHandle, NetworkStatus);
	}
}

void FEIKSDKManager::OnLogVerbosityChanged(const FLogCategoryName& CategoryName, ELogVerbosity::Type OldVerbosity, ELogVerbosity::Type NewVerbosity)
{
#if !NO_LOGGING
	if (IsInitialized() &&
		CategoryName == LogEIKSDK.GetCategoryName())
	{
		const EOS_EResult EosResult = EOS_Logging_SetLogLevel(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, ConvertLogLevel(NewVerbosity));
		if (EosResult != EOS_EResult::EOS_Success)
		{
			UE_LOG(LogEIKSDK, Warning, TEXT("EOS_Logging_SetLogLevel failed Verbosity=%s error=[%s]"), ToString(NewVerbosity), *EIK_LexToString(EosResult));
		}
	}
#endif // !NO_LOGGING
}

FString FEIKSDKManager::GetProductName() const
{
	FString ProductName;
	if (!GConfig->GetString(TEXT("EOSSDK"), TEXT("ProductName"), ProductName, GEngineIni))
	{
		ProductName = FApp::GetProjectName();
	}
	return ProductName;
}

FString FEIKSDKManager::GetProductVersion() const
{
	return FApp::GetBuildVersion();
}

FString FEIKSDKManager::GetCacheDirBase() const
{
	return FPlatformProcess::UserDir();
}

FString FEIKSDKManager::GetOverrideCountryCode(const EOS_HPlatform Platform) const
{
	FString Result;

	char CountryCode[EOS_COUNTRYCODE_MAX_LENGTH + 1];
	int32_t CountryCodeLength = sizeof(CountryCode);
	if (EOS_Platform_GetOverrideCountryCode(Platform, CountryCode, &CountryCodeLength) == EOS_EResult::EOS_Success)
	{
		Result = UTF8_TO_TCHAR(CountryCode);
	}

	return Result;
}

FString FEIKSDKManager::GetOverrideLocaleCode(const EOS_HPlatform Platform) const
{
	FString Result;

	char LocaleCode[EOS_LOCALECODE_MAX_LENGTH + 1];
	int32_t LocaleCodeLength = sizeof(LocaleCode);
	if (EOS_Platform_GetOverrideLocaleCode(Platform, LocaleCode, &LocaleCodeLength) == EOS_EResult::EOS_Success)
	{
		Result = UTF8_TO_TCHAR(LocaleCode);
	}

	return Result;
}

void FEIKSDKManager::ReleasePlatform(EOS_HPlatform PlatformHandle)
{
	if (ensure(ActivePlatforms.Contains(PlatformHandle)
		&& !ReleasedPlatforms.Contains(PlatformHandle)))
	{
		ReleasedPlatforms.Emplace(PlatformHandle);
	}
}

void FEIKSDKManager::ReleaseReleasedPlatforms()
{
	if (ReleasedPlatforms.Num() > 0)
	{
		for (EOS_HPlatform PlatformHandle : ReleasedPlatforms)
		{
			if (ensure(ActivePlatforms.Contains(PlatformHandle)))
			{
				EOS_Platform_Release(PlatformHandle);
				ActivePlatforms.Remove(PlatformHandle);
			}
		}
		ReleasedPlatforms.Empty();
		SetupTicker();
	}
}

void FEIKSDKManager::Shutdown()
{
	if (IsInitialized())
	{
		// Release already released platforms
		ReleaseReleasedPlatforms();

		if (ActivePlatforms.Num() > 0)
		{
			UE_LOG(LogEIKSDK, Warning, TEXT("FEIKSDKManager::Shutdown Releasing %d remaining platforms"), ActivePlatforms.Num());
			ReleasedPlatforms.Append(ActivePlatforms);
			ReleaseReleasedPlatforms();
		}

#if ENGINE_MAJOR_VERSION ==5 && ENGINE_MINOR_VERSION >= 2
		FCoreDelegates::TSOnConfigSectionsChanged().RemoveAll(this);
#elif ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 1
		FCoreDelegates::OnConfigSectionsChanged.RemoveAll(this);
#endif


#if !NO_LOGGING
		FCoreDelegates::OnLogVerbosityChanged.RemoveAll(this);
#endif // !NO_LOGGING

		const EOS_EResult Result = EOS_Shutdown();
		UE_LOG(LogEIKSDK, Log, TEXT("FEIKSDKManager::Shutdown EOS_Shutdown Result=[%s]"), *EIK_LexToString(Result));

		CallbackObjects.Empty();

		bInitialized = false;
	}
}

EOS_EResult FEIKSDKManager::EOSInitialize(EOS_InitializeOptions& Options)
{
	OnPreInitializeSDK.Broadcast(Options);

	return EOS_Initialize(&Options);
}

bool FEIKSDKManager::Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (!FParse::Command(&Cmd, TEXT("EOSSDK")))
	{
		return false;
	}

	if (FParse::Command(&Cmd, TEXT("INFO")))
	{
		LogInfo();
	}
	else
	{
		UE_LOG(LogEIKSDK, Warning, TEXT("Unknown exec command: %s]"), Cmd);
	}

	return true;
}

#define UE_LOG_EOSSDK_INFO(Format, ...) UE_LOG(LogEIKSDK, Log, TEXT("Info: %*s" Format), Indent * 2, TEXT(""), ##__VA_ARGS__)

void FEIKSDKManager::LogInfo(int32 Indent) const
{
	UE_LOG_EOSSDK_INFO("ProductName=%s", *GetProductName());
	UE_LOG_EOSSDK_INFO("ProductVersion=%s", *GetProductVersion());
	UE_LOG_EOSSDK_INFO("CacheDirBase=%s", *GetCacheDirBase());
	UE_LOG_EOSSDK_INFO("Platforms=%d", ActivePlatforms.Num());

	for (int32 PlatformIndex = 0; PlatformIndex < ActivePlatforms.Num(); PlatformIndex++)
	{
		const EOS_HPlatform Platform = ActivePlatforms[PlatformIndex];
		UE_LOG_EOSSDK_INFO("Platform=%d", PlatformIndex);
		Indent++;
		LogPlatformInfo(Platform, Indent);
		Indent--;
	}
}

void FEIKSDKManager::LogPlatformInfo(const EOS_HPlatform Platform, int32 Indent) const
{
	UE_LOG_EOSSDK_INFO("ApplicationStatus=%s", EIK_LexToString(EOS_Platform_GetApplicationStatus(Platform)));
	UE_LOG_EOSSDK_INFO("NetworkStatus=%s", EIK_LexToString(EOS_Platform_GetNetworkStatus(Platform)));
	UE_LOG_EOSSDK_INFO("OverrideCountryCode=%s", *GetOverrideCountryCode(Platform));
	UE_LOG_EOSSDK_INFO("OverrideLocaleCode=%s", *GetOverrideLocaleCode(Platform));

	static_assert(EOS_PLATFORM_GETDESKTOPCROSSPLAYSTATUS_API_LATEST == 1, "EOS_Platform_GetDesktopCrossplayStatusOptions updated");
	EOS_Platform_GetDesktopCrossplayStatusOptions GetDesktopCrossplayStatusOptions;
	GetDesktopCrossplayStatusOptions.ApiVersion = EOS_PLATFORM_GETDESKTOPCROSSPLAYSTATUS_API_LATEST;

	EOS_Platform_GetDesktopCrossplayStatusInfo GetDesktopCrossplayStatusInfo;
	EOS_EResult Result = EOS_Platform_GetDesktopCrossplayStatus(Platform, &GetDesktopCrossplayStatusOptions, &GetDesktopCrossplayStatusInfo);
	if (Result == EOS_EResult::EOS_Success)
	{
		UE_LOG_EOSSDK_INFO("DesktopCrossplayStatusInfo Status=%s ServiceInitResult=%d",
			EIK_LexToString(GetDesktopCrossplayStatusInfo.Status),
			GetDesktopCrossplayStatusInfo.ServiceInitResult);
	}
	else
	{
		UE_LOG_EOSSDK_INFO("DesktopCrossplayStatusInfo (EOS_Platform_GetDesktopCrossplayStatus failed: %s)", *EIK_LexToString(Result));
	}

	const EOS_HAuth AuthHandle = EOS_Platform_GetAuthInterface(Platform);
	const int32_t AuthLoggedInAccountsCount = EOS_Auth_GetLoggedInAccountsCount(AuthHandle);
	UE_LOG_EOSSDK_INFO("AuthLoggedInAccounts=%d", AuthLoggedInAccountsCount);

	for (int32_t AuthLoggedInAccountIndex = 0; AuthLoggedInAccountIndex < AuthLoggedInAccountsCount; AuthLoggedInAccountIndex++)
	{
		const EOS_EpicAccountId LoggedInAccount = EOS_Auth_GetLoggedInAccountByIndex(AuthHandle, AuthLoggedInAccountIndex);
		UE_LOG_EOSSDK_INFO("AuthLoggedInAccount=%d", AuthLoggedInAccountIndex);
		Indent++;
		LogUserInfo(Platform, LoggedInAccount, LoggedInAccount, Indent);
		LogAuthInfo(Platform, LoggedInAccount, Indent);
		LogPresenceInfo(Platform, LoggedInAccount, LoggedInAccount, Indent);
		LogFriendsInfo(Platform, LoggedInAccount, Indent);
		Indent--;
	}

	const EOS_HConnect ConnectHandle = EOS_Platform_GetConnectInterface(Platform);
	const int32_t ConnectLoggedInAccountsCount = EOS_Connect_GetLoggedInUsersCount(ConnectHandle);
	UE_LOG_EOSSDK_INFO("ConnectLoggedInAccounts=%d", ConnectLoggedInAccountsCount);

	for (int32_t ConnectLoggedInAccountIndex = 0; ConnectLoggedInAccountIndex < ConnectLoggedInAccountsCount; ConnectLoggedInAccountIndex++)
	{
		const EOS_ProductUserId LoggedInAccount = EOS_Connect_GetLoggedInUserByIndex(ConnectHandle, ConnectLoggedInAccountIndex);
		UE_LOG_EOSSDK_INFO("ConnectLoggedInAccount=%d", ConnectLoggedInAccountIndex);
		Indent++;
		LogConnectInfo(Platform, LoggedInAccount, Indent);
		Indent--;
	}
}

void FEIKSDKManager::LogAuthInfo(const EOS_HPlatform Platform, const EOS_EpicAccountId LoggedInAccount, int32 Indent) const
{
	const EOS_HAuth AuthHandle = EOS_Platform_GetAuthInterface(Platform);
	
	UE_LOG_EOSSDK_INFO("LoginStatus=%s", EIK_LexToString(EOS_Auth_GetLoginStatus(AuthHandle, LoggedInAccount)));

	static_assert(EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST == 1, "EOS_Auth_CopyUserAuthTokenOptions updated");
	EOS_Auth_CopyUserAuthTokenOptions CopyUserAuthTokenOptions;
	CopyUserAuthTokenOptions.ApiVersion = EOS_AUTH_COPYUSERAUTHTOKEN_API_LATEST;

	EOS_Auth_Token* AuthToken;
	EOS_EResult Result = EOS_Auth_CopyUserAuthToken(AuthHandle, &CopyUserAuthTokenOptions, LoggedInAccount, &AuthToken);
	if (Result == EOS_EResult::EOS_Success)
	{
		UE_LOG_EOSSDK_INFO("AuthToken");
		Indent++;
		UE_LOG_EOSSDK_INFO("App=%s", UTF8_TO_TCHAR(AuthToken->App));
		UE_LOG_EOSSDK_INFO("ClientId=%s", UTF8_TO_TCHAR(AuthToken->ClientId));
#if !UE_BUILD_SHIPPING
		UE_LOG_EOSSDK_INFO("AccessToken=%s", UTF8_TO_TCHAR(AuthToken->AccessToken));
#endif // !UE_BUILD_SHIPPING
		UE_LOG_EOSSDK_INFO("ExpiresIn=%f", AuthToken->ExpiresIn);
		UE_LOG_EOSSDK_INFO("ExpiresAt=%s", UTF8_TO_TCHAR(AuthToken->ExpiresAt));
		UE_LOG_EOSSDK_INFO("AuthType=%s", EIK_LexToString(AuthToken->AuthType));
#if !UE_BUILD_SHIPPING
		UE_LOG_EOSSDK_INFO("RefreshToken=%s", UTF8_TO_TCHAR(AuthToken->RefreshToken));
#endif // !UE_BUILD_SHIPPING
		UE_LOG_EOSSDK_INFO("RefreshExpiresIn=%f", AuthToken->RefreshExpiresIn);
		UE_LOG_EOSSDK_INFO("RefreshExpiresAt=%s", UTF8_TO_TCHAR(AuthToken->RefreshExpiresAt));
		Indent--;
		EOS_Auth_Token_Release(AuthToken);
	}
	else
	{
		UE_LOG_EOSSDK_INFO("AuthToken (EOS_Auth_CopyUserAuthToken failed: %s)", *EIK_LexToString(Result));
	}

#if !UE_BUILD_SHIPPING
	static_assert(EOS_AUTH_COPYIDTOKEN_API_LATEST == 1, "EOS_Auth_CopyIdTokenOptions updated");
	EOS_Auth_CopyIdTokenOptions CopyIdTokenOptions;
	CopyIdTokenOptions.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;
	CopyIdTokenOptions.AccountId = LoggedInAccount;

	EOS_Auth_IdToken* IdToken;
	Result = EOS_Auth_CopyIdToken(AuthHandle, &CopyIdTokenOptions, &IdToken);
	if (Result == EOS_EResult::EOS_Success)
	{
		UE_LOG_EOSSDK_INFO("IdToken=%s", UTF8_TO_TCHAR(IdToken->JsonWebToken));
		EOS_Auth_IdToken_Release(IdToken);
	}
	else
	{
		UE_LOG_EOSSDK_INFO("IdToken (EOS_Auth_CopyIdToken failed: %s)", *EIK_LexToString(Result));
	}
#endif // !UE_BUILD_SHIPPING
}

void FEIKSDKManager::LogUserInfo(const EOS_HPlatform Platform, const EOS_EpicAccountId LoggedInAccount, const EOS_EpicAccountId TargetAccount, int32 Indent) const
{
	UE_LOG_EOSSDK_INFO("EpicAccountId=%s", *EIK_LexToString(TargetAccount));

	static_assert(EOS_USERINFO_COPYUSERINFO_API_LATEST >=2 && EOS_USERINFO_COPYUSERINFO_API_LATEST <= 3, "EOS_UserInfo_CopyUserInfo updated");
	EOS_UserInfo_CopyUserInfoOptions CopyUserInfoOptions;
	CopyUserInfoOptions.ApiVersion = EOS_USERINFO_COPYUSERINFO_API_LATEST;
	CopyUserInfoOptions.LocalUserId = LoggedInAccount;
	CopyUserInfoOptions.TargetUserId = TargetAccount;

	const EOS_HUserInfo UserInfoHandle = EOS_Platform_GetUserInfoInterface(Platform);
	EOS_UserInfo* UserInfo;
	EOS_EResult Result = EOS_UserInfo_CopyUserInfo(UserInfoHandle, &CopyUserInfoOptions, &UserInfo);
	if (Result == EOS_EResult::EOS_Success)
	{
		UE_LOG_EOSSDK_INFO("UserInfo");
		Indent++;
		UE_LOG_EOSSDK_INFO("Country=%s", UTF8_TO_TCHAR(UserInfo->Country));
		UE_LOG_EOSSDK_INFO("DisplayName=%s", UTF8_TO_TCHAR(UserInfo->DisplayName));
		UE_LOG_EOSSDK_INFO("PreferredLanguage=%s", UTF8_TO_TCHAR(UserInfo->PreferredLanguage));
		UE_LOG_EOSSDK_INFO("Nickname=%s", UTF8_TO_TCHAR(UserInfo->Nickname));
		UE_LOG_EOSSDK_INFO("DisplayNameSanitized=%s", UTF8_TO_TCHAR(UserInfo->DisplayNameSanitized));
		Indent--;

		EOS_UserInfo_Release(UserInfo);
	}
	else
	{
		UE_LOG_EOSSDK_INFO("UserInfo (EOS_UserInfo_CopyUserInfo failed: %s)", *EIK_LexToString(Result));
	}
}

void FEIKSDKManager::LogPresenceInfo(const EOS_HPlatform Platform, const EOS_EpicAccountId LoggedInAccount, const EOS_EpicAccountId TargetAccount, int32 Indent) const
{
	static_assert(EOS_PRESENCE_HASPRESENCE_API_LATEST == 1, "EOS_Presence_HasPresenceOptions updated");
	EOS_Presence_HasPresenceOptions HasPresenceOptions;
	HasPresenceOptions.ApiVersion = EOS_PRESENCE_HASPRESENCE_API_LATEST;
	HasPresenceOptions.LocalUserId = LoggedInAccount;
	HasPresenceOptions.TargetUserId = TargetAccount;

	const EOS_HPresence PresenceHandle = EOS_Platform_GetPresenceInterface(Platform);
	if (!EOS_Presence_HasPresence(PresenceHandle, &HasPresenceOptions))
	{
		UE_LOG_EOSSDK_INFO("Presence (None)");
		return;
	}

	static_assert(EOS_PRESENCE_COPYPRESENCE_API_LATEST >=2 && EOS_PRESENCE_COPYPRESENCE_API_LATEST <= 3, "EOS_Presence_CopyPresenceOptions updated");
	EOS_Presence_CopyPresenceOptions CopyPresenceOptions;
	CopyPresenceOptions.ApiVersion = EOS_PRESENCE_COPYPRESENCE_API_LATEST;
	CopyPresenceOptions.LocalUserId = LoggedInAccount;
	CopyPresenceOptions.TargetUserId = TargetAccount;

	EOS_Presence_Info* PresenceInfo;
	EOS_EResult Result = EOS_Presence_CopyPresence(PresenceHandle, &CopyPresenceOptions, &PresenceInfo);
	if (Result == EOS_EResult::EOS_Success)
	{
		UE_LOG_EOSSDK_INFO("Presence");
		Indent++;
		UE_LOG_EOSSDK_INFO("Status=%s", EIK_LexToString(PresenceInfo->Status));
		UE_LOG_EOSSDK_INFO("ProductId=%s", UTF8_TO_TCHAR(PresenceInfo->ProductId));
		UE_LOG_EOSSDK_INFO("ProductName=%s", UTF8_TO_TCHAR(PresenceInfo->ProductName));
		UE_LOG_EOSSDK_INFO("ProductVersion=%s", UTF8_TO_TCHAR(PresenceInfo->ProductVersion));
		UE_LOG_EOSSDK_INFO("Platform=%s", UTF8_TO_TCHAR(PresenceInfo->Platform));
		UE_LOG_EOSSDK_INFO("IntegratedPlatform=%s", UTF8_TO_TCHAR(PresenceInfo->IntegratedPlatform));
		UE_LOG_EOSSDK_INFO("RichText=%s", UTF8_TO_TCHAR(PresenceInfo->RichText));
		UE_LOG_EOSSDK_INFO("RecordsCount=%d", PresenceInfo->RecordsCount);
		Indent++;
		for (int32_t Index = 0; Index < PresenceInfo->RecordsCount; Index++)
		{
			UE_LOG_EOSSDK_INFO("Key=%s Value=%s", UTF8_TO_TCHAR(PresenceInfo->Records[Index].Key), UTF8_TO_TCHAR(PresenceInfo->Records[Index].Value));
		}
		Indent--;
		Indent--;

		EOS_Presence_Info_Release(PresenceInfo);
	}
	else
	{
		UE_LOG_EOSSDK_INFO("Presence (EOS_Presence_CopyPresence failed: %s)", *EIK_LexToString(Result));
	}
}

void FEIKSDKManager::LogFriendsInfo(const EOS_HPlatform Platform, const EOS_EpicAccountId LoggedInAccount, int32 Indent) const
{
	static_assert(EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST == 1, "EOS_Friends_GetFriendsCountOptions updated");
	EOS_Friends_GetFriendsCountOptions FriendsCountOptions;
	FriendsCountOptions.ApiVersion = EOS_FRIENDS_GETFRIENDSCOUNT_API_LATEST;
	FriendsCountOptions.LocalUserId = LoggedInAccount;

	const EOS_HFriends FriendsHandle = EOS_Platform_GetFriendsInterface(Platform);
	const int32_t FriendsCount = EOS_Friends_GetFriendsCount(FriendsHandle, &FriendsCountOptions);
	UE_LOG_EOSSDK_INFO("Friends=%d", FriendsCount);

	static_assert(EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST == 1, "EOS_Friends_GetFriendAtIndexOptions updated");
	EOS_Friends_GetFriendAtIndexOptions FriendAtIndexOptions;
	FriendAtIndexOptions.ApiVersion = EOS_FRIENDS_GETFRIENDATINDEX_API_LATEST;
	FriendAtIndexOptions.LocalUserId = LoggedInAccount;

	for (int32_t FriendIndex = 0; FriendIndex < FriendsCount; FriendIndex++)
	{
		FriendAtIndexOptions.Index = FriendIndex;
		const EOS_EpicAccountId FriendId = EOS_Friends_GetFriendAtIndex(FriendsHandle, &FriendAtIndexOptions);
		UE_LOG_EOSSDK_INFO("Friend=%d", FriendIndex);
		Indent++;

		static_assert(EOS_FRIENDS_GETSTATUS_API_LATEST == 1, "EOS_Friends_GetStatusOptions updated");
		EOS_Friends_GetStatusOptions GetStatusOptions;
		GetStatusOptions.ApiVersion = EOS_FRIENDS_GETSTATUS_API_LATEST;
		GetStatusOptions.LocalUserId = LoggedInAccount;
		GetStatusOptions.TargetUserId = FriendId;

		const EOS_EFriendsStatus FriendStatus = EOS_Friends_GetStatus(FriendsHandle, &GetStatusOptions);
		UE_LOG_EOSSDK_INFO("FriendStatus=%s", EIK_LexToString(FriendStatus));

		LogUserInfo(Platform, LoggedInAccount, FriendId, Indent);
		LogPresenceInfo(Platform, LoggedInAccount, FriendId, Indent);
		Indent--;
	}
}

void FEIKSDKManager::LogConnectInfo(const EOS_HPlatform Platform, const EOS_ProductUserId LoggedInAccount, int32 Indent) const
{
	UE_LOG_EOSSDK_INFO("ProductUserId=%s", *EIK_LexToString(LoggedInAccount));

	const EOS_HConnect ConnectHandle = EOS_Platform_GetConnectInterface(Platform);
	UE_LOG_EOSSDK_INFO("LoginStatus=%s", EIK_LexToString(EOS_Connect_GetLoginStatus(ConnectHandle, LoggedInAccount)));

	EOS_EResult Result;

#if !UE_BUILD_SHIPPING
	static_assert(EOS_CONNECT_COPYIDTOKEN_API_LATEST == 1, "EOS_Connect_CopyIdTokenOptions updated");
	EOS_Connect_CopyIdTokenOptions CopyIdTokenOptions;
	CopyIdTokenOptions.ApiVersion = EOS_CONNECT_COPYIDTOKEN_API_LATEST;
	CopyIdTokenOptions.LocalUserId = LoggedInAccount;

	EOS_Connect_IdToken* IdToken;
	Result = EOS_Connect_CopyIdToken(ConnectHandle, &CopyIdTokenOptions, &IdToken);
	if (Result == EOS_EResult::EOS_Success)
	{
		UE_LOG_EOSSDK_INFO("IdToken=%s", UTF8_TO_TCHAR(IdToken->JsonWebToken));
		EOS_Connect_IdToken_Release(IdToken);
	}
	else
	{
		UE_LOG_EOSSDK_INFO("IdToken (EOS_Connect_CopyIdToken failed: %s)", *EIK_LexToString(Result));
	}
#endif // !UE_BUILD_SHIPPING

	static_assert(EOS_CONNECT_GETPRODUCTUSEREXTERNALACCOUNTCOUNT_API_LATEST == 1, "EOS_Connect_GetProductUserExternalAccountCountOptions updated");
	EOS_Connect_GetProductUserExternalAccountCountOptions ExternalAccountCountOptions;
	ExternalAccountCountOptions.ApiVersion = EOS_CONNECT_GETPRODUCTUSEREXTERNALACCOUNTCOUNT_API_LATEST;
	ExternalAccountCountOptions.TargetUserId = LoggedInAccount;

	const uint32_t ExternalAccountCount = EOS_Connect_GetProductUserExternalAccountCount(ConnectHandle, &ExternalAccountCountOptions);
	UE_LOG_EOSSDK_INFO("ExternalAccounts=%d", ExternalAccountCount);

	static_assert(EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYINDEX_API_LATEST == 1, "EOS_Connect_CopyProductUserExternalAccountByIndexOptions updated");
	EOS_Connect_CopyProductUserExternalAccountByIndexOptions ExternalAccountByIndexOptions;
	ExternalAccountByIndexOptions.ApiVersion = EOS_CONNECT_COPYPRODUCTUSEREXTERNALACCOUNTBYINDEX_API_LATEST;
	ExternalAccountByIndexOptions.TargetUserId = LoggedInAccount;

	for (uint32_t ExternalAccountIndex = 0; ExternalAccountIndex < ExternalAccountCount; ExternalAccountIndex++)
	{
		UE_LOG_EOSSDK_INFO("ExternalAccount=%u", ExternalAccountIndex);
		Indent++;

		ExternalAccountByIndexOptions.ExternalAccountInfoIndex = ExternalAccountIndex;
		EOS_Connect_ExternalAccountInfo* ExternalAccountInfo;
		Result = EOS_Connect_CopyProductUserExternalAccountByIndex(ConnectHandle, &ExternalAccountByIndexOptions, &ExternalAccountInfo);
		if (Result == EOS_EResult::EOS_Success)
		{
			UE_LOG_EOSSDK_INFO("ExternalAccountInfo");
			Indent++;
			UE_LOG_EOSSDK_INFO("DisplayName=%s", UTF8_TO_TCHAR(ExternalAccountInfo->DisplayName));
			UE_LOG_EOSSDK_INFO("AccountId=%s", UTF8_TO_TCHAR(ExternalAccountInfo->AccountId));
			UE_LOG_EOSSDK_INFO("AccountIdType=%s", EIK_LexToString(ExternalAccountInfo->AccountIdType));
			UE_LOG_EOSSDK_INFO("LastLoginTime=%lld", ExternalAccountInfo->LastLoginTime);
			Indent--;

			EOS_Connect_ExternalAccountInfo_Release(ExternalAccountInfo);
		}
		else
		{
			UE_LOG_EOSSDK_INFO("ExternalAccountInfo (EOS_Connect_CopyProductUserExternalAccountByIndex failed: %s)", *EIK_LexToString(Result));
		}

		Indent--;
	}
}

void FEIKSDKManager::AddCallbackObject(TUniquePtr<FCallbackBase> CallbackObj)
{
	CallbackObjects.Emplace(MoveTemp(CallbackObj));
}

FEIKPlatformHandle::~FEIKPlatformHandle()
{
	Manager.ReleasePlatform(PlatformHandle);
}

void FEIKPlatformHandle::Tick()
{
	LLM_SCOPE(ELLMTag::RealTimeCommunications);
	QUICK_SCOPE_CYCLE_COUNTER(FEIKPlatformHandle_Tick);
	CSV_SCOPED_TIMING_STAT_EXCLUSIVE(EOSSDK);
	EOS_Platform_Tick(PlatformHandle);
}

FString FEIKPlatformHandle::GetOverrideCountryCode() const
{
	return Manager.GetOverrideCountryCode(PlatformHandle);
}

FString FEIKPlatformHandle::GetOverrideLocaleCode() const
{
	return Manager.GetOverrideLocaleCode(PlatformHandle);
}

void FEIKPlatformHandle::LogInfo(int32 Indent) const
{
	Manager.LogPlatformInfo(PlatformHandle, Indent);
}

void FEIKPlatformHandle::LogAuthInfo(const EOS_EpicAccountId LoggedInAccount, int32 Indent) const
{
	Manager.LogAuthInfo(PlatformHandle, LoggedInAccount, Indent);
}

void FEIKPlatformHandle::LogUserInfo(const EOS_EpicAccountId LoggedInAccount, const EOS_EpicAccountId TargetAccount, int32 Indent) const
{
	Manager.LogUserInfo(PlatformHandle, LoggedInAccount, TargetAccount, Indent);
}

void FEIKPlatformHandle::LogPresenceInfo(const EOS_EpicAccountId LoggedInAccount, const EOS_EpicAccountId TargetAccount, int32 Indent) const
{
	Manager.LogPresenceInfo(PlatformHandle, LoggedInAccount, TargetAccount, Indent);
}

void FEIKPlatformHandle::LogFriendsInfo(const EOS_EpicAccountId LoggedInAccount, int32 Indent) const
{
	Manager.LogFriendsInfo(PlatformHandle, LoggedInAccount, Indent);
}

void FEIKPlatformHandle::LogConnectInfo(const EOS_ProductUserId LoggedInAccount, int32 Indent) const
{
	Manager.LogConnectInfo(PlatformHandle, LoggedInAccount, Indent);
}

#endif // WITH_EOS_SDK
