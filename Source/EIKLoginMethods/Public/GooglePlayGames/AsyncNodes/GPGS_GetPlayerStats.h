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
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="EOS Integration Kit|Google Play Games")
	static UGPGS_GetPlayerStats* GetPlayerStats(UObject* WorldContextObject);

	virtual void Activate() override;
	virtual void BeginDestroy() override;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_GetPlayerStatsCallbackSignature Success;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_GetPlayerStatsCallbackSignature Failure;
	
	static TWeakObjectPtr<UGPGS_GetPlayerStats> StaticInstance;
};
