// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_sessions.h"
#include "eos_sessions_types.h"
THIRD_PARTY_INCLUDES_END
#include "EIK_SessionsSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIK_Sessions_OnJoinSessionAcceptedCallback, const FEIK_ProductUserId&, LocalUserId, const FEIK_UI_EventId&, UIEventId);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIK_Sessions_OnLeaveSessionRequestedCallback, const FEIK_ProductUserId&, LocalUserId, const FString&, SessionName);
DECLARE_DYNAMIC_DELEGATE_FiveParams(FEIK_Sessions_OnSendSessionInviteCallback, const FEIK_ProductUserId&, LocalUserId, const FEIK_UI_EventId&, UIEventId, const FString&, TargetNativeAccountType, const FString&, TargetUserNativeAccountId, const FString&, SessionId);
DECLARE_DYNAMIC_DELEGATE_FourParams(FEIK_Sessions_OnSessionInviteAcceptedCallback, const FEIK_ProductUserId&, LocalUserId, const FString&, SessionId, const FEIK_ProductUserId&, TargetUserId, const FString&, InviteId);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEIK_Sessions_OnSessionInviteReceivedCallback, const FEIK_ProductUserId&, LocalUserId, const FString&, InviteId, const FEIK_ProductUserId&, TargetUserId);
DECLARE_DYNAMIC_DELEGATE_FourParams(FEIK_Sessions_OnSessionInviteRejectedCallback, const FEIK_ProductUserId&, LocalUserId, const FString&, SessionId, const FEIK_ProductUserId&, TargetUserId, const FString&, InviteId);

USTRUCT(BlueprintType)
struct FEIK_Sessions_CreateSessionModificationOptions
{
	GENERATED_BODY()

	//Name of the session to create
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | Sessions Interface")
	FString SessionName;

	//Bucket ID associated with the session
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | Sessions Interface")
	FString BucketId;

	//Maximum number of players allowed in the session
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | Sessions Interface")
	int32 MaxPlayers;

	//The Product User ID of the local user associated with the session
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | Sessions Interface")
	FEIK_ProductUserId LocalUserId;

	//Determines whether or not this session should be the one associated with the local user's presence information. If true, this session will be associated with presence. Only one session at a time can have this flag true. This affects the ability of the Social Overlay to show game related actions to take in the user's social graph. * using the bPresenceEnabled flags within the Sessions interface * using the bPresenceEnabled flags within the Lobby interface * using EOS_PresenceModification_SetJoinInfo
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | Sessions Interface")
	bool bPresenceEnabled;

	//Optional session id - set to a globally unique value to override the backend assignment If not specified the backend service will assign one to the session. Do not mix and match. This value can be of size [EOS_SESSIONMODIFICATION_MIN_SESSIONIDOVERRIDE_LENGTH, EOS_SESSIONMODIFICATION_MAX_SESSIONIDOVERRIDE_LENGTH]
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | Sessions Interface")
	FString SessionId;

	//If true, sanctioned players can neither join nor register with this session and, in the case of join, will return EOS_EResult code EOS_Sessions_PlayerSanctioned
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | Sessions Interface")
	bool bSanctionsEnabled;

	//Array of platform IDs indicating the player platforms allowed to register with the session. Platform IDs are found in the EOS header file, e.g. EOS_OPT_Epic. For some platforms, the value will be in the EOS Platform specific header file. If null, the session will be unrestricted.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | Sessions Interface")
	TArray<int32> AllowedPlatformIds;

