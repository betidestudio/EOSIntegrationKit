// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_AchievementsSubsystem.h"

#include "eos_achievements.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

FEIK_NotificationId UEIK_AchievementsSubsystem::EIK_Achievements_AddNotifyAchievementsUnlockedV2(FOnAchievementsUnlockedV2Callback Callback)
{
	OnAchievementsUnlockedV2 = Callback;
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

TEnumAsByte<EEIK_Result> UEIK_AchievementsSubsystem::EIK_Achievements_CopyAchievementDefinitionV2ByIndex(int32 Index,
	FEIK_Achievements_DefinitionV2& OutAchievementDefinition)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_COPYACHIEVEMENTDEFINITIONV2BYINDEX_API_LATEST;
			Options.AchievementIndex = Index;
			EOS_Achievements_DefinitionV2* OutAchievementDefinition1 = nullptr;
			EOS_EResult Result = EOS_Achievements_CopyAchievementDefinitionV2ByIndex(EOSRef->AchievementsHandle, &Options, &OutAchievementDefinition1);
			OutAchievementDefinition = *OutAchievementDefinition1;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return EEIK_Result::EOS_ServiceFailure;
}

TEnumAsByte<EEIK_Result> UEIK_AchievementsSubsystem::EIK_Achievements_CopyPlayerAchievementByIndex(
	FEIK_ProductUserId TargetUserId, int32 Index, FEIK_ProductUserId LocalUserId,
	FEIK_Achievements_PlayerAchievement& OutPlayerAchievement)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_CopyPlayerAchievementByIndexOptions Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYINDEX_API_LATEST;
			Options.AchievementIndex = Index;
			EOS_Achievements_PlayerAchievement* OutPlayerAchievement1 = nullptr;
			EOS_EResult Result = EOS_Achievements_CopyPlayerAchievementByIndex(EOSRef->AchievementsHandle, &Options, &OutPlayerAchievement1);
			OutPlayerAchievement = *OutPlayerAchievement1;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return EEIK_Result::EOS_ServiceFailure;
}

void UEIK_AchievementsSubsystem::EIK_Achievements_DefinitionV2_Release(
	FEIK_Achievements_DefinitionV2& AchievementDefinition)
{
	EOS_Achievements_DefinitionV2_Release(AchievementDefinition.GetValueAsEosType());
}

int32 UEIK_AchievementsSubsystem::EIK_Achievements_GetAchievementDefinitionCount()
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_GetAchievementDefinitionCountOptions Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_GETACHIEVEMENTDEFINITIONCOUNT_API_LATEST;
			return EOS_Achievements_GetAchievementDefinitionCount(EOSRef->AchievementsHandle, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return 0;
}

int32 UEIK_AchievementsSubsystem::EIK_Achievements_GetPlayerAchievementCount(FEIK_ProductUserId UserId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_GetPlayerAchievementCountOptions Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_GETPLAYERACHIEVEMENTCOUNT_API_LATEST;
			Options.UserId = UserId.GetValueAsEosType();
			return EOS_Achievements_GetPlayerAchievementCount(EOSRef->AchievementsHandle, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return 0;
}

void UEIK_AchievementsSubsystem::EIK_Achievements_PlayerAchievement_Release(
	FEIK_Achievements_PlayerAchievement& PlayerAchievement)
{
	EOS_Achievements_PlayerAchievement_Release(PlayerAchievement.GetValueAsEosType());
}

TEnumAsByte<EEIK_Result> UEIK_AchievementsSubsystem::EIK_Achievements_QueryDefinitions(FEIK_ProductUserId UserId)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_QueryDefinitionsOptions Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_QUERYDEFINITIONS_API_LATEST;
			Options.LocalUserId = UserId.GetValueAsEosType();
			EOS_Achievements_QueryDefinitions(EOSRef->AchievementsHandle, &Options, this, nullptr);
			return static_cast<EEIK_Result>(EOS_EResult::EOS_Success);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return EEIK_Result::EOS_ServiceFailure;
}

void UEIK_AchievementsSubsystem::EIK_Achievements_RemoveNotifyAchievementsUnlocked(FEIK_NotificationId Id)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_RemoveNotifyAchievementsUnlocked(EOSRef->AchievementsHandle, Id.GetValueAsEosType());
		}
	}
}

TEnumAsByte<EEIK_Result> UEIK_AchievementsSubsystem::EIK_Achievements_CopyPlayerAchievementByAchievementId(FEIK_ProductUserId TargetUserId, FString AchievementId, FEIK_ProductUserId LocalUserId, FEIK_Achievements_PlayerAchievement& OutPlayerAchievement)
{
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Achievements_CopyPlayerAchievementByAchievementIdOptions Options;
			Options.ApiVersion = EOS_ACHIEVEMENTS_COPYPLAYERACHIEVEMENTBYACHIEVEMENTID_API_LATEST;
			if(AchievementId.IsEmpty())
			{
				UE_LOG(LogEIK, Error, TEXT("AchievementId is empty"));
				return EEIK_Result::EOS_ServiceFailure;
			}
			Options.AchievementId = TCHAR_TO_ANSI(*AchievementId);
			EOS_Achievements_PlayerAchievement* OutPlayerAchievement1 = nullptr;
			EOS_EResult Result = EOS_Achievements_CopyPlayerAchievementByAchievementId(EOSRef->AchievementsHandle, &Options, &OutPlayerAchievement1);
			OutPlayerAchievement = *OutPlayerAchievement1;
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to get EOS subsystem"));
	return EEIK_Result::EOS_ServiceFailure;
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
