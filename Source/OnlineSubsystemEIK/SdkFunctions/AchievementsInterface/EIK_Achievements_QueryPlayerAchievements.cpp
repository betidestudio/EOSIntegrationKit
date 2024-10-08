// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Achievements_QueryPlayerAchievements.h"

#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"


UEIK_Achievements_QueryPlayerAchievements* UEIK_Achievements_QueryPlayerAchievements::
EIK_Achievements_QueryPlayerAchievements(FEIK_ProductUserId LocalUserId, FEIK_ProductUserId TargetUserId)
{
	UEIK_Achievements_QueryPlayerAchievements* Node = NewObject<UEIK_Achievements_QueryPlayerAchievements>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_TargetUserId = TargetUserId;
	return Node;
}

void UEIK_Achievements_QueryPlayerAchievements::Activate()
{
	Super::Activate();
	
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_QueryPlayerAchievementsOptions Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_QUERYPLAYERACHIEVEMENTS_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.TargetUserId = Var_TargetUserId.GetValueAsEosType();
			EOS_Achievements_QueryPlayerAchievements(EOSRef->AchievementsHandle, &Options, this, [](const EOS_Achievements_OnQueryPlayerAchievementsCompleteCallbackInfo* Data)
			{
				if (UEIK_Achievements_QueryPlayerAchievements* QueryPlayerAchievements = static_cast<UEIK_Achievements_QueryPlayerAchievements*>(Data->ClientData))
				{
					QueryPlayerAchievements->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId, Data->TargetUserId);
					QueryPlayerAchievements->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
QueryPlayerAchievements->MarkAsGarbage();
#else
QueryPlayerAchievements->MarkPendingKill();
#endif
				}
			});
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	OnCallback.Broadcast(EEIK_Result::EOS_ServiceFailure, FEIK_ProductUserId(), FEIK_ProductUserId());
}
