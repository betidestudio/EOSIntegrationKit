// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_StatsSubsystem.h"

#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

TEnumAsByte<EEIK_Result> UEIK_StatsSubsystem::EIK_Stats_CopyStatByIndex(FEIK_ProductUserId TargetUserId,
                                                                        int32 StatIndex, FEIK_Stats_Stat& OutStat)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Stats_CopyStatByIndexOptions Options;
			Options.ApiVersion = EOS_STATS_COPYSTATBYINDEX_API_LATEST;
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			Options.StatIndex = StatIndex;
			EOS_Stats_Stat *Stat = nullptr;
			auto Result = EOS_Stats_CopyStatByIndex(EOSRef->StatsHandle, &Options, &Stat);
			if (Result == EOS_EResult::EOS_Success)
			{
				OutStat = *Stat;
				EOS_Stats_Stat_Release(Stat);
			}
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_StatsSubsystem::EIK_Stats_CopyStatByIndex: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

TEnumAsByte<EEIK_Result> UEIK_StatsSubsystem::EIK_Stats_CopyStatByName(FEIK_ProductUserId TargetUserId,
	const FString& Name, FEIK_Stats_Stat& OutStat)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Stats_CopyStatByNameOptions Options;
			Options.ApiVersion = EOS_STATS_COPYSTATBYNAME_API_LATEST;
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			Options.Name = TCHAR_TO_ANSI(*Name);
			EOS_Stats_Stat *Stat = nullptr;
			auto Result = EOS_Stats_CopyStatByName(EOSRef->StatsHandle, &Options, &Stat);
			if (Result == EOS_EResult::EOS_Success)
			{
				OutStat = *Stat;
				EOS_Stats_Stat_Release(Stat);
			}
			return static_cast<EEIK_Result>(Result);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_StatsSubsystem::EIK_Stats_CopyStatByName: Failed to get EOS subsystem"));
	return EEIK_Result::EOS_NotFound;
}

int32 UEIK_StatsSubsystem::EIK_Stats_GetStatsCount(FEIK_ProductUserId TargetUserId)
{
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Stats_GetStatCountOptions Options;
			Options.ApiVersion = EOS_STATS_GETSTATSCOUNT_API_LATEST;
			Options.TargetUserId = TargetUserId.GetValueAsEosType();
			return EOS_Stats_GetStatsCount(EOSRef->StatsHandle, &Options);
		}
	}
	UE_LOG(LogEIK, Error, TEXT("UEIK_StatsSubsystem::EIK_Stats_GetStatsCount: Failed to get EOS subsystem"));
	return 0;
}
