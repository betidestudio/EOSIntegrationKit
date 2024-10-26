// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#include "PingClient.h"

#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"

DEFINE_LOG_CATEGORY(LogPingClient)

APingClient::APingClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void APingClient::OnFailure()
{
	Super::OnFailure();
	UE_LOG(LogPingClient, Log, TEXT("APingClient connection failed"));
	OnPingComplete.ExecuteIfBound(0, false);
	DestroyBeacon();
}

void APingClient::ClientPingBegin_Implementation()
{
	UE_LOG(LogPingClient, Log, TEXT("APingClient::ClientPingBegin_Implementation"));
	PingBeginTime = FDateTime::Now().GetTicks();
	ServerPong();
}

bool APingClient::ServerPong_Validate()
{
	return true;
}

void APingClient::ServerPong_Implementation()
{
	UE_LOG(LogPingClient, Log, TEXT("APingClient::ServerPong_Implementation"));
	ClientPingEnd();
}

void APingClient::ClientPingEnd_Implementation()
{
	UE_LOG(LogPingClient, Log, TEXT("APingClient::ClientPingEnd_Implementation"));
	int64 PingTime = FDateTime::Now().GetTicks() - PingBeginTime;
	PingMS = (int32)(PingTime / 10000);
	UE_LOG(LogPingClient, Log, TEXT("Ping Complete: %d ms"), PingMS);
	OnPingComplete.ExecuteIfBound(PingMS, true);
	DestroyBeacon();
}

bool APingClient::ConnectToHost(FString Address, int32 Port, const bool bPortOverride, FEIK_PingComplete Ref)
{
	OnPingComplete = Ref;
	FURL url(nullptr, *Address, ETravelType::TRAVEL_Absolute);
	if (bPortOverride)
	{
		url.Port = Port;
	}
	else
	{
		int32 ListenPort;
		if (GConfig->GetInt(TEXT("/Script/OnlineSubsystemUtils.OnlineBeaconHost"), TEXT("ListenPort"), ListenPort, GEngineIni))
		{
			url.Port = ListenPort;
		}
		else
		{
			url.Port = 8888;
		}
	}
	UE_LOG(LogPingClient, Log, TEXT("Connecting to %s:%d"), *url.Host, url.Port);
	return InitClient(url);
}

bool APingClient::ConnectToSession(FBlueprintSessionResult SearchResult, FEIK_PingComplete Ref)
{
	OnPingComplete = Ref;
	if(IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld()))
	{
		if(IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface())
		{
			FString Address;
			Sessions->GetResolvedConnectString(SearchResult.OnlineResult, EName::BeaconPort, Address);
			UE_LOG(LogPingClient, Log, TEXT("Resolved Address: %s"), *Address);
			return ConnectToHost(Address, 0, false, Ref);
		}
	}
	UE_LOG(LogPingClient, Log, TEXT("Failed to connect to session"));
	OnPingComplete.ExecuteIfBound(0, false);
	return false;
}

void APingClient::Disconnect()
{
	DestroyBeacon();
}
