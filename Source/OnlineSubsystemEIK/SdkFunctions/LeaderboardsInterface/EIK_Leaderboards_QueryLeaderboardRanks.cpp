// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Leaderboards_QueryLeaderboardRanks.h"

UEIK_Leaderboards_QueryLeaderboardRanks* UEIK_Leaderboards_QueryLeaderboardRanks::EIK_Leaderboards_QueryLeaderboardRanks(const FEIK_Leaderboards_QueryLeaderboardRanksOptions& Options)
{
	UEIK_Leaderboards_QueryLeaderboardRanks* BlueprintNode = NewObject<UEIK_Leaderboards_QueryLeaderboardRanks>();
	BlueprintNode->Var_Options = Options;
	return BlueprintNode;
}

void UEIK_Leaderboards_QueryLeaderboardRanks::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Leaderboards_QueryLeaderboardRanksOptions Options = Var_Options.ToEOSLeaderboardsQueryLeaderboardRanksOptions();
			EOS_Leaderboards_QueryLeaderboardRanks(EOSRef->LeaderboardsHandle, &Options, this, &UEIK_Leaderboards_QueryLeaderboardRanks::Internal_OnQueryLeaderboardRanksCompleteCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query leaderboard ranks either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, "");
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Leaderboards_QueryLeaderboardRanks::Internal_OnQueryLeaderboardRanksCompleteCallback(
	const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo* Data)
{
	if(UEIK_Leaderboards_QueryLeaderboardRanks* CallbackObj = static_cast<UEIK_Leaderboards_QueryLeaderboardRanks*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [CallbackObj, Data]()
		{
			CallbackObj->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LeaderboardId);
			CallbackObj->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
CallbackObj->MarkAsGarbage();
#else
CallbackObj->MarkPendingKill();
#endif
		});
	}
}