// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Engine/World.h"
#include "OnlineSubsystemEIK/Subsystem/EIK_Subsystem.h"
#include "EIK_CalculatePingForSession_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_OnCalculatePingForSessionComplete, int32, Ping);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_CalculatePingForSession_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | Beacons | Ping", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), DisplayName = "Calculate Ping For Session")
	static UEIK_CalculatePingForSession_AsyncFunction* CalculatePingForSession(FSessionFindStruct Session, UObject* WorldContextObject);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Beacons | Ping")
	FEIK_OnCalculatePingForSessionComplete OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Beacons | Ping")
	FEIK_OnCalculatePingForSessionComplete OnFailure;

private:
	UFUNCTION()
	void OnPingComplete(int32 Ping, bool bSuccess);
	virtual void Activate() override;

	FSessionFindStruct Var_SessionFindStruct;

	UPROPERTY()
	UObject* WorldContextObject;
};
