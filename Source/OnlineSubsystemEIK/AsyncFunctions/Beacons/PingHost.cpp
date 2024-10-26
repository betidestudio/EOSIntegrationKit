// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#include "PingHost.h"

DEFINE_LOG_CATEGORY(LogPingHost)

APingHost::APingHost(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BeaconState = EBeaconState::AllowRequests;
}

bool APingHost::InitializeHost()
{
	bInitSuccess = InitHost();
	return bInitSuccess;
}

void APingHost::RegisterHostObject(AOnlineBeaconHostObject* HostObject)
{
	if (bInitSuccess)
	{
		RegisterHost(HostObject);
	}
}