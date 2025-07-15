// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EOSShared.h"

#if WITH_EOS_RTC

#include "Misc/CoreMisc.h"
#include "VoiceChat.h"
#include "Runtime/Launch/Resources/Version.h"
#include "eos_types.h"
#include "eos_rtc_types.h"
#include "eos_rtc_audio_types.h"
#include "eos_lobby_types.h"

EIKVOICECHAT_API const TCHAR* EIK_LexToString(EOS_ERTCAudioInputStatus Status);

struct FVoiceChatMetadataItem
{
	FString Key;
	FString Value;
};

class EIKVOICECHAT_API FEIKVoiceChatDelegates
{
public:
	/** Delegate called when the status of the audio device has changed. */
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAudioInputDeviceStatusChanged, FString /*PlayerName*/, EOS_ERTCAudioInputStatus /*Status*/);
	static FOnAudioInputDeviceStatusChanged OnAudioInputDeviceStatusChanged;
	/** Delegate called when the "connection state" changes (happens when a sidekick device toggles it's "mode" on/off) **/
	DECLARE_MULTICAST_DELEGATE_FourParams(FOnVoiceChatChannelConnectionStateDelegate, const FString& /*PlayerName*/, const FString& /* ChannelName */, bool /* bIsChannelEnabled */, bool /* bSendAudioEnabled */);
	static FOnVoiceChatChannelConnectionStateDelegate OnVoiceChatChannelConnectionStateChanged;
	/** Delegate called when ParticipantMetadata is included in the player joined event **/
	DECLARE_MULTICAST_DELEGATE_FourParams(FOnVoiceChatPlayerAddedMetadataDelegate, const FString& /*LoginPlayerName*/, const FString& /* ChannelName */, const FString& /*PlayerName*/, const TArray<FVoiceChatMetadataItem>& /* PlayerMetadata */);
	static FOnVoiceChatPlayerAddedMetadataDelegate OnVoiceChatPlayerAddedMetadata;
	/** Delegate called when the audio status has changed **/
	DECLARE_MULTICAST_DELEGATE_FourParams(FOnAudioStatusChanged, const FString& /*LoginPlayerName*/, const FString& /* ChannelName */, const FString& /*PlayerName*/, const EOS_ERTCAudioStatus /*Status*/);
	static FOnAudioStatusChanged OnAudioStatusChanged;
};

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnVoiceChatDataReceivedDelegate, const FString& /* ChannelName */, const FString& /* PlayerName */, TArrayView<const uint8> /* Buffer */);

class FEOSAudioDevicePool;
class FEOSVoiceChatUser;
class IEOSSDKManager;
using IEIKPlatformHandlePtr = TSharedPtr<class IEIKPlatformHandle, ESPMode::ThreadSafe>;

typedef TSharedPtr<class FEOSVoiceChat, ESPMode::ThreadSafe> FEOSVoiceChatPtr;
typedef TSharedPtr<class FEOSVoiceChatUser, ESPMode::ThreadSafe> FEOSVoiceChatUserPtr;
typedef TSharedRef<class FEOSVoiceChatUser, ESPMode::ThreadSafe> FEOSVoiceChatUserRef;
#if ENGINE_MAJOR_VERSION != 5
typedef TWeakPtr<class FEOSVoiceChat, ESPMode::ThreadSafe> FEOSVoiceChatWeakPtr;
typedef TWeakPtr<class FEOSVoiceChatUser, ESPMode::ThreadSafe> FEOSVoiceChatUserWeakPtr;
#endif

class FEOSVoiceChat : public TSharedFromThis<FEOSVoiceChat, ESPMode::ThreadSafe>, public IVoiceChat
{
public:
	FEOSVoiceChat(IEOSSDKManager& InSDKManager, const IEIKPlatformHandlePtr& PlatformHandle);

	virtual ~FEOSVoiceChat();

