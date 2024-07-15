// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_KickMember.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIKLobbyKickMember, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_LobbyId&, LobbyId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_KickMember : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Kick an existing member from the lobby
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_KickMember")
	static UEIK_Lobby_KickMember* EIK_Lobby_KickMember(FEIK_LobbyId LobbyId, FEIK_ProductUserId ProductUserId, FEIK_ProductUserId TargetUserId);

	UPROPERTY(BlueprintAssignable)
	FEIKLobbyKickMember OnCallback;

private:
	static void EOS_CALL OnKickMemberComplete(const EOS_Lobby_KickMemberCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_LobbyId Var_LobbyId;
	FEIK_ProductUserId Var_ProductUserId;
	FEIK_ProductUserId Var_TargetUserId;
};
