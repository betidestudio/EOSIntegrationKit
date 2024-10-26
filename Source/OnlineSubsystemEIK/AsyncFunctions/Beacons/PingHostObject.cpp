// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#include "PingHostObject.h"
#include "PingClient.h"

DEFINE_LOG_CATEGORY(LogPingHostObject)

APingHostObject::APingHostObject(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ClientBeaconActorClass = APingClient::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

AOnlineBeaconClient* APingHostObject::SpawnBeaconActor(UNetConnection* ClientConnection)
{
	return Super::SpawnBeaconActor(ClientConnection);
}

void APingHostObject::OnClientConnected(AOnlineBeaconClient* ClientActor, UNetConnection* ClientConnection)
{
	Super::OnClientConnected(ClientActor, ClientConnection);
	APingClient* BeaconClient = Cast<APingClient>(ClientActor);
	if (BeaconClient != nullptr)
	{
		BeaconClient->ClientPingBegin();
	}
}

bool APingHostObject::Init()
{
	return true;
}