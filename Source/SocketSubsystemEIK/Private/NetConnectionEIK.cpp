// Copyright Epic Games, Inc. All Rights Reserved.

#include "NetConnectionEIK.h"
#include "NetDriverEIKBase.h"
#include "InternetAddrEIK.h"
#include "SocketEIK.h"
#if ENGINE_MAJOR_VERSION >= 5
#include UE_INLINE_GENERATED_CPP_BY_NAME(NetConnectionEIK)
#else
#include "OnlineSubsystem.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystemUtils.h"
#include "Misc/EngineVersionComparison.h"
#include "SocketSubsystemEIK.h"
#endif

UNetConnectionEIK::UNetConnectionEIK(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsPassthrough(false)
	, bHasP2PSession(false)
{
}

void UNetConnectionEIK::InitLocalConnection(UNetDriver* InDriver, FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket, int32 InPacketOverhead)
{
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 6
	// UE 5.6+ removed bIsUsingP2PSockets - always use EOS sockets when URL is EOS format
	bIsPassthrough = !InURL.Host.StartsWith(EOS_CONNECTION_URL_PREFIX, ESearchCase::IgnoreCase);
#else
	// UE 5.5 and below still check bIsUsingP2PSockets
	bIsPassthrough = !static_cast<UNetDriverEIKBase*>(InDriver)->bIsUsingP2PSockets || !InURL.Host.StartsWith(EOS_CONNECTION_URL_PREFIX, ESearchCase::IgnoreCase);
#endif
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
#if ENGINE_MAJOR_VERSION == 5
	FSocket* CurSocket = GetSocket();
#else
	if (!Socket)
	{
		return;
	}
	FSocket* CurSocket = Socket;
#endif
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

