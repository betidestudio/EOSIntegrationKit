// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_sessions.h"
#include "eos_sessions_types.h"
THIRD_PARTY_INCLUDES_END
#include "EIK_Sessions_UnregisterPlayers.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_OnUnregisterPlayersCallback, const TEnumAsByte<EEIK_Result>&, ResultCode, const TArray<FEIK_ProductUserId>&, UnregisteredPlayers);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sessions_UnregisterPlayers : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Unregister a group of players with the session, freeing up space for others to join
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_UnregisterPlayers")
	static UEIK_Sessions_UnregisterPlayers* EIK_Sessions_UnregisterPlayers(FString SessionName, TArray<FEIK_ProductUserId> PlayersToUnregister);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_OnUnregisterPlayersCallback OnCallback;

private:
	FString Var_SessionName;
	TArray<FEIK_ProductUserId> Var_PlayersToUnregister;
	virtual void Activate() override;
	static void EOS_CALL OnUnregisterPlayersCallback(const EOS_Sessions_UnregisterPlayersCallbackInfo* Data);
};
