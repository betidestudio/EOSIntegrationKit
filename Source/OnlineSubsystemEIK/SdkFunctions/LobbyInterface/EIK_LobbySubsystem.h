// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_LobbySubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIK_Lobby_OnJoinLobbyAcceptedCallback, FEIK_ProductUserId, LocalUserId, const FEIK_UI_EventId&, UiEventId);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIK_Lobby_OnLeaveLobbyRequestedCallback, FEIK_ProductUserId, LocalUserId, const FEIK_LobbyId&, LobbyId);
DECLARE_DYNAMIC_DELEGATE_FourParams(FEIK_Lobby_OnLobbyInviteAcceptedCallback, FEIK_ProductUserId, LocalUserId, FEIK_ProductUserId, TargetUserId, const FEIK_LobbyId&, LobbyId, const FString&, InviteId);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEIK_Lobby_OnLobbyInviteReceivedCallback, FEIK_ProductUserId, LocalUserId, FEIK_ProductUserId, TargetUserId, const FString&, InviteId);
DECLARE_DYNAMIC_DELEGATE_FourParams(FEIK_Lobby_OnLobbyInviteRejectedCallback, FEIK_ProductUserId, LocalUserId, FEIK_ProductUserId, TargetUserId, const FEIK_LobbyId&, LobbyId, const FString&, InviteId);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEIK_Lobby_OnLobbyMemberStatusReceivedCallback, FEIK_ProductUserId, TargetUserId, const FEIK_LobbyId&, LobbyId, const TEnumAsByte<EEIK_ELobbyMemberStatus>&, CurrentStatus);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEIK_Lobby_OnLobbyMemberUpdateReceivedCallback, FEIK_ProductUserId, TargetUserId, const FEIK_LobbyId&, LobbyId);
DECLARE_DYNAMIC_DELEGATE_OneParam(FEIK_Lobby_OnLobbyUpdateReceivedCallback, const FEIK_LobbyId&, LobbyId);
DECLARE_DYNAMIC_DELEGATE_FourParams(FEIK_Lobby_OnRTCRoomConnectionChangedCallback, const FEIK_LobbyId&, LobbyId, const FEIK_ProductUserId&, LocalUserId, bool, bIsConnected, const TEnumAsByte<EEIK_Result>&, DisconnectReason);
DECLARE_DYNAMIC_DELEGATE_FiveParams(FEIK_Lobby_OnSendLobbyNativeInviteCallback, FEIK_UI_EventId, UiEventId, FEIK_ProductUserId, LocalUserId, const FString&, TargetNativeAccountType, const FString&, TargetUserNativeAccountId, const FEIK_LobbyId&, LobbyId);

