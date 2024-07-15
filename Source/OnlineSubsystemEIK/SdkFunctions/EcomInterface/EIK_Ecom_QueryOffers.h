// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Ecom_QueryOffers.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Ecom_QueryOffersCallback, FEIK_EpicAccountId, LocalUserId, const TEnumAsByte<EEIK_Result>&, ResultCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Ecom_QueryOffers : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	
	//Query for a list of catalog offers defined with Epic Online Services. This data will be cached for a limited time and retrieved again from the backend when necessary.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName = "EOS_Ecom_QueryOffers")
	static UEIK_Ecom_QueryOffers* EIK_Ecom_QueryOffers(FEIK_EpicAccountId LocalUserId, FString OverrideCatalogNamespace);

	UPROPERTY(BlueprintAssignable)
	FEIK_Ecom_QueryOffersCallback OnCallback;
private:
	FEIK_EpicAccountId Var_LocalUserId;
	FString Var_OverrideCatalogNamespace;
	static void EOS_CALL OnQueryOffersCallback(const EOS_Ecom_QueryOffersCallbackInfo* Data);
	virtual void Activate() override;
};
