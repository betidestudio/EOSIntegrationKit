// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EOSVoiceChat.h"
#include "EOSVoiceChatLog.h"
#include "Runtime/Launch/Resources/Version.h"

#if WITH_EOS_RTC

#include "eos_rtc_types.h"
#include "eos_rtc_audio_types.h"
#include "eos_types.h"

class EIKVOICECHAT_API FEOSVoiceChatUser : public TSharedFromThis<FEOSVoiceChatUser, ESPMode::ThreadSafe>, public IVoiceChatUser
{
public:
	FEOSVoiceChatUser(FEOSVoiceChat& InEOSVoiceChat);
	virtual ~FEOSVoiceChatUser();

	FEOSVoiceChatUser(const FEOSVoiceChatUser&) = delete;

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
	virtual FOnVoiceChatAvailableAudioDevicesChangedDelegate& OnVoiceChatAvailableAudioDevicesChanged() override { return EOSVoiceChat.OnVoiceChatAvailableAudioDevicesChangedDelegate; }
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
	virtual FOnVoiceChatLoggedInDelegate& OnVoiceChatLoggedIn() override { return OnVoiceChatLoggedInDelegate; }
	virtual FOnVoiceChatLoggedOutDelegate& OnVoiceChatLoggedOut() override { return OnVoiceChatLoggedOutDelegate; }
	virtual FString GetLoggedInPlayerName() const override;
	virtual void BlockPlayers(const TArray<FString>& PlayerNames) override;
	virtual void UnblockPlayers(const TArray<FString>& PlayerNames) override;
	virtual void JoinChannel(const FString& ChannelName, const FString& ChannelCredentials, EVoiceChatChannelType ChannelType, const FOnVoiceChatChannelJoinCompleteDelegate& Delegate, TOptional<FVoiceChatChannel3dProperties> Channel3dProperties = TOptional<FVoiceChatChannel3dProperties>()) override;
	virtual void LeaveChannel(const FString& Channel, const FOnVoiceChatChannelLeaveCompleteDelegate& Delegate) override;
	virtual FOnVoiceChatChannelJoinedDelegate& OnVoiceChatChannelJoined() override { return OnVoiceChatChannelJoinedDelegate; }
	virtual FOnVoiceChatChannelExitedDelegate& OnVoiceChatChannelExited() override { return OnVoiceChatChannelExitedDelegate; }
	virtual FOnVoiceChatCallStatsUpdatedDelegate& OnVoiceChatCallStatsUpdated() override { return OnVoiceChatCallStatsUpdatedDelegate; }
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	virtual void Set3DPosition(const FString& ChannelName, const FVector& Position) override;
#else
	virtual void Set3DPosition(const FString& ChannelName, const FVector& SpeakerPosition, const FVector& ListenerPosition, const FVector& ListenerForwardDirection, const FVector& ListenerUpDirection) override;
#endif
	virtual TArray<FString> GetChannels() const override;
	virtual TArray<FString> GetPlayersInChannel(const FString& ChannelName) const override;
	virtual EVoiceChatChannelType GetChannelType(const FString& ChannelName) const override;
	virtual FOnVoiceChatPlayerAddedDelegate& OnVoiceChatPlayerAdded() override { return OnVoiceChatPlayerAddedDelegate; }
	virtual FOnVoiceChatPlayerRemovedDelegate& OnVoiceChatPlayerRemoved() override { return OnVoiceChatPlayerRemovedDelegate; }
	virtual bool IsPlayerTalking(const FString& PlayerName) const override;
	virtual FOnVoiceChatPlayerTalkingUpdatedDelegate& OnVoiceChatPlayerTalkingUpdated() override { return OnVoiceChatPlayerTalkingUpdatedDelegate; }
	virtual void SetPlayerMuted(const FString& PlayerName, bool bAudioMuted) override;
	virtual bool IsPlayerMuted(const FString& PlayerName) const override;
#if ENGINE_MAJOR_VERSION == 5
	virtual void SetChannelPlayerMuted(const FString& ChannelName, const FString& PlayerName, bool bAudioMuted) override;
	virtual bool IsChannelPlayerMuted(const FString& ChannelName, const FString& PlayerName) const override;
#else
	virtual void SetChannelPlayerMuted(const FString& ChannelName, const FString& PlayerName, bool bAudioMuted);
	virtual bool IsChannelPlayerMuted(const FString& ChannelName, const FString& PlayerName) const;
#endif
	virtual FOnVoiceChatPlayerMuteUpdatedDelegate& OnVoiceChatPlayerMuteUpdated() override { return OnVoiceChatPlayerMuteUpdatedDelegate; }
	virtual void SetPlayerVolume(const FString& PlayerName, float Volume) override;
	virtual float GetPlayerVolume(const FString& PlayerName) const override;
	virtual FOnVoiceChatPlayerVolumeUpdatedDelegate& OnVoiceChatPlayerVolumeUpdated() override { return OnVoiceChatPlayerVolumeUpdatedDelegate; }
	virtual void TransmitToAllChannels() override;
	virtual void TransmitToNoChannels() override;
	
