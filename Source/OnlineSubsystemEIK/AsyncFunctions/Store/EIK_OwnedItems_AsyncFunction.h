// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Runtime/Launch/Resources/Version.h"
#include "EIK_OwnedItems_AsyncFunction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetOwnedItems_Delegate, const TArray<FString>&, OwnedItemNames, const FString&, ErrorString);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_OwnedItems_AsyncFunction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, DisplayName="Success")
	FGetOwnedItems_Delegate OnSuccess;
	UPROPERTY(BlueprintAssignable, DisplayName="Failure")
	FGetOwnedItems_Delegate OnFail;

	bool bDelegateCalled = false;
	void Activate() override;
	void GetOwnedItems();

	/*
	This C++ method gets the owned items in the online subsystem using the selected method and sets up a callback function to handle the response.
	Documentation link: https://betide-studio.gitbook.io/eos-integration-kit/stats/
	*/
	UFUNCTION(BlueprintCallable, DisplayName="Get EIK Owned Items",meta = (BlueprintInternalUseOnly = "true"), Category="EOS Integration Kit || Store")
	static UEIK_OwnedItems_AsyncFunction* GetEIKOwnedItems();
};
