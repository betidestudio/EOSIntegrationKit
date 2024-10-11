// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_GetAchievement_AsyncFunction.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"

UEIK_GetAchievement_AsyncFunction* UEIK_GetAchievement_AsyncFunction::GetEIKAchievements()
{
	UEIK_GetAchievement_AsyncFunction* Ueik_GetAchievementObject = NewObject<UEIK_GetAchievement_AsyncFunction>();
	return Ueik_GetAchievementObject;
}

void UEIK_GetAchievement_AsyncFunction::Activate()
{
	GetAchievements();
	Super::Activate();
}

void UEIK_GetAchievement_AsyncFunction::GetAchievements()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
	{
		if(const IOnlineAchievementsPtr AchievementsPtrRef = SubsystemRef->GetAchievementsInterface())
		{
			if(const IOnlineIdentityPtr IdentityPtrRef = SubsystemRef->GetIdentityInterface())
			{
				AchievementsPtrRef->QueryAchievements(*IdentityPtrRef->GetUniquePlayerId(0), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UEIK_GetAchievement_AsyncFunction::OnAchievementsCompleted));
			}
			else
			{
				if(!bDelegateCalled)
				{
					OnFail.Broadcast(TArray<FEIK_Achievement>());
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
				OnFail.Broadcast(TArray<FEIK_Achievement>());
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
			OnFail.Broadcast(TArray<FEIK_Achievement>());
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

void UEIK_GetAchievement_AsyncFunction::OnAchievementsCompleted(const FUniqueNetId& UniqueNetId, bool bWasSuccess)
{
	if(bWasSuccess)
	{
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
		{
			if(const IOnlineAchievementsPtr AchievementsPtrRef = SubsystemRef->GetAchievementsInterface())
			{
				TArray<FEIK_Achievement> AchievementsArray;
				TArray<FOnlineAchievement> CachedAchievements;
				AchievementsPtrRef->GetCachedAchievements(UniqueNetId, CachedAchievements);
				for(auto Achievement : CachedAchievements)
				{
					FEIK_Achievement LocalAchievement;
					LocalAchievement.Id = Achievement.Id;
					LocalAchievement.Progress = Achievement.Progress;
					AchievementsArray.Add(LocalAchievement);
				}
				OnSuccess.Broadcast(AchievementsArray);
				SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
				MarkAsGarbage();
#else
				MarkPendingKill();
#endif
				return;
			}
		}
	}
	if(!bDelegateCalled)
	{
		OnFail.Broadcast(TArray<FEIK_Achievement>());
		bDelegateCalled = true;
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
}