	// ~Begin IVoiceChat Interface
	virtual bool Initialize() override;
	virtual void Initialize(const FOnVoiceChatInitializeCompleteDelegate& Delegate) override;
	virtual bool Uninitialize() override;
	virtual void Uninitialize(const FOnVoiceChatUninitializeCompleteDelegate& Delegate) override;
	virtual bool IsInitialized() const override;
	virtual void Connect(const FOnVoiceChatConnectCompleteDelegate& Delegate) override;
	virtual void Disconnect(const FOnVoiceChatDisconnectCompleteDelegate& Delegate) override;
	virtual bool IsConnecting() const override;
	virtual bool IsConnected() const override;
	virtual FOnVoiceChatConnectedDelegate& OnVoiceChatConnected() override { return OnVoiceChatConnectedDelegate; }
	virtual FOnVoiceChatDisconnectedDelegate& OnVoiceChatDisconnected() override { return OnVoiceChatDisconnectedDelegate; }
	virtual FOnVoiceChatReconnectedDelegate& OnVoiceChatReconnected() override { return OnVoiceChatReconnectedDelegate; }
	virtual IVoiceChatUser* CreateUser() override;
	virtual void ReleaseUser(IVoiceChatUser* VoiceChatUser) override;
	// ~End IVoiceChat Interface
#if ENGINE_MAJOR_VERSION != 5
	FEOSVoiceChatWeakPtr CreateWeakThis();
#endif
	// ~Begin IVoiceChatUser Interface
	virtual void SetSetting(const FString& Name, const FString& Value) override;
	virtual FString GetSetting(const FString& Name) override;
	virtual void SetAudioInputVolume(float Volume) override;
	virtual void SetAudioOutputVolume(float Volume) override;
	virtual float GetAudioInputVolume() const override;
	virtual float GetAudioOutputVolume() const override;
	virtual void SetAudioInputDeviceMuted(bool bIsMuted) override;
	virtual void SetAudioOutputDeviceMuted(bool bIsMuted) override;
	virtual bool GetAudioInputDeviceMuted() const override;
	virtual bool GetAudioOutputDeviceMuted() const override;
	virtual TArray<FVoiceChatDeviceInfo> GetAvailableInputDeviceInfos() const override;
	virtual TArray<FVoiceChatDeviceInfo> GetAvailableOutputDeviceInfos() const override;
	virtual FOnVoiceChatAvailableAudioDevicesChangedDelegate& OnVoiceChatAvailableAudioDevicesChanged() override;
	virtual void SetInputDeviceId(const FString& InputDeviceId) override;
	virtual void SetOutputDeviceId(const FString& OutputDeviceId) override;
	virtual FVoiceChatDeviceInfo GetInputDeviceInfo() const override;
	virtual FVoiceChatDeviceInfo GetOutputDeviceInfo() const override;
	virtual FVoiceChatDeviceInfo GetDefaultInputDeviceInfo() const override;
	virtual FVoiceChatDeviceInfo GetDefaultOutputDeviceInfo() const override;
	virtual void Login(FPlatformUserId PlatformId, const FString& PlayerName, const FString& Credentials, const FOnVoiceChatLoginCompleteDelegate& Delegate) override;
	virtual void Logout(const FOnVoiceChatLogoutCompleteDelegate& Delegate) override;
	virtual bool IsLoggingIn() const override;
	virtual bool IsLoggedIn() const override;
	virtual FOnVoiceChatLoggedInDelegate& OnVoiceChatLoggedIn() override;
	virtual FOnVoiceChatLoggedOutDelegate& OnVoiceChatLoggedOut() override;
	virtual FString GetLoggedInPlayerName() const override;
	virtual void BlockPlayers(const TArray<FString>& PlayerNames) override;
	virtual void UnblockPlayers(const TArray<FString>& PlayerNames) override;
	virtual void JoinChannel(const FString& ChannelName, const FString& ChannelCredentials, EVoiceChatChannelType ChannelType, const FOnVoiceChatChannelJoinCompleteDelegate& Delegate, TOptional<FVoiceChatChannel3dProperties> Channel3dProperties = TOptional<FVoiceChatChannel3dProperties>()) override;
	virtual void LeaveChannel(const FString& Channel, const FOnVoiceChatChannelLeaveCompleteDelegate& Delegate) override;
	virtual FOnVoiceChatChannelJoinedDelegate& OnVoiceChatChannelJoined() override;
	virtual FOnVoiceChatChannelExitedDelegate& OnVoiceChatChannelExited() override;
	virtual FOnVoiceChatCallStatsUpdatedDelegate& OnVoiceChatCallStatsUpdated() override;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	virtual void Set3DPosition(const FString& ChannelName, const FVector& Position) override;
#else
	virtual void Set3DPosition(const FString& ChannelName, const FVector& SpeakerPosition, const FVector& ListenerPosition, const FVector& ListenerForwardDirection, const FVector& ListenerUpDirection) override;
#endif
	virtual TArray<FString> GetChannels() const override;
	virtual TArray<FString> GetPlayersInChannel(const FString& ChannelName) const override;
	virtual EVoiceChatChannelType GetChannelType(const FString& ChannelName) const override;
	virtual FOnVoiceChatPlayerAddedDelegate& OnVoiceChatPlayerAdded() override;
	virtual FOnVoiceChatPlayerRemovedDelegate& OnVoiceChatPlayerRemoved() override;
	virtual bool IsPlayerTalking(const FString& PlayerName) const override;
	virtual FOnVoiceChatPlayerTalkingUpdatedDelegate& OnVoiceChatPlayerTalkingUpdated() override;
	virtual void SetPlayerMuted(const FString& PlayerName, bool bMuted) override;
	virtual bool IsPlayerMuted(const FString& PlayerName) const override;
#if ENGINE_MAJOR_VERSION == 5
	virtual void SetChannelPlayerMuted(const FString& ChannelName, const FString& PlayerName, bool bMuted) override;
	virtual bool IsChannelPlayerMuted(const FString& ChannelName, const FString& PlayerName) const override;
#else
	virtual void SetChannelPlayerMuted(const FString& ChannelName, const FString& PlayerName, bool bMuted);
	virtual bool IsChannelPlayerMuted(const FString& ChannelName, const FString& PlayerName) const;
#endif
	virtual FOnVoiceChatPlayerMuteUpdatedDelegate& OnVoiceChatPlayerMuteUpdated() override;
	virtual void SetPlayerVolume(const FString& PlayerName, float Volume) override;
	virtual float GetPlayerVolume(const FString& PlayerName) const override;
	virtual FOnVoiceChatPlayerVolumeUpdatedDelegate& OnVoiceChatPlayerVolumeUpdated() override;
	virtual void TransmitToAllChannels() override;
	virtual void TransmitToNoChannels() override;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >=3
	virtual void TransmitToSpecificChannels(const TSet<FString>& ChannelNames) override;
	virtual TSet<FString> GetTransmitChannels() const override;
#else
	virtual void TransmitToSpecificChannel(const FString& ChannelName) override;
	virtual FString GetTransmitChannel() const override;
#endif
	virtual EVoiceChatTransmitMode GetTransmitMode() const override;
	virtual FDelegateHandle StartRecording(const FOnVoiceChatRecordSamplesAvailableDelegate::FDelegate& Delegate) override;
	virtual void StopRecording(FDelegateHandle Handle) override;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	virtual FDelegateHandle RegisterOnVoiceChatAfterCaptureAudioReadDelegate(const FOnVoiceChatAfterCaptureAudioReadDelegate2::FDelegate& Delegate) override;
	virtual void UnregisterOnVoiceChatAfterCaptureAudioReadDelegate(FDelegateHandle Handle) override;
	virtual FDelegateHandle RegisterOnVoiceChatBeforeCaptureAudioSentDelegate(const FOnVoiceChatBeforeCaptureAudioSentDelegate2::FDelegate& Delegate) override;
	virtual void UnregisterOnVoiceChatBeforeCaptureAudioSentDelegate(FDelegateHandle Handle) override;
#else
	virtual FDelegateHandle RegisterOnVoiceChatAfterCaptureAudioReadDelegate(const FOnVoiceChatAfterCaptureAudioReadDelegate::FDelegate& Delegate) override;
	virtual void UnregisterOnVoiceChatAfterCaptureAudioReadDelegate(FDelegateHandle Handle) override;
	virtual FDelegateHandle RegisterOnVoiceChatBeforeCaptureAudioSentDelegate(const FOnVoiceChatBeforeCaptureAudioSentDelegate::FDelegate& Delegate) override;
	virtual void UnregisterOnVoiceChatBeforeCaptureAudioSentDelegate(FDelegateHandle Handle) override;
#endif
	virtual FDelegateHandle RegisterOnVoiceChatBeforeRecvAudioRenderedDelegate(const FOnVoiceChatBeforeRecvAudioRenderedDelegate::FDelegate& Delegate) override;
	virtual void UnregisterOnVoiceChatBeforeRecvAudioRenderedDelegate(FDelegateHandle Handle) override;
	virtual FDelegateHandle RegisterOnVoiceChatDataReceivedDelegate(const FOnVoiceChatDataReceivedDelegate::FDelegate& Delegate);
	virtual void UnregisterOnVoiceChatDataReceivedDelegate(FDelegateHandle Handle);
	virtual FString InsecureGetLoginToken(const FString& PlayerName) override;
	virtual FString InsecureGetJoinToken(const FString& ChannelName, EVoiceChatChannelType ChannelType, TOptional<FVoiceChatChannel3dProperties> Channel3dProperties = TOptional<FVoiceChatChannel3dProperties>()) override;
	// ~End IVoiceChatUser Interface

