// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GooglePlayGames/GooglePlayGamesStructures.h"
#include "GPGS_LoadEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGPGS_LoadEventCallbackSignature, const FString&, ID, const FGPGS_Event&, Data, const FString&, Error);

/**
 * 
 */
UCLASS()
class EIKLOGINMETHODS_API UGPGS_LoadEvent : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * Load Event from Google Play Console
	 * @param ID ID of the Event from Google Play Console
	 * @return Event Data
	 */
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="EOS Integration Kit|Google Play Games|Events")
	static UGPGS_LoadEvent* LoadEvent(UObject* WorldContextObject, const FString& ID);

	virtual void Activate() override;
	virtual void BeginDestroy() override;

	// Executed after Event is Loaded successfully
	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_LoadEventCallbackSignature Success;

	// Executed if there was an error loading the Event
	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_LoadEventCallbackSignature Failure;
	
	static TWeakObjectPtr<UGPGS_LoadEvent> StaticInstance;

private:
	FString Var_ID;
};
