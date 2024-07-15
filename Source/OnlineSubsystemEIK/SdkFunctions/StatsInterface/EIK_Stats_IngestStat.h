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
#include "EIK_Stats_IngestStat.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Stats_IngestData
{
	GENERATED_BODY()

	/** The name of the stat to ingest. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Stats Interface")
	FString Name;

	/** The amount to ingest the stat. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Stats Interface")
	int32 IngestAmount;

	FEIK_Stats_IngestData()
		: Name("")
		, IngestAmount(0)
	{}
	EOS_Stats_IngestData ToEOSStatsIngestData()
	{
		EOS_Stats_IngestData Data;
		Data.ApiVersion = EOS_STATS_INGESTDATA_API_LATEST;
		Data.StatName = TCHAR_TO_ANSI(*Name);
		Data.IngestAmount = IngestAmount;
		return Data;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_OnStatsIngestStatComplete, const FEIK_ProductUserId&, LocalUserId, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_ProductUserId&, TargetUserId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Stats_IngestStat : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	
public:
	
	//Ingest a stat by the amount specified in Options. When the operation is complete and the delegate is triggered the stat will be uploaded to the backend to be processed. The stat may not be updated immediately and an achievement using the stat may take a while to be unlocked once the stat has been uploaded.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Stats Interface", DisplayName="EOS_Stats_IngestStat")
	static UEIK_Stats_IngestStat* EIK_Stats_IngestStat(FEIK_ProductUserId LocalUserId, const TArray<FEIK_Stats_IngestData>& Stats, const FEIK_ProductUserId& TargetUserId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Stats Interface")
	FEIK_OnStatsIngestStatComplete OnCallback;

private:
	FEIK_ProductUserId Var_LocalUserId;
	TArray<FEIK_Stats_IngestData> Var_Stats;
	FEIK_ProductUserId Var_TargetUserId;
	virtual void Activate() override;
	static void EOS_CALL Internal_OnStatsIngestStatComplete(const EOS_Stats_IngestStatCompleteCallbackInfo* Data);
};
