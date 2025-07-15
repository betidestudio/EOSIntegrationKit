// Copyright Epic Games, Inc. All Rights Reserved.

#include "EOSVoiceChat.h" 

#include "EIKSettings.h"

#if WITH_EOS_RTC

#include "Async/Async.h"
#include "Containers/Ticker.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/ScopeLock.h"
#include "ProfilingDebugging/CsvProfiler.h"
#include "Stats/Stats.h"

#include "EOSShared.h"
#include "EOSVoiceChatLog.h"
#include "EIKVoiceChatModule.h"
#include "EOSVoiceChatUser.h"
#include "EOSAudioDevicePool.h"
#include "IEOSSDKManager.h"
#include "VoiceChatErrors.h"
#include "VoiceChatResult.h"

#include "eos_sdk.h"
#include "eos_rtc.h"
#include "eos_rtc_audio.h"

#define CHECKPIN() FEOSVoiceChatPtr StrongThis = WeakThis.Pin(); if(!StrongThis) return

DEFINE_LOG_CATEGORY(LogEOSVoiceChat);

const TCHAR* EIK_LexToString(EOS_ERTCAudioInputStatus Status)
{
	switch (Status)
	{
	case EOS_ERTCAudioInputStatus::EOS_RTCAIS_Idle: return TEXT("EOS_RTCAIS_Idle");
	case EOS_ERTCAudioInputStatus::EOS_RTCAIS_Recording: return TEXT("EOS_RTCAIS_Recording");
	case EOS_ERTCAudioInputStatus::EOS_RTCAIS_RecordingSilent: return TEXT("EOS_RTCAIS_RecordingSilent");
	case EOS_ERTCAudioInputStatus::EOS_RTCAIS_RecordingDisconnected: return TEXT("EOS_RTCAIS_RecordingDisconnected");
	case EOS_ERTCAudioInputStatus::EOS_RTCAIS_Failed: return TEXT("EOS_RTCEOS_RTCAIS_Failed_AudioInputFailed");
	default: return TEXT("Unknown");
	}
}

FEIKVoiceChatDelegates::FOnAudioInputDeviceStatusChanged FEIKVoiceChatDelegates::OnAudioInputDeviceStatusChanged;
FEIKVoiceChatDelegates::FOnVoiceChatChannelConnectionStateDelegate FEIKVoiceChatDelegates::OnVoiceChatChannelConnectionStateChanged;
FEIKVoiceChatDelegates::FOnVoiceChatPlayerAddedMetadataDelegate FEIKVoiceChatDelegates::OnVoiceChatPlayerAddedMetadata;
FEIKVoiceChatDelegates::FOnAudioStatusChanged FEIKVoiceChatDelegates::OnAudioStatusChanged;

int64 FEOSVoiceChat::StaticInstanceIdCount = 0;

#define EOS_VOICE_TODO 0

FEOSVoiceChat::FEOSVoiceChat(IEOSSDKManager& InSDKManager, const IEIKPlatformHandlePtr& InPlatformHandle)
	: SDKManager(InSDKManager)
	, EosPlatformHandle(InPlatformHandle)
{
}

FEOSVoiceChat::~FEOSVoiceChat()
{
}

#pragma region IVoiceChat
bool FEOSVoiceChat::Initialize()
{
	if (!IsInitialized())
	{
		Initialize(FOnVoiceChatInitializeCompleteDelegate());
	}

	return IsInitialized();
}

bool FEOSVoiceChat::Uninitialize()
{
	bool bIsDone = false;
	Uninitialize(FOnVoiceChatUninitializeCompleteDelegate::CreateLambda([&bIsDone](const FVoiceChatResult& Result)
	{
		bIsDone = true;
	}));

	while (!bIsDone)
	{
		EosPlatformHandle->Tick();
	}
	
	return !IsInitialized();
}

