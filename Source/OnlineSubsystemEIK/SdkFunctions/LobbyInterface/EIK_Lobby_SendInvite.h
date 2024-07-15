// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_SendInvite.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIKLobbySendInvite, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_LobbyId&, LobbyId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_SendInvite : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	//Send an invite to another user. User must be a member of the lobby or else the call will fail
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_SendInvite")
	static UEIK_Lobby_SendInvite* EIK_Lobby_SendInvite(FEIK_ProductUserId LocalUserId, FEIK_LobbyId LobbyId, FEIK_ProductUserId TargetUserId);

	UPROPERTY(BlueprintAssignable)
	FEIKLobbySendInvite OnCallback;
private:
	virtual void Activate() override;
	static void EOS_CALL OnSendInviteComplete(const EOS_Lobby_SendInviteCallbackInfo* Data);
	FEIK_ProductUserId Var_LocalUserId;
	FEIK_LobbyId Var_LobbyId;
	FEIK_ProductUserId Var_TargetUserId;
};
