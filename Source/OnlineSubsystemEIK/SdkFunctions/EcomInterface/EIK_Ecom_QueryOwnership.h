// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Ecom_QueryOwnership.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Ecom_QueryOwnershipOptions
{
	GENERATED_BODY()

	//The Epic Account ID of the local user whose ownership to query
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_EpicAccountId LocalUserId;

	//The array of Catalog Item IDs to check for ownership
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TArray<FEIK_Ecom_CatalogItemId> CatalogItemIds;

	//Optional product namespace, if not the one specified during initialization
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString CatalogNamespace;
	
	FEIK_Ecom_QueryOwnershipOptions()
	{
		LocalUserId = FEIK_EpicAccountId();
		CatalogItemIds = TArray<FEIK_Ecom_CatalogItemId>();
		CatalogNamespace = "";
	}
	EOS_Ecom_QueryOwnershipOptions ToEOS_Ecom_QueryOwnershipOptions()
	{
		EOS_Ecom_QueryOwnershipOptions Options;
		Options.ApiVersion = EOS_ECOM_QUERYOWNERSHIP_API_LATEST;
		Options.LocalUserId = LocalUserId.GetValueAsEosType();
		Options.CatalogItemIdCount = CatalogItemIds.Num();
		Options.CatalogItemIds = new const char*[CatalogItemIds.Num()];
		for (int i = 0; i < CatalogItemIds.Num(); i++)
		{
			Options.CatalogItemIds[i] = CatalogItemIds[i].Ref;
		}
		Options.CatalogNamespace = TCHAR_TO_ANSI(*CatalogNamespace);
		return Options;
	}
};


USTRUCT(BlueprintType)
struct FEIK_Ecom_QueryOwnershipCallbackInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_EpicAccountId LocalUserId;
	
	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TEnumAsByte<EEIK_Result> ResultCode;

	UPROPERTY(BlueprintReadOnly, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TArray<FEIK_Ecom_ItemOwnership> ItemOwnership;
	
	FEIK_Ecom_QueryOwnershipCallbackInfo()
	{
		LocalUserId = FEIK_EpicAccountId();
		ResultCode = EEIK_Result::EOS_ServiceFailure;
		ItemOwnership = TArray<FEIK_Ecom_ItemOwnership>();
	}
	FEIK_Ecom_QueryOwnershipCallbackInfo(const EOS_Ecom_QueryOwnershipCallbackInfo* Data)
	{
		LocalUserId = Data->LocalUserId;
		ResultCode = static_cast<EEIK_Result>(Data->ResultCode);
		ItemOwnership = TArray<FEIK_Ecom_ItemOwnership>();
		int32 ItemOwnershipCount = Data->ItemOwnershipCount;
		for (int i = 0; i < ItemOwnershipCount; i++)
		{
			ItemOwnership.Add(Data->ItemOwnership[i]);
		}
	}
	
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_Ecom_QueryOwnershipCallback, const FEIK_Ecom_QueryOwnershipCallbackInfo&, Data);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Ecom_QueryOwnership : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Query the ownership status for a given list of catalog item IDs defined with Epic Online Services. This data will be cached for a limited time and retrieved again from the backend when necessary Depending on the number of catalog item ids passed, the SDK splits the query into smaller batch requests to the backend and aggregates the result. Note: If one of the request batches fails, no data is cached and the entire query is marked as failed.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName = "EOS_Ecom_QueryOwnership")
	static UEIK_Ecom_QueryOwnership* EIK_Ecom_QueryOwnership(FEIK_Ecom_QueryOwnershipOptions QueryOwnershipOptions);

	UPROPERTY(BlueprintAssignable)
	FEIK_Ecom_QueryOwnershipCallback OnCallback;
private:
	FEIK_Ecom_QueryOwnershipOptions Var_QueryOwnershipOptions;
	static void EOS_CALL OnQueryOwnershipCallback(const EOS_Ecom_QueryOwnershipCallbackInfo* Data);
	virtual void Activate() override;
};
