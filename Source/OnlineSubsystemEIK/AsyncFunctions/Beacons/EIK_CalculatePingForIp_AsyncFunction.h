// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Launch/Resources/Version.h"
#include "EIK_CalculatePingForIp_AsyncFunction.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_OnCalculatePingForIpComplete, int32, Ping);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_CalculatePingForIp_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | Beacons | Ping", meta = (BlueprintInternalUseOnly = "true"), DisplayName = "Calculate Ping For Ip")
	static UEIK_CalculatePingForIp_AsyncFunction* CalculatePingForIp(FString IpAddress, int32 Port, bool bPortOverride);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Beacons | Ping")
	FEIK_OnCalculatePingForIpComplete OnSuccess;

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Beacons | Ping")
	FEIK_OnCalculatePingForIpComplete OnFailure;

private:
	UFUNCTION()
	void OnPingComplete(int32 Ping, bool bSuccess);
	virtual void Activate() override;

	FString Var_IpAddress;
	int32 Var_Port;
	bool Var_bPortOverride;
};
