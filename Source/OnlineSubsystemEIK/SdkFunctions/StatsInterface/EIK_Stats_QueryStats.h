// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_stats.h"
#include "eos_stats_types.h"
THIRD_PARTY_INCLUDES_END
#include "EIK_Stats_QueryStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_OnStatsQueryStatsComplete, const FEIK_ProductUserId&, LocalUserId, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_ProductUserId&, TargetUserId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Stats_QueryStats : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Query for a list of stats for a specific player.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Stats Interface", DisplayName="EOS_Stats_QueryStats")
	static UEIK_Stats_QueryStats* EIK_Stats_QueryStats(FEIK_ProductUserId LocalUserId, const FEIK_ProductUserId& TargetUserId, int64 StartTime, int64 EndTime, const TArray<FString>& StatNames);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Stats Interface")
	FEIK_OnStatsQueryStatsComplete OnCallback;

private:
	FEIK_ProductUserId Var_LocalUserId;
	FEIK_ProductUserId Var_TargetUserId;
	int64 Var_StartTime;
	int64 Var_EndTime;
	TArray<FString> Var_StatNames;

	static void EOS_CALL Internal_OnStatsQueryStatsComplete(const EOS_Stats_OnQueryStatsCompleteCallbackInfo* Data);
	void Activate() override;
};
