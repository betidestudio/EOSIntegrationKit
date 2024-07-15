// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Lobby_DestroyLobby.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Lobby_DestroyLobbyDelegate, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_LobbyId&, LobbyId);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Lobby_DestroyLobby : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	/**Destroy a lobby given a lobby ID */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface", DisplayName="EOS_Lobby_DestroyLobby")
	static UEIK_Lobby_DestroyLobby* EIK_Lobby_DestroyLobby(FEIK_ProductUserId LocalUserId, FEIK_LobbyId LobbyId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Lobby Interface")
	FEIK_Lobby_DestroyLobbyDelegate OnCallback;

private:
	static void EOS_CALL OnDestroyLobbyComplete(const EOS_Lobby_DestroyLobbyCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_ProductUserId Var_LocalUserId;
	FEIK_LobbyId Var_LobbyId;
};
