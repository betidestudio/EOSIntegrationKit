// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "VoiceChat.h"
#include "eos_lobby_types.h"
#include "eos_lobby.h"

#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OnlineSubsystemEIK/AsyncFunctions/Sessions/EIK_UpdateSession_AsyncFunction.h"
#include "EIK_BlueprintFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnResponseFromSanctions, bool, bWasSuccess);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnResponseFromEpicForAccessToken, bool, bWasSuccess, const FString&, AccessToken);

UENUM(BlueprintType)
enum EEOSSanctionType
{
	IncorrectSanction,
	CompromisedAccount,
	UnfairPunishment,
	AppealForForgiveness,
};

UENUM(BlueprintType)
enum EEIK_LoginStatus
{
	/** Player has not logged in or chosen a local profile */
	NotLoggedIn,
	/** Player is using a local profile but is not logged in */
	UsingLocalProfile,
	/** Player has been validated by the platform specific authentication service */
	LoggedIn,
};

UENUM(BlueprintType)
enum ESessionCurrentState
{
	/** An online session has not been created yet */
	NoSession,
	/** An online session is in the process of being created */
	Creating,
	/** Session has been created but the session hasn't started (pre match lobby) */
	Pending,
	/** Session has been asked to start (may take time due to communication with backend) */
	Starting,
	/** The current session has started. Sessions with join in progress disabled are no longer joinable */
	InProgress,
	/** The session is still valid, but the session is no longer being played (post match lobby) */
	Ending,
	/** The session is closed and any stats committed */
	Ended,
	/** The session is being destroyed */
	Destroying
};

USTRUCT(BlueprintType)
struct FEIK_OnlineSessionSettings
{
	GENERATED_BODY()

	/** The number of publicly available connections advertised */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	int32 NumPublicConnections;
	/** The number of connections that are private (invite/password) only */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	int32 NumPrivateConnections;
	/** Whether this match is publicly advertised on the online service */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bShouldAdvertise;
	/** Whether joining in progress is allowed or not */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bAllowJoinInProgress;
	/** This game will be lan only and not be visible to external players */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bIsLANMatch;
	/** Whether the server is dedicated or player hosted */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bIsDedicated;
	/** Whether the match should gather stats or not */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bUsesStats;
	/** Whether the match allows invitations for this session or not */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bAllowInvites;
	/** Whether to display user presence information or not */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bUsesPresence;
	/** Whether joining via player presence is allowed or not */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bAllowJoinViaPresence;
	/** Whether joining via player presence is allowed for friends only or not */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bAllowJoinViaPresenceFriendsOnly;
	/** Whether the server employs anti-cheat (punkbuster, vac, etc) */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bAntiCheatProtected;
	/** Whether to prefer lobbies APIs if the platform supports them */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bUseLobbiesIfAvailable;
	/** Whether to create (and auto join) a voice chat room for the lobby, if the platform supports it */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bUseLobbiesVoiceChatIfAvailable;
	/** Manual override for the Session Id instead of having one assigned by the backend. Its size may be restricted depending on the platform */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	FString SessionIdOverride;

	/** Used to keep different builds from seeing each other during searches */
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	int32 BuildUniqueId;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	TMap<FString, FEIKAttribute> SessionSettings;
	
	FEIK_OnlineSessionSettings(FOnlineSessionSettings Settings)
	{
		NumPublicConnections = Settings.NumPublicConnections;
		NumPrivateConnections = Settings.NumPrivateConnections;
		bShouldAdvertise = Settings.bShouldAdvertise;
		bAllowJoinInProgress = Settings.bAllowJoinInProgress;
		bIsLANMatch = Settings.bIsLANMatch;
		bIsDedicated = Settings.bIsDedicated;
		bUsesStats = Settings.bUsesStats;
		bAllowInvites = Settings.bAllowInvites;
		bUsesPresence = Settings.bUsesPresence;
		bAllowJoinViaPresence = Settings.bAllowJoinViaPresence;
		bAllowJoinViaPresenceFriendsOnly = Settings.bAllowJoinViaPresenceFriendsOnly;
		bAntiCheatProtected = Settings.bAntiCheatProtected;
		bUseLobbiesIfAvailable = Settings.bUseLobbiesIfAvailable;
		bUseLobbiesVoiceChatIfAvailable = Settings.bUseLobbiesVoiceChatIfAvailable;
#if ENGINE_MAJOR_VERSION == 5
		SessionIdOverride = Settings.SessionIdOverride;
#endif
		BuildUniqueId = Settings.BuildUniqueId;
		TMap<FName, FOnlineSessionSetting>::TIterator It(Settings.Settings);
		TMap<FString, FEIKAttribute> LocalArraySettings;
		while (It)
		{
			const FName& SettingName = It.Key();
			const FOnlineSessionSetting& Setting = It.Value();
			LocalArraySettings.Add(*SettingName.ToString(), Setting.Data);
			++It;
		}
		SessionSettings = LocalArraySettings;
	}