	virtual EVoiceChatTransmitMode GetTransmitMode() const override;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >=3
	virtual void TransmitToSpecificChannels(const TSet<FString>& ChannelNames) override;
	virtual TSet<FString> GetTransmitChannels() const override;
#else
	virtual void TransmitToSpecificChannel(const FString& ChannelName) override;
	virtual FString GetTransmitChannel() const override;
#endif
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

	bool AddLobbyRoom(const FString& LobbyId);
	bool RemoveLobbyRoom(const FString& LobbyId);

protected:
	friend class FEOSVoiceChat;

	// Global state for a given user.
	struct FGlobalParticipant
	{
		// The player id
		FString PlayerName;
		// Current talking state (we cache this on the global participant in addition to the channel one)
		bool bTalking = false;
		// Desired block state
		bool bBlocked = false;
		// Desired mute state
		bool bAudioMuted = false;
		// Desired Volume
		float Volume = 1.f;
	};

	/**
	 * TODO some of these "current states" e.g. bAudioMuted don't work correctly / the same way they do in VivoxVoiceChat right now,
	 * they don't reflect the current state on the API side as they do in VivoxVoiceChat (i.e. updated by API callbacks),
	 * they are set to whatever the desired state is just before we call the API to update them. Need to move to setting them
	 * when we receive the success callback.
	 */ 
	// Current state of a user in a channel
	struct FChannelParticipant
	{
		// The player id
		FString PlayerName;
		// Current talking state
		bool bTalking = false;
		// Combined audio mute and isListening state
		bool bAudioDisabled = false;
		// Desired channel mute state
		bool bMutedInChannel = false;
		// Current audio status
		TOptional<EOS_ERTCAudioStatus> AudioStatus;
	};

	enum class EChannelJoinState
	{
		NotJoined,
		Leaving,
		Joining,
		Joined
	};

	// Sending options passed to/received from UpdateSending and its completion callback
	struct FSendingState
	{
		// Microphone input
		bool bAudioEnabled = true;
	};

	// Representation of a particular channel
	struct FChannelSession
	{
		FChannelSession() = default;
		FChannelSession(FChannelSession&&) = default;
		~FChannelSession();

		FChannelSession(const FChannelSession&) = delete;

		bool IsLocalUser(const FChannelParticipant& Participant);
		bool IsLobbySession() const;
		
		// The channel name
		FString ChannelName;
		// The channel type
		EVoiceChatChannelType ChannelType = EVoiceChatChannelType::NonPositional;
		// Current join state of the channel
		EChannelJoinState JoinState = EChannelJoinState::NotJoined;
		// Name of the local player in the channel (can differ from LoginSession.PlayerName if an override ID was provided to JoinChannel)
		FString PlayerName;
		// Current participants in the channel, and the current blocked/muted state
		TMap<FString, FChannelParticipant> Participants;
		// Is the connection enabled
		bool bConnectionEnabled = false;
		// Is this connection allowed to send audio? Will be false if houseparty is being used as a mic
		bool bSendAudioEnabled = true;
		// Did the user toggle "off" this channel
		bool bIsNotListening = false;

		// Lobby Id, only relevant for lobby rooms
		FString LobbyId;
		// Lobby channel connection state, only relevant for lobby rooms
		bool bLobbyChannelConnected = false;

		// Desired sending state
		FSendingState DesiredSendingState;
		// Active sending state
		FSendingState ActiveSendingState;

