// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_AchievementsSubsystem.h"

#include "eos_achievements.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

FEIK_NotificationId UEIK_AchievementsSubsystem::EIK_Achievements_AddNotifyAchievementsUnlockedV2()
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_AddNotifyAchievementsUnlockedV2Options Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_ADDNOTIFYACHIEVEMENTSUNLOCKEDV2_API_LATEST;
			return EOS_Achievements_AddNotifyAchievementsUnlockedV2(EOSRef->AchievementsHandle, &Options, this,[](const EOS_Achievements_OnAchievementsUnlockedCallbackV2Info* Data)
			{
				if (UEIK_AchievementsSubsystem* AchievementsSubsystem = static_cast<UEIK_AchievementsSubsystem*>(Data->ClientData))
				{
					AchievementsSubsystem->OnAchievementsUnlockedV2.ExecuteIfBound(Data->UserId, Data->AchievementId, Data->UnlockTime);
				}
			});
		}
	}
	return FEIK_NotificationId();
}

TEnumAsByte<EEIK_Result> UEIK_AchievementsSubsystem::EIK_Achievements_CopyAchievementDefinitionV2ByAchievementId(FString AchievementId, FEIK_Achievements_DefinitionV2& OutAchievementDefinition)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_CopyAchievementDefinitionV2ByAchievementIdOptions Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_COPYACHIEVEMENTDEFINITIONV2BYACHIEVEMENTID_API_LATEST;
			if(AchievementId.IsEmpty())
			{
				UE_LOG(LogEIK, Error, TEXT("AchievementId is empty"));
				return EEIK_Result::EOS_ServiceFailure;
			}
			Options.AchievementId = TCHAR_TO_ANSI(*AchievementId);
			EOS_Achievements_DefinitionV2* OutAchievementDefinition1 = nullptr;
			EOS_EResult Result = EOS_Achievements_CopyAchievementDefinitionV2ByAchievementId(EOSRef->AchievementsHandle, &Options, &OutAchievementDefinition1);
			OutAchievementDefinition = *OutAchievementDefinition1;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return EEIK_Result::EOS_ServiceFailure;
}
