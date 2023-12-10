// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "eos_common.h"
#include "eos_sdk.h"
#include "eos_anticheatserver.h"
#include "eos_anticheatcommon_types.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AntiCheatServer.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAntiCheatActionRequired, APlayerController*, ControllerRef, bool, bRemoveFromSession);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAntiCheatRegisterClient, APlayerController*, ControllerRef, const TArray<uint8>&, ClientData);
UCLASS()
class ONLINESUBSYSTEMEIK_API UAntiCheatServer : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|AntiCheat")
	static void PrintAdvancedLogs(FString Log);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|AntiCheat")
	bool RegisterAntiCheatServer(FString ServerName, FString ClientProductID);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|AntiCheat")
	bool RegisterClientForAntiCheat(FString ClientProductID, APlayerController* ControllerRef);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|AntiCheat")
	bool RecievedMessageFromClient(APlayerController* Controller, const TArray<uint8>& Message);

	EOS_NotificationId MessageToClientId;
	
	EOS_NotificationId ClientActionRequiredId;
	
	static void EOS_CALL OnMessageToClientCb(const EOS_AntiCheatCommon_OnMessageToClientCallbackInfo* Data);
	static void EOS_CALL OnClientActionRequiredCb(const EOS_AntiCheatCommon_OnClientActionRequiredCallbackInfo* Data);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit|AntiCheat")
	FAntiCheatActionRequired OnAntiCheatActionRequired;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit|AntiCheat")
	FAntiCheatRegisterClient OnAntiCheatRegisterClient;
};