		// Set by JoinChannel and fired on success/failure
		FOnVoiceChatChannelJoinCompleteDelegate JoinDelegate;
		// Set by LeaveChannel and fired on success/failure
		TArray<FOnVoiceChatChannelLeaveCompleteDelegate> LeaveDelegates;

		// Handles for channel callbacks
		EOS_NotificationId OnChannelDisconnectedNotificationId = EOS_INVALID_NOTIFICATIONID;
		EOS_NotificationId OnParticipantStatusChangedNotificationId = EOS_INVALID_NOTIFICATIONID;
		EOS_NotificationId OnParticipantAudioUpdatedNotificationId = EOS_INVALID_NOTIFICATIONID;
		EOS_NotificationId OnAudioBeforeSendNotificationId = EOS_INVALID_NOTIFICATIONID;
		EOS_NotificationId OnAudioBeforeRenderNotificationId = EOS_INVALID_NOTIFICATIONID;
		EOS_NotificationId OnAudioInputStateNotificationId = EOS_INVALID_NOTIFICATIONID;

		TUniquePtr<class FCallbackBase> AudioBeforeSendCallback;
	};

	enum class ELoginState
	{
		LoggedOut,
		LoggingOut,
		LoggingIn,
		LoggedIn
	};

	// When logged in, contains the state for the current login session. Reset by Logout
	struct FLoginSession
	{
		FLoginSession() = default;
		FLoginSession& operator=(FLoginSession&&) = default;
		~FLoginSession();

		FLoginSession(const FLoginSession&) = delete;

		// The numeric platform id for the local user
		FPlatformUserId PlatformId = PLATFORMUSERID_NONE;
		// Name of the local player
		FString PlayerName;
		// EpicAccountId of the local player, converted from PlayerName
		EOS_ProductUserId LocalUserProductUserId;
		// Current login state
		ELoginState State = ELoginState::LoggedOut;
		// Set of channels the user is interacting with
		TMap<FString, FChannelSession> ChannelSessions;
		// Contains participants from all channels and the desired blocked/muted state
		TMap<FString, FGlobalParticipant> Participants;

		// Maps LobbyId to RTC ChannelName, only relevant when VoiceChatFlowMode == Lobby
		TMap<FString, FString> LobbyIdToChannelName;

		// State while handling a logout request
		struct FLogoutState
		{
			FVoiceChatResult Result = FVoiceChatResult::CreateSuccess();
			TSet<FString> ChannelNamesExpectingCallback;
			// Delegates to fire when Logout completes
			TArray<FOnVoiceChatLogoutCompleteDelegate> CompletionDelegates;
		};
		TOptional<FLogoutState> LogoutState;

		// Handles for callbacks
		EOS_NotificationId OnLobbyChannelConnectionChangedNotificationId = EOS_INVALID_NOTIFICATIONID;
	};
	FLoginSession LoginSession;

	// Audio Output state
	struct FAudioOutputOptions
	{
		bool bMuted = false;
		float Volume = 1.0f;
		TOptional<FVoiceChatDeviceInfo> SpecificDeviceInfo;
	};
	FAudioOutputOptions AudioOutputOptions;

	// Audio Input state
	struct FAudioInputOptions
	{
		bool bMuted = false;
		bool bPlatformAEC = false;
		float Volume = 1.0f;
		TOptional<FVoiceChatDeviceInfo> SpecificDeviceInfo;
	};
	FAudioInputOptions AudioInputOptions;

	struct FTransmitState
	{
		EVoiceChatTransmitMode Mode = EVoiceChatTransmitMode::All;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >=3
		TSet<FString> SpecificChannels;
#else
		FString ChannelName;
#endif
	};
	FTransmitState TransmitState;

	// IVoiceChatUser Delegates
	FOnVoiceChatLoggedInDelegate OnVoiceChatLoggedInDelegate;
	FOnVoiceChatLoggedOutDelegate OnVoiceChatLoggedOutDelegate;
	FOnVoiceChatChannelJoinedDelegate OnVoiceChatChannelJoinedDelegate;
	FOnVoiceChatChannelExitedDelegate OnVoiceChatChannelExitedDelegate;
	FOnVoiceChatPlayerAddedDelegate OnVoiceChatPlayerAddedDelegate;
	FOnVoiceChatPlayerTalkingUpdatedDelegate OnVoiceChatPlayerTalkingUpdatedDelegate;
	FOnVoiceChatPlayerMuteUpdatedDelegate OnVoiceChatPlayerMuteUpdatedDelegate;
	FOnVoiceChatPlayerVolumeUpdatedDelegate OnVoiceChatPlayerVolumeUpdatedDelegate;
	FOnVoiceChatPlayerRemovedDelegate OnVoiceChatPlayerRemovedDelegate;
	FOnVoiceChatCallStatsUpdatedDelegate OnVoiceChatCallStatsUpdatedDelegate;

