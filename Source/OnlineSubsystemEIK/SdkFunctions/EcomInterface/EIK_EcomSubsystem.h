// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EIK_EcomSubsystem.generated.h"

/**
 * 
 */
UCLASS(meta=(DisplayName="Ecom Interface"), Category="EOS Integration Kit", DisplayName="Ecom Interface")
class ONLINESUBSYSTEMEIK_API UEIK_EcomSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	//Release the memory associated with an EOS_Ecom_CatalogItem structure. This must be called on data retrieved from EOS_Ecom_CopyOfferItemByIndex.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CatalogItem_Release")
	void EIK_Ecom_CatalogItem_Release(FEIK_Ecom_CatalogItem CatalogItem);

	//Release the memory associated with an EOS_Ecom_CatalogOffer structure. This must be called on data retrieved from EOS_Ecom_CopyOfferByIndex.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CatalogOffer_Release")
	void EIK_Ecom_CatalogOffer_Release(FEIK_Ecom_CatalogOffer CatalogOffer);

	//Release the memory associated with an EOS_Ecom_CatalogRelease structure. This must be called on data retrieved from EOS_Ecom_CopyItemReleaseByIndex.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CatalogRelease_Release")
	void EIK_Ecom_CatalogRelease_Release(FEIK_Ecom_CatalogRelease CatalogRelease);

	//Fetches the entitlement with the given ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyEntitlementById")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyEntitlementById(FEIK_EpicAccountId LocalUserId, const FString& EntitlementId, FEIK_Ecom_Entitlement& OutEntitlement);

	//Fetches an entitlement from a given index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyEntitlementByIndex")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyEntitlementByIndex(FEIK_EpicAccountId LocalUserId, int32 EntitlementIndex, FEIK_Ecom_Entitlement& OutEntitlement);

	//Fetches a single entitlement with a given Entitlement Name. The Index is used to access individual entitlements among those with the same Entitlement Name. The Index can be a value from 0 to one less than the result from EOS_Ecom_GetEntitlementsByNameCount.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyEntitlementByNameAndIndex")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyEntitlementByNameAndIndex(FEIK_EpicAccountId LocalUserId, const FString& EntitlementName, int32 Index, FEIK_Ecom_Entitlement& OutEntitlement);

	//Fetches an item with a given ID.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyItemById")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyItemById(FEIK_EpicAccountId LocalUserId, const FString& ItemId, FEIK_Ecom_CatalogItem& OutCatalogItem);

	//Fetches an image from a given index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyItemImageInfoByIndex")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyItemImageInfoByIndex(FEIK_EpicAccountId LocalUserId, FEIK_Ecom_CatalogItemId ItemId, int32 ImageInfoIndex, FEIK_Ecom_KeyImageInfo& OutKeyImageInfo);

	//Fetches a release from a given index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyItemReleaseByIndex")
	TEnumAsByte<EEIK_Result>  EIK_Ecom_CopyItemReleaseByIndex(FEIK_EpicAccountId LocalUserId, FEIK_Ecom_CatalogItemId ItemId, int32 ReleaseIndex, FEIK_Ecom_CatalogRelease& OutRelease);

	//Fetches a redeemed entitlement id from a given index. Only entitlements that were redeemed during the last EOS_Ecom_RedeemEntitlements call can be copied.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyLastRedeemedEntitlementByIndex")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyLastRedeemedEntitlementByIndex(FEIK_EpicAccountId LocalUserId, int32 RedeemedEntitlementIndex, FString& OutRedeemedEntitlementId);

	//Fetches an offer with a given ID. The pricing and text are localized to the provided account.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyOfferById")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyOfferById(FEIK_EpicAccountId LocalUserId, const FEIK_Ecom_CatalogOfferId & OfferId, FEIK_Ecom_CatalogOffer& OutCatalogOffer);

	//Fetches an image from a given index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyOfferImageInfoByIndex")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyOfferImageInfoByIndex(FEIK_EpicAccountId LocalUserId, const FEIK_Ecom_CatalogOfferId & OfferId, int32 ImageInfoIndex, FEIK_Ecom_KeyImageInfo& OutKeyImageInfo);

	//Fetches an item from a given index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyOfferItemByIndex")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyOfferItemByIndex(FEIK_EpicAccountId LocalUserId, const FEIK_Ecom_CatalogOfferId & OfferId, int32 ItemIndex, FEIK_Ecom_CatalogItem& OutCatalogItem);

	//Fetches an offer from a given index. The pricing and text are localized to the provided account.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyOfferByIndex")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyOfferByIndex(FEIK_EpicAccountId LocalUserId, int32 OfferIndex, FEIK_Ecom_CatalogOffer& OutCatalogOffer);

	//Fetches the transaction handle at the given index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyTransactionById")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyTransactionById(FEIK_EpicAccountId LocalUserId, const FString& TransactionId, FEIK_Ecom_HTransaction& OutTransaction);

	//Fetches the transaction handle at the given index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_CopyTransactionByIndex")
	TEnumAsByte<EEIK_Result> EIK_Ecom_CopyTransactionByIndex(FEIK_EpicAccountId LocalUserId, int32 TransactionIndex, FEIK_Ecom_HTransaction& OutTransaction);

	//Release the memory associated with an EOS_Ecom_Entitlement structure. This must be called on data retrieved from EOS_Ecom_CopyEntitlementByIndex and EOS_Ecom_CopyEntitlementById.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_Entitlement_Release")
	void EIK_Ecom_Entitlement_Release(FEIK_Ecom_Entitlement Entitlement);

	//Fetch the number of entitlements with the given Entitlement Name that are cached for a given local user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_GetEntitlementsByNameCount")
	int32 EIK_Ecom_GetEntitlementsByNameCount(FEIK_EpicAccountId LocalUserId, const FEIK_Ecom_EntitlementName& EntitlementName);

	//Fetch the number of entitlements that are cached for a given local user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_GetEntitlementsCount")
	int32 EIK_Ecom_GetEntitlementsCount(FEIK_EpicAccountId LocalUserId);

	//Fetch the number of images that are associated with a given cached item for a local user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_GetItemImageInfoCount")
	int32 EIK_Ecom_GetItemImageInfoCount(FEIK_EpicAccountId LocalUserId, FEIK_Ecom_CatalogItemId ItemId);

	//Fetch the number of releases that are associated with a given cached item for a local user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_GetItemReleaseCount")
	int32 EIK_Ecom_GetItemReleaseCount(FEIK_EpicAccountId LocalUserId, FEIK_Ecom_CatalogItemId ItemId);

	//Fetch the number of entitlements that were redeemed during the last EOS_Ecom_RedeemEntitlements call.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_GetLastRedeemedEntitlementsCount")
	int32 EIK_Ecom_GetLastRedeemedEntitlementsCount(FEIK_EpicAccountId LocalUserId);

	//Fetch the number of offers that are cached for a given local user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_GetOfferCount")
	int32 EIK_Ecom_GetOfferCount(FEIK_EpicAccountId LocalUserId);

	//Fetch the number of images that are associated with a given cached offer for a local user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_GetOfferImageInfoCount")
	int32 EIK_Ecom_GetOfferImageInfoCount(FEIK_EpicAccountId LocalUserId, const FEIK_Ecom_CatalogOfferId & OfferId);

	//Fetch the number of items that are associated with a given cached offer for a local user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_GetOfferItemCount")
	int32 EIK_Ecom_GetOfferItemCount(FEIK_EpicAccountId LocalUserId, const FEIK_Ecom_CatalogOfferId & OfferId);

	//Fetch the number of transactions that are cached for a given local user.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_GetTransactionCount")
	int32 EIK_Ecom_GetTransactionCount(FEIK_EpicAccountId LocalUserId);

	//Release the memory associated with an EOS_Ecom_KeyImageInfo structure. This must be called on data retrieved from EOS_Ecom_CopyItemImageInfoByIndex.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_KeyImageInfo_Release")
	void EIK_Ecom_KeyImageInfo_Release(FEIK_Ecom_KeyImageInfo KeyImageInfo);

	//Fetches an entitlement from a given index.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_Transaction_CopyEntitlementByIndex")
	TEnumAsByte<EEIK_Result> EIK_Ecom_Transaction_CopyEntitlementByIndex(int32 EntitlementIndex, FEIK_Ecom_HTransaction Transaction, FEIK_Ecom_Entitlement& OutEntitlement);

	//Fetch the number of entitlements that are part of this transaction.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_Transaction_GetEntitlementsCount")
	int32 EIK_Ecom_Transaction_GetEntitlementsCount(FEIK_Ecom_HTransaction Transaction);

	//The Ecom Transaction Interface exposes getters for accessing information about a completed transaction. All Ecom Transaction Interface calls take a handle of type EOS_Ecom_HTransaction as the first parameter. An EOS_Ecom_HTransaction handle is originally returned as part of the EOS_Ecom_CheckoutCallbackInfo struct. An EOS_Ecom_HTransaction handle can also be retrieved from an EOS_HEcom handle using EOS_Ecom_CopyTransactionByIndex. It is expected that after a transaction that EOS_Ecom_Transaction_Release is called. When EOS_Platform_Release is called any remaining transactions will also be released.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_Transaction_GetTransactionId")
	FString EIK_Ecom_Transaction_GetTransactionId(FEIK_Ecom_HTransaction Transaction);
};
