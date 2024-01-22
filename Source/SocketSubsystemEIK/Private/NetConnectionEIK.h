// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IpConnection.h"
#include "NetConnectionEIK.generated.h"

UCLASS(Transient, Config=Engine)
class SOCKETSUBSYSTEMEIK_API UNetConnectionEIK
	: public UIpConnection
{
	GENERATED_BODY()

public:
	explicit UNetConnectionEIK(const FObjectInitializer& ObjectInitializer);

//~ Begin NetConnection Interface
	virtual void InitLocalConnection(UNetDriver* InDriver, FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void InitRemoteConnection(UNetDriver* InDriver, FSocket* InSocket, const FURL& InURL, const FInternetAddr& InRemoteAddr, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void CleanUp() override;
//~ End NetConnection Interface

	void DestroyEOSConnection();

public:
	bool bIsPassthrough;

protected:
	bool bHasP2PSession;
};