	FEIK_OnlineSessionSettings()
	{
		NumPublicConnections = 0;
		NumPrivateConnections = 0;
		bShouldAdvertise = false;
		bAllowJoinInProgress = false;
		bIsLANMatch = false;
		bIsDedicated = false;
		bUsesStats = false;
		bAllowInvites = false;
		bUsesPresence = false;
		bAllowJoinViaPresence = false;
		bAllowJoinViaPresenceFriendsOnly = false;
		bAntiCheatProtected = false;
		bUseLobbiesIfAvailable = false;
		bUseLobbiesVoiceChatIfAvailable = false;
		SessionIdOverride = "";
		BuildUniqueId = 0;
	}
};

USTRUCT(BlueprintType)
struct FEIK_CurrentSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bHostingSession;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bPublicJoinable;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bFriendJoinable;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bInviteOnly;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	bool bAllowInvites;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	TArray<FEIKUniqueNetId> RegisteredPlayers;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	TArray<FEIK_MemberSpecificAttribute> MemberSettings;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	FEIKUniqueNetId SessionOwner;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	TEnumAsByte<ESessionCurrentState> SessionState;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	int32 NumOpenPublicConnections;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	int32 MaxPublicConnections;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	int32 NumOpenPrivateConnections;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	int32 MaxPrivateConnections;
	
	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	FString SessionIdString;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	FString CompleteDebugString;

	UPROPERTY(BlueprintReadWrite, Category="EOS Integration Kit || Sessions")
	FEIK_OnlineSessionSettings OriginalSessionSettings;
	
	FEIK_CurrentSessionInfo(): bAllowInvites(false), SessionState(), NumOpenPublicConnections(0),
	                           MaxPublicConnections(0),
	                           NumOpenPrivateConnections(0), MaxPrivateConnections(0), OriginalSessionSettings()
	{
		bHostingSession = false;
		bPublicJoinable = false;
		bFriendJoinable = false;
		bInviteOnly = false;
	}

	FEIK_CurrentSessionInfo(FNamedOnlineSession Session)
	{
		CompleteDebugString = Session.SessionInfo->ToDebugString();
		NumOpenPublicConnections = Session.NumOpenPublicConnections;
		NumOpenPrivateConnections = Session.NumOpenPrivateConnections;
		MaxPublicConnections = Session.SessionSettings.NumPublicConnections;
		MaxPrivateConnections = Session.SessionSettings.NumPrivateConnections;
		SessionIdString = Session.GetSessionIdStr();
		OriginalSessionSettings = Session.SessionSettings;
		bHostingSession = Session.bHosting;
		Session.GetJoinability(bPublicJoinable, bFriendJoinable, bInviteOnly, bAllowInvites);
		FEIKUniqueNetId Temp;
		Temp.SetUniqueNetId(Session.OwningUserId);
		SessionOwner = Temp;
		for (auto& Player : Session.RegisteredPlayers)
		{
			FEIKUniqueNetId Temp1;
			Temp1.SetUniqueNetId(Player);
			RegisteredPlayers.Add(Temp1);
		}
		for (auto& LocalMemberSettings : Session.SessionSettings.MemberSettings)
		{
			FEIK_MemberSpecificAttribute Temp2;
			FEIKUniqueNetId MemberId;
			MemberId.SetUniqueNetId(LocalMemberSettings.Key);
			Temp2.MemberId = MemberId;
			TMap<FString, FEIKAttribute> LocalAttributes;
			for (auto& Attribute : LocalMemberSettings.Value)
			{
				FEIKAttribute Temp3 = Attribute.Value.Data;
				LocalAttributes.Add(Attribute.Key.ToString(), Temp3);
			}
			Temp2.Attributes = LocalAttributes;
			MemberSettings.Add(Temp2);
		}
		
		switch (Session.SessionState)
		{
		case EOnlineSessionState::NoSession:
			SessionState = ESessionCurrentState::NoSession;
			break;
		case EOnlineSessionState::Creating:
			SessionState = ESessionCurrentState::Creating;
			break;
		case EOnlineSessionState::Pending:
			SessionState = ESessionCurrentState::Pending;
			break;
		case EOnlineSessionState::Starting:
			SessionState = ESessionCurrentState::Starting;
			break;
		case EOnlineSessionState::InProgress:
			SessionState = ESessionCurrentState::InProgress;
			break;
		case EOnlineSessionState::Ending:
			SessionState = ESessionCurrentState::Ending;
			break;
		case EOnlineSessionState::Ended:
			SessionState = ESessionCurrentState::Ended;
			break;
		case EOnlineSessionState::Destroying:
			SessionState = ESessionCurrentState::Destroying;
			break;
		}
	}
};

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_BlueprintFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Extra", meta=( WorldContext = "Context" ))
	static FString GetEpicAccountId(UObject* Context);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Extra", meta=( WorldContext = "Context" ), DisplayName="Get EIK Session Info")
	static FEIK_CurrentSessionInfo GetCurrentSessionInfo(UObject* Context, bool& bIsSessionPresent, FName SessionName = "GameSession");

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Extra", meta=( WorldContext = "Context" ), DisplayName="Get All EIK Session Names")
	static TArray<FName> GetAllCurrentSessionNames(UObject* Context);
	
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Extra", meta=( WorldContext = "Context" ))
	static FString GetProductUserID(UObject* Context);


	/** Lobby Voice Functions START */

	static IVoiceChatUser* GetLobbyVoiceChat(UObject* Context);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static bool MuteLobbyVoiceChat(UObject* Context, bool bMute);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static bool IsLobbyVoiceChatMuted(UObject* Context);
	
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static bool SetLobbyOutputMethod(UObject* Context, FString MethodID);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static bool SetLobbyInputMethod(UObject* Context, FString MethodID);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static bool BlockLobbyVoiceChatPlayers(UObject* Context, TArray<FString> BlockedPlayers);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static bool UnblockLobbyVoiceChatPlayers(UObject* Context, TArray<FString> UnblockedPlayers);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static float GetLobbyVoiceChatOutputVolume(UObject* Context);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static bool SetLobbyVoiceChatOutputVolume(UObject* Context, float Volume);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static bool SetLobbyVoiceChatInputVolume(UObject* Context, float Volume);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static float GetLobbyVoiceChatInputVolume(UObject* Context);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static bool SetLobbyPlayerVoiceChatVolume(UObject* Context, FString PlayerName, float Volume);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || Lobby Voice", meta=( WorldContext = "Context" ))
	static float GetLobbyPlayerVoiceChatVolume(UObject* Context, FString PlayerName);

	/** Voice Chat Functions END */

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Friends")
	static bool ShowFriendsList();

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit || UserInfo")
	static FEIKUniqueNetId MakeEIKUniqueNetId(FString EpicAccountId, FString ProductUserId);

	UFUNCTION(BlueprintCallable, DisplayName="Accept EIK Session Invite", Category="EOS Integration Kit || Sessions")
	static bool AcceptSessionInvite(FString InviteId, FString LocalUserId, FString InviterUserId);

	UFUNCTION(BlueprintCallable, DisplayName="Reject EIK Session Invite", Category="EOS Integration Kit || Sessions")
	static bool RejectSessionInvite(FString InviteId, FString LocalUserId);
	
	// This is a C++ method definition for starting lobbies and sessions
	UFUNCTION(BlueprintCallable, DisplayName="Start EIK Session", Category="EOS Integration Kit || Sessions")
	static bool StartSession(FName SessionName = "GameSession");

	// This is a C++ method definition for registering players in lobbies and sessions
	UFUNCTION(BlueprintCallable, DisplayName="Register EIK Player In Session",	Category="EOS Integration Kit || Sessions")
	static bool RegisterPlayer(FName SessionName, FEIKUniqueNetId PlayerId, bool bWasInvited = false);

	// This is a C++ method definition for unregistering players from lobbies and sessions
	UFUNCTION(BlueprintCallable, DisplayName="Unregister EIK Player In Session",
		Category="EOS Integration Kit || Sessions")
	static bool UnRegisterPlayer(FName SessionName, FEIKUniqueNetId PlayerId);

	// This is a C++ method definition for ending lobbies and sessions
	UFUNCTION(BlueprintCallable, DisplayName="End EIK Session", Category="EOS Integration Kit || Sessions")
	static bool EndSession(FName SessionName = "GameSession");

	// This is a C++ method definition for checking if user is in a lobby or session
	UFUNCTION(BlueprintCallable, DisplayName="Is In EIK Session", Category="EOS Integration Kit || Sessions")
	static bool IsInSession(FName SessionName, FEIKUniqueNetId PlayerId);

	// This is a C++ method definition for getting the nickname of a player from an online subsystem.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/extra-functions/getplayernickname
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Extra")
	static FString GetPlayerNickname(const int32 LocalUserNum);

	// This is a C++ method definition for getting the login status of a player from an online subsystem.
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/extra-functions/getloginstatus
	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Extra")
	static EEIK_LoginStatus GetLoginStatus(const int32 LocalUserNum);

	// Generate a session code of the specified length.
	// This function returns a randomly generated alphanumeric session code.
	// The 'CodeLength' parameter determines the length of the generated code (default: 9).
	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra")
	static FString GenerateSessionCode(int32 CodeLength = 9);

	// Check if EOS (Epic Online Services) is active or not.
	// This function returns true if EOS is active, false otherwise.
	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra")
	static bool IsEIKActive();

	// Get the active EOS subsystem name.
	// This function returns the name of the active EOS subsystem as a FName.
	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra")
	static FName GetActiveSubsystem();

	// Convert a TArray<uint8> to a FString using Base64 encoding.
	// This is useful for converting binary data to a human-readable string format.
	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra || Conversions")
	static FString ByteArrayToString(const TArray<uint8>& DataToConvert);

	// Convert an FString to a TArray<uint8> using Base64 decoding.
	// This is useful for converting a string back to its original binary data representation.
	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra || Conversions")
	static TArray<uint8> StringToByteArray(const FString& DataToConvert);

	// Convert a TArray<uint8> to a SaveGame object.
	// The TArray<uint8> is deserialized to construct the SaveGame object.
	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra || Conversions")
	static USaveGame* ByteArrayToSaveGameObject(const TArray<uint8>& DataToConvert)
	{
		return UGameplayStatics::LoadGameFromMemory(DataToConvert);
	}

	// Convert a SaveGame object to a TArray<uint8>.
	// The SaveGame object is serialized into a binary data array (TArray<uint8>).
	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra || Conversions")
	static TArray<uint8> SaveGameObjectToByteArray(USaveGame* DataToConvert)
	{
		TArray<uint8> Result;
		UGameplayStatics::SaveGameToMemory(DataToConvert, Result);
		return Result;
	}

	// This is a C++ method definition for getting the User Unique NetID
	// Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/extra-functions/get-useruniqueid
	UFUNCTION(BlueprintPure, DisplayName="Get User Unique NetID", Category="EOS Integration Kit || Extra")
	static FEIKUniqueNetId GetUserUniqueID(const APlayerController* PlayerController, bool& bIsValid);

	UFUNCTION(BlueprintPure, DisplayName="Get User Unique NetID From PlayerState", Category="EOS Integration Kit || Extra")
	static FEIKUniqueNetId GetUserUniqueIDFromPlayerState(const APlayerState* PlayerState, bool& bIsValid);

	UFUNCTION(BlueprintPure, DisplayName="EOS SDK Version", Category="EOS Integration Kit || Extra")
	static FString GetEOSSDKVersion();

	//Get Plugin Version
	UFUNCTION(BlueprintPure, DisplayName="EIK Plugin Version", Category="EOS Integration Kit || Extra")
	static FString GetEIKPluginVersion();

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Extra")
	static bool IsValidSession(FSessionFindStruct Session);

	UFUNCTION(BlueprintPure, Category="EOS Integration Kit || Extra")
	static FString GetCurrentPort(AGameModeBase* CurrentGameMode);

	UFUNCTION(BlueprintCallable, Category="EOS Integration Kit || Extra")
	static void MakeSanctionAppeal(FString AccessToken, EEOSSanctionType Reason,
	                               const FOnResponseFromSanctions& OnResponseFromSanctions);

	UFUNCTION(BlueprintCallable, DisplayName="Request EOS Access Token", Category="EOS Integration Kit || Extra")
	static void RequestEOSAccessToken(const FOnResponseFromEpicForAccessToken& Response);

	UFUNCTION(BlueprintCallable, DisplayName="Convert POSIX Time to DateTime", Category="EOS Integration Kit || Extra")
	static FDateTime ConvertPosixTimeToDateTime(int64 PosixTime);

	UFUNCTION(BlueprintCallable, DisplayName="Get Resolved Connect String", Category="EOS Integration Kit || Extra")
	static FString GetResolvedConnectString(FName SessionName);

	UFUNCTION(BlueprintCallable, DisplayName="Get AutoLogin Attempted Status", Category="EOS Integration Kit || Extra")
	static bool GetAutoLoginAttemptedStatus();

	UFUNCTION(BlueprintCallable, DisplayName="Get AutoLogin In Progress Status", Category="EOS Integration Kit || Extra")
	static bool GetAutoLoginInProgressStatus();

	UFUNCTION(BlueprintCallable, DisplayName="Get Environment Variable", Category="EOS Integration Kit || Extra")
	static FString GetEnvironmentVariable(const FString& EnvVariableName);

	UFUNCTION(BlueprintCallable, DisplayName="Init Ping Beacon", Category="EOS Integration Kit || Extra", meta=(WorldContext="Context"))
	static bool InitPingBeacon(UObject* Context, AGameModeBase* GameMode);

};
