//Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_GetLeaderboards_AsyncFunction.h"

UEIK_GetLeaderboards_AsyncFunction* UEIK_GetLeaderboards_AsyncFunction::GetLeaderboard(const FName LeaderboardName,
	const int32 Range, const int32 AroundRank)
{
	UEIK_GetLeaderboards_AsyncFunction* BlueprintNode = NewObject<UEIK_GetLeaderboards_AsyncFunction>();
	BlueprintNode->LeaderboardName = LeaderboardName;
	BlueprintNode->Range = Range;
	BlueprintNode->AroundRank = AroundRank;
	return BlueprintNode;
}

void UEIK_GetLeaderboards_AsyncFunction::Activate()
{
	GetLeaderboardLocal();
	Super::Activate();
}

void UEIK_GetLeaderboards_AsyncFunction::GetLeaderboardLocal()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineLeaderboardsPtr LeaderboardsPointerRef = SubsystemRef->GetLeaderboardsInterface())
			{
				TArray<TSharedRef<const FUniqueNetId>> Usersvar;
				Usersvar.Add(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef());
				FOnlineLeaderboardReadRef LeaderboardRead = MakeShareable(new FOnlineLeaderboardRead());
				LeaderboardRead->LeaderboardName = LeaderboardName;
				LeaderboardsPointerRef->OnLeaderboardReadCompleteDelegates.AddUObject(this,&UEIK_GetLeaderboards_AsyncFunction::OnGetLeaderboardCompleted,LeaderboardRead);
				if(!LeaderboardsPointerRef->ReadLeaderboardsAroundRank(AroundRank,Range,LeaderboardRead))
				{
					if(!bDelegateCalled)
					{
						OnFail.Broadcast(TArray<FEIKLeaderboardValue>());
						bDelegateCalled = true;
						SetReadyToDestroy();
					}
				}
			}
			else
			{
				if(!bDelegateCalled)
				{
					OnFail.Broadcast(TArray<FEIKLeaderboardValue>());
					bDelegateCalled = true;
					SetReadyToDestroy();
				}
			}
		}
		else
		{
			if(!bDelegateCalled)
			{
				OnFail.Broadcast(TArray<FEIKLeaderboardValue>());
				bDelegateCalled = true;
				SetReadyToDestroy();
			}
		}
	}
	else
	{
		if(!bDelegateCalled)
		{
			OnFail.Broadcast(TArray<FEIKLeaderboardValue>());
			bDelegateCalled = true;
			SetReadyToDestroy();
		}
	}
}

void UEIK_GetLeaderboards_AsyncFunction::OnGetLeaderboardCompleted(bool bWasSuccessful,
	FOnlineLeaderboardReadRef LeaderboardRead)
{
	if(bWasSuccessful)
	{
		TArray<FEIKLeaderboardValue> Result;

		for (auto Row : LeaderboardRead->Rows)
		{
			int32 Score;
			Row.Columns.Find("None")->GetValue(Score);
			FEIKLeaderboardValue LocalRow;
			LocalRow.Rank = Row.Rank;
			LocalRow.Score = Score;
			LocalRow.NickName = Row.NickName;
			Result.Add(LocalRow);
		}
		if(!bDelegateCalled)
		{
			OnSuccess.Broadcast(Result);
			bDelegateCalled = true;
			SetReadyToDestroy();
		}
	}
	else
	{
		if(!bDelegateCalled)
		{
			OnFail.Broadcast(TArray<FEIKLeaderboardValue>());
			bDelegateCalled = true;
			SetReadyToDestroy();
		}
	}
}
