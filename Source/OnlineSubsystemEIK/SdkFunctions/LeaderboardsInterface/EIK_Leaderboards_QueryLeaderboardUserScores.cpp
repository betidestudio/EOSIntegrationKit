// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Leaderboards_QueryLeaderboardUserScores.h"

UEIK_Leaderboards_QueryLeaderboardUserScores* UEIK_Leaderboards_QueryLeaderboardUserScores::
EIK_Leaderboards_QueryLeaderboardUserScores(const FEIK_Leaderboards_QueryLeaderboardUserScoresOptions& Options)
{
	UEIK_Leaderboards_QueryLeaderboardUserScores* BlueprintNode = NewObject<UEIK_Leaderboards_QueryLeaderboardUserScores>();
	BlueprintNode->Var_Options = Options;
	return BlueprintNode;
}

void UEIK_Leaderboards_QueryLeaderboardUserScores::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Leaderboards_QueryLeaderboardUserScoresOptions Options = Var_Options.ToEOSLeaderboardsQueryLeaderboardUserScoresOptions();
			EOS_Leaderboards_QueryLeaderboardUserScores(EOSRef->LeaderboardsHandle, &Options, this, &UEIK_Leaderboards_QueryLeaderboardUserScores::Internal_OnQueryLeaderboardUserScoresCompleteCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query leaderboard user scores either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Leaderboards_QueryLeaderboardUserScores::Internal_OnQueryLeaderboardUserScoresCompleteCallback(
	const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo* Data)
{
	if(UEIK_Leaderboards_QueryLeaderboardUserScores* CallbackObj = static_cast<UEIK_Leaderboards_QueryLeaderboardUserScores*>(Data->ClientData))
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