void FEOSVoiceChat::Initialize(const FOnVoiceChatInitializeCompleteDelegate& InitCompleteDelegate)
{
	FVoiceChatResult Result(FVoiceChatResult::CreateSuccess());

	switch (InitSession.State)
	{
	case EInitializationState::Uninitialized:
	{
		const UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>();
		bool bEnabled = true;
		if(!EIKSettings || EIKSettings->VoiceArtifactName.IsEmpty())
		{
			bEnabled = false;
		}
		if (bEnabled)
		{
			InitSession.State = EInitializationState::Initializing;

			if (!EosPlatformHandle)
			{
				EOS_EResult EosResult = SDKManager.Initialize();
				if (EosResult == EOS_EResult::EOS_Success)
				{
					FString ConfigProductId;
					FString ConfigSandboxId;
					FString ConfigDeploymentId;
					FString ConfigClientId;
					FString ConfigClientSecret;
					FString ConfigEncryptionKey;
					FString ConfigOverrideCountryCode;
					FString ConfigOverrideLocaleCode;
					
					FEOSArtifactSettings VoiceArtifactSettings;
					EIKSettings->GetSettingsForArtifact(EIKSettings->VoiceArtifactName, VoiceArtifactSettings);
					ConfigProductId = VoiceArtifactSettings.ProductId;
					ConfigSandboxId = VoiceArtifactSettings.SandboxId;
					ConfigDeploymentId = VoiceArtifactSettings.DeploymentId;
					ConfigClientId = VoiceArtifactSettings.ClientId;
					ConfigClientSecret = VoiceArtifactSettings.ClientSecret;
					ConfigEncryptionKey = VoiceArtifactSettings.EncryptionKey;
					
					if(ConfigProductId.IsEmpty() || ConfigSandboxId.IsEmpty() || ConfigDeploymentId.IsEmpty() || ConfigClientId.IsEmpty() || ConfigClientSecret.IsEmpty() || ConfigEncryptionKey.IsEmpty())
					{
						UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChat::Initialize ProductID, SandboxID, DeploymentID, ClientID, ClientSecret, EncryptionKey are empty. Using EOSVoiceChat section in Engine.ini"));
						GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("ProductId"), ConfigProductId, GEngineIni);
						GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("SandboxId"), ConfigSandboxId, GEngineIni);
						GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("DeploymentId"), ConfigDeploymentId, GEngineIni);
						GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("ClientId"), ConfigClientId, GEngineIni);
						GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("ClientSecret"), ConfigClientSecret, GEngineIni);
						GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("ClientEncryptionKey"), ConfigEncryptionKey, GEngineIni);
						GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("OverrideCountryCode"), ConfigOverrideCountryCode, GEngineIni);
						GConfig->GetString(TEXT("EOSVoiceChat"), TEXT("OverrideLocaleCode"), ConfigOverrideLocaleCode, GEngineIni);
						
					}
					const FTCHARToUTF8 Utf8ProductId(*ConfigProductId);
					const FTCHARToUTF8 Utf8SandboxId(*ConfigSandboxId);
					const FTCHARToUTF8 Utf8DeploymentId(*ConfigDeploymentId);
					const FTCHARToUTF8 Utf8ClientId(*ConfigClientId);
					const FTCHARToUTF8 Utf8ClientSecret(*ConfigClientSecret);
					const FTCHARToUTF8 Utf8EncryptionKey(*ConfigEncryptionKey);
					const FTCHARToUTF8 Utf8OverrideCountryCode(*ConfigOverrideCountryCode);
					const FTCHARToUTF8 Utf8OverrideLocaleCode(*ConfigOverrideLocaleCode);

					EOS_Platform_Options PlatformOptions = {};
					PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
					PlatformOptions.Reserved = nullptr;
					PlatformOptions.ProductId = ConfigProductId.IsEmpty() ? nullptr : Utf8ProductId.Get();
					PlatformOptions.SandboxId = ConfigSandboxId.IsEmpty() ? nullptr : Utf8SandboxId.Get();
					PlatformOptions.ClientCredentials.ClientId = ConfigClientId.IsEmpty() ? nullptr : Utf8ClientId.Get();
					PlatformOptions.ClientCredentials.ClientSecret = ConfigClientSecret.IsEmpty() ? nullptr : Utf8ClientSecret.Get();
					PlatformOptions.bIsServer = false;
					PlatformOptions.EncryptionKey = ConfigEncryptionKey.IsEmpty() ? nullptr : Utf8EncryptionKey.Get();
					PlatformOptions.OverrideCountryCode = ConfigOverrideCountryCode.IsEmpty() ? nullptr : Utf8OverrideCountryCode.Get();
					PlatformOptions.OverrideLocaleCode = ConfigOverrideLocaleCode.IsEmpty() ? nullptr : Utf8OverrideLocaleCode.Get();
					PlatformOptions.DeploymentId = ConfigDeploymentId.IsEmpty() ? nullptr : Utf8DeploymentId.Get();
					PlatformOptions.Flags = EOS_PF_DISABLE_OVERLAY;
					PlatformOptions.CacheDirectory = nullptr;
					PlatformOptions.TickBudgetInMilliseconds = 1;
					PlatformOptions.IntegratedPlatformOptionsContainerHandle = nullptr;
#if UE_EDITOR
					//PlatformCreateOptions.Flags |= EOS_PF_LOADING_IN_EDITOR;
#endif

					EOS_Platform_RTCOptions PlatformRTCOptions = {};
					PlatformRTCOptions.ApiVersion = EOS_PLATFORM_RTCOPTIONS_API_LATEST;
					PlatformOptions.RTCOptions = &PlatformRTCOptions;

					EosPlatformHandle = EOSPlatformCreate(PlatformOptions);
					if (!EosPlatformHandle)
					{
						UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChat::Initialize CreatePlatform failed"));
						Result = FVoiceChatResult(EVoiceChatResult::ImplementationError);
					}
				}
				else
				{
					UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChat::Initialize SDKManager.Initialize failed"));
					Result = FVoiceChatResult(EVoiceChatResult::ImplementationError);
				}
			}

			if (Result.IsSuccess())
			{
				InitSession.EosRtcInterface = EOS_Platform_GetRTCInterface(*EosPlatformHandle);
				InitSession.EosLobbyInterface = EOS_Platform_GetLobbyInterface(*EosPlatformHandle);
				if (InitSession.EosRtcInterface && InitSession.EosLobbyInterface)
				{
					BindInitCallbacks();
					InitSession.State = EInitializationState::Initialized;
					PostInitialize();
					Result = FVoiceChatResult::CreateSuccess();
				}
				else
				{
					UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChat::Initialize failed to get interface handles"));
					Result = FVoiceChatResult(EVoiceChatResult::ImplementationError);
				}
			}
		}
		else
		{
			Result = VoiceChat::Errors::NotEnabled();
		}

		if (!Result.IsSuccess())
		{
			InitSession.Reset();
		}

		break;
	}
	case EInitializationState::Uninitializing:
		UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChat::Initialize call unexpected while State=Uninitializing"));
		Result = VoiceChat::Errors::InvalidState();
		break;
	case EInitializationState::Initializing:
		checkNoEntry(); // Should not be possible, Initialize is a synchronous call.
		UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChat::Initialize call unexpected while State=Initializing"));
		Result = VoiceChat::Errors::InvalidState();
		break;
	case EInitializationState::Initialized:
		Result = FVoiceChatResult::CreateSuccess();
		break;
	}

	InitCompleteDelegate.ExecuteIfBound(Result);
}

