// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_leaderboards.h"
#include "eos_leaderboards_types.h"
THIRD_PARTY_INCLUDES_END
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_LeaderboardsSubsystem.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Leaderboards Interface", meta=(DisplayName="Leaderboards Interface"))
class ONLINESUBSYSTEMEIK_API UEIK_LeaderboardsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	//Fetches a leaderboard definition from the cache using an index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_CopyLeaderboardDefinitionByIndex")
	TEnumAsByte<EEIK_Result> EIK_Leaderboards_CopyLeaderboardDefinitionByIndex(int32 LeaderboardIndex, FEIK_Leaderboards_Definition& OutLeaderboardDefinition);

	//Fetches a leaderboard definition from the cache using a leaderboard ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardId")
	TEnumAsByte<EEIK_Result> EIK_Leaderboards_CopyLeaderboardDefinitionByLeaderboardId(FString LeaderboardId, FEIK_Leaderboards_Definition& OutLeaderboardDefinition);

	//Fetches a leaderboard record from a given index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_CopyLeaderboardRecordByIndex")
	TEnumAsByte<EEIK_Result> EIK_Leaderboards_CopyLeaderboardRecordByIndex(int32 LeaderboardRecordIndex, FEIK_Leaderboards_LeaderboardRecord& OutLeaderboardRecord);

	//Fetches a leaderboard record from a given user ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_CopyLeaderboardRecordByUserId")
	TEnumAsByte<EEIK_Result> EIK_Leaderboards_CopyLeaderboardRecordByUserId(FEIK_ProductUserId UserId, FEIK_Leaderboards_LeaderboardRecord& OutLeaderboardRecord);

	//Fetches leaderboard user score from a given index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_CopyLeaderboardUserScoreByIndex")
	TEnumAsByte<EEIK_Result> EIK_Leaderboards_CopyLeaderboardUserScoreByIndex(int32 LeaderboardUserScoreIndex, FString StatName, FEIK_Leaderboards_LeaderboardUserScore& OutLeaderboardUserScore);

	//Fetches leaderboard user score from a given user ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_CopyLeaderboardUserScoreByUserId")
	TEnumAsByte<EEIK_Result> EIK_Leaderboards_CopyLeaderboardUserScoreByUserId(FEIK_ProductUserId UserId, FString StatName, FEIK_Leaderboards_LeaderboardUserScore& OutLeaderboardUserScore);

	//Release the memory associated with a leaderboard definition. This must be called on data retrieved from EOS_Leaderboards_CopyLeaderboardDefinitionByIndex or EOS_Leaderboards_CopyLeaderboardDefinitionByLeaderboardId.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_Definition_Release")
	void EIK_Leaderboards_LeaderboardDefinition_Release(FEIK_Leaderboards_Definition& LeaderboardDefinition);

	//Fetch the number of leaderboards definitions that are cached locally.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_GetLeaderboardDefinitionCount")
	int32 EIK_Leaderboards_GetLeaderboardDefinitionCount();

	//Fetch the number of leaderboard records that are cached locally.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_GetLeaderboardRecordCount")
	int32 EIK_Leaderboards_GetLeaderboardRecordCount();

	//Fetch the number of leaderboard user scores that are cached locally.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_GetLeaderboardUserScoreCount")
	int32 EIK_Leaderboards_GetLeaderboardUserScoreCount();

	//Release the memory associated with leaderboard record. This must be called on data retrieved from EOS_Leaderboards_CopyLeaderboardRecordByIndex or EOS_Leaderboards_CopyLeaderboardRecordByUserId.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_LeaderboardRecord_Release")
	void EIK_Leaderboards_LeaderboardRecord_Release(FEIK_Leaderboards_LeaderboardRecord& LeaderboardRecord);

	//Release the memory associated with leaderboard user score. This must be called on data retrieved from EOS_Leaderboards_CopyLeaderboardUserScoreByIndex or EOS_Leaderboards_CopyLeaderboardUserScoreByUserId.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName="EOS_Leaderboards_LeaderboardUserScore_Release")
	void EIK_Leaderboards_LeaderboardUserScore_Release(FEIK_Leaderboards_LeaderboardUserScore& LeaderboardUserScore);
};