	// IVoiceChatUser Recording Delegates
	FCriticalSection AudioRecordLock;
	FOnVoiceChatRecordSamplesAvailableDelegate OnVoiceChatRecordSamplesAvailableDelegate;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 6
	FOnVoiceChatAfterCaptureAudioReadDelegate2 OnVoiceChatAfterCaptureAudioReadDelegate;
	FCriticalSection BeforeCaptureAudioSentLock;
	FOnVoiceChatBeforeCaptureAudioSentDelegate2 OnVoiceChatBeforeCaptureAudioSentDelegate;
#else
	FOnVoiceChatAfterCaptureAudioReadDelegate OnVoiceChatAfterCaptureAudioReadDelegate;
	FCriticalSection BeforeCaptureAudioSentLock;
	FOnVoiceChatBeforeCaptureAudioSentDelegate OnVoiceChatBeforeCaptureAudioSentDelegate;
#endif
	FCriticalSection BeforeRecvAudioRenderedLock;
	FOnVoiceChatBeforeRecvAudioRenderedDelegate OnVoiceChatBeforeRecvAudioRenderedDelegate;
	FCriticalSection DataReceivedLock;
	FOnVoiceChatDataReceivedDelegate OnVoiceChatDataReceivedDelegate;

	FEOSVoiceChat& EOSVoiceChat;

	bool bFakeAudioInput = true;
	bool bInDestructor = false;

	// Helper methods
	bool IsInitialized();
	bool IsConnected();
	EOS_HRTC GetRtcInterface() const { return EOSVoiceChat.GetRtcInterface(); }
	EOS_HLobby GetLobbyInterface() const { return EOSVoiceChat.GetLobbyInterface(); }
	FGlobalParticipant& GetGlobalParticipant(const FString& PlayerName);
	const FGlobalParticipant& GetGlobalParticipant(const FString& PlayerName) const;
	FChannelSession& GetChannelSession(const FString& ChannelName);
	const FChannelSession& GetChannelSession(const FString& ChannelName) const;
	void RemoveChannelSession(const FString& ChannelName);
	void ApplyAudioInputOptions();
	void ApplyAudioOutputOptions();
	void ApplyPlayerBlock(const FGlobalParticipant& GlobalParticipant, const FChannelSession& ChannelSession, FChannelParticipant& ChannelParticipant);
	void ApplyReceivingOptions(const FChannelSession& ChannelSession);
	void ApplyPlayerReceivingOptions(const FString& PlayerName);
	void ApplyPlayerReceivingOptions(const FGlobalParticipant& GlobalParticipant, const FChannelSession& ChannelSession, FChannelParticipant& ChannelParticipant);
	void ApplySendingOptions();
	void ApplySendingOptions(FChannelSession& ChannelSession);
	void BindLoginCallbacks();
	void UnbindLoginCallbacks();
#if ENGINE_MAJOR_VERSION != 5
	static void OnChannelAudioBeforeSendStatic(const EOS_RTCAudio_AudioBeforeSendCallbackInfo* Data);
	FEOSVoiceChatUserWeakPtr CreateWeakThis();
#endif
	void BindChannelCallbacks(FChannelSession& ChannelSession);
	void UnbindChannelCallbacks(FChannelSession& ChannelSession);
	void LeaveChannelInternal(const FString& ChannelName, const FOnVoiceChatChannelLeaveCompleteDelegate& Delegate);
	void LogoutInternal(const FOnVoiceChatLogoutCompleteDelegate& Delegate);
	void ClearLoginSession();
	DECLARE_DELEGATE_OneParam(FOnVoiceChatUserRtcRegisterUserCompleteDelegate, const EOS_EResult /* Result */);
	void RtcRegisterUser(const FString& UserId, const FOnVoiceChatUserRtcRegisterUserCompleteDelegate& Delegate);
	DECLARE_DELEGATE_OneParam(FOnVoiceChatUserRtcUnregisterUserCompleteDelegate, const EOS_EResult /* Result */);
	void RtcUnregisterUser(const FString& UserId, const FOnVoiceChatUserRtcUnregisterUserCompleteDelegate& Delegate);
	