void FEOSVoiceChat::Uninitialize(const FOnVoiceChatUninitializeCompleteDelegate& UninitCompleteDelegate)
{
	switch (InitSession.State)
	{
	case EInitializationState::Uninitialized:
		UninitCompleteDelegate.ExecuteIfBound(FVoiceChatResult::CreateSuccess());
		break;
	case EInitializationState::Uninitializing:
		InitSession.UninitializeCompleteDelegates.Emplace(UninitCompleteDelegate);
		break;
	case EInitializationState::Initializing:
		UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChat::Uninitialize call unexpected while State=Initializing"));
		UninitCompleteDelegate.ExecuteIfBound(VoiceChat::Errors::InvalidState());
		break;
	case EInitializationState::Initialized:
		InitSession.State = EInitializationState::Uninitializing;
		InitSession.UninitializeCompleteDelegates.Emplace(UninitCompleteDelegate);

		auto CompleteUninitialize = [this]()
		{
			PreUninitialize();
			UnbindInitCallbacks();

			const TArray<FOnVoiceChatUninitializeCompleteDelegate> UninitializeCompleteDelegates = MoveTemp(InitSession.UninitializeCompleteDelegates);
			InitSession.Reset();
			for (const FOnVoiceChatUninitializeCompleteDelegate& UninitializeCompleteDelegate : UninitializeCompleteDelegates)
			{
				UninitializeCompleteDelegate.ExecuteIfBound(FVoiceChatResult::CreateSuccess());
			}
		};

		if (IsConnected())
		{
			Disconnect(FOnVoiceChatDisconnectCompleteDelegate::CreateLambda([this, CompleteUninitialize](const FVoiceChatResult& Result)
			{
				if (Result.IsSuccess())
				{
					CompleteUninitialize();
				}
				else
				{
					UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChat::Uninitialize failed %s"), *LexToString(Result));

					InitSession.State = EInitializationState::Initialized;

					const TArray<FOnVoiceChatUninitializeCompleteDelegate> Delegates = MoveTemp(InitSession.UninitializeCompleteDelegates);
					for (const FOnVoiceChatUninitializeCompleteDelegate& Delegate : Delegates)
					{
						Delegate.ExecuteIfBound(Result);
					}
				}
			}));
		}
		else
		{
			CompleteUninitialize();
		}
		break;
	}
}

bool FEOSVoiceChat::IsInitialized() const
{
	return InitSession.State == EInitializationState::Initialized;
}

IVoiceChatUser* FEOSVoiceChat::CreateUser()
{
	const FEOSVoiceChatUserRef& User = VoiceChatUsers.Emplace_GetRef(MakeShared<FEOSVoiceChatUser, ESPMode::ThreadSafe>(*this));

	return &User.Get();
}

#if ENGINE_MAJOR_VERSION != 5
FEOSVoiceChatWeakPtr FEOSVoiceChat::CreateWeakThis()
{
	return FEOSVoiceChatWeakPtr(AsShared());
}
#endif