	FEIK_Sessions_CreateSessionModificationOptions()
	{
		SessionName = "";
		BucketId = "";
		MaxPlayers = 0;
		LocalUserId = FEIK_ProductUserId();
		bPresenceEnabled = false;
		SessionId = "";
		bSanctionsEnabled = false;
	}
	EOS_Sessions_CreateSessionModificationOptions ToEosStruct()
	{
		EOS_Sessions_CreateSessionModificationOptions EosStruct;
		EosStruct.ApiVersion = EOS_SESSIONS_CREATESESSIONMODIFICATION_API_LATEST;
		EosStruct.SessionName = TCHAR_TO_ANSI(*SessionName);
		EosStruct.BucketId = TCHAR_TO_ANSI(*BucketId);
		EosStruct.MaxPlayers = MaxPlayers;
		EosStruct.LocalUserId = LocalUserId.GetValueAsEosType();
		EosStruct.bPresenceEnabled = bPresenceEnabled;
		EosStruct.SessionId = TCHAR_TO_ANSI(*SessionId);
		EosStruct.bSanctionsEnabled = bSanctionsEnabled;
		EosStruct.AllowedPlatformIdsCount = AllowedPlatformIds.Num();
		uint32_t* AllowedPlatformIdsArray = new uint32_t[AllowedPlatformIds.Num()];
		for (int i = 0; i < AllowedPlatformIds.Num(); i++)
		{
			AllowedPlatformIdsArray[i] = AllowedPlatformIds[i];
		}
		EosStruct.AllowedPlatformIds = AllowedPlatformIdsArray;
		return EosStruct;
	}
};
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	//EOS_ActiveSession_CopyInfo is used to immediately retrieve a copy of active session information If the call returns an EOS_Success result, the out parameter, OutActiveSessionInfo, must be passed to EOS_ActiveSession_Info_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_ActiveSession_CopyInfo")
	static TEnumAsByte<EEIK_Result> EIK_ActiveSession_CopyInfo(FEIK_HActiveSession Handle, FEIK_ActiveSession_Info& OutActiveSessionInfo);

	//EOS_ActiveSession_GetRegisteredPlayerByIndex is used to immediately retrieve individual players registered with the active session.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_ActiveSession_GetRegisteredPlayerByIndex")
	static FEIK_ProductUserId EIK_ActiveSession_GetRegisteredPlayerByIndex(FEIK_HActiveSession Handle, int32 PlayerIndex);

	//Release the memory associated with an active session. This must be called on data retrieved from EOS_Sessions_CopyActiveSessionHandle
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_ActiveSession_Release")
	static void EIK_ActiveSession_Release(FEIK_HActiveSession Handle);

	//EOS_SessionDetails_CopyInfo is used to immediately retrieve a copy of session information from a given source such as a active session or a search result. If the call returns an EOS_Success result, the out parameter, OutSessionInfo, must be passed to EOS_SessionDetails_Info_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionDetails_CopyInfo")
	static TEnumAsByte<EEIK_Result> EIK_SessionDetails_CopyInfo(FEIK_HSessionDetails Handle, FEIK_SessionDetails_Info& OutSessionInfo);

	//EOS_SessionDetails_CopySessionAttributeByIndex is used to immediately retrieve a copy of session attribution from a given source such as a active session or a search result. If the call returns an EOS_Success result, the out parameter, OutSessionAttribute, must be passed to EOS_SessionDetails_Attribute_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionDetails_CopySessionAttributeByIndex")
	static TEnumAsByte<EEIK_Result> EIK_SessionDetails_CopySessionAttributeByIndex(FEIK_HSessionDetails Handle, int32 AttrIndex, FEIK_SessionDetails_Attribute& OutSessionAttribute);

	//EOS_SessionDetails_CopySessionAttributeByKey is used to immediately retrieve a copy of session attribution from a given source such as a active session or a search result. If the call returns an EOS_Success result, the out parameter, OutSessionAttribute, must be passed to EOS_SessionDetails_Attribute_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionDetails_CopySessionAttributeByKey")
	static TEnumAsByte<EEIK_Result> EIK_SessionDetails_CopySessionAttributeByKey(FEIK_HSessionDetails Handle, const FString& AttrKey, FEIK_SessionDetails_Attribute& OutSessionAttribute);

	//Get the number of attributes associated with this session
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionDetails_GetSessionAttributeCount")
	static int32 EIK_SessionDetails_GetSessionAttributeCount(FEIK_HSessionDetails Handle);

	//Associate an attribute with this session An attribute is something that may or may not be advertised with the session. If advertised, it can be queried for in a search, otherwise the data remains local to the client
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionModification_AddAttribute")
	static TEnumAsByte<EEIK_Result> EIK_SessionModification_AddAttribute(FEIK_HSessionModification Handle, FEIK_Sessions_AttributeData AttrData, TEnumAsByte<EIK_ESessionAttributeAdvertisementType> AdvertisementType);

	//Release the memory associated with session modification. This must be called on data retrieved from EOS_Sessions_CreateSessionModification or EOS_Sessions_UpdateSessionModification
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionModification_Release")
	static void EIK_SessionModification_Release(FEIK_HSessionModification Handle);

	//Remove an attribute from this session
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionModification_RemoveAttribute")
	static TEnumAsByte<EEIK_Result> EIK_SessionModification_RemoveAttribute(FEIK_HSessionModification Handle, const FString& Key);

	//Set the Allowed Platform IDs for the session.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionModification_SetAllowedPlatformIds")
	static TEnumAsByte<EEIK_Result> EIK_SessionModification_SetAllowedPlatformIds(FEIK_HSessionModification Handle, const TArray<int32>& PlatformIds);

	//Set the bucket ID associated with this session. Values such as region, game mode, etc can be combined here depending on game need. Setting this is strongly recommended to improve search performance.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionModification_SetBucketId")
	static TEnumAsByte<EEIK_Result> EIK_SessionModification_SetBucketId(FEIK_HSessionModification Handle, const FString& BucketId);

	//Set the host address associated with this session Setting this is optional, if the value is not set the SDK will fill the value in from the service. It is useful to set if other addressing mechanisms are desired or if LAN addresses are preferred during development
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionModification_SetHostAddress")
	static TEnumAsByte<EEIK_Result> EIK_SessionModification_SetHostAddress(FEIK_HSessionModification Handle, const FString& HostAddress);

	//Allows enabling or disabling invites for this session. The session will also need to have bPresenceEnabled true.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionModification_SetInvitesAllowed")
	static TEnumAsByte<EEIK_Result> EIK_SessionModification_SetInvitesAllowed(FEIK_HSessionModification Handle, bool bInvitesAllowed);

	//Set whether or not join in progress is allowed Once a session is started, it will no longer be visible to search queries unless this flag is set or the session returns to the pending or ended state
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionModification_SetJoinInProgressAllowed")
	static TEnumAsByte<EEIK_Result> EIK_SessionModification_SetJoinInProgressAllowed(FEIK_HSessionModification Handle, bool bAllowJoinInProgress);

	//Set the maximum number of players allowed in this session. When updating the session, it is not possible to reduce this number below the current number of existing players
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionModification_SetMaxPlayers")
	static TEnumAsByte<EEIK_Result> EIK_SessionModification_SetMaxPlayers(FEIK_HSessionModification Handle, int32 MaxPlayers);

	//Set the session permissions associated with this session. The permissions range from "public" to "invite only" and are described by EOS_EOnlineSessionPermissionLevel
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionModification_SetPermissionLevel")
	static TEnumAsByte<EEIK_Result> EIK_SessionModification_SetPermissionLevel(FEIK_HSessionModification Handle, TEnumAsByte<EEIK_EOnlineSessionPermissionLevel> PermissionLevel);

	FEIK_Sessions_OnJoinSessionAcceptedCallback OnJoinSessionAcceptedCallback;
	//Register to receive notifications when a user accepts a session join game via the social overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_AddNotifyJoinSessionAccepted")
	FEIK_NotificationId EIK_Sessions_AddNotifyJoinSessionAccepted( const FEIK_Sessions_OnJoinSessionAcceptedCallback& Callback);

	FEIK_Sessions_OnLeaveSessionRequestedCallback OnLeaveSessionRequestedCallback;
	//Register to receive notifications about leave session requests performed by local user via the overlay. When user requests to leave the session in the social overlay, the SDK does not automatically leave the session, it is up to the game to perform any necessary cleanup and call the EOS_Sessions_DestroySession method using the SessionName sent in the notification function.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_AddNotifyLeaveSessionRequested")
	FEIK_NotificationId EIK_Sessions_AddNotifyLeaveSessionRequested( const FEIK_Sessions_OnLeaveSessionRequestedCallback& Callback);


	FEIK_Sessions_OnSendSessionInviteCallback OnSendSessionInviteCallback;
	//Register to receive notifications about a session "INVITE" performed by a local user via the overlay. This is only needed when a configured integrated platform has EOS_IPMF_DisableSDKManagedSessions set. The EOS SDK will then use the state of EOS_IPMF_PreferEOSIdentity and EOS_IPMF_PreferIntegratedIdentity to determine when the NotificationFn is called.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_AddNotifySendSessionNativeInviteRequested")
	FEIK_NotificationId EIK_Sessions_AddNotifySendSessionNativeInviteRequested( const FEIK_Sessions_OnSendSessionInviteCallback& Callback);

	FEIK_Sessions_OnSessionInviteAcceptedCallback OnSessionInviteAcceptedCallback;
	//Register to receive notifications when a user accepts a session invite via the social overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_AddNotifySessionInviteAccepted")
	FEIK_NotificationId EIK_Sessions_AddNotifySessionInviteAccepted( const FEIK_Sessions_OnSessionInviteAcceptedCallback& Callback);

	FEIK_Sessions_OnSessionInviteReceivedCallback OnSessionInviteReceivedCallback;
	//Register to receive session invites.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_AddNotifySessionInviteReceived")
	FEIK_NotificationId EIK_Sessions_AddNotifySessionInviteReceived( const FEIK_Sessions_OnSessionInviteReceivedCallback& Callback);

	FEIK_Sessions_OnSessionInviteRejectedCallback OnSessionInviteRejectedCallback;
	//Register to receive notifications when a user rejects a session invite.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_AddNotifySessionInviteRejected")
	FEIK_NotificationId EIK_Sessions_AddNotifySessionInviteRejected( const FEIK_Sessions_OnSessionInviteRejectedCallback& Callback);


	//Create a handle to an existing active session.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_CopyActiveSessionHandle")
	static TEnumAsByte<EEIK_Result> EIK_Sessions_CopyActiveSessionHandle(FString SessionName, FEIK_HActiveSession& OutActiveSessionHandle);

	//EOS_Sessions_CopySessionHandleByInviteId is used to immediately retrieve a handle to the session information from after notification of an invite If the call returns an EOS_Success result, the out parameter, OutSessionHandle, must be passed to EOS_SessionDetails_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_CopySessionHandleByInviteId")
	static TEnumAsByte<EEIK_Result> EIK_Sessions_CopySessionHandleByInviteId(FString InviteId, FEIK_HSessionDetails& OutSessionHandle);

	//EOS_Sessions_CopySessionHandleByUiEventId is used to immediately retrieve a handle to the session information from after notification of a join game event. If the call returns an EOS_Success result, the out parameter, OutSessionHandle, must be passed to EOS_SessionDetails_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_CopySessionHandleByUiEventId")
	static TEnumAsByte<EEIK_Result> EIK_Sessions_CopySessionHandleByUiEventId(FEIK_UI_EventId UiEventId, FEIK_HSessionDetails& OutSessionHandle);

	//EOS_Sessions_CopySessionHandleForPresence is used to immediately retrieve a handle to the session information which was marked with bPresenceEnabled on create or join. If the call returns an EOS_Success result, the out parameter, OutSessionHandle, must be passed to EOS_SessionDetails_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_CopySessionHandleForPresence")
	static TEnumAsByte<EEIK_Result> EIK_Sessions_CopySessionHandleForPresence(FEIK_ProductUserId LocalUserId, FEIK_HSessionDetails& OutSessionHandle);

	//Creates a session modification handle (EOS_HSessionModification). The session modification handle is used to build a new session and can be applied with EOS_Sessions_UpdateSession The EOS_HSessionModification must be released by calling EOS_SessionModification_Release once it no longer needed.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_CreateSessionModification")
	static TEnumAsByte<EEIK_Result> EIK_Sessions_CreateSessionModification(FEIK_Sessions_CreateSessionModificationOptions Options, FEIK_HSessionModification& OutSessionModificationHandle);

	/*
	*Create a session search handle. This handle may be modified to include various search parameters. Searching is possible in three methods, all mutually exclusive

		set the session ID to find a specific session
		set the target user ID to find a specific user
		set session parameters to find an array of sessions that match the search criteria
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_CreateSessionSearch")
	static TEnumAsByte<EEIK_Result> EIK_Sessions_CreateSessionSearch(int32 MaxSearchResults, FEIK_HSessionSearch& OutSessionSearchHandle);

	//Dump the contents of active sessions that exist locally to the log output, purely for debug purposes
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_DumpSessionState")
	static void EIK_Sessions_DumpSessionState(FString SessionName);

	//Get the number of known invites for a given user
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_GetInviteCount")
	static int32 EIK_Sessions_GetInviteCount(FEIK_ProductUserId LocalUserId);

	//Retrieve an invite ID from a list of active invites for a given user
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_GetInviteIdByIndex")
	static FString EIK_Sessions_GetInviteIdByIndex(FEIK_ProductUserId LocalUserId, int32 Index);

	//EOS_Sessions_IsUserInSession returns whether or not a given user can be found in a specified session
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_IsUserInSession")
	static TEnumAsByte<EEIK_Result> EIK_Sessions_IsUserInSession(FEIK_ProductUserId TargetUserId, FString SessionName);

	//Unregister from receiving notifications when a user accepts a session join game via the social overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_RemoveNotifyJoinSessionAccepted")
	static void EIK_Sessions_RemoveNotifyJoinSessionAccepted(FEIK_NotificationId InId);

	//Unregister from receiving notifications when a user performs a leave lobby action via the overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_RemoveNotifyLeaveSessionRequested")
	static void EIK_Sessions_RemoveNotifyLeaveSessionRequested(FEIK_NotificationId InId);

	//Unregister from receiving notifications when a user requests a send invite via the overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_RemoveNotifySendSessionNativeInviteRequested")
	static void EIK_Sessions_RemoveNotifySendSessionNativeInviteRequested(FEIK_NotificationId InId);

	//Unregister from receiving notifications when a user accepts a session invite via the social overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_RemoveNotifySessionInviteAccepted")
	static void EIK_Sessions_RemoveNotifySessionInviteAccepted(FEIK_NotificationId InId);

	//Unregister from receiving session invites.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_RemoveNotifySessionInviteReceived")
	static void EIK_Sessions_RemoveNotifySessionInviteReceived(FEIK_NotificationId InId);

	//Unregister from receiving notifications when a user rejects a session invite via the social overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_RemoveNotifySessionInviteRejected")
	static void EIK_Sessions_RemoveNotifySessionInviteRejected(FEIK_NotificationId InId);

	//Creates a session modification handle (EOS_HSessionModification). The session modification handle is used to modify an existing session and can be applied with EOS_Sessions_UpdateSession. The EOS_HSessionModification must be released by calling EOS_SessionModification_Release once it is no longer needed.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_UpdateSessionModification")
	static TEnumAsByte<EEIK_Result> EIK_Sessions_UpdateSessionModification(FString SessionName, FEIK_HSessionModification& OutSessionModificationHandle);

	//EOS_SessionSearch_CopySearchResultByIndex is used to immediately retrieve a handle to the session information from a given search result. If the call returns an EOS_Success result, the out parameter, OutSessionHandle, must be passed to EOS_SessionDetails_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionSearch_CopySearchResultByIndex")
	static TEnumAsByte<EEIK_Result> EIK_SessionSearch_CopySearchResultByIndex(FEIK_HSessionSearch Handle, int32 SessionIndex, FEIK_HSessionDetails& OutSessionHandle);

	//Get the number of search results found by the search parameters in this search
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionSearch_GetSearchResultCount")
	static int32 EIK_SessionSearch_GetSearchResultCount(FEIK_HSessionSearch Handle);

	//Release the memory associated with a session search. This must be called on data retrieved from EOS_Sessions_CreateSessionSearch.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionSearch_Release")
	static void EIK_SessionSearch_Release(FEIK_HSessionSearch Handle);

	//Remove a parameter from the array of search criteria.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionSearch_RemoveParameter")
	static TEnumAsByte<EEIK_Result> EIK_SessionSearch_RemoveParameter(FEIK_HSessionSearch Handle, const FString& Key, const TEnumAsByte<EEIK_EComparisonOp>& ComparisonOp);

	//Set the maximum number of search results to return in the query, can't be more than EOS_SESSIONS_MAX_SEARCH_RESULTS
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionSearch_SetMaxResults")
	static TEnumAsByte<EEIK_Result> EIK_SessionSearch_SetMaxResults(FEIK_HSessionSearch Handle, int32 MaxSearchResults);

	//Add a parameter to an array of search criteria combined via an implicit AND operator. Setting SessionId or TargetUserId will result in EOS_SessionSearch_Find failing
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionSearch_SetParameter")
	static TEnumAsByte<EEIK_Result> EIK_SessionSearch_SetParameter(FEIK_HSessionSearch Handle, FEIK_Sessions_AttributeData Parameter, const TEnumAsByte<EEIK_EComparisonOp>& ComparisonOp);

	//Set a session ID to find and will return at most one search result. Setting TargetUserId or SearchParameters will result in EOS_SessionSearch_Find failing
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionSearch_SetSessionId")
	static TEnumAsByte<EEIK_Result> EIK_SessionSearch_SetSessionId(FEIK_HSessionSearch Handle, const FString& SessionId);

	//Set a target user ID to find and will return at most one search result. Setting SessionId or SearchParameters will result in EOS_SessionSearch_Find failing
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionSearch_SetTargetUserId")
	static TEnumAsByte<EEIK_Result> EIK_SessionSearch_SetTargetUserId(FEIK_HSessionSearch Handle, FEIK_ProductUserId TargetUserId);
	
};
