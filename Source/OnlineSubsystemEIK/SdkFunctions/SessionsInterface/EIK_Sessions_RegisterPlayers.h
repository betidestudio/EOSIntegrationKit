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
#include "EIK_Sessions_RegisterPlayers.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_OnRegisterPlayersCallback, const TEnumAsByte<EEIK_Result>&, ResultCode, const TArray<FEIK_ProductUserId>&, RegisteredPlayers, const TArray<FEIK_ProductUserId>&, SanctionedPlayers);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sessions_RegisterPlayers : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Register a group of players with the session, allowing them to invite others or otherwise indicate they are part of the session for determining a full session
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_RegisterPlayers")
	static UEIK_Sessions_RegisterPlayers* EIK_Sessions_RegisterPlayers(FString SessionName, TArray<FEIK_ProductUserId> PlayersToRegister);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_OnRegisterPlayersCallback OnCallback;

private:
	FString Var_SessionName;
	TArray<FEIK_ProductUserId> Var_PlayersToRegister;
	virtual void Activate() override;
	static void EOS_CALL OnRegisterPlayersCallback(const EOS_Sessions_RegisterPlayersCallbackInfo* Data);
};
