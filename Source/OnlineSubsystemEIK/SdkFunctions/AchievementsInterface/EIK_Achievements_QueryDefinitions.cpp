// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Achievements_QueryDefinitions.h"
#include "eos_achievements.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Achievements_QueryDefinitions* UEIK_Achievements_QueryDefinitions::EIK_Achievements_QueryDefinitions(
	FEIK_ProductUserId UserId)
{
	UEIK_Achievements_QueryDefinitions* Node = NewObject<UEIK_Achievements_QueryDefinitions>();
	Node->Var_UserId = UserId;
	return Node;
}

void UEIK_Achievements_QueryDefinitions::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_QueryDefinitionsOptions Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_QUERYDEFINITIONS_API_LATEST;
			Options.LocalUserId = Var_UserId.GetValueAsEosType();
			EOS_Achievements_QueryDefinitions(EOSRef->AchievementsHandle, &Options, this, [](const EOS_Achievements_OnQueryDefinitionsCompleteCallbackInfo* Data)
			{
				if (UEIK_Achievements_QueryDefinitions* QueryDefinitions = static_cast<UEIK_Achievements_QueryDefinitions*>(Data->ClientData))
				{
					QueryDefinitions->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode));
					QueryDefinitions->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
					QueryDefinitions->MarkAsGarbage();
#else
					QueryDefinitions->MarkPendingKill();
#endif
				}
			});
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	OnCallback.Broadcast(EEIK_Result::EOS_ServiceFailure);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
