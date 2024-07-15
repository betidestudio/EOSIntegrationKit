// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_ecom.h"
#include "eos_ecom_types.h"
THIRD_PARTY_INCLUDES_END
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_Ecom_Checkout.generated.h"

USTRUCT(BlueprintType)
struct FEIK_Ecom_CheckoutOptions
{
	GENERATED_BODY()

	//The Epic Account ID of the local user who is making the purchase
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_EpicAccountId LocalUserId;

	//The catalog namespace will be the current Sandbox ID (in EOS_Platform_Options) unless overridden by this field
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString OverrideCatalogNamespace;

	//The number of EOS_Ecom_CheckoutEntry elements contained in Entries
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	int32 EntryCount;

	//An array of EOS_Ecom_CheckoutEntry elements, each containing the details of a single offer
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TArray<FEIK_Ecom_CheckoutEntry> Entries;

	FEIK_Ecom_CheckoutOptions()
	{
		LocalUserId = FEIK_EpicAccountId();
		OverrideCatalogNamespace = "";
		EntryCount = 0;
		Entries = TArray<FEIK_Ecom_CheckoutEntry>();
	}
	EOS_Ecom_CheckoutOptions ToEOS_Ecom_CheckoutOptions()
	{
		EOS_Ecom_CheckoutOptions Options;
		Options.ApiVersion = EOS_ECOM_CHECKOUT_API_LATEST;
		Options.LocalUserId = LocalUserId.GetValueAsEosType();
		Options.OverrideCatalogNamespace = TCHAR_TO_ANSI(*OverrideCatalogNamespace);
		Options.EntryCount = EntryCount;
		EOS_Ecom_CheckoutEntry* EntriesArray = new EOS_Ecom_CheckoutEntry[EntryCount];
		for (int i = 0; i < EntryCount; i++)
		{
			EntriesArray[i] = Entries[i].EOS_Ecom_CheckoutEntry_FromStruct();
		}
		Options.Entries = EntriesArray;
		return Options;
	}
};

USTRUCT(BlueprintType)
struct FEIK_Ecom_CheckoutCallbackInfo
{
	GENERATED_BODY()

	//Result code for the operation. EOS_Success is returned for a successful request, otherwise one of the error codes is returned. See eos_common.h
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	TEnumAsByte<EEIK_Result> ResultCode;

	//The transaction ID which can be used to obtain an EOS_Ecom_HTransaction using EOS_Ecom_CopyTransactionById.
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FString TransactionId;

	//The Epic Account ID of the user who initiated the purchase
	UPROPERTY(BlueprintReadWrite, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_EpicAccountId LocalUserId;

	FEIK_Ecom_CheckoutCallbackInfo()
	{
		ResultCode = EEIK_Result::EOS_NotFound;
		TransactionId = "";
		LocalUserId = FEIK_EpicAccountId();
	}

	FEIK_Ecom_CheckoutCallbackInfo(const EOS_Ecom_CheckoutCallbackInfo* Data)
	{
		ResultCode = static_cast<EEIK_Result>(Data->ResultCode);
		TransactionId = Data->TransactionId;
		LocalUserId = Data->LocalUserId;
	}
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_Ecom_CheckoutCallback, const FEIK_Ecom_CheckoutCallbackInfo&, Data);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Ecom_Checkout : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Initiates the purchase flow for a set of offers. The callback is triggered after the purchase flow. On success, the set of entitlements that were unlocked will be cached. On success, a Transaction ID will be returned. The Transaction ID can be used to obtain an EOS_Ecom_HTransaction handle. The handle can then be used to retrieve the entitlements rewarded by the purchase.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName = "EOS_Ecom_Checkout")
	static UEIK_Ecom_Checkout* EIK_Ecom_Checkout(FEIK_Ecom_CheckoutOptions CheckoutOptions);
	
	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_CheckoutCallback OnCallback;
	
private:
	FEIK_Ecom_CheckoutOptions Local_CheckoutOptions;
	static void EOS_CALL OnCheckoutCallback(const EOS_Ecom_CheckoutCallbackInfo* Data);
	virtual void Activate() override;
};
