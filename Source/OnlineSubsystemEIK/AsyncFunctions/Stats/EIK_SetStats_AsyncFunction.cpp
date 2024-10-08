// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_SetStats_AsyncFunction.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineStatsInterface.h"

UEIK_SetStats_AsyncFunction* UEIK_SetStats_AsyncFunction::SetEIKStats(const FString& StatName, const int32 StatValue)
{
	UEIK_SetStats_AsyncFunction* BlueprintNode = NewObject<UEIK_SetStats_AsyncFunction>();
	BlueprintNode->StatName = StatName;
	BlueprintNode->StatValue = StatValue;
	return BlueprintNode;
}

void UEIK_SetStats_AsyncFunction::OnEUpdateStatsCompleted(const FOnlineError& Result)
{
	if(Result.WasSuccessful() || Result.bSucceeded)
	{
		if(!bDelegateCalled)
		{
			OnSuccess.Broadcast();
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
			OnFail.Broadcast();
			bDelegateCalled = true;
			SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
			MarkAsGarbage();
#else
			MarkPendingKill();
#endif
		}
	}
	return;
}

void UEIK_SetStats_AsyncFunction::Activate()
{
	SetEIKStatsLocal();
	Super::Activate();
}

void UEIK_SetStats_AsyncFunction::SetEIKStatsLocal()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get())
	{
		if(const IOnlineIdentityPtr IdentityPointerRef = SubsystemRef->GetIdentityInterface())
		{
			if(const IOnlineStatsPtr StatsPointerRef = SubsystemRef->GetStatsInterface())
			{
				FOnlineStatsUserUpdatedStats StatVar = FOnlineStatsUserUpdatedStats(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef());
				StatVar.Stats.Add(StatName, FOnlineStatUpdate(StatValue,FOnlineStatUpdate::EOnlineStatModificationType::Sum));
				TArray<FOnlineStatsUserUpdatedStats> StatArray;
				StatArray.Add(StatVar);
				StatsPointerRef->UpdateStats(IdentityPointerRef->GetUniquePlayerId(0).ToSharedRef(),StatArray,FOnlineStatsUpdateStatsComplete::CreateUObject(this, &UEIK_SetStats_AsyncFunction::OnEUpdateStatsCompleted));
			}
			else
			{
				if(!bDelegateCalled)
				{
					OnFail.Broadcast();
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
				OnFail.Broadcast();
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
			OnFail.Broadcast();
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
