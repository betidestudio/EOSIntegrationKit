// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_GooglePlayGames.generated.h"

UCLASS()
class EIKLOGINMETHODS_API UBFL_GooglePlayGames : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UBFL_GooglePlayGames();

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	static void ManualSignIn();

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	static FString GetUsername();

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	static FString GetPlayerID();

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	void UnlockAchievement(const FString& AchievementID);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	void IncrementAchievement(const FString& AchievementID, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	void DisplayAchievementsUI();
	
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	void SubmitLeaderboardScore(const FString& LeaderboardID, int64 Value);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	void ShowLeaderboard(const FString& LeaderboardID);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	void ComparePlayerProfile(const FString& PlayerID);
	
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games")
	void SubmitEvent(const FString& EventID, int32 NumberOfOccurrences);
	
};
