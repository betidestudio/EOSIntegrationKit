//Copyright (c) 2023 Betide Studio. All Rights Reserved.

#include "NetConnectionEIK.h"
#include "NetDriverEIKBase.h"
#include "InternetAddrEIK.h"
#include "SocketEIK.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NetConnectionEIK)

UNetConnectionEIK::UNetConnectionEIK(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsPassthrough(false)
	, bHasP2PSession(false)
{
}

void UNetConnectionEIK::InitLocalConnection(UNetDriver* InDriver, FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket, int32 InPacketOverhead)
{
	bIsPassthrough = !static_cast<UNetDriverEIKBase*>(InDriver)->bIsUsingP2PSockets || !InURL.Host.StartsWith(EOS_CONNECTION_URL_PREFIX, ESearchCase::IgnoreCase);
	bHasP2PSession = !bIsPassthrough;
	  
	if (bHasP2PSession)
	{
		DisableAddressResolution();
	}

	Super::InitLocalConnection(InDriver, InSocket, InURL, InState, InMaxPacket, InPacketOverhead);

	// We re-set the original port, as if might have been overwritten by Super::InitLocalConnection
	if (bHasP2PSession)
	{
		RemoteAddr->SetPort(InSocket->GetPortNo());
	}
}

void UNetConnectionEIK::InitRemoteConnection(UNetDriver* InDriver, FSocket* InSocket, const FURL& InURL, const FInternetAddr& InRemoteAddr, EConnectionState InState, int32 InMaxPacket, int32 InPacketOverhead)
{
	bIsPassthrough = static_cast<UNetDriverEIKBase*>(InDriver)->bIsPassthrough;
	bHasP2PSession = !bIsPassthrough;

	if (bHasP2PSession)
	{
		DisableAddressResolution();
	}

	Super::InitRemoteConnection(InDriver, InSocket, InURL, InRemoteAddr, InState, InMaxPacket, InPacketOverhead);
}

void UNetConnectionEIK::CleanUp()
{
	Super::CleanUp();

	if (bHasP2PSession)
	{
		DestroyEOSConnection();
	}
}

void UNetConnectionEIK::DestroyEOSConnection()
{
	FSocket* CurSocket = GetSocket();

	if (CurSocket != nullptr && bHasP2PSession)
	{
		bHasP2PSession = false;

		TSharedPtr<FInternetAddrEOS> RemoteAddrEOS = StaticCastSharedPtr<FInternetAddrEOS>(RemoteAddr);

		if (RemoteAddrEOS.IsValid())
		{
			static_cast<FSocketEOS*>(CurSocket)->Close(*RemoteAddrEOS);
		}
	}
}

