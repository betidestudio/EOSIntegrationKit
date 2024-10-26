// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "PingHostObject.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPingHostObject, Log, All);

/**
* 
*/
UCLASS(Blueprintable, BlueprintType, Transient, NotPlaceable, Config = Engine)
class ONLINESUBSYSTEMEIK_API APingHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()

public:
	APingHostObject(const FObjectInitializer& ObjectInitializer);

	virtual AOnlineBeaconClient* SpawnBeaconActor(class UNetConnection* ClientConnection) override;

	virtual void OnClientConnected(class AOnlineBeaconClient* ClientActor, class UNetConnection* ClientConnection) override;

	virtual bool Init();
};