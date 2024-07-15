// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_stats.h"
#include "eos_stats_types.h"
THIRD_PARTY_INCLUDES_END
#include "EIK_StatsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_StatsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//Fetches a stat from a given index. Use EOS_Stats_Stat_Release when finished with the data.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Stats Interface", DisplayName="EOS_Stats_CopyStatByIndex")
	static TEnumAsByte<EEIK_Result> EIK_Stats_CopyStatByIndex( FEIK_ProductUserId TargetUserId, int32 StatIndex, FEIK_Stats_Stat& OutStat);

	//Fetches a stat from cached stats by name. Use EOS_Stats_Stat_Release when finished with the data.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Stats Interface", DisplayName="EOS_Stats_CopyStatByName")
	static TEnumAsByte<EEIK_Result> EIK_Stats_CopyStatByName( FEIK_ProductUserId TargetUserId, const FString& Name, FEIK_Stats_Stat& OutStat);

	//Fetch the number of stats that are cached locally.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Stats Interface", DisplayName="EOS_Stats_GetStatsCount")
	static int32 EIK_Stats_GetStatsCount( FEIK_ProductUserId TargetUserId);

};
