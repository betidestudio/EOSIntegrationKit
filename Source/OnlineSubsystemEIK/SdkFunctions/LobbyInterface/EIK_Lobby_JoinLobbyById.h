// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_JoinLobbyById.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Lobby_JoinLobbyByIdOptions
{
	GENERATED_BODY()

	//The ID of the lobby
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_LobbyId LobbyId;

	//The Product User ID of the local user joining the lobby
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_ProductUserId LocalUserId;

	//If true, this lobby will be associated with the user's presence information. A user can only associate one lobby at a time with their presence information. This affects the ability of the Social Overlay to show game related actions to take in the user's social graph. * using the bPresenceEnabled flags within the Sessions interface * using the bPresenceEnabled flags within the Lobby interface * using EOS_PresenceModification_SetJoinInfo
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bPresenceEnabled;

	//(Optional) Set this value to override the default local options for the RTC Room, if it is enabled for this lobby. Set this to NULL if your application does not use the Lobby RTC Rooms feature, or if you would like to use the default settings. This option is ignored if the specified lobby does not have an RTC Room enabled and will not cause errors.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_LocalRTCOptions LobbyRTCOptions;

	//This value indicates whether or not the local user allows crossplay interactions. If it is false, the local user will be treated as allowing crossplay.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	bool bCrossplayOptOut;

	FEIK_Lobby_JoinLobbyByIdOptions()
	{
		bPresenceEnabled = false;
		bCrossplayOptOut = false;
	}
	EOS_Lobby_JoinLobbyByIdOptions ToEOSOptions()
	{
		EOS_Lobby_JoinLobbyByIdOptions Options;
		Options.ApiVersion = EOS_LOBBY_JOINLOBBY_API_LATEST;
		Options.LobbyId = LobbyId.Ref;
		Options.LocalUserId = LocalUserId.GetValueAsEosType();
		Options.bPresenceEnabled = bPresenceEnabled ? EOS_TRUE : EOS_FALSE;
		EOS_Lobby_LocalRTCOptions VarTemp = LobbyRTCOptions.GetValueAsEosType();
		Options.LocalRTCOptions = &VarTemp;
		Options.bCrossplayOptOut = bCrossplayOptOut ? EOS_TRUE : EOS_FALSE;
		return Options;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Lobby_JoinLobbyByIdDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_LobbyId&, LobbyId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_JoinLobbyById : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//This is a special case of EOS_Lobby_JoinLobby. It should only be used if the lobby has had Join-by-ID enabled. Additionally, Join-by-ID should only be enabled to support native invites on an integrated platform.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_JoinLobbyById")
	static UEIK_Lobby_JoinLobbyById* EIK_Lobby_JoinLobbyById(FEIK_Lobby_JoinLobbyByIdOptions Options);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_JoinLobbyByIdDelegate OnCallback;
private:
	static void EOS_CALL OnJoinLobbyByIdComplete(const EOS_Lobby_JoinLobbyByIdCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_Lobby_JoinLobbyByIdOptions Var_Options;
};
