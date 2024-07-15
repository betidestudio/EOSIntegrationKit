// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_QueryInvites.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIKLobbyQueryInvites, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_LobbyId&, LobbyId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_QueryInvites : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Retrieve all existing invites for a single user
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_QueryInvites")
	static UEIK_Lobby_QueryInvites* EIK_Lobby_QueryInvites(FEIK_ProductUserId LocalUserId);

	UPROPERTY(BlueprintAssignable)
	FEIKLobbyQueryInvites OnCallback;

private:
	static void EOS_CALL OnQueryInvitesComplete(const EOS_Lobby_QueryInvitesCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_ProductUserId Var_LocalUserId;
};