	void SetHardwareAECEnabled(bool bEnabled);
	// EOS operation callbacks
	static void EOS_CALL OnJoinRoomStatic(const EOS_RTC_JoinRoomCallbackInfo* CallbackInfo);
	void OnJoinRoom(const EOS_RTC_JoinRoomCallbackInfo* CallbackInfo);
	static void EOS_CALL OnLeaveRoomStatic(const EOS_RTC_LeaveRoomCallbackInfo* CallbackInfo);
	void OnLeaveRoom(const EOS_RTC_LeaveRoomCallbackInfo* CallbackInfo);
	static void EOS_CALL OnBlockParticipantStatic(const EOS_RTC_BlockParticipantCallbackInfo* CallbackInfo);
	void OnBlockParticipant(const EOS_RTC_BlockParticipantCallbackInfo* CallbackInfo);
	static void EOS_CALL OnUpdateParticipantVolumeStatic(const EOS_RTCAudio_UpdateParticipantVolumeCallbackInfo* CallbackInfo);
	void OnUpdateParticipantVolume(const EOS_RTCAudio_UpdateParticipantVolumeCallbackInfo* CallbackInfo);
	static void EOS_CALL OnUpdateReceivingAudioStatic(const EOS_RTCAudio_UpdateReceivingCallbackInfo* CallbackInfo);
	void OnUpdateReceivingAudio(const EOS_RTCAudio_UpdateReceivingCallbackInfo* CallbackInfo);
	static void EOS_CALL OnUpdateSendingAudioStatic(const EOS_RTCAudio_UpdateSendingCallbackInfo* CallbackInfo);
	void OnUpdateSendingAudio(const EOS_RTCAudio_UpdateSendingCallbackInfo* CallbackInfo);

	// EOS notification callbacks
	static void EOS_CALL OnChannelDisconnectedStatic(const EOS_RTC_DisconnectedCallbackInfo* CallbackInfo);
	void OnChannelDisconnected(const EOS_RTC_DisconnectedCallbackInfo* CallbackInfo);
	static void EOS_CALL OnLobbyChannelConnectionChangedStatic(const EOS_Lobby_RTCRoomConnectionChangedCallbackInfo* CallbackInfo);
	void OnLobbyChannelConnectionChanged(const EOS_Lobby_RTCRoomConnectionChangedCallbackInfo* CallbackInfo);
	static void EOS_CALL OnChannelParticipantStatusChangedStatic(const EOS_RTC_ParticipantStatusChangedCallbackInfo* CallbackInfo);
	void OnChannelParticipantStatusChanged(const EOS_RTC_ParticipantStatusChangedCallbackInfo* CallbackInfo);
	static void EOS_CALL OnChannelParticipantAudioUpdatedStatic(const EOS_RTCAudio_ParticipantUpdatedCallbackInfo* CallbackInfo);
	void OnChannelParticipantAudioUpdated(const EOS_RTCAudio_ParticipantUpdatedCallbackInfo* CallbackInfo);
	void OnChannelAudioBeforeSend(const EOS_RTCAudio_AudioBeforeSendCallbackInfo* CallbackInfo);
	static void EOS_CALL OnChannelAudioBeforeRenderStatic(const EOS_RTCAudio_AudioBeforeRenderCallbackInfo* CallbackInfo);
	void OnChannelAudioBeforeRender(const EOS_RTCAudio_AudioBeforeRenderCallbackInfo* CallbackInfo);
	static void EOS_CALL OnChannelAudioInputStateStatic(const EOS_RTCAudio_AudioInputStateCallbackInfo* CallbackInfo);
	virtual void OnChannelAudioInputState(const EOS_RTCAudio_AudioInputStateCallbackInfo* CallbackInfo);

	bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar);

	friend const TCHAR* LexToString(ELoginState State);
	friend const TCHAR* LexToString(EChannelJoinState State);
};

#endif // WITH_EOS_RTC