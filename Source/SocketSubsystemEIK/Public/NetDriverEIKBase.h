//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IpNetDriver.h"
#include "NetDriverEIKBase.generated.h"

class ISocketSubsystem;

UCLASS(Transient, Config=Engine)
class SOCKETSUBSYSTEMEIK_API UNetDriverEIKBase
	: public UIpNetDriver
{
	GENERATED_BODY()

public:
//~ Begin UNetDriver Interface
	virtual bool IsAvailable() const override;
	virtual bool InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) override;
	virtual bool InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error) override;
	virtual bool InitListen(FNetworkNotify* InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error) override;
	virtual ISocketSubsystem* GetSocketSubsystem() override;
	virtual void Shutdown() override;
	virtual int GetClientPort() override;
//~ End UNetDriver Interface

	UWorld* FindWorld() const;

public:
	UPROPERTY()
	bool bIsPassthrough;

	UPROPERTY(Config)
	bool bIsUsingP2PSockets;
};
