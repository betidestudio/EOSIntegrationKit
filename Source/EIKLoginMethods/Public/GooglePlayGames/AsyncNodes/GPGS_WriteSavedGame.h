// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GPGS_WriteSavedGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGPGS_WriteSavedGameCallbackSignature, const FString&, ID, const FString&, Error);

/**
 * 
 */
UCLASS()
class EIKLOGINMETHODS_API UGPGS_WriteSavedGame : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="EOS Integration Kit|Google Play Games")
	static UGPGS_WriteSavedGame* WriteSavedGame(UObject* WorldContextObject, const FString& ID, const TArray<uint8>& Data, const FString& Description);

	virtual void Activate() override;
	virtual void BeginDestroy() override;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_WriteSavedGameCallbackSignature Success;

	UPROPERTY(BlueprintAssignable, Category="EOS Integration Kit|Google Play Games")
	FGPGS_WriteSavedGameCallbackSignature Failure;
	
	static TWeakObjectPtr<UGPGS_WriteSavedGame> StaticInstance;

private:
	FString Var_ID;
	FString Var_Description;
	TArray<uint8> Var_Data;
};
