// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Json.h"
#include "GooglePlayGamesStructures.generated.h"

/**
 * Google Play Games Services Player
 */
USTRUCT(BlueprintType)
struct FGPGS_Player
{
	GENERATED_BODY()

	/* Display Name of the Player */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	FString DisplayName;

	/* Player ID from Google Play Games of the Player */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	FString PlayerID;

	/* TimeStamp when this data was retrieved */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	int64 RetrievedTimeStamp;

	/* Does player have High Resolution Image */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	bool bHasHiResImage;

	/* Does player have Icon Image */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	bool bHasIconImage;

	/* URL of High Resolution Image */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	FString HiResImageUrl;

	/* URL of Icon Image */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	FString IconImageUrl;

	/* Title of Player from Google Play Games */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	FString Title;

	/* Player Landscape Banner URL */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	FString BannerImageLandscapeUrl;

	/* Player Portrait Banner URL */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	FString BannerImagePortraitUrl;
	
	FGPGS_Player()
		: DisplayName(TEXT(""))
		, PlayerID(TEXT(""))
		, RetrievedTimeStamp(0)
		, bHasHiResImage(false)
		, bHasIconImage(false)
		, HiResImageUrl(TEXT(""))
		, IconImageUrl(TEXT(""))
		, Title(TEXT(""))
		, BannerImageLandscapeUrl(TEXT(""))
		, BannerImagePortraitUrl(TEXT(""))
	{
	}

	static FGPGS_Player ParseFromJson(const TSharedPtr<FJsonObject>& JsonObject)
	{
		FGPGS_Player Player;
		if (JsonObject.IsValid())
		{
			// Read all fields from the JSON object
			JsonObject->TryGetStringField("displayName", Player.DisplayName);
			JsonObject->TryGetStringField("playerId", Player.PlayerID);
			JsonObject->TryGetNumberField("retrievedTimeStamp", Player.RetrievedTimeStamp);
			JsonObject->TryGetBoolField("hasHiResImage", Player.bHasHiResImage);
			JsonObject->TryGetBoolField("hasIconImage", Player.bHasIconImage);
			JsonObject->TryGetStringField("hiResImageUrl", Player.HiResImageUrl);
			JsonObject->TryGetStringField("iconImageUrl", Player.IconImageUrl);
			JsonObject->TryGetStringField("title", Player.Title);
			JsonObject->TryGetStringField("bannerImageLandscapeUrl", Player.BannerImageLandscapeUrl);
			JsonObject->TryGetStringField("bannerImagePortraitUrl", Player.BannerImagePortraitUrl);
		}
		return Player;
	}

	static FGPGS_Player ParseFromJson(const FString& JsonString)
	{
		FGPGS_Player Player;
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			if (JsonObject.IsValid())
			{
				// Read all fields from the JSON object
				JsonObject->TryGetStringField("displayName", Player.DisplayName);
				JsonObject->TryGetStringField("playerId", Player.PlayerID);
				JsonObject->TryGetNumberField("retrievedTimeStamp", Player.RetrievedTimeStamp);
				JsonObject->TryGetBoolField("hasHiResImage", Player.bHasHiResImage);
				JsonObject->TryGetBoolField("hasIconImage", Player.bHasIconImage);
				JsonObject->TryGetStringField("hiResImageUrl", Player.HiResImageUrl);
				JsonObject->TryGetStringField("iconImageUrl", Player.IconImageUrl);
				JsonObject->TryGetStringField("title", Player.Title);
				JsonObject->TryGetStringField("bannerImageLandscapeUrl", Player.BannerImageLandscapeUrl);
				JsonObject->TryGetStringField("bannerImagePortraitUrl", Player.BannerImagePortraitUrl);
			}
		}
		return Player;
	}

	static TArray<FGPGS_Player> ParseFriendArrayFromJson(const FString& JsonString)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		TArray<FGPGS_Player> Friends;

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			// Get the array of friends
			const TArray<TSharedPtr<FJsonValue>>* FriendsJsonArray;
			if (JsonObject->TryGetArrayField("friends", FriendsJsonArray))
			{
				// Process each friend in the array
				for (const TSharedPtr<FJsonValue>& Value : *FriendsJsonArray)
				{
					TSharedPtr<FJsonObject> FriendObject = Value->AsObject();
					if (FriendObject.IsValid())
					{
						// Create a new friend, parse it, and add it to the array
						Friends.Add(FGPGS_Player::ParseFromJson(FriendObject));
					}
				}
			}
		}

		return Friends;
	}
};

/**
 * Google Play Games Services Event
 */
USTRUCT(BlueprintType)
struct FGPGS_Event
{
	GENERATED_BODY()

	/* Name of the Event */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	FString Name;

	/* Description of the Event */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	FString Description;

	/* Event ID of the Event from Google Play Games Services */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	FString EventID;

	/* Number of occurences (Value) of the Event */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	int64 Value;

	/* Is the Event Visible? */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	bool bIsVisible;
	
	FGPGS_Event()
		: Name(TEXT(""))
		, Description(TEXT(""))
		, EventID(TEXT(""))
		, Value(0)
		, bIsVisible(false)
	{
	}

	static FGPGS_Event ParseFromJson(const FString& JsonString)
	{
		FGPGS_Event Event;
		
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			if (JsonObject.IsValid())
			{
				JsonObject->TryGetStringField("name", Event.Name);
				JsonObject->TryGetStringField("description", Event.Description);
				JsonObject->TryGetStringField("eventId", Event.EventID);
				JsonObject->TryGetNumberField("value", Event.Value);
				JsonObject->TryGetBoolField("isVisible", Event.bIsVisible);
			}
		}
		return Event;
	}
};

/**
 * Google Play Games Services Player Statistics
 */
USTRUCT(BlueprintType)
struct FGPGS_PlayerStats
{
	GENERATED_BODY()

	/* Average Session Length of the Player */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	float AverageSessionLength;

	/* Days since last played */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	int32 DaysSinceLastPlayed;

	/* Number of purchases */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	int32 NumberOfPurchases;

	/* Number of sessions */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	int32 NumberOfSessions;

	/* Session Percentile from Google Play Games Services */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	float SessionPercentile;

	/* Spend Percentile from Google Play Games Services */
	UPROPERTY(BlueprintReadOnly, Category = "Google Play Games")
	float SpendPercentile;

	FGPGS_PlayerStats()
		: AverageSessionLength(0)
		, DaysSinceLastPlayed(0)
		, NumberOfPurchases(0)
		, NumberOfSessions(0)
		, SessionPercentile(0)
		, SpendPercentile(0)
	{
	}

	static FGPGS_PlayerStats ParseFromJson(const FString& JsonString)
	{
		FGPGS_PlayerStats PlayerStats;
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			if (JsonObject.IsValid())
			{
				JsonObject->TryGetNumberField("averageSessionLength", PlayerStats.AverageSessionLength);
				JsonObject->TryGetNumberField("daysSinceLastPlayed", PlayerStats.DaysSinceLastPlayed);
				JsonObject->TryGetNumberField("numberOfPurchases", PlayerStats.NumberOfPurchases);
				JsonObject->TryGetNumberField("numberOfSessions", PlayerStats.NumberOfSessions);
				JsonObject->TryGetNumberField("sessionPercentile", PlayerStats.SessionPercentile);
				JsonObject->TryGetNumberField("spendPercentile", PlayerStats.SpendPercentile);
			}
		}
		return PlayerStats;
	}
	
};
