// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GPGS_ReadSavedGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGPGS_ReadSavedGameCallbackSignature, const FString&, ID, const TArray<uint8>&, Data, const FString&, Error);

/**
 * 
 */
UCLASS()
class EIKLOGINMETHODS_API UGPGS_ReadSavedGame : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="EOS Integration Kit|Google Play Games")
	static UGPGS_ReadSavedGame* ReadSavedGame(UObject* WorldContextObject, const FString& ID);

	virtual void Activate() override;
	virtual void BeginDestroy() override;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_ReadSavedGameCallbackSignature Success;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_ReadSavedGameCallbackSignature Failure;
	
	static TWeakObjectPtr<UGPGS_ReadSavedGame> StaticInstance;

private:
	FString Var_ID;
};
