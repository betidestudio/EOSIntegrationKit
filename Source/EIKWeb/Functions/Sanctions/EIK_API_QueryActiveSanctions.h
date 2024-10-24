// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_QueryActiveSanctions.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_QueryActiveSanctions : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	//The queryActiveSanctions request will return a list of all active sanctions for a given player.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_QueryActiveSanctions* QueryActiveSanctions(FString Authorization, FString ProductUserId, TArray<FString> Action);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_ProductUserId;
	TArray<FString> Var_Action;
};
