// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#if WITH_EOS_SDK

#include "Features/IModularFeatures.h"
#include "Templates/UniquePtr.h"

#if defined(EOS_PLATFORM_BASE_FILE_NAME)
#include EOS_PLATFORM_BASE_FILE_NAME
#endif
#include "eos_init.h"
#include "eos_types.h"

struct FEOSSDKPlatformConfig
{
	FString Name;
	FString ProductId;
	FString SandboxId;
	FString ClientId;
	FString ClientSecret;
	FString EncryptionKey;
	FString OverrideCountryCode;
	FString OverrideLocaleCode;
	FString DeploymentId;
	FString CacheDirectory;
	bool bIsServer = false;
	bool bLoadingInEditor = false;
	bool bDisableOverlay = false;
	bool bDisableSocialOverlay = false;
	bool bWindowsEnableOverlayD3D9 = false;
	bool bWindowsEnableOverlayD3D10 = false;
	bool bWindowsEnableOverlayOpenGL = false;
	bool bEnableRTC = true;
	int32 TickBudgetInMilliseconds = 1;
	TArray<FString> OptionalConfig;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FEIKSDKManagerOnPreInitializeSDK, EOS_InitializeOptions& Options);
DECLARE_MULTICAST_DELEGATE_TwoParams(FEIKSDKManagerOnDefaultPlatformConfigNameChanged, const FString& NewName, const FString& OldName);
DECLARE_MULTICAST_DELEGATE_TwoParams(FEIKSDKManagerOnPreCreateNamedPlatform, const FEOSSDKPlatformConfig& Config, EOS_Platform_Options& Options);
DECLARE_MULTICAST_DELEGATE_OneParam(FEIKSDKManagerOnPreCreatePlatform, EOS_Platform_Options& Options);

class IEIKPlatformHandle
{
public:
	IEIKPlatformHandle(EOS_HPlatform InPlatformHandle) : PlatformHandle(InPlatformHandle) {}
	virtual ~IEIKPlatformHandle() = default;

	virtual void Tick() = 0;

	operator EOS_HPlatform() const { return PlatformHandle; }

	virtual FString GetOverrideCountryCode() const = 0;
	virtual FString GetOverrideLocaleCode() const = 0;

	virtual void LogInfo(int32 Indent = 0) const = 0;
	virtual void LogAuthInfo(const EOS_EpicAccountId LoggedInAccount, int32 Indent = 0) const = 0;
	virtual void LogUserInfo(const EOS_EpicAccountId LoggedInAccount, const EOS_EpicAccountId TargetAccount, int32 Indent = 0) const = 0;
	virtual void LogPresenceInfo(const EOS_EpicAccountId LoggedInAccount, const EOS_EpicAccountId TargetAccount, int32 Indent = 0) const = 0;
	virtual void LogFriendsInfo(const EOS_EpicAccountId LoggedInAccount, int32 Indent = 0) const = 0;
	virtual void LogConnectInfo(const EOS_ProductUserId LoggedInAccount, int32 Indent = 0) const = 0;

	EOS_HPlatform PlatformHandle;
protected:
	//EOS_HPlatform PlatformHandle;
};

using IEIKPlatformHandlePtr = TSharedPtr<IEIKPlatformHandle, ESPMode::ThreadSafe>;
using IEIKPlatformHandleWeakPtr = TWeakPtr<IEIKPlatformHandle, ESPMode::ThreadSafe>;

class IEOSSDKManager : public IModularFeature
{
public:
	static IEOSSDKManager* Get()
	{
		if (IModularFeatures::Get().IsModularFeatureAvailable(GetModularFeatureName()))
		{
			return &IModularFeatures::Get().GetModularFeature<IEOSSDKManager>(GetModularFeatureName());
		}
		return nullptr;
	}

	static FName GetModularFeatureName()
	{
		static const FName FeatureName = TEXT("EOSSDKManager");
		return FeatureName;
	}

	virtual ~IEOSSDKManager() = default;

	virtual EOS_EResult Initialize() = 0;
	virtual bool IsInitialized() const = 0;

	virtual const FEOSSDKPlatformConfig* GetPlatformConfig(const FString& PlatformConfigName, bool bLoadIfMissing = false) = 0;
	virtual bool AddPlatformConfig(const FEOSSDKPlatformConfig& PlatformConfig) = 0;
	virtual const FString& GetDefaultPlatformConfigName() = 0;
	virtual void SetDefaultPlatformConfigName(const FString& PlatformConfigName) = 0;

	/**
	 * Create a platform handle for a platform config name. Config is loaded from .ini files if it was not added with AddPlatformConfig.
	 * If a platform handle already exists for the config name, this will return a shared pointer to that handle and not create a new one.
	 */
	virtual IEIKPlatformHandlePtr CreatePlatform(const FString& PlatformConfigName, FName InstanceName = NAME_None) = 0;

	/** Create a platform handle using EOSSDK options directly. */
	virtual IEIKPlatformHandlePtr CreatePlatform(EOS_Platform_Options& PlatformOptions) = 0;

	virtual FString GetProductName() const = 0;
	virtual FString GetProductVersion() const = 0;
	virtual FString GetCacheDirBase() const = 0;
	virtual FString GetOverrideCountryCode(const EOS_HPlatform Platform) const = 0;
	virtual FString GetOverrideLocaleCode(const EOS_HPlatform Platform) const = 0;

	virtual void LogInfo(int32 Indent = 0) const = 0;
	virtual void LogPlatformInfo(const EOS_HPlatform Platform, int32 Indent = 0) const = 0;
	virtual void LogAuthInfo(const EOS_HPlatform Platform, const EOS_EpicAccountId LoggedInAccount, int32 Indent = 0) const = 0;
	virtual void LogUserInfo(const EOS_HPlatform Platform, const EOS_EpicAccountId LoggedInAccount, const EOS_EpicAccountId TargetAccount, int32 Indent = 0) const = 0;
	virtual void LogPresenceInfo(const EOS_HPlatform Platform, const EOS_EpicAccountId LoggedInAccount, const EOS_EpicAccountId TargetAccount, int32 Indent = 0) const = 0;
	virtual void LogFriendsInfo(const EOS_HPlatform Platform, const EOS_EpicAccountId LoggedInAccount, int32 Indent = 0) const = 0;
	virtual void LogConnectInfo(const EOS_HPlatform Platform, const EOS_ProductUserId LoggedInAccount, int32 Indent = 0) const = 0;

	/** Assign ownership of a callback object, to be free'd after EOS_Shutdown */
	virtual void AddCallbackObject(TUniquePtr<class FCallbackBase> CallbackObj) = 0;

	FEIKSDKManagerOnPreInitializeSDK OnPreInitializeSDK;
	FEIKSDKManagerOnDefaultPlatformConfigNameChanged OnDefaultPlatformConfigNameChanged;
	FEIKSDKManagerOnPreCreateNamedPlatform OnPreCreateNamedPlatform;
	FEIKSDKManagerOnPreCreatePlatform OnPreCreatePlatform;
};

#endif // WITH_EOS_SDK