	IEIKPlatformHandlePtr GetPlatformHandle() const { return EosPlatformHandle; }
	EOS_HRTC GetRtcInterface() const { return InitSession.EosRtcInterface; }
	EOS_HLobby GetLobbyInterface() const { return InitSession.EosLobbyInterface; }

protected:
	virtual void PostInitialize() {};
	virtual void PreUninitialize() {};

	friend class FEOSVoiceChatUser;
	friend class FEOSVoiceChatFactory;

	// When initialized, contains the state for this initialize session. Reset during Uninitialize.
	enum class EInitializationState
	{
		Uninitialized,
		Uninitializing,
		Initializing,
		Initialized
	};

	struct FInitSession
	{
		FInitSession();

		// `FEOSAudioDevicePool` holds a ref to an object field itself which can't be copied or moved
		UE_NONCOPYABLE(FInitSession)

		// reverts fields to their default state
		void Reset();

		EInitializationState State = EInitializationState::Uninitialized;

		TArray<FOnVoiceChatUninitializeCompleteDelegate> UninitializeCompleteDelegates;

		EOS_HRTC EosRtcInterface = nullptr;
		EOS_HLobby EosLobbyInterface = nullptr;

		EOS_NotificationId OnAudioDevicesChangedNotificationId = EOS_INVALID_NOTIFICATIONID;