UCLASS(DisplayName="Lobby Interface", meta=(DisplayName="Lobby Interface"))
class ONLINESUBSYSTEMEIK_API UEIK_LobbySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	FEIK_Lobby_OnJoinLobbyAcceptedCallback OnJoinLobbyAccepted;
	//Register to receive notifications about lobby "JOIN" performed by local user (when no invite) via the overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_AddNotifyJoinLobbyAccepted")
	FEIK_NotificationId EIK_Lobby_AddNotifyJoinLobbyAccepted(FEIK_Lobby_OnJoinLobbyAcceptedCallback Callback);

	//Unregister from receiving notifications when a user accepts a lobby invitation via the overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RemoveNotifyJoinLobbyAccepted")
	void EIK_Lobby_RemoveNotifyJoinLobbyAccepted(FEIK_NotificationId InId);


	FEIK_Lobby_OnLeaveLobbyRequestedCallback OnLeaveLobbyRequested;
	//Register to receive notifications about leave lobby requests performed by the local user via the overlay. When user requests to leave the lobby in the social overlay, the SDK does not automatically leave the lobby, it is up to the game to perform any necessary cleanup and call the EOS_Lobby_LeaveLobby method using the lobbyId sent in the notification function.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_AddNotifyLeaveLobbyRequested")
	FEIK_NotificationId EIK_Lobby_AddNotifyLeaveLobbyRequested(FEIK_Lobby_OnLeaveLobbyRequestedCallback Callback);

	//Unregister from receiving notifications when a user performs a leave lobby action via the overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RemoveNotifyLeaveLobbyRequested")
	void EIK_Lobby_RemoveNotifyLeaveLobbyRequested(FEIK_NotificationId InId);
	
	FEIK_Lobby_OnLobbyInviteAcceptedCallback OnLobbyInviteAccepted;
	//Register to receive notifications about lobby invites accepted by local user via the overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_AddNotifyLobbyInviteAccepted")
	FEIK_NotificationId EIK_Lobby_AddNotifyLobbyInviteAccepted(FEIK_Lobby_OnLobbyInviteAcceptedCallback Callback);

	//Unregister from receiving notifications when a user accepts a lobby invitation via the overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RemoveNotifyLobbyInviteAccepted")
	void EIK_Lobby_RemoveNotifyLobbyInviteAccepted(FEIK_NotificationId InId);

	
	FEIK_Lobby_OnLobbyInviteReceivedCallback OnLobbyInviteReceived;
	//Register to receive notifications about lobby invites sent to local users.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_AddNotifyLobbyInviteReceived")
	FEIK_NotificationId EIK_Lobby_AddNotifyLobbyInviteReceived(FEIK_Lobby_OnLobbyInviteReceivedCallback Callback);

	//Unregister from receiving notifications when a user receives a lobby invitation.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RemoveNotifyLobbyInviteReceived")
	void EIK_Lobby_RemoveNotifyLobbyInviteReceived(FEIK_NotificationId InId);


	
	FEIK_Lobby_OnLobbyInviteRejectedCallback OnLobbyInviteRejected;
	//Register to receive notifications about lobby invites rejected by local user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_AddNotifyLobbyInviteRejected")
	FEIK_NotificationId EIK_Lobby_AddNotifyLobbyInviteRejected(FEIK_Lobby_OnLobbyInviteRejectedCallback Callback);

	//Unregister from receiving notifications when a user rejects a lobby invitation via the overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RemoveNotifyLobbyInviteRejected")
	void EIK_Lobby_RemoveNotifyLobbyInviteRejected(FEIK_NotificationId InId);

	
	
	FEIK_Lobby_OnLobbyMemberStatusReceivedCallback OnLobbyMemberStatusReceived;
	//Register to receive notifications about the changing status of lobby members.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_AddNotifyLobbyMemberStatusReceived")
	FEIK_NotificationId EIK_Lobby_AddNotifyLobbyMemberStatusReceived(FEIK_Lobby_OnLobbyMemberStatusReceivedCallback Callback);

	//Unregister from receiving notifications when lobby members status change.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RemoveNotifyLobbyMemberStatusReceived")
	void EIK_Lobby_RemoveNotifyLobbyMemberStatusReceived(FEIK_NotificationId InId);

	

	FEIK_Lobby_OnLobbyMemberUpdateReceivedCallback OnLobbyMemberUpdateReceived;
	//Register to receive notifications when a lobby member updates the attributes associated with themselves inside the lobby.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_AddNotifyLobbyMemberUpdateReceived")
	FEIK_NotificationId EIK_Lobby_AddNotifyLobbyMemberUpdateReceived(FEIK_Lobby_OnLobbyMemberUpdateReceivedCallback Callback);

	//Unregister from receiving notifications when lobby members change their data.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RemoveNotifyLobbyMemberUpdateReceived")
	void EIK_Lobby_RemoveNotifyLobbyMemberUpdateReceived(FEIK_NotificationId InId);

	
	
	FEIK_Lobby_OnLobbyUpdateReceivedCallback OnLobbyUpdateReceived;
	//Register to receive notifications when a lobby owner updates the attributes associated with the lobby.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_AddNotifyLobbyUpdateReceived")
	FEIK_NotificationId EIK_Lobby_AddNotifyLobbyUpdateReceived(FEIK_Lobby_OnLobbyUpdateReceivedCallback Callback);

	//Unregister from receiving notifications when a lobby changes its data.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RemoveNotifyLobbyUpdateReceived")
	void EIK_Lobby_RemoveNotifyLobbyUpdateReceived(FEIK_NotificationId InId);


	
	FEIK_Lobby_OnRTCRoomConnectionChangedCallback OnRTCRoomConnectionChanged;
	//Register to receive notifications of when the RTC Room for a particular lobby has a connection status change. The RTC Room connection status is independent of the lobby connection status, however the lobby system will attempt to keep them consistent, automatically connecting to the RTC room after joining a lobby which has an associated RTC room and disconnecting from the RTC room when a lobby is left or disconnected. This notification is entirely informational and requires no action in response by the application. If the connected status is offline (bIsConnected is EOS_FALSE), the connection will automatically attempt to reconnect. The purpose of this notification is to allow applications to show the current connection status of the RTC room when the connection is not established. Unlike EOS_RTC_AddNotifyDisconnected, EOS_RTC_LeaveRoom should not be called when the RTC room is disconnected. This function will only succeed when called on a lobby the local user is currently a member of.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_AddNotifyRTCRoomConnectionChanged")
	FEIK_NotificationId EIK_Lobby_AddNotifyRTCRoomConnectionChanged(FEIK_Lobby_OnRTCRoomConnectionChangedCallback Callback);

	//Unregister from receiving notifications when an RTC Room's connection status changes. This should be called when the local user is leaving a lobby.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RemoveNotifyRTCRoomConnectionChanged")
	void EIK_Lobby_RemoveNotifyRTCRoomConnectionChanged(FEIK_NotificationId InId);


	
	FEIK_Lobby_OnSendLobbyNativeInviteCallback OnSendLobbyNativeInvite;
	//Register to receive notifications about a lobby "INVITE" performed by a local user via the overlay. This is only needed when a configured integrated platform has EOS_IPMF_DisableSDKManagedSessions set. The EOS SDK will then use the state of EOS_IPMF_PreferEOSIdentity and EOS_IPMF_PreferIntegratedIdentity to determine when the NotificationFn is called.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_AddNotifySendLobbyNativeInviteRequested")
	FEIK_NotificationId EIK_Lobby_AddNotifySendLobbyNativeInviteRequested(FEIK_Lobby_OnSendLobbyNativeInviteCallback Callback);

	//Unregister from receiving notifications when a user requests a send invite via the overlay.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RemoveNotifySendLobbyNativeInviteRequested")
	void EIK_Lobby_RemoveNotifySendLobbyNativeInviteRequested(FEIK_NotificationId InId);





	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_Attribute_Release")
	void EIK_Lobby_Attribute_Release(const FEIK_Lobby_Attribute& Attribute);

	//Create a handle to an existing lobby. If the call returns an EOS_Success result, the out parameter, OutLobbyDetailsHandle, must be passed to EOS_LobbyDetails_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_CopyLobbyDetailsHandle")
	TEnumAsByte<EEIK_Result> EIK_Lobby_CopyLobbyDetailsHandle(FEIK_LobbyId LobbyId, FEIK_ProductUserId LocalUserId, FEIK_HLobbyDetails& OutLobbyDetailsHandle);

	//EOS_Lobby_CopyLobbyDetailsHandleByInviteId is used to immediately retrieve a handle to the lobby information from after notification of an invite If the call returns an EOS_Success result, the out parameter, OutLobbyDetailsHandle, must be passed to EOS_LobbyDetails_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_CopyLobbyDetailsHandleByInviteId")
	TEnumAsByte<EEIK_Result> EIK_Lobby_CopyLobbyDetailsHandleByInviteId(FString InviteId, FEIK_HLobbyDetails& OutLobbyDetailsHandle);

	//EOS_Lobby_CopyLobbyDetailsHandleByUiEventId is used to immediately retrieve a handle to the lobby information from after notification of an join game If the call returns an EOS_Success result, the out parameter, OutLobbyDetailsHandle, must be passed to EOS_LobbyDetails_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_CopyLobbyDetailsHandleByUiEventId")
	TEnumAsByte<EEIK_Result> EIK_Lobby_CopyLobbyDetailsHandleByUiEventId(const FEIK_UI_EventId& UiEventId, FEIK_HLobbyDetails& OutLobbyDetailsHandle);


	/*
	Create a lobby search handle. This handle may be modified to include various search parameters. Searching is possible in three methods, all mutually exclusive

	set the lobby ID to find a specific lobby
	set the target user ID to find a specific user
	set lobby parameters to find an array of lobbies that match the search criteria
	*/
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_CreateLobbySearch")
	TEnumAsByte<EEIK_Result> EIK_Lobby_CreateLobbySearch(int32 MaxResults, FEIK_HLobbySearch& OutLobbySearchHandle);

	//Get the Connection string for an EOS lobby. The connection string describes the presence of a player in terms of game state. Xbox platforms expect titles to embed this into their MultiplayerActivity at creation. When present, the SDK will use this value to populate session presence in the social overlay and facilitate platform invitations.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_GetConnectString")
	TEnumAsByte<EEIK_Result> EIK_Lobby_GetConnectString(FEIK_ProductUserId LocalUserId, FEIK_LobbyId LobbyId, FString& OutConnectString);

	//Get the number of known invites for a given user
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_GetInviteCount")
	int32 EIK_Lobby_GetInviteCount(FEIK_ProductUserId LocalUserId);

	//Retrieve an invite ID from a list of active invites for a given user
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_GetInviteIdByIndex")
	TEnumAsByte<EEIK_Result> EIK_Lobby_GetInviteIdByIndex(FEIK_ProductUserId LocalUserId, int32 Index, FString& OutInviteId);

	//Get the name of the RTC room associated with a specific lobby a local user belongs to. This value can be used whenever you need a RoomName value in the RTC_* suite of functions. RTC Room Names must not be used with EOS_RTC_JoinRoom, EOS_RTC_LeaveRoom, or EOS_RTC_AddNotifyDisconnected. Doing so will return EOS_AccessDenied or EOS_INVALID_NOTIFICATIONID if used with those functions. This function will only succeed when called on a lobby the local user is currently a member of.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_GetRTCRoomName")
	TEnumAsByte<EEIK_Result> EIK_Lobby_GetRTCRoomName(FEIK_ProductUserId LocalUserId, FEIK_LobbyId LobbyId, FString& OutRTCRoomName);
	
	//Get the current connection status of the RTC Room for a lobby. The RTC Room connection status is independent of the lobby connection status, however the lobby system will attempt to keep them consistent, automatically connecting to the RTC room after joining a lobby which has an associated RTC room and disconnecting from the RTC room when a lobby is left or disconnected. This function will only succeed when called on a lobby the local user is currently a member of.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_IsRTCRoomConnected")
	TEnumAsByte<EEIK_Result> EIK_Lobby_IsRTCRoomConnected(FEIK_ProductUserId LocalUserId, FEIK_LobbyId LobbyId, bool& bOutIsConnected);

	//Parse the ConnectString for an EOS lobby invitation to extract just the lobby ID. Used for joining a lobby from a connection string (as generated by GetConnectString) found in a platform invitation or presence.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_ParseLobbyIdFromConnectString")
	TEnumAsByte<EEIK_Result> EIK_Lobby_ParseLobbyIdFromConnectString(FString ConnectString, FEIK_LobbyId& OutLobbyId);

	//Creates a lobby modification handle (EOS_HLobbyModification). The lobby modification handle is used to modify an existing lobby and can be applied with EOS_Lobby_UpdateLobby. The EOS_HLobbyModification must be released by calling EOS_LobbyModification_Release once it is no longer needed.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_UpdateLobbyModification")
	TEnumAsByte<EEIK_Result> EIK_Lobby_UpdateLobbyModification(FEIK_ProductUserId LocalUserId, FEIK_LobbyId LobbyId, FEIK_HLobbyModification& OutLobbyModificationHandle);
	
	//EOS_LobbyDetails_CopyAttributeByIndex is used to immediately retrieve a copy of a lobby attribute from a given source such as a existing lobby or a search result. If the call returns an EOS_Success result, the out parameter, OutAttribute, must be passed to EOS_Lobby_Attribute_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_CopyAttributeByIndex")
	TEnumAsByte<EEIK_Result> EIK_LobbyDetails_CopyAttributeByIndex(FEIK_HLobbyDetails LobbyDetailsHandle, int32 AttrIndex, FEIK_Lobby_Attribute& OutAttribute);

	//EOS_LobbyDetails_CopyAttributeByKey is used to immediately retrieve a copy of a lobby attribute from a given source such as a existing lobby or a search result. If the call returns an EOS_Success result, the out parameter, OutAttribute, must be passed to EOS_Lobby_Attribute_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_CopyAttributeByKey")
	TEnumAsByte<EEIK_Result> EIK_LobbyDetails_CopyAttributeByKey(FEIK_HLobbyDetails LobbyDetailsHandle, const FString& AttrKey, FEIK_Lobby_Attribute& OutAttribute);

	//EOS_LobbyDetails_CopyInfo is used to immediately retrieve a copy of lobby information from a given source such as a existing lobby or a search result. If the call returns an EOS_Success result, the out parameter, OutLobbyDetailsInfo, must be passed to EOS_LobbyDetails_Info_Release to release the memory associated with it.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_CopyInfo")
	TEnumAsByte<EEIK_Result> EIK_LobbyDetails_CopyInfo(FEIK_HLobbyDetails LobbyDetailsHandle, FEIK_LobbyDetailsInfo& OutLobbyDetailsInfo);

	//EOS_LobbyDetails_CopyMemberAttributeByIndex is used to immediately retrieve a copy of a lobby member attribute from an existing lobby. If the call returns an EOS_Success result, the out parameter, OutAttribute, must be passed to EOS_Lobby_Attribute_Release to release the memory associated with it. Note: this information is only available if you are actively in the lobby. It is not available for search results.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_CopyMemberAttributeByIndex")
	TEnumAsByte<EEIK_Result> EIK_LobbyDetails_CopyMemberAttributeByIndex(FEIK_HLobbyDetails LobbyDetailsHandle, FEIK_ProductUserId TargetUserId, int32 AttrIndex, FEIK_Lobby_Attribute& OutAttribute);

	//EOS_LobbyDetails_CopyMemberAttributeByIndex is used to immediately retrieve a copy of a lobby member attribute from an existing lobby. If the call returns an EOS_Success result, the out parameter, OutAttribute, must be passed to EOS_Lobby_Attribute_Release to release the memory associated with it. Note: this information is only available if you are actively in the lobby. It is not available for search results.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_CopyMemberAttributeByKey")
	TEnumAsByte<EEIK_Result> EIK_LobbyDetails_CopyMemberAttributeByKey(FEIK_HLobbyDetails LobbyDetailsHandle, FEIK_ProductUserId TargetUserId, const FString& AttrKey, FEIK_Lobby_Attribute& OutAttribute);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_CopyMemberInfo")
	TEnumAsByte<EEIK_Result> EIK_LobbyDetails_CopyMemberInfo(FEIK_HLobbyDetails LobbyDetailsHandle, FEIK_ProductUserId TargetUserId, FEIK_LobbyDetails_MemberInfo& OutMemberInfo);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_GetAttributeCount")
	int32 EIK_LobbyDetails_GetAttributeCount(FEIK_HLobbyDetails LobbyDetailsHandle);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_GetLobbyOwner")
	FEIK_ProductUserId EIK_LobbyDetails_GetLobbyOwner(FEIK_HLobbyDetails LobbyDetailsHandle);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_GetMemberAttributeCount")
	int32 EIK_LobbyDetails_GetMemberAttributeCount(FEIK_HLobbyDetails LobbyDetailsHandle, FEIK_ProductUserId TargetUserId);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_GetMemberByIndex")
	FEIK_ProductUserId EIK_LobbyDetails_GetMemberByIndex(FEIK_HLobbyDetails LobbyDetailsHandle, int32 MemberIndex);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_GetMemberCount")
	int32 EIK_LobbyDetails_GetMemberCount(FEIK_HLobbyDetails LobbyDetailsHandle);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_Info_Release")
	void EIK_LobbyDetails_Info_Release(FEIK_LobbyDetailsInfo& LobbyDetailsInfo);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_MemberInfo_Release")
	void EIK_LobbyDetails_MemberInfo_Release(FEIK_LobbyDetails_MemberInfo& MemberInfo);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyDetails_Release")
	void EIK_LobbyDetails_Release(FEIK_HLobbyDetails LobbyDetailsHandle);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyModification_AddAttribute")
	TEnumAsByte<EEIK_Result> EIK_LobbyModification_AddAttribute(FEIK_HLobbyModification LobbyModificationHandle, const FEIK_Lobby_AttributeData& Attribute, const TEnumAsByte<EEIK_ELobbyAttributeVisibility>& Visibility);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyModification_AddMemberAttribute")
	TEnumAsByte<EEIK_Result> EIK_LobbyModification_AddMemberAttribute(FEIK_HLobbyModification LobbyModificationHandle, const FEIK_Lobby_AttributeData& Attribute, const TEnumAsByte<EEIK_ELobbyAttributeVisibility>& Visibility);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyModification_Release")
	void EIK_LobbyModification_Release(FEIK_HLobbyModification LobbyModificationHandle);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyModification_RemoveAttribute")
	TEnumAsByte<EEIK_Result> EIK_LobbyModification_RemoveAttribute(FEIK_HLobbyModification LobbyModificationHandle, const FString& Options);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyModification_RemoveMemberAttribute")
	TEnumAsByte<EEIK_Result> EIK_LobbyModification_RemoveMemberAttribute(FEIK_HLobbyModification LobbyModificationHandle, const FString& Options);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyModification_SetAllowedPlatformIds")
	TEnumAsByte<EEIK_Result> EIK_LobbyModification_SetAllowedPlatformIds(FEIK_HLobbyModification LobbyModificationHandle, const TArray<int32>& Options);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyModification_SetBucketId")
	TEnumAsByte<EEIK_Result> EIK_LobbyModification_SetBucketId(FEIK_HLobbyModification LobbyModificationHandle, const FString& Options);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyModification_SetInvitesAllowed")
	TEnumAsByte<EEIK_Result> EIK_LobbyModification_SetInvitesAllowed(FEIK_HLobbyModification LobbyModificationHandle, const bool& Options);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyModification_SetMaxMembers")
	TEnumAsByte<EEIK_Result> EIK_LobbyModification_SetMaxMembers(FEIK_HLobbyModification LobbyModificationHandle, const int32& Options);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbyModification_SetPermissionLevel")
	TEnumAsByte<EEIK_Result> EIK_LobbyModification_SetPermissionLevel(FEIK_HLobbyModification LobbyModificationHandle, const TEnumAsByte<EEIK_ELobbyPermissionLevel>& Options);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbySearch_CopySearchResultByIndex")
	TEnumAsByte<EEIK_Result> EIK_LobbySearch_CopySearchResultByIndex(FEIK_HLobbySearch LobbySearchHandle, int32 LobbyIndex, FEIK_HLobbyDetails& OutLobbyDetailsHandle);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbySearch_GetSearchResultCount")
	int32 EIK_LobbySearch_GetSearchResultCount(FEIK_HLobbySearch LobbySearchHandle);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbySearch_Release")
	void EIK_LobbySearch_Release(FEIK_HLobbySearch LobbySearchHandle);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbySearch_RemoveParameter")
	TEnumAsByte<EEIK_Result> EIK_LobbySearch_RemoveParameter(FEIK_HLobbySearch LobbySearchHandle, const FString& Key, const TEnumAsByte<EEIK_EComparisonOp>& ComparisonOp);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbySearch_SetLobbyId")
	TEnumAsByte<EEIK_Result> EIK_LobbySearch_SetLobbyId(FEIK_HLobbySearch LobbySearchHandle, const FEIK_LobbyId& Options);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbySearch_SetMaxResults")
	TEnumAsByte<EEIK_Result> EIK_LobbySearch_SetMaxResults(FEIK_HLobbySearch LobbySearchHandle, const int32 MaxResults);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbySearch_SetParameter")
	TEnumAsByte<EEIK_Result> EIK_LobbySearch_SetParameter(FEIK_HLobbySearch LobbySearchHandle, const FEIK_Lobby_AttributeData& Parameter, const TEnumAsByte<EEIK_EComparisonOp>& ComparisonOp);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_LobbySearch_SetTargetUserId")
	TEnumAsByte<EEIK_Result> EIK_LobbySearch_SetTargetUserId(FEIK_HLobbySearch LobbySearchHandle, FEIK_ProductUserId Options);
};
