// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_CreateLobby.generated.h"

//Input parameters for the EOS_Lobby_CreateLobby function.
USTRUCT(BlueprintType)
struct FEIK_Lobby_CreateLobbyOptions
{
	GENERATED_BODY()

	//The Product User ID of the local user creating the lobby; this user will automatically join the lobby as its owner
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_ProductUserId LocalUserId;

	//The maximum number of users who can be in the lobby at a time
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	int32 MaxLobbyMembers;

	//The initial permission level of the lobby
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	TEnumAsByte<EEIK_ELobbyPermissionLevel> PermissionLevel;

	//If true, this lobby will be associated with the local user's presence information. A user's presence can only be associated with one lobby at a time. This affects the ability of the Social Overlay to show game related actions to take in the user's social graph. * using the bPresenceEnabled flags within the Sessions interface * using the bPresenceEnabled flags within the Lobby interface * using EOS_PresenceModification_SetJoinInfo
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bPresenceEnabled;

	//Are members of the lobby allowed to invite others
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bAllowInvites;

	//Bucket ID associated with the lobby
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FString BucketId;

	//Is host migration allowed (will the lobby stay open if the original host leaves?) NOTE: EOS_Lobby_PromoteMember is still allowed regardless of this setting
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bDisableHostMigration;

	//Creates a real-time communication (RTC) room for all members of this lobby. All members of the lobby will automatically join the RTC room when they connect to the lobby and they will automatically leave the RTC room when they leave or are removed from the lobby. While the joining and leaving of the RTC room is automatic, applications will still need to use the EOS RTC interfaces to handle all other functionality for the room.
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bEnableRTCRoom;

	//(Optional) Allows the local application to set local audio options for the RTC Room if it is enabled. Set this to NULL if the RTC RTC room is disabled or you would like to use the defaults.
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_LocalRTCOptions LocalRTCOptions;

	//(Optional) Set to a globally unique value to override the backend assignment If not specified the backend service will assign one to the lobby. Do not mix and match override and non override settings. This value can be of size [EOS_LOBBY_MIN_LOBBYIDOVERRIDE_LENGTH, EOS_LOBBY_MAX_LOBBYIDOVERRIDE_LENGTH]
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_LobbyId LobbyId;

	//Is EOS_Lobby_JoinLobbyById allowed. This is provided to support cases where an integrated platform's invite system is used. In these cases the game should provide the lobby ID securely to the invited player. Such as by attaching the lobby ID to the integrated platform's session data or sending the lobby ID within the invite data.
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bEnableJoinById;

	//Does rejoining after being kicked require an invite? When this is set, a kicked player cannot return to the session even if the session was set with EOS_LPL_PUBLICADVERTISED. When this is set, a player with invite privileges must use EOS_Lobby_SendInvite to allow the kicked player to return to the session.
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bRejoinAfterKickRequiresInvite;

	//Array of platform IDs indicating the player platforms allowed to register with the session. Platform IDs are found in the EOS header file, e.g. EOS_OPT_Epic. For some platforms, the value will be in the EOS Platform specific header file. If null, the lobby will be unrestricted.
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	TArray<int32> AllowedPlatformIds;

	//This value indicates whether or not the lobby owner allows crossplay interactions. If false, the lobby owner will be treated as allowing crossplay. If it is set to true, AllowedPlatformIds must have a single entry that matches the platform of the lobby owner.
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bCrossplayOptOut;

	FEIK_Lobby_CreateLobbyOptions()
	{
		MaxLobbyMembers = 0;
		PermissionLevel = EEIK_ELobbyPermissionLevel::EIK_LPL_InviteOnly;
		bPresenceEnabled = false;
		bAllowInvites = false;
		bDisableHostMigration = false;
		bEnableRTCRoom = false;
		bEnableJoinById = false;
		bRejoinAfterKickRequiresInvite = false;
		bCrossplayOptOut = false;
	}
	EOS_Lobby_CreateLobbyOptions GetCreateLobbyOptions()
	{
		EOS_Lobby_CreateLobbyOptions Options;
		Options.ApiVersion = EOS_LOBBY_CREATELOBBY_API_LATEST;
		Options.LocalUserId = LocalUserId.GetValueAsEosType();
		Options.MaxLobbyMembers = MaxLobbyMembers;
		Options.PermissionLevel = static_cast<EOS_ELobbyPermissionLevel>(PermissionLevel.GetValue());
		Options.bPresenceEnabled = bPresenceEnabled;
		Options.bAllowInvites = bAllowInvites;
		Options.BucketId = TCHAR_TO_ANSI(*BucketId);
		Options.bDisableHostMigration = bDisableHostMigration;
		Options.bEnableRTCRoom = bEnableRTCRoom;
		EOS_Lobby_LocalRTCOptions TempValue = LocalRTCOptions.GetValueAsEosType();
		Options.LocalRTCOptions = &TempValue;
		Options.LobbyId = LobbyId.GetValueAsEosType();
		Options.bEnableJoinById = bEnableJoinById;
		Options.bRejoinAfterKickRequiresInvite = bRejoinAfterKickRequiresInvite;
		Options.AllowedPlatformIdsCount = AllowedPlatformIds.Num();
		uint32_t* TempVar = new uint32_t[AllowedPlatformIds.Num()];
		for (int32 i = 0; i < AllowedPlatformIds.Num(); i++)
		{
			TempVar[i] = AllowedPlatformIds[i];
		}
		Options.AllowedPlatformIds = TempVar;
		Options.bCrossplayOptOut = bCrossplayOptOut;
		return Options;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Lobby_CreateLobbyComplete, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_LobbyId&, LobbyId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_CreateLobby : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Creates a lobby and adds the user to the lobby membership. There is no data associated with the lobby at the start and can be added vis EOS_Lobby_UpdateLobbyModification If the lobby is successfully created with an RTC Room enabled, the lobby system will automatically join and maintain the connection to the RTC room as long as the local user remains in the lobby. Applications can use the EOS_Lobby_GetRTCRoomName to get the name of the RTC Room associated with a lobby, which may be used with many of the EOS_RTC_* suite of functions. This can be useful to: register for notifications for talking status; to mute or unmute the local user's audio output; to block or unblock room participants; to set local audio device settings; and more.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_CreateLobby")
	static UEIK_Lobby_CreateLobby* EIK_Lobby_CreateLobby(FEIK_Lobby_CreateLobbyOptions CreateLobbyOptions);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_CreateLobbyComplete OnCallback;
	
private:
	FEIK_Lobby_CreateLobbyOptions Var_CreateLobbyOptions;
	static void EOS_CALL OnCreateLobbyComplete(const EOS_Lobby_CreateLobbyCallbackInfo* Data);
	virtual void Activate() override;
};
