// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Leaderboards_QueryLeaderboardDefinitions.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Leaderboards_QueryLeaderboardDefinitions* UEIK_Leaderboards_QueryLeaderboardDefinitions::
EIK_Leaderboards_QueryLeaderboardDefinitions(const FEIK_Leaderboards_QueryLeaderboardDefinitionsOptions& Options)
{
	UEIK_Leaderboards_QueryLeaderboardDefinitions* BlueprintNode = NewObject<UEIK_Leaderboards_QueryLeaderboardDefinitions>();
	BlueprintNode->Var_Options = Options;
	return BlueprintNode;
}

void UEIK_Leaderboards_QueryLeaderboardDefinitions::OnQueryLeaderboardDefinitionsCompleteCallback(
	const EOS_Leaderboards_OnQueryLeaderboardDefinitionsCompleteCallbackInfo* Data)
{
	if(UEIK_Leaderboards_QueryLeaderboardDefinitions* CallbackObj = static_cast<UEIK_Leaderboards_QueryLeaderboardDefinitions*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
		{
			CallbackObj->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode));
			CallbackObj->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
CallbackObj->MarkAsGarbage();
#else
CallbackObj->MarkPendingKill();
#endif
		});
	}
}

void UEIK_Leaderboards_QueryLeaderboardDefinitions::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Leaderboards_QueryLeaderboardDefinitionsOptions Options = Var_Options.ToEOSLeaderboardsQueryLeaderboardDefinitionsOptions();
			EOS_Leaderboards_QueryLeaderboardDefinitions(EOSRef->LeaderboardsHandle, &Options, this, &UEIK_Leaderboards_QueryLeaderboardDefinitions::OnQueryLeaderboardDefinitionsCompleteCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query leaderboard definitions either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
