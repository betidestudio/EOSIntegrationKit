// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
THIRD_PARTY_INCLUDES_START
#include "eos_leaderboards.h"
#include "eos_leaderboards_types.h"
THIRD_PARTY_INCLUDES_END
#include "EIK_Leaderboards_QueryLeaderboardRanks.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Leaderboards_QueryLeaderboardRanksOptions
{
	GENERATED_BODY()

	//The ID of the leaderboard whose information you want to retrieve.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	FString LeaderboardId;

	//Product User ID for user who is querying ranks. Must be set when using a client policy that requires a valid logged in user. Not used for Dedicated Server where no user is available.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	FEIK_ProductUserId LocalUserId;

	FEIK_Leaderboards_QueryLeaderboardRanksOptions()
		: LeaderboardId("")
		, LocalUserId()
	{
	}
	EOS_Leaderboards_QueryLeaderboardRanksOptions ToEOSLeaderboardsQueryLeaderboardRanksOptions()
	{
		EOS_Leaderboards_QueryLeaderboardRanksOptions Result;
		Result.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDRANKS_API_LATEST;
		Result.LeaderboardId = TCHAR_TO_ANSI(*LeaderboardId);
		Result.LocalUserId = LocalUserId.GetValueAsEosType();
		return Result;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Leaderboards_OnQueryLeaderboardRanksCompleteCallback, const TEnumAsByte<EEIK_Result>&, Result, const FString&, LeaderboardId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Leaderboards_QueryLeaderboardRanks : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Retrieves top leaderboard records by rank in the leaderboard matching the given leaderboard ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface", DisplayName = "EOS_Leaderboards_QueryLeaderboardRanks")
	static UEIK_Leaderboards_QueryLeaderboardRanks* EIK_Leaderboards_QueryLeaderboardRanks(const FEIK_Leaderboards_QueryLeaderboardRanksOptions& Options);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Leaderboards Interface")
	FEIK_Leaderboards_OnQueryLeaderboardRanksCompleteCallback OnCallback;

private:
	virtual void Activate() override;
	static void EOS_CALL Internal_OnQueryLeaderboardRanksCompleteCallback(const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo* Data);
	FEIK_Leaderboards_QueryLeaderboardRanksOptions Var_Options;
	
};
