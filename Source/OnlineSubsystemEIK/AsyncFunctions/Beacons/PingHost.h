// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHost.h"
#include "PingHost.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPingHost, Log, All);

/**
* 
*/
UCLASS(Blueprintable, BlueprintType, Transient, NotPlaceable, Config=Engine)
class ONLINESUBSYSTEMEIK_API APingHost : public AOnlineBeaconHost
{
	GENERATED_BODY()	

public:
	APingHost(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category=Ping)
	bool InitializeHost();

	UFUNCTION(BlueprintCallable, Category=Ping)
	void RegisterHostObject(AOnlineBeaconHostObject* HostObject);

protected:
	bool bInitSuccess;
};