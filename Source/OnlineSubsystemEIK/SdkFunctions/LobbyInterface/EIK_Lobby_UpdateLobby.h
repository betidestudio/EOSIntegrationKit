// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_UpdateLobby.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIKLobbyUpdateLobby, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_LobbyId&, LobbyId);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_UpdateLobby : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	//Update a lobby given a lobby modification handle created by EOS_Lobby_UpdateLobbyModification
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_UpdateLobby")
	static UEIK_Lobby_UpdateLobby* EIK_Lobby_UpdateLobby(FEIK_HLobbyModification LobbyModificationHandle);

	UPROPERTY(BlueprintAssignable)
	FEIKLobbyUpdateLobby OnCallback;
private:
	virtual void Activate() override;
	static void EOS_CALL OnUpdateLobbyComplete(const EOS_Lobby_UpdateLobbyCallbackInfo* Data);
	FEIK_HLobbyModification Var_LobbyModificationHandle;
};
