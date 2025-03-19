// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "GooglePlayGamesStructures.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_GooglePlayGames.generated.h"

UCLASS()
class EIKLOGINMETHODS_API UBFL_GooglePlayGames : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UBFL_GooglePlayGames();

	// ---------------------- Sign In ----------------------

	/**
	 * Try to SignIn Manually to Google Play Games.
	 * (Should be called if User doesn't sign in automatically on startup)
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|SignIn")
	static void ManualSignIn();

	/**
	 * Get is User Signed in to Google Play Games
	 * @return True if User signed in, else False.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|SignIn")
	static bool IsSignedIn();

	/**
	 * Get the current Player Data
	 * @return The Currently Signed In Player Data
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|SignIn")
	static FGPGS_Player GetPlayer();

	// ------------------- Achievements -------------------

	/**
	 * Unlocks an Achievement by AchievementID (Only for Standard Achievements)
	 * @param AchievementID Achievement ID from Play Console
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Achievements")
	static void UnlockAchievement(const FString& AchievementID);

	/**
	 * Increment an Achievement (Only for Incremental Achievements)
	 * @param AchievementID Achievement ID from Play Console
	 * @param Value Value to increment Achievement by
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Achievements")
	static void IncrementAchievement(const FString& AchievementID, int32 Value);

	/**
	 * Display the Built-In Achievements UI from Google Play Games Services
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Achievements")
	static void DisplayAchievementsUI();

	// ------------------- Leaderboards -------------------

	/**
	 * Submit Leaderboard Score for the Current Player
	 * @param LeaderboardID Leaderboard ID from Google Play Console
	 * @param Value Value to set the current Player's Leaderboard score to
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Leaderboards")
	static void SubmitLeaderboardScore(const FString& LeaderboardID, int64 Value);

	/**
	 * Show Leaderboard
	 * @param LeaderboardID Leaderboard ID from Google Play Console
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Leaderboards")
	static void ShowLeaderboard(const FString& LeaderboardID);

	// --------------------- Friends ----------------------

	/**
	 * Compare to another Player's Profile
	 * @param PlayerID Player ID to compare to, mostly of a Friend
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Friends")
	static void ComparePlayerProfile(const FString& PlayerID);

	// ---------------------- Events ----------------------

	/**
	 * Submit an Event to Google Play Console
	 * @param EventID EventID from Google Play Console
	 * @param NumberOfOccurrences Number Of Occurrences that have taken place
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Google Play Games|Events")
	static void SubmitEvent(const FString& EventID, int32 NumberOfOccurrences);
	
};
