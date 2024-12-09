// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Launch/Resources/Version.h"
#include "EIK_GetOffers_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetOffers_Delegate, const TArray<FOffersStruct>&, Offers);

/**
 * 
 */
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_GetOffers_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, DisplayName="Success")
	FGetOffers_Delegate OnSuccess;
	UPROPERTY(BlueprintAssignable, DisplayName="Failure")
	FGetOffers_Delegate OnFail;

	bool bDelegateCalled = false;
	void Activate() override;
	void GetOffers();

	/*
	This C++ method gets the offers in the online subsystem using the selected method and sets up a callback function to handle the response.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/store/
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Get EIK Offers",meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Store")
	static UEIK_GetOffers_AsyncFunction* GetEIKOffers();
};
