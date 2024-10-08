// Copyright Epic Games, Inc. All Rights Reserved.


#include "EIK_GetAchievementDetails_AsyncFunction.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"

UEIK_GetAchievementDetails_AsyncFunction* UEIK_GetAchievementDetails_AsyncFunction::GetEIKAchievementDescription(FEIK_Achievement Achievement)	
{
	UEIK_GetAchievementDetails_AsyncFunction* Ueik_GetAchievementDescriptionObject = NewObject<UEIK_GetAchievementDetails_AsyncFunction>();
	Ueik_GetAchievementDescriptionObject->Var_Achievement = Achievement;
	return Ueik_GetAchievementDescriptionObject;
}

void UEIK_GetAchievementDetails_AsyncFunction::Activate()
{
	GetAchievementDescription();
	Super::Activate();
}

void UEIK_GetAchievementDetails_AsyncFunction::GetAchievementDescription()
{
	if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
	{
		if(const IOnlineAchievementsPtr AchievementsPtrRef = SubsystemRef->GetAchievementsInterface())
		{
			if(const IOnlineIdentityPtr IdentityPtrRef = SubsystemRef->GetIdentityInterface())
			{
				AchievementsPtrRef->QueryAchievementDescriptions(*IdentityPtrRef->GetUniquePlayerId(0), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UEIK_GetAchievementDetails_AsyncFunction::OnAchievementDescriptionCompleted));
			}
			else
			{
				if(!bDelegateCalled)
				{
					OnFail.Broadcast(FEIK_AchievementDescription());
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
				OnFail.Broadcast(FEIK_AchievementDescription());
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
			OnFail.Broadcast(FEIK_AchievementDescription());
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
void UEIK_GetAchievementDetails_AsyncFunction::OnAchievementDescriptionCompleted(const FUniqueNetId& UniqueNetId,bool bWasSuccess)
{
	if(bWasSuccess)
	{
		if(const IOnlineSubsystem *SubsystemRef = IOnlineSubsystem::Get("EIK"))
		{
			if(const IOnlineAchievementsPtr AchievementsPtrRef = SubsystemRef->GetAchievementsInterface())
			{
				FEIK_AchievementDescription LocalAchievementDescription;
				FOnlineAchievementDesc CachedAchievements;
				AchievementsPtrRef->GetCachedAchievementDescription(Var_Achievement.Id, CachedAchievements);
				LocalAchievementDescription.Id = Var_Achievement.Id;
				LocalAchievementDescription.Title = CachedAchievements.Title;
				LocalAchievementDescription.Progress = Var_Achievement.Progress;
				LocalAchievementDescription.LockedDesc= CachedAchievements.LockedDesc;
				LocalAchievementDescription.bIsHidden = CachedAchievements.bIsHidden;
				LocalAchievementDescription.UnlockedDesc = CachedAchievements.UnlockedDesc;
				LocalAchievementDescription.UnlockTime = CachedAchievements.UnlockTime;
				OnSuccess.Broadcast(LocalAchievementDescription);
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
		OnFail.Broadcast(FEIK_AchievementDescription());
		bDelegateCalled = true;
		SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		MarkAsGarbage();
#else
		MarkPendingKill();
#endif
	}
}
