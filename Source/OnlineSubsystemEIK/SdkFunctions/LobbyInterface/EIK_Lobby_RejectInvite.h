// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_RejectInvite.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIKLobbyRejectInvite, const TEnumAsByte<EEIK_Result>&, ResultCode, const FString&, InviteId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_RejectInvite : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	//Reject an invite from another user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_RejectInvite")
	static UEIK_Lobby_RejectInvite* EIK_Lobby_RejectInvite(FEIK_ProductUserId LocalUserId, FString InviteId);

	UPROPERTY(BlueprintAssignable)
	FEIKLobbyRejectInvite OnCallback;

private:
	static void EOS_CALL OnRejectInviteComplete(const EOS_Lobby_RejectInviteCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_ProductUserId Var_LocalUserId;
	FString Var_InviteId;
};
