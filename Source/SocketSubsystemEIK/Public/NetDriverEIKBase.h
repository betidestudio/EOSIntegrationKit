// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IpNetDriver.h"
#include "OnlineBeacon.h"
#include "NetDriverEIKBase.generated.h"

class ISocketSubsystem;

UCLASS(Transient, Config=Engine)
class SOCKETSUBSYSTEMEIK_API UNetDriverEIKBase
	: public UIpNetDriver
{
	GENERATED_BODY()

public:
	UNetDriverEIKBase(const FObjectInitializer& ObjectInitializer);
//~ Begin UNetDriver Interface
	virtual bool IsAvailable() const override;
	virtual bool InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) override;
	virtual bool InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error) override;
	virtual bool InitListen(FNetworkNotify* InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error) override;
	virtual ISocketSubsystem* GetSocketSubsystem() override;
	virtual void Shutdown() override;
	virtual int GetClientPort() override;
	bool IsBeaconDriver() const;
//~ End UNetDriver Interface

	UWorld* FindWorld() const;

public:
	UPROPERTY()
	bool bIsPassthrough = false;

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 6
	UE_DEPRECATED(5.6, "bIsUsingP2PSockets is deprecated. All code that used it now operates as if it were true")
#endif
	UPROPERTY(Config)
	bool bIsUsingP2PSockets = true;
};
