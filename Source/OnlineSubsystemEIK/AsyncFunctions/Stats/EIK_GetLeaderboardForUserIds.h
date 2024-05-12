// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_GetLeaderboards_AsyncFunction.h"
#include "eos_leaderboards.h"

#include "EIK_GetLeaderboardForUserIds.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ELeaderboardAggregation : uint8
{
	EOS_LA_Min UMETA(DisplayName = "Minimum"),
	EOS_LA_Max UMETA(DisplayName = "Maximum"),
	EOS_LA_Sum UMETA(DisplayName = "Sum"),
	EOS_LA_Latest UMETA(DisplayName = "Latest")
};

USTRUCT(BlueprintType)
struct FEIKExtendedLeaderboardValue
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIKLeaderboardValue")
	int32 Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIKLeaderboardValue")
	FString UserId = "";

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetUserIdLeaderboardResult, const TArray<FEIKExtendedLeaderboardValue>&, LeaderboardValues);


UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetLeaderboardForUserIds : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	TArray<FString> Var_TargetProductUserIds;

	FString Var_LocalProductUserId;

	bool Var_bUseTime;

	FDateTime Var_StartTime;

	FDateTime Var_EndTime;

	ELeaderboardAggregation Var_AggregationType;

	FString Var_StatName;

	UFUNCTION(BlueprintCallable, DisplayName = "Get EIK Leaderboard For User Ids", meta = (BlueprintInternalUseOnly = "true"), Category = "EOS Integration Kit || Statistics")
	static UEIK_GetLeaderboardForUserIds* GetEIKLeaderboardForUserIds(TArray<FString> TargetProductUserIds, FString LocalProductUserId, bool UseTime, FDateTime StartTime, FDateTime EndTime, ELeaderboardAggregation AggregationType, FString StatName);

	void GetLeaderboardForUserIds();

	static void EOS_CALL GetLeaderboardForUserIdsCallback(const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo* Data);

	void ResultSuccess();

	void ResultFaliure();

	void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FGetUserIdLeaderboardResult Success;

	UPROPERTY(BlueprintAssignable)
	FGetUserIdLeaderboardResult Faliure;

};
