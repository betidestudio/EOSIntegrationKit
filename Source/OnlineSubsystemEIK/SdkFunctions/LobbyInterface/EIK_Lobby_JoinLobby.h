// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_JoinLobby.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Lobby_JoinLobbyOptions
{
	GENERATED_BODY()

	//The handle of the lobby to join
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_HLobbyDetails LobbyDetailsHandle;

	//The Product User ID of the local user joining the lobby
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_ProductUserId LocalUserId;

	//If true, this lobby will be associated with the local user's presence information. A user can only associate one lobby at a time with their presence information. This affects the ability of the Social Overlay to show game related actions to take in the user's social graph. * using the bPresenceEnabled flags within the Sessions interface * using the bPresenceEnabled flags within the Lobby interface * using EOS_PresenceModification_SetJoinInfo
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bPresenceEnabled;

	//(Optional) Set this value to override the default local options for the RTC Room, if it is enabled for this lobby. Set this to NULL if your application does not use the Lobby RTC Rooms feature, or if you would like to use the default settings. This option is ignored if the specified lobby does not have an RTC Room enabled and will not cause errors.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_LocalRTCOptions LobbyRTCOptions;

	//This value indicates whether or not the local user allows crossplay interactions. If it is false, the local user will be treated as allowing crossplay.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bCrossplayOptOut;

	FEIK_Lobby_JoinLobbyOptions()
	{
		bPresenceEnabled = false;
		bCrossplayOptOut = false;
	}
	EOS_Lobby_JoinLobbyOptions ToEOSOptions()
	{
		EOS_Lobby_JoinLobbyOptions Options;
		Options.ApiVersion = EOS_LOBBY_JOINLOBBY_API_LATEST;
		Options.LobbyDetailsHandle = LobbyDetailsHandle.Ref;
		Options.LocalUserId = LocalUserId.GetValueAsEosType();
		Options.bPresenceEnabled = bPresenceEnabled ? EOS_TRUE : EOS_FALSE;
		EOS_Lobby_LocalRTCOptions VarTemp = LobbyRTCOptions.GetValueAsEosType();
		Options.LocalRTCOptions = &VarTemp;
		Options.bCrossplayOptOut = bCrossplayOptOut ? EOS_TRUE : EOS_FALSE;
		return Options;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Lobby_JoinLobbyDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_LobbyId&, LobbyId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_JoinLobby : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Join a lobby, creating a local instance under a given lobby ID. Backend will validate various conditions to make sure it is possible to join the lobby. If the lobby is successfully join has an RTC Room enabled, the lobby system will automatically join and maintain the connection to the RTC room as long as the local user remains in the lobby. Applications can use the EOS_Lobby_GetRTCRoomName to get the name of the RTC Room associated with a lobby, which may be used with many of the EOS_RTC_* suite of functions. This can be useful to: register for notifications for talking status; to mute or unmute the local user's audio output; to block or unblock room participants; to set local audio device settings; and more.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_JoinLobby")
	static UEIK_Lobby_JoinLobby* EIK_Lobby_JoinLobby(FEIK_Lobby_JoinLobbyOptions Options);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_JoinLobbyDelegate OnCallback;
	
private:
	static void EOS_CALL OnJoinLobbyComplete(const EOS_Lobby_JoinLobbyCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_Lobby_JoinLobbyOptions Var_Options;
};
