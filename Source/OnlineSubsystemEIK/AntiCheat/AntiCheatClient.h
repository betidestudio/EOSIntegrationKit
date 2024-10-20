// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "eos_common.h"
#include "eos_sdk.h"
#include "eos_anticheatclient.h"
#include "EIKSettings.h"
#include "eos_anticheatcommon_types.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AntiCheatClient.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSendMessageToServer, const TArray<uint8>&, Message);
UCLASS()
class ONLINESUBSYSTEMEIK_API UAntiCheatClient : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "EOS Integration Kit|AntiCheat", meta = (WorldContext = "WorldContextObject"))
	static bool IsAntiCheatClientAvailable(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|AntiCheat")
	bool RegisterAntiCheatClient(FString ClientProductID);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|AntiCheat")
	bool RecievedMessageFromServer(const TArray<uint8>& Message);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|AntiCheat", meta = (Keywords = "EOS_AntiCheatClient_EndSession"))
	bool UnregisterAntiCheatClient();
	
	static void EOS_CALL OnMessageToServerCallback(const EOS_AntiCheatClient_OnMessageToServerCallbackInfo* Data);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit|AntiCheat")
	FSendMessageToServer OnSendMessageToServer;

	void PrintAdvancedLogs(const FString& Log) const
	{
		if(const UEIKSettings* EIKSettings = GetMutableDefault<UEIKSettings>())
		{
			{
				UE_LOG(LogEIK,Warning, TEXT("EIK: %s"), *Log);
			}
		}
	}

	EOS_NotificationId MessageToServerId;
};