		TSharedRef<FEOSAudioDevicePool> EosAudioDevicePool;
	};
	FInitSession InitSession;

	void BindInitCallbacks();
	void UnbindInitCallbacks();
	static void EOS_CALL OnAudioDevicesChangedStatic(const EOS_RTCAudio_AudioDevicesChangedCallbackInfo* CallbackInfo);
	void OnAudioDevicesChanged();

	// The current state of the connection.
	enum class EConnectionState
	{
		Disconnected,
		Disconnecting,
		Connecting,
		Connected
	};
	EConnectionState ConnectionState = EConnectionState::Disconnected;

	// IVoiceChat Delegates
	FOnVoiceChatConnectedDelegate OnVoiceChatConnectedDelegate;
	FOnVoiceChatDisconnectedDelegate OnVoiceChatDisconnectedDelegate;
	FOnVoiceChatReconnectedDelegate OnVoiceChatReconnectedDelegate;
	// IVoiceChatUser Delegates
	FOnVoiceChatAvailableAudioDevicesChangedDelegate OnVoiceChatAvailableAudioDevicesChangedDelegate;

	TArray<FEOSVoiceChatUserRef> VoiceChatUsers;

	FEOSVoiceChatUser& GetVoiceChatUser();
	FEOSVoiceChatUser& GetVoiceChatUser() const;
	FEOSVoiceChatUser* SingleUserVoiceChatUser = nullptr;

	// ~Begin FSelfRegisteringExec Interface
	bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar);
	// ~End FSelfRegisteringExec Interface
	TArray<IVoiceChatUser*> UsersCreatedByConsoleCommand;

	IEOSSDKManager& SDKManager;
	IEIKPlatformHandlePtr EosPlatformHandle = nullptr;
	virtual IEIKPlatformHandlePtr EOSPlatformCreate(EOS_Platform_Options& PlatformOptions);

	static int64 StaticInstanceIdCount;
	int64 InstanceId = StaticInstanceIdCount++;

	friend const TCHAR* LexToString(FEOSVoiceChat::EConnectionState State);
};

#endif // WITH_EOS_RTC