void FEOSVoiceChat::ReleaseUser(IVoiceChatUser* User)
{
	if (User)
	{		
		if (IsInitialized()
			&& IsConnected()
			&& User->IsLoggedIn())
		{
			UE_LOG(LogEOSVoiceChat, Log, TEXT("ReleaseUser User=[%p] Logging out"), User);
#if ENGINE_MAJOR_VERSION == 5
			User->Logout(FOnVoiceChatLogoutCompleteDelegate::CreateLambda([this, WeakThis = AsWeak(), User](const FString& PlayerName, const FVoiceChatResult& Result)
#else
			User->Logout(FOnVoiceChatLogoutCompleteDelegate::CreateLambda([this, WeakThis = CreateWeakThis(), User](const FString& PlayerName, const FVoiceChatResult& Result)
#endif
			{
				CHECKPIN();

				if (!Result.IsSuccess())
				{
					UE_LOG(LogEOSVoiceChat, Warning, TEXT("ReleaseUser User=[%p] Logout failed, Result=[%s]"), User, *LexToString(Result))
				}

				UE_LOG(LogEOSVoiceChat, Log, TEXT("ReleaseUser User=[%p] Removing"), User);
				VoiceChatUsers.RemoveAll([User](const FEOSVoiceChatUserRef& OtherUser)
				{
					return User == &OtherUser.Get();
				});
			}));
		}
		else
		{
			UE_LOG(LogEOSVoiceChat, Log, TEXT("ReleaseUser User=[%p] Removing"), User);
			VoiceChatUsers.RemoveAll([User](const FEOSVoiceChatUserRef& OtherUser)
			{
				return User == &OtherUser.Get();
			});
		}
	}
}
#pragma endregion IVoiceChat

#pragma region IVoiceChatUser
void FEOSVoiceChat::SetSetting(const FString& Name, const FString& Value)
{
	GetVoiceChatUser().SetSetting(Name, Value);
}

FString FEOSVoiceChat::GetSetting(const FString& Name)
{
	return GetVoiceChatUser().GetSetting(Name);
}

void FEOSVoiceChat::SetAudioInputVolume(float Volume)
{
	GetVoiceChatUser().SetAudioInputVolume(Volume);
}

void FEOSVoiceChat::SetAudioOutputVolume(float Volume)
{
	GetVoiceChatUser().SetAudioOutputVolume(Volume);
}

float FEOSVoiceChat::GetAudioInputVolume() const
{
	return GetVoiceChatUser().GetAudioInputVolume();
}

float FEOSVoiceChat::GetAudioOutputVolume() const
{
	return GetVoiceChatUser().GetAudioOutputVolume();
}

void FEOSVoiceChat::SetAudioInputDeviceMuted(bool bIsMuted)
{
	GetVoiceChatUser().SetAudioInputDeviceMuted(bIsMuted);
}

void FEOSVoiceChat::SetAudioOutputDeviceMuted(bool bIsMuted)
{
	GetVoiceChatUser().SetAudioOutputDeviceMuted(bIsMuted);
}

bool FEOSVoiceChat::GetAudioInputDeviceMuted() const
{
	return GetVoiceChatUser().GetAudioInputDeviceMuted();
}

bool FEOSVoiceChat::GetAudioOutputDeviceMuted() const
{
	return GetVoiceChatUser().GetAudioOutputDeviceMuted();
}

TArray<FVoiceChatDeviceInfo> FEOSVoiceChat::GetAvailableInputDeviceInfos() const
{
	return GetVoiceChatUser().GetAvailableInputDeviceInfos();
}

TArray<FVoiceChatDeviceInfo> FEOSVoiceChat::GetAvailableOutputDeviceInfos() const
{
	return GetVoiceChatUser().GetAvailableOutputDeviceInfos();
}

FOnVoiceChatAvailableAudioDevicesChangedDelegate& FEOSVoiceChat::OnVoiceChatAvailableAudioDevicesChanged()
{
	return GetVoiceChatUser().OnVoiceChatAvailableAudioDevicesChanged();
}

void FEOSVoiceChat::SetInputDeviceId(const FString& InputDeviceId)
{
	GetVoiceChatUser().SetInputDeviceId(InputDeviceId);
}

void FEOSVoiceChat::SetOutputDeviceId(const FString& OutputDeviceId)
{
	GetVoiceChatUser().SetOutputDeviceId(OutputDeviceId);
}

FVoiceChatDeviceInfo FEOSVoiceChat::GetInputDeviceInfo() const
{
	return GetVoiceChatUser().GetInputDeviceInfo();
}

FVoiceChatDeviceInfo FEOSVoiceChat::GetOutputDeviceInfo() const
{
	return GetVoiceChatUser().GetOutputDeviceInfo();
}

FVoiceChatDeviceInfo FEOSVoiceChat::GetDefaultInputDeviceInfo() const
{
	return GetVoiceChatUser().GetDefaultInputDeviceInfo();
}

FVoiceChatDeviceInfo FEOSVoiceChat::GetDefaultOutputDeviceInfo() const
{
	return GetVoiceChatUser().GetDefaultOutputDeviceInfo();
}

void FEOSVoiceChat::Connect(const FOnVoiceChatConnectCompleteDelegate& Delegate)
{
	FVoiceChatResult Result = FVoiceChatResult::CreateSuccess();

	if (!IsInitialized())
	{
		Result = VoiceChat::Errors::NotInitialized();
	}
	else if (ConnectionState == EConnectionState::Disconnecting)
	{
		Result = VoiceChat::Errors::DisconnectInProgress();
	}

	if (!Result.IsSuccess())
	{
		UE_LOG(LogEOSVoiceChat, Warning, TEXT("Connect %s"), *LexToString(Result));
		Delegate.ExecuteIfBound(Result);
	}
	else if (IsConnected())
	{
		Delegate.ExecuteIfBound(FVoiceChatResult::CreateSuccess());
	}
	else
	{
		ConnectionState = EConnectionState::Connected;
		Delegate.ExecuteIfBound(FVoiceChatResult::CreateSuccess());
		OnVoiceChatConnected().Broadcast();
	}
}

void FEOSVoiceChat::Disconnect(const FOnVoiceChatDisconnectCompleteDelegate& Delegate)
{
	// TODO Handle Disconnecting / Connecting states now this is async.
	if (IsConnected())
	{
		ConnectionState = EConnectionState::Disconnecting;

		TSet<FEOSVoiceChatUser*> UsersToLogout;

		if (SingleUserVoiceChatUser)
		{
			FEOSVoiceChatUser::ELoginState LoginState = SingleUserVoiceChatUser->LoginSession.State;
			if (LoginState == FEOSVoiceChatUser::ELoginState::LoggedIn || LoginState == FEOSVoiceChatUser::ELoginState::LoggingOut)
			{
				UsersToLogout.Emplace(SingleUserVoiceChatUser);
			}
		}
		else
		{
			for (const FEOSVoiceChatUserRef& VoiceChatUser : VoiceChatUsers)
			{
				FEOSVoiceChatUser::ELoginState LoginState = VoiceChatUser->LoginSession.State;
				if (LoginState == FEOSVoiceChatUser::ELoginState::LoggedIn || LoginState == FEOSVoiceChatUser::ELoginState::LoggingOut)
				{
					UsersToLogout.Emplace(&VoiceChatUser.Get());
				}
			}
		}

		if (UsersToLogout.Num() > 0)
		{
			struct FEOSVoiceChatDisconnectState
			{
				FVoiceChatResult Result = FVoiceChatResult::CreateSuccess();
				FOnVoiceChatDisconnectCompleteDelegate CompletionDelegate;
				int32 UsersToLogoutCount;
			};
			TSharedPtr<FEOSVoiceChatDisconnectState> DisconnectState = MakeShared<FEOSVoiceChatDisconnectState>();
			DisconnectState->UsersToLogoutCount = UsersToLogout.Num();
			DisconnectState->CompletionDelegate = Delegate;

			for (FEOSVoiceChatUser* User : UsersToLogout)
			{
				User->LogoutInternal(FOnVoiceChatLogoutCompleteDelegate::CreateLambda([this, User, DisconnectState](const FString& PlayerName, const FVoiceChatResult& PlayerResult)
				{
					if (!PlayerResult.IsSuccess())
					{
						UE_LOG(LogEOSVoiceChat, Warning, TEXT("Disconnect LogoutCompleteDelegate PlayerName=[%s] Result=%s"), *PlayerName, *LexToString(PlayerResult));
						DisconnectState->Result = PlayerResult;
					}

					DisconnectState->UsersToLogoutCount--;

					if (DisconnectState->UsersToLogoutCount == 0)
					{
						ConnectionState = DisconnectState->Result.IsSuccess() ? EConnectionState::Disconnected : EConnectionState::Connected;
						DisconnectState->CompletionDelegate.ExecuteIfBound(DisconnectState->Result);
						if (ConnectionState == EConnectionState::Disconnected)
						{
							OnVoiceChatDisconnected().Broadcast(DisconnectState->Result);
						}
					}
				}));
			}
		}
		else
		{
			ConnectionState = EConnectionState::Disconnected;
			Delegate.ExecuteIfBound(FVoiceChatResult::CreateSuccess());
			OnVoiceChatDisconnected().Broadcast(FVoiceChatResult::CreateSuccess());
		}
	}
	else
	{
		Delegate.ExecuteIfBound(FVoiceChatResult::CreateSuccess());
	}
}

bool FEOSVoiceChat::IsConnecting() const
{
	return false;
}

bool FEOSVoiceChat::IsConnected() const
{
	return ConnectionState == EConnectionState::Connected;
}

void FEOSVoiceChat::Login(FPlatformUserId PlatformId, const FString& PlayerName, const FString& Credentials, const FOnVoiceChatLoginCompleteDelegate& Delegate)
{
	GetVoiceChatUser().Login(PlatformId, PlayerName, Credentials, Delegate);
}

void FEOSVoiceChat::Logout(const FOnVoiceChatLogoutCompleteDelegate& Delegate)
{
	GetVoiceChatUser().Logout(Delegate);
}

bool FEOSVoiceChat::IsLoggingIn() const
{
	return GetVoiceChatUser().IsLoggingIn();
}

bool FEOSVoiceChat::IsLoggedIn() const
{
	return GetVoiceChatUser().IsLoggedIn();
}

FOnVoiceChatLoggedInDelegate& FEOSVoiceChat::OnVoiceChatLoggedIn()
{
	return GetVoiceChatUser().OnVoiceChatLoggedIn();
}

FOnVoiceChatLoggedOutDelegate& FEOSVoiceChat::OnVoiceChatLoggedOut()
{
	return GetVoiceChatUser().OnVoiceChatLoggedOut();
}

FString FEOSVoiceChat::GetLoggedInPlayerName() const
{
	return GetVoiceChatUser().GetLoggedInPlayerName();
}

void FEOSVoiceChat::BlockPlayers(const TArray<FString>& PlayerNames)
{
	GetVoiceChatUser().BlockPlayers(PlayerNames);
}

void FEOSVoiceChat::UnblockPlayers(const TArray<FString>& PlayerNames)
{
	GetVoiceChatUser().UnblockPlayers(PlayerNames);
}

void FEOSVoiceChat::JoinChannel(const FString& ChannelName, const FString& ChannelCredentials, EVoiceChatChannelType ChannelType, const FOnVoiceChatChannelJoinCompleteDelegate& Delegate, TOptional<FVoiceChatChannel3dProperties> Channel3dProperties)
{
	GetVoiceChatUser().JoinChannel(ChannelName, ChannelCredentials, ChannelType, Delegate, Channel3dProperties);
}

void FEOSVoiceChat::LeaveChannel(const FString& Channel, const FOnVoiceChatChannelLeaveCompleteDelegate& Delegate)
{
	GetVoiceChatUser().LeaveChannel(Channel, Delegate);
}

FOnVoiceChatChannelJoinedDelegate& FEOSVoiceChat::OnVoiceChatChannelJoined()
{
	return GetVoiceChatUser().OnVoiceChatChannelJoined();
}

FOnVoiceChatChannelExitedDelegate& FEOSVoiceChat::OnVoiceChatChannelExited()
{
	return GetVoiceChatUser().OnVoiceChatChannelExited();
}

FOnVoiceChatCallStatsUpdatedDelegate& FEOSVoiceChat::OnVoiceChatCallStatsUpdated()
{
	return GetVoiceChatUser().OnVoiceChatCallStatsUpdated();
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
void FEOSVoiceChat::Set3DPosition(const FString& ChannelName, const FVector& Position)
{
	GetVoiceChatUser().Set3DPosition(ChannelName, Position);
}
#else
void FEOSVoiceChat::Set3DPosition(const FString& ChannelName, const FVector& SpeakerPosition, const FVector& ListenerPosition, const FVector& ListenerForwardDirection, const FVector& ListenerUpDirection)
{
	GetVoiceChatUser().Set3DPosition(ChannelName, SpeakerPosition, ListenerPosition, ListenerForwardDirection, ListenerUpDirection);
}
#endif

TArray<FString> FEOSVoiceChat::GetChannels() const
{
	return GetVoiceChatUser().GetChannels();
}

TArray<FString> FEOSVoiceChat::GetPlayersInChannel(const FString& ChannelName) const
{
	return GetVoiceChatUser().GetPlayersInChannel(ChannelName);
}

EVoiceChatChannelType FEOSVoiceChat::GetChannelType(const FString& ChannelName) const
{
	return GetVoiceChatUser().GetChannelType(ChannelName);
}

FOnVoiceChatPlayerAddedDelegate& FEOSVoiceChat::OnVoiceChatPlayerAdded()
{
	return GetVoiceChatUser().OnVoiceChatPlayerAdded();
}

FOnVoiceChatPlayerRemovedDelegate& FEOSVoiceChat::OnVoiceChatPlayerRemoved()
{
	return GetVoiceChatUser().OnVoiceChatPlayerRemoved();
}

bool FEOSVoiceChat::IsPlayerTalking(const FString& PlayerName) const
{
	return GetVoiceChatUser().IsPlayerTalking(PlayerName);
}

FOnVoiceChatPlayerTalkingUpdatedDelegate& FEOSVoiceChat::OnVoiceChatPlayerTalkingUpdated()
{
	return GetVoiceChatUser().OnVoiceChatPlayerTalkingUpdated();
}

void FEOSVoiceChat::SetPlayerMuted(const FString& PlayerName, bool bMuted)
{
	GetVoiceChatUser().SetPlayerMuted(PlayerName, bMuted);
}

bool FEOSVoiceChat::IsPlayerMuted(const FString& PlayerName) const
{
	return GetVoiceChatUser().IsPlayerMuted(PlayerName);
}

void FEOSVoiceChat::SetChannelPlayerMuted(const FString& ChannelName, const FString& PlayerName, bool bMuted)
{
	GetVoiceChatUser().SetChannelPlayerMuted(ChannelName, PlayerName, bMuted);
}

bool FEOSVoiceChat::IsChannelPlayerMuted(const FString& ChannelName, const FString& PlayerName) const
{
	return GetVoiceChatUser().IsChannelPlayerMuted(ChannelName, PlayerName);
}

FOnVoiceChatPlayerMuteUpdatedDelegate& FEOSVoiceChat::OnVoiceChatPlayerMuteUpdated()
{
	return GetVoiceChatUser().OnVoiceChatPlayerMuteUpdated();
}

void FEOSVoiceChat::SetPlayerVolume(const FString& PlayerName, float Volume)
{
	UE_LOG(LogEOSVoiceChat, Warning, TEXT("FEOSVoiceChat::SetPlayerVolume called"));
	GetVoiceChatUser().SetPlayerVolume(PlayerName, Volume);
}

float FEOSVoiceChat::GetPlayerVolume(const FString& PlayerName) const
{
	return GetVoiceChatUser().GetPlayerVolume(PlayerName);
}

FOnVoiceChatPlayerVolumeUpdatedDelegate& FEOSVoiceChat::OnVoiceChatPlayerVolumeUpdated()
{
	return GetVoiceChatUser().OnVoiceChatPlayerVolumeUpdated();
}

void FEOSVoiceChat::TransmitToAllChannels()
{
	GetVoiceChatUser().TransmitToAllChannels();
}

void FEOSVoiceChat::TransmitToNoChannels()
{
	GetVoiceChatUser().TransmitToNoChannels();
}

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >=3
void FEOSVoiceChat::TransmitToSpecificChannels(const TSet<FString>& ChannelNames)
{
	GetVoiceChatUser().TransmitToSpecificChannels(ChannelNames);
}

TSet<FString> FEOSVoiceChat::GetTransmitChannels() const
{
	return GetVoiceChatUser().GetTransmitChannels();
}

#else

void FEOSVoiceChat::TransmitToSpecificChannel(const FString& ChannelName)
{
	GetVoiceChatUser().TransmitToSpecificChannel(ChannelName);
}

FString FEOSVoiceChat::GetTransmitChannel() const
{
	return GetVoiceChatUser().GetTransmitChannel();
}

#endif

EVoiceChatTransmitMode FEOSVoiceChat::GetTransmitMode() const
{
	return GetVoiceChatUser().GetTransmitMode();
}

FDelegateHandle FEOSVoiceChat::StartRecording(const FOnVoiceChatRecordSamplesAvailableDelegate::FDelegate& Delegate)
{
	return GetVoiceChatUser().StartRecording(Delegate);
}

void FEOSVoiceChat::StopRecording(FDelegateHandle Handle)
{
	GetVoiceChatUser().StopRecording(Handle);
}

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
FDelegateHandle FEOSVoiceChat::RegisterOnVoiceChatAfterCaptureAudioReadDelegate(const FOnVoiceChatAfterCaptureAudioReadDelegate2::FDelegate& Delegate)
{
	return GetVoiceChatUser().RegisterOnVoiceChatAfterCaptureAudioReadDelegate(Delegate);
}

void FEOSVoiceChat::UnregisterOnVoiceChatAfterCaptureAudioReadDelegate(FDelegateHandle Handle)
{
	GetVoiceChatUser().UnregisterOnVoiceChatAfterCaptureAudioReadDelegate(Handle);
}

FDelegateHandle FEOSVoiceChat::RegisterOnVoiceChatBeforeCaptureAudioSentDelegate(const FOnVoiceChatBeforeCaptureAudioSentDelegate2::FDelegate& Delegate)
{
	return GetVoiceChatUser().RegisterOnVoiceChatBeforeCaptureAudioSentDelegate(Delegate);
}

void FEOSVoiceChat::UnregisterOnVoiceChatBeforeCaptureAudioSentDelegate(FDelegateHandle Handle)
{
	GetVoiceChatUser().UnregisterOnVoiceChatBeforeCaptureAudioSentDelegate(Handle);
}
#else
FDelegateHandle FEOSVoiceChat::RegisterOnVoiceChatAfterCaptureAudioReadDelegate(const FOnVoiceChatAfterCaptureAudioReadDelegate::FDelegate& Delegate)
{
	return GetVoiceChatUser().RegisterOnVoiceChatAfterCaptureAudioReadDelegate(Delegate);
}

void FEOSVoiceChat::UnregisterOnVoiceChatAfterCaptureAudioReadDelegate(FDelegateHandle Handle)
{
	GetVoiceChatUser().UnregisterOnVoiceChatAfterCaptureAudioReadDelegate(Handle);
}

FDelegateHandle FEOSVoiceChat::RegisterOnVoiceChatBeforeCaptureAudioSentDelegate(const FOnVoiceChatBeforeCaptureAudioSentDelegate::FDelegate& Delegate)
{
	return GetVoiceChatUser().RegisterOnVoiceChatBeforeCaptureAudioSentDelegate(Delegate);
}

void FEOSVoiceChat::UnregisterOnVoiceChatBeforeCaptureAudioSentDelegate(FDelegateHandle Handle)
{
	GetVoiceChatUser().UnregisterOnVoiceChatBeforeCaptureAudioSentDelegate(Handle);
}
#endif

FDelegateHandle FEOSVoiceChat::RegisterOnVoiceChatBeforeRecvAudioRenderedDelegate(const FOnVoiceChatBeforeRecvAudioRenderedDelegate::FDelegate& Delegate)
{
	return GetVoiceChatUser().RegisterOnVoiceChatBeforeRecvAudioRenderedDelegate(Delegate);
}

void FEOSVoiceChat::UnregisterOnVoiceChatBeforeRecvAudioRenderedDelegate(FDelegateHandle Handle)
{
	GetVoiceChatUser().UnregisterOnVoiceChatBeforeRecvAudioRenderedDelegate(Handle);
}

FDelegateHandle FEOSVoiceChat::RegisterOnVoiceChatDataReceivedDelegate(const FOnVoiceChatDataReceivedDelegate::FDelegate& Delegate)
{
	return GetVoiceChatUser().RegisterOnVoiceChatDataReceivedDelegate(Delegate);
}

void FEOSVoiceChat::UnregisterOnVoiceChatDataReceivedDelegate(FDelegateHandle Handle)
{
	GetVoiceChatUser().UnregisterOnVoiceChatDataReceivedDelegate(Handle);
}

FString FEOSVoiceChat::InsecureGetLoginToken(const FString& PlayerName)
{
	return GetVoiceChatUser().InsecureGetLoginToken(PlayerName);
}

FString FEOSVoiceChat::InsecureGetJoinToken(const FString& ChannelName, EVoiceChatChannelType ChannelType, TOptional<FVoiceChatChannel3dProperties> Channel3dProperties)
{
	return GetVoiceChatUser().InsecureGetJoinToken(ChannelName, ChannelType, Channel3dProperties);
}
#pragma endregion IVoiceChatUser

void FEOSVoiceChat::BindInitCallbacks()
{
	EOS_RTCAudio_AddNotifyAudioDevicesChangedOptions AudioDevicesChangedOptions = {};
	AudioDevicesChangedOptions.ApiVersion = EOS_RTCAUDIO_ADDNOTIFYAUDIODEVICESCHANGED_API_LATEST;
	static_assert(EOS_RTCAUDIO_ADDNOTIFYAUDIODEVICESCHANGED_API_LATEST == 1, "EOS_RTC_AddNotifyAudioDevicesChangedOptions updated, check new fields");
	InitSession.OnAudioDevicesChangedNotificationId = EOS_RTCAudio_AddNotifyAudioDevicesChanged(EOS_RTC_GetAudioInterface(GetRtcInterface()), &AudioDevicesChangedOptions, this, &FEOSVoiceChat::OnAudioDevicesChangedStatic);
	if (InitSession.OnAudioDevicesChangedNotificationId == EOS_INVALID_NOTIFICATIONID)
	{
		UE_LOG(LogEOSVoiceChat, Warning, TEXT("BindInitCallbacks EOS_RTC_AddNotifyAudioDevicesChanged failed"));
	}

	OnAudioDevicesChanged();
}

void FEOSVoiceChat::UnbindInitCallbacks()
{
	if (InitSession.OnAudioDevicesChangedNotificationId != EOS_INVALID_NOTIFICATIONID)
	{
		EOS_RTCAudio_RemoveNotifyAudioDevicesChanged(EOS_RTC_GetAudioInterface(GetRtcInterface()), InitSession.OnAudioDevicesChangedNotificationId);
		InitSession.OnAudioDevicesChangedNotificationId = EOS_INVALID_NOTIFICATIONID;
	}
}

void FEOSVoiceChat::OnAudioDevicesChangedStatic(const EOS_RTCAudio_AudioDevicesChangedCallbackInfo* CallbackInfo)
{
	if (CallbackInfo)
	{
		if (FEOSVoiceChat* EosVoiceChatPtr = static_cast<FEOSVoiceChat*>(CallbackInfo->ClientData))
		{
			EosVoiceChatPtr->OnAudioDevicesChanged();
		}
		else
		{
			UE_LOG(LogEOSVoiceChat, Warning, TEXT("OnAudioDevicesChangedStatic Error EosVoiceChatPtr=nullptr"));
		}
	}
	else
	{
		UE_LOG(LogEOSVoiceChat, Warning, TEXT("OnAudioDevicesChangedStatic Error CallbackInfo=nullptr"));
	}
}

void FEOSVoiceChat::OnAudioDevicesChanged()
{
#if ENGINE_MAJOR_VERSION == 5
	InitSession.EosAudioDevicePool->RefreshAudioDevices(FEOSAudioDevicePool::FOnAudioDevicePoolRefreshAudioDevicesCompleteDelegate::CreateLambda([WeakThis = AsWeak()](const FVoiceChatResult& Result) -> void
#else
	InitSession.EosAudioDevicePool->RefreshAudioDevices(FEOSAudioDevicePool::FOnAudioDevicePoolRefreshAudioDevicesCompleteDelegate::CreateLambda([WeakThis = CreateWeakThis()](const FVoiceChatResult& Result) -> void
#endif
	{
		CHECKPIN();

		if (!Result.IsSuccess())
		{
			UE_LOG(LogEOSVoiceChat, Warning, TEXT("OnAudioDevicesChanged RefreshAudioDevicesCompletionDelegate failed, Result=[%s]"), *LexToString(Result));
		}

		StrongThis->OnVoiceChatAvailableAudioDevicesChangedDelegate.Broadcast();
	}));
}

FEOSVoiceChatUser& FEOSVoiceChat::GetVoiceChatUser()
{
	if (!SingleUserVoiceChatUser)
	{
		SingleUserVoiceChatUser = static_cast<FEOSVoiceChatUser*>(CreateUser());
		ensureMsgf(VoiceChatUsers.Num() == 1, TEXT("When using multiple users, all connections should be managed by an IVoiceChatUser"));
	}

	return *SingleUserVoiceChatUser;
}

FEOSVoiceChatUser& FEOSVoiceChat::GetVoiceChatUser() const
{
	return const_cast<FEOSVoiceChat*>(this)->GetVoiceChatUser();
}

bool FEOSVoiceChat::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
#if !NO_LOGGING
#define EOS_EXEC_LOG(Fmt, ...) Ar.CategorizedLogf(LogEOSVoiceChat.GetCategoryName(), ELogVerbosity::Log, Fmt, ##__VA_ARGS__)
#else
#define EOS_EXEC_LOG(Fmt, ...) 
#endif

	if (FParse::Command(&Cmd, TEXT("EOSVOICECHAT")))
	{
		const TCHAR* SubCmd = Cmd;
		if (FParse::Command(&Cmd, TEXT("LIST")))
		{
			EOS_EXEC_LOG(TEXT("InstanceId=%d Users=%d"), InstanceId, VoiceChatUsers.Num());
			if (VoiceChatUsers.Num() > 0)
			{
				for (int UserIndex = 0; UserIndex < VoiceChatUsers.Num(); ++UserIndex)
				{
					const FEOSVoiceChatUserRef& User = VoiceChatUsers[UserIndex];
					EOS_EXEC_LOG(TEXT("  EOSUser Index:%i PlayerName:%s"), UserIndex, *User->GetLoggedInPlayerName());
				}
			}
			return true;
		}

		int64 InstanceIdParam = 0;
		FParse::Value(Cmd, TEXT("InstanceId="), InstanceIdParam);
		if (InstanceIdParam == InstanceId)
		{
			if (FParse::Command(&Cmd, TEXT("INFO")))
			{
				EOS_EXEC_LOG(TEXT("Initialized: %s"), *LexToString(IsInitialized()));
				if (IsInitialized())
				{
					EOS_EXEC_LOG(TEXT("Connection Status: %s"), LexToString(ConnectionState));

					for (int UserIndex = 0; UserIndex < VoiceChatUsers.Num(); ++UserIndex)
					{
						const FEOSVoiceChatUserRef& User = VoiceChatUsers[UserIndex];
						EOS_EXEC_LOG(TEXT("  User Index:%i PlayerName:%s"), UserIndex, *User->GetLoggedInPlayerName());
						User->Exec(InWorld, SubCmd, Ar);
					}
				}
				return true;
			}
	#if !UE_BUILD_SHIPPING
			else if (FParse::Command(&Cmd, TEXT("INITIALIZE")))
			{
				Initialize(FOnVoiceChatInitializeCompleteDelegate::CreateLambda([](const FVoiceChatResult& Result)
				{
					UE_LOG(LogEOSVoiceChat, Display, TEXT("EOS INITIALIZE success:%s"), *LexToString(Result));
				}));
				return true;
			}
			else if (FParse::Command(&Cmd, TEXT("UNINITIALIZE")))
			{
				Uninitialize(FOnVoiceChatUninitializeCompleteDelegate::CreateLambda([](const FVoiceChatResult& Result)
				{
					UE_LOG(LogEOSVoiceChat, Display, TEXT("EOS UNINITIALIZE success:%s"), *LexToString(Result));
				}));
				return true;
			}
			else if (FParse::Command(&Cmd, TEXT("CONNECT")))
			{
				Connect(FOnVoiceChatConnectCompleteDelegate::CreateLambda([](const FVoiceChatResult& Result)
				{
					UE_LOG(LogEOSVoiceChat, Display, TEXT("EOS CONNECT result:%s"), *LexToString(Result));
				}));
				return true;
			}
			else if (FParse::Command(&Cmd, TEXT("DISCONNECT")))
			{
				Disconnect(FOnVoiceChatDisconnectCompleteDelegate::CreateLambda([](const FVoiceChatResult& Result)
				{
					UE_LOG(LogEOSVoiceChat, Display, TEXT("EOS DISCONNECT result:%s"), *LexToString(Result));
				}));
				return true;
			}
			else if (FParse::Command(&Cmd, TEXT("CREATEUSER")))
			{
				if (!SingleUserVoiceChatUser)
				{
					UsersCreatedByConsoleCommand.Add(CreateUser());
					EOS_EXEC_LOG(TEXT("EOS CREATEUSER success"));
					return true;
				}
				else
				{
					EOS_EXEC_LOG(TEXT("EOS CREATEUSER failed, single user set."));
					return true;
				}
			}
			else if (FParse::Command(&Cmd, TEXT("CREATESINGLEUSER")))
			{
				if (SingleUserVoiceChatUser)
				{
					EOS_EXEC_LOG(TEXT("EOS CREATESINGLEUSER already exists"));
					return true;
				}
				else if (VoiceChatUsers.Num() == 0)
				{
					GetVoiceChatUser();
					EOS_EXEC_LOG(TEXT("EOS CREATESINGLEUSER success"));
					return true;
				}
				else
				{
					EOS_EXEC_LOG(TEXT("EOS CREATESINGLEUSER failed, VoiceChatUsers not empty."));
					return true;
				}
			}
			else
			{	
				int UserIndex = 0;
				if (FParse::Value(Cmd, TEXT("UserIndex="), UserIndex))
				{
					if (UserIndex < VoiceChatUsers.Num())
					{
						const FEOSVoiceChatUserRef& UserRef = VoiceChatUsers[UserIndex];
						if (FParse::Command(&Cmd, TEXT("RELEASEUSER")))
						{
							IVoiceChatUser* User = &UserRef.Get();
							if (UsersCreatedByConsoleCommand.RemoveSwap(User))
							{
								EOS_EXEC_LOG(TEXT("EOS RELEASEUSER releasing UserIndex=%d..."), UserIndex);
								ReleaseUser(User);
							}
							else
							{
								EOS_EXEC_LOG(TEXT("EOS RELEASEUSER UserIndex=%d not created by CREATEUSER call."), UserIndex);
							}
							return true;
						}
						else
						{
							return UserRef->Exec(InWorld, Cmd, Ar);
						}
					}
					else
					{
						EOS_EXEC_LOG(TEXT("EOS RELEASEUSER UserIndex=%d not found, VoiceChatUsers.Num=%d"), UserIndex, VoiceChatUsers.Num());
						return true;
					}
				}
				else if (SingleUserVoiceChatUser)
				{
					return SingleUserVoiceChatUser->Exec(InWorld, SubCmd, Ar);
				}
				else
				{
					EOS_EXEC_LOG(TEXT("EOS User index not specified, and no single user created. Either CREATEUSER and specify UserIndex=n in subsequent commands, or CREATESINGLEUSER (no UserIndex=n necessary in subsequent commands)"));
					return true;
				}
			}
#endif // !UE_BUILD_SHIPPING
		}
	}

#undef EOS_EXEC_LOG

	return false;
}

FEOSVoiceChat::FInitSession::FInitSession()
	: EosAudioDevicePool{ MakeShared<FEOSAudioDevicePool>(EosRtcInterface) }
{
}

void FEOSVoiceChat::FInitSession::Reset()
{
	State = EInitializationState::Uninitialized;

	UninitializeCompleteDelegates = TArray<FOnVoiceChatUninitializeCompleteDelegate>{};

	EosRtcInterface = nullptr;
	EosLobbyInterface = nullptr;

	OnAudioDevicesChangedNotificationId = EOS_INVALID_NOTIFICATIONID;

	EosAudioDevicePool = MakeShared<FEOSAudioDevicePool>(EosRtcInterface);
}

IEIKPlatformHandlePtr FEOSVoiceChat::EOSPlatformCreate(EOS_Platform_Options& PlatformOptions)
{
	return SDKManager.CreatePlatform(PlatformOptions);
}

const TCHAR* LexToString(FEOSVoiceChat::EConnectionState State)
{
	switch (State)
	{
	case FEOSVoiceChat::EConnectionState::Disconnected:		return TEXT("Disconnected");
	case FEOSVoiceChat::EConnectionState::Disconnecting:	return TEXT("Disconnecting");
	case FEOSVoiceChat::EConnectionState::Connecting:		return TEXT("Connecting");
	case FEOSVoiceChat::EConnectionState::Connected:		return TEXT("Connected");
	default:												return TEXT("Unknown");
	}
}

#undef CHECKPIN

#endif // WITH_EOS_RTC