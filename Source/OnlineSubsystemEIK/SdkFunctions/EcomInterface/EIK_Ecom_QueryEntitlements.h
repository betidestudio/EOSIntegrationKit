// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Ecom_QueryEntitlements.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Ecom_QueryEntitlementsOptions
{
	GENERATED_BODY()

	//The Epic Account ID of the local user whose Entitlements you want to retrieve
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_EpicAccountId LocalUserId;

	//An array of Entitlement Names that you want to check
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TArray<FString> EntitlementNames;

	//If true, Entitlements that have been redeemed will be included in the results.
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	bool bIncludeRedeemed;

	FEIK_Ecom_QueryEntitlementsOptions()
	{
		LocalUserId = FEIK_EpicAccountId();
		EntitlementNames = TArray<FString>();
		bIncludeRedeemed = false;
	}
	EOS_Ecom_QueryEntitlementsOptions ToEOS_Ecom_QueryEntitlementsOptions()
	{
		EOS_Ecom_QueryEntitlementsOptions Options;
		Options.ApiVersion = EOS_ECOM_QUERYENTITLEMENTS_API_LATEST;
		Options.LocalUserId = LocalUserId.GetValueAsEosType();
		Options.EntitlementNameCount = EntitlementNames.Num();
		Options.EntitlementNames = new const char*[EntitlementNames.Num()];
		for (int i = 0; i < EntitlementNames.Num(); i++)
		{
			Options.EntitlementNames[i] = TCHAR_TO_ANSI(*EntitlementNames[i]);
		}
		Options.bIncludeRedeemed = bIncludeRedeemed;
		return Options;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Ecom_QueryEntitlementsCallback, const FEIK_EpicAccountId&, LocalUserId, const TEnumAsByte<EEIK_Result>&, Result);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Ecom_QueryEntitlements : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Query the entitlement information defined with Epic Online Services. A set of entitlement names can be provided to filter the set of entitlements associated with the account. This data will be cached for a limited time and retrieved again from the backend when necessary. Depending on the number of entitlements passed, the SDK splits the query into smaller batch requests to the backend and aggregates the result. Note: If one of the request batches fails, no data is cached and the entire query is marked as failed. Use EOS_Ecom_CopyEntitlementByIndex, EOS_Ecom_CopyEntitlementByNameAndIndex, and EOS_Ecom_CopyEntitlementById to get the entitlement details. Use EOS_Ecom_GetEntitlementsByNameCount to retrieve the number of entitlements with a specific entitlement name.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_QueryEntitlements")
	static UEIK_Ecom_QueryEntitlements* EIK_Ecom_QueryEntitlements(FEIK_Ecom_QueryEntitlementsOptions QueryEntitlementsOptions);

	UPROPERTY(BlueprintAssignable)
	FEIK_Ecom_QueryEntitlementsCallback OnCallback;
private:
	static void EOS_CALL OnQueryEntitlementsCallback(const EOS_Ecom_QueryEntitlementsCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_Ecom_QueryEntitlementsOptions Local_QueryEntitlementsOptions;
};
