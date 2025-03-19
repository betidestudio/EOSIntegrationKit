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

	// ---------------------- Sign In ----------------------
	
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|SignIn")
	static void ManualSignIn();

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|SignIn")
	static FString GetUsername();

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|SignIn")
	static FString GetPlayerID();

	// ------------------- Achievements -------------------

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Achievements")
	static void UnlockAchievement(const FString& AchievementID);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Achievements")
	static void IncrementAchievement(const FString& AchievementID, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Achievements")
	static void DisplayAchievementsUI();

	// ------------------- Leaderboards -------------------
	
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Leaderboards")
	static void SubmitLeaderboardScore(const FString& LeaderboardID, int64 Value);

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Leaderboards")
	static void ShowLeaderboard(const FString& LeaderboardID);

	// --------------------- Friends ----------------------
	
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Friends")
	static void ComparePlayerProfile(const FString& PlayerID);

	// ---------------------- Events ----------------------
	
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Events")
	static void SubmitEvent(const FString& EventID, int32 NumberOfOccurrences);
	
};
