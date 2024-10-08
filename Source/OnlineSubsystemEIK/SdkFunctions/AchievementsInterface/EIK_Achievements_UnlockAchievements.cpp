// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Achievements_UnlockAchievements.h"

#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Achievements_UnlockAchievements* UEIK_Achievements_UnlockAchievements::EIK_Achievements_UnlockAchievements(
	FEIK_ProductUserId UserId, TArray<FString> AchievementIds)
{
	UEIK_Achievements_UnlockAchievements* Node = NewObject<UEIK_Achievements_UnlockAchievements>();
	Node->Var_UserId = UserId;
	Node->Var_AchievementIds = AchievementIds;
	return Node;
}

void UEIK_Achievements_UnlockAchievements::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_UnlockAchievementsOptions Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_UNLOCKACHIEVEMENTS_API_LATEST;
			Options.UserId = Var_UserId.GetValueAsEosType();
			Options.AchievementsCount = Var_AchievementIds.Num();
			Options.AchievementIds = new const char*[Var_AchievementIds.Num()];
			for (int i = 0; i < Var_AchievementIds.Num(); i++)
			{
				Options.AchievementIds[i] = TCHAR_TO_ANSI(*Var_AchievementIds[i]);
			}
			EOS_Achievements_UnlockAchievements(EOSRef->AchievementsHandle, &Options, this, [](const EOS_Achievements_OnUnlockAchievementsCompleteCallbackInfo* Data)
			{
				if (UEIK_Achievements_UnlockAchievements* UnlockAchievements = static_cast<UEIK_Achievements_UnlockAchievements*>(Data->ClientData))
				{
					UnlockAchievements->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->UserId, Data->AchievementsCount);
					UnlockAchievements->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
					UnlockAchievements->MarkAsGarbage();
#else
					UnlockAchievements->MarkPendingKill();
#endif
				}
			});
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	OnCallback.Broadcast(EEIK_Result::EOS_ServiceFailure, FEIK_ProductUserId(), 0);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
