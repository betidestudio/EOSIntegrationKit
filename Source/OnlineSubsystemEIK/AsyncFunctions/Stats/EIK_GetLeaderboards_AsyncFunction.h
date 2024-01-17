// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineLeaderboardsEOS.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_GetLeaderboards_AsyncFunction.generated.h"

USTRUCT(BlueprintType)
struct FEIKLeaderboardValue
{
	GENERATED_USTRUCT_BODY()
public:
	/** Name of player in this row */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIKLeaderboardValue")
	FString NickName = "";
	/** Player's rank in this leaderboard */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIKLeaderboardValue")
	int32 Rank = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EIKLeaderboardValue")
	int32 Score = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetLeaderboardResult, const TArray<FEIKLeaderboardValue>&, LeaderboardValues);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetLeaderboards_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	bool bDelegateCalled = false;
	
	UFUNCTION(BlueprintCallable, DisplayName="Get EIK Leaderboard", meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Statistics")
	static UEIK_GetLeaderboards_AsyncFunction* GetLeaderboard(const FName LeaderboardName,const int32 Range, const int32 AroundRank);

	virtual void Activate() override;
	FName LeaderboardName;
	int32 Range;
	int32 AroundRank;
	void GetLeaderboardLocal();

	UPROPERTY(BlueprintAssignable)
	FGetLeaderboardResult OnSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FGetLeaderboardResult OnFail;

	
	void OnGetLeaderboardCompleted(bool bWasSuccessful, FOnlineLeaderboardReadRef LeaderboardRead);
};
