// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_GetLeaderboards_AsyncFunction.h"

#include "OnlineSubsystemUtils.h"

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
	if(const IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(GetWorld()))
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineLeaderboardsPtr LeaderboardsPointerRef = SubsystemRef->GetLeaderboardsInterface())
			{
				if(IdentityPointerRef->GetLoginStatus(0) != ELoginStatus::LoggedIn)
				{
					if(!bDelegateCalled)
					{
						OnFail.Broadcast(TArray<FEIKLeaderboardValue>());
						bDelegateCalled = true;
						SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
						MarkAsGarbage();
#else
						MarkPendingKill();
#endif
					}
					return;
				}
				TArray<TSharedRef<const FUniqueNetId>> Usersvar;
				Usersvar.Add(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef());
				FOnlineLeaderboardReadRef LeaderboardRead = MakeShareable(new FOnlineLeaderboardRead());
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
				FString LeaderboardNameString = LeaderboardName.ToString();
				LeaderboardRead->LeaderboardName = LeaderboardNameString;
#else
				LeaderboardRead->LeaderboardName = LeaderboardName;
#endif
				LeaderboardsPointerRef->OnLeaderboardReadCompleteDelegates.AddUObject(this,&UEIK_GetLeaderboards_AsyncFunction::OnGetLeaderboardCompleted,LeaderboardRead);
				if(!LeaderboardsPointerRef->ReadLeaderboardsAroundRank(AroundRank,Range,LeaderboardRead))
				{
					if(!bDelegateCalled)
					{
						OnFail.Broadcast(TArray<FEIKLeaderboardValue>());
						bDelegateCalled = true;
						SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
						MarkAsGarbage();
#else
						MarkPendingKill();
#endif
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
#if ENGINE_MAJOR_VERSION == 5
					MarkAsGarbage();
#else
					MarkPendingKill();
#endif
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
#if ENGINE_MAJOR_VERSION == 5
				MarkAsGarbage();
#else
				MarkPendingKill();
#endif
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
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
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
			int32 Score = -1;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
			if (Row.Columns.Num() > 0)
			{
				const FString& FirstKey = Row.Columns.begin()->Key;
				Row.Columns.Find(FirstKey)->GetValue(Score);
			}
#else
			Row.Columns.Find("None")->GetValue(Score);
#endif
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
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
		}
	}
	else
	{
		if(!bDelegateCalled)
		{
			OnFail.Broadcast(TArray<FEIKLeaderboardValue>());
			bDelegateCalled = true;
			SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
		}
	}
}
