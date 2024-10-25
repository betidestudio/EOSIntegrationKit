// Copyright (c) 2024 Betide Studio. All Rights Reserved.


#include "EIK_CalculatePingForSession_AsyncFunction.h"

#include "PingClient.h"

UEIK_CalculatePingForSession_AsyncFunction* UEIK_CalculatePingForSession_AsyncFunction::CalculatePingForSession(
	FSessionFindStruct SessionFindStruct)
{
	UEIK_CalculatePingForSession_AsyncFunction* BlueprintNode = NewObject<UEIK_CalculatePingForSession_AsyncFunction>();
	BlueprintNode->Var_SessionFindStruct = SessionFindStruct;
	return BlueprintNode;
}

void UEIK_CalculatePingForSession_AsyncFunction::OnPingComplete(int32 Ping, bool bSuccess)
{
	if(bSuccess)
	{
		OnSuccess.Broadcast(Ping);
	}
	else
	{
		OnFailure.Broadcast(Ping);
	}
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_CalculatePingForSession_AsyncFunction::Activate()
{
	Super::Activate();
	if(GetWorld())
	{
		if(APingClient* PingClient = GetWorld()->SpawnActor<APingClient>(APingClient::StaticClass()))
		{
			FEIK_PingComplete OnPingComplete;
			OnPingComplete.BindDynamic(this, &UEIK_CalculatePingForSession_AsyncFunction::OnPingComplete);
			if(!PingClient->ConnectToSession(Var_SessionFindStruct.SessionResult, OnPingComplete))
			{
				OnFailure.Broadcast(0);
				SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
				MarkAsGarbage();
#else
				MarkPendingKill();
#endif
			}
		}
		else
		{
			OnFailure.Broadcast(0);
			SetReadyToDestroy();
		}
	}
	else
	{
		OnFailure.Broadcast(0);
		SetReadyToDestroy();
	}
}
