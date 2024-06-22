// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_PromoteMember.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIKLobbyPromoteMember, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_LobbyId&, LobbyId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_PromoteMember : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	//Promote an existing member of the lobby to owner, allowing them to make lobby data modifications
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_PromoteMember")
	static UEIK_Lobby_PromoteMember* EIK_Lobby_PromoteMember(FEIK_LobbyId LobbyId, FEIK_ProductUserId ProductUserId, FEIK_ProductUserId TargetUserId);

	UPROPERTY(BlueprintAssignable)
	FEIKLobbyPromoteMember OnCallback;
	
private:
	static void EOS_CALL OnPromoteMemberComplete(const EOS_Lobby_PromoteMemberCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_LobbyId Var_LobbyId;
	FEIK_ProductUserId Var_ProductUserId;
	FEIK_ProductUserId Var_TargetUserId;
};
