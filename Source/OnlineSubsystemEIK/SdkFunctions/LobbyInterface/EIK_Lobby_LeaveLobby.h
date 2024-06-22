// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_LeaveLobby.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIKLobbyLeaveLobby, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_LobbyId&, LobbyId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_LeaveLobby : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	//Leave a lobby given a lobby ID If the lobby you are leaving had an RTC Room enabled, leaving the lobby will also automatically leave the RTC room.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_LeaveLobby")
	static UEIK_Lobby_LeaveLobby* EIK_Lobby_LeaveLobby(FEIK_ProductUserId LocalUserId, FEIK_LobbyId LobbyId);

	UPROPERTY(BlueprintAssignable)
	FEIKLobbyLeaveLobby OnCallback;
	
private:
	static void EOS_CALL OnLeaveLobbyComplete(const EOS_Lobby_LeaveLobbyCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_ProductUserId Var_LocalUserId;
	FEIK_LobbyId Var_LobbyId;
};
