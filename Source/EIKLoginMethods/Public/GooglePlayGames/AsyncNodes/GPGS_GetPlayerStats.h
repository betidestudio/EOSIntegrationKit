// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GooglePlayGames/GooglePlayGamesStructures.h"
#include "GPGS_GetPlayerStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGPGS_GetPlayerStatsCallbackSignature, const FGPGS_PlayerStats&, Data, const FString&, Error);

/**
 * 
 */
UCLASS()
class EIKLOGINMETHODS_API UGPGS_GetPlayerStats : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * Get Player Stats of the Signed In Player
	 * @return PlayerStats of the currently Signed In Player
	 */
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="EOS Integration Kit|Google Play Games|PlayerStats")
	static UGPGS_GetPlayerStats* GetPlayerStats(UObject* WorldContextObject);

	virtual void Activate() override;
	virtual void BeginDestroy() override;

	// Executed after PlayerStats are retrieved Successfully
	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_GetPlayerStatsCallbackSignature Success;

	// Executed if GetPlayerStats Fails
	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_GetPlayerStatsCallbackSignature Failure;
	
	static TWeakObjectPtr<UGPGS_GetPlayerStats> StaticInstance;
};
