// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "GooglePlayBillingStructures.generated.h"

inline FString JsonObjToString(const TSharedRef<FJsonObject>& Obj)
{
	FString Out;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Out);
	FJsonSerializer::Serialize(Obj, Writer);
	return Out;
}

/**
 * One–time purchase offer
 */
USTRUCT(BlueprintType)
struct FGPB_OneTimePurchaseOfferDetails
{
	GENERATED_BODY()

	/** The formatted price of the item, including its currency form. For example, '$3.50' */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString FormattedPrice;

	/** The price in micro-units, where 1,000,000 micro-units equals one unit of the currency. For example, if price is '€7.99', price_amount_micros is '7990000' */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	int64   PriceAmountMicros = 0;

	/** The ISO 4217 currency code for price. For example, if price is specified in British pounds sterling, price_currency_code is 'GBP' */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString PriceCurrencyCode;

	FString ToJson() const
	{
		const TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();

		Obj->SetStringField(TEXT("formattedPrice"),   FormattedPrice);
		Obj->SetNumberField(TEXT("priceAmountMicros"), static_cast<double>(PriceAmountMicros));
		Obj->SetStringField(TEXT("priceCurrencyCode"), PriceCurrencyCode);

		return JsonObjToString(Obj);
	}


};

/** Recurrence mode as defined by Play Billing */
UENUM(BlueprintType)
enum class ERecurrenceMode : uint8
{
	NONE				= 0	UMETA(DisplayName = "None"),
	InfiniteRecurring	= 1	UMETA(DisplayName = "Infinite Recurring"),
	FiniteRecurring		= 2	UMETA(DisplayName = "Finite Recurring"),
	NonRecurring		= 3	UMETA(DisplayName = "Non-recurring")
};

/**
 * Subscription pricing phase
 */
USTRUCT(BlueprintType)
struct FGPB_PricingPhase
{
	GENERATED_BODY()

	/** The number of billing cycles for which the pricing phase is valid */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	int32 BillingCycleCount = 0;

	/** The billing period for the pricing phase, specified in ISO 8601 format */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString BillingPeriod;

	/** The formatted price of the item, including its currency form. For example, '$3.50' */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString FormattedPrice;

	/** The price in micro-units, where 1,000,000 micro-units equals one unit of the currency */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	int64 PriceAmountMicros = 0;

	/** The ISO 4217 currency code for price */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString PriceCurrencyCode;

	/** The recurrence mode of the pricing phase */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	ERecurrenceMode RecurrenceMode = ERecurrenceMode::NonRecurring;

	FString ToJson() const
	{
		const TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();

		Obj->SetNumberField(TEXT("billingCycleCount"),  BillingCycleCount);
		Obj->SetStringField(TEXT("billingPeriod"),      BillingPeriod);
		Obj->SetStringField(TEXT("formattedPrice"),     FormattedPrice);
		Obj->SetNumberField(TEXT("priceAmountMicros"),  static_cast<double>(PriceAmountMicros));
		Obj->SetStringField(TEXT("priceCurrencyCode"),  PriceCurrencyCode);
		Obj->SetNumberField(TEXT("recurrenceMode"),     static_cast<int32>(RecurrenceMode));

		return JsonObjToString(Obj);
	}

};

/**
 * Installment plan details inside a subscription offer
 */
USTRUCT(BlueprintType)
struct FGPB_InstallmentPlanDetails
{
	GENERATED_BODY()

	/** The number of payments in the installment plan */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	int32 InstallmentPlanCommitmentPaymentsCount = 0;

	/** The number of subsequent payments in the installment plan */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	int32 SubsequentInstallmentPlanCommitmentPaymentsCount = 0;

	FString ToJson() const
	{
		// Emit only when we have non-default values
		if (InstallmentPlanCommitmentPaymentsCount == 0 &&
			SubsequentInstallmentPlanCommitmentPaymentsCount == 0)
		{
			return TEXT("{}");			// matches Java omitting the object
		}

		const TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();
		Obj->SetNumberField(TEXT("installmentPlanCommitmentPaymentsCount"),
							InstallmentPlanCommitmentPaymentsCount);
		Obj->SetNumberField(TEXT("subsequentInstallmentPlanCommitmentPaymentsCount"),
							SubsequentInstallmentPlanCommitmentPaymentsCount);
		return JsonObjToString(Obj);
	}

};

/**
 * Subscription offer
 */
USTRUCT(BlueprintType)
struct FGPB_SubscriptionOfferDetails
{
	GENERATED_BODY()

	/** The base plan ID of the subscription offer */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString BasePlanId;

	/** The offer ID of the subscription offer */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString OfferId;

	/** The list of tags associated with the subscription offer */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	TArray<FString> OfferTags;

	/** The offer token of the subscription offer */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString OfferToken;

	/** The installment plan details of the subscription offer */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FGPB_InstallmentPlanDetails InstallmentPlanDetails;

	/** The list of pricing phases for the subscription offer */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	TArray<FGPB_PricingPhase> PricingPhases;

	FString ToJson() const
	{
		const TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();

		Obj->SetStringField(TEXT("basePlanId"), BasePlanId);
		Obj->SetStringField(TEXT("offerId"),    OfferId);
		Obj->SetStringField(TEXT("offerToken"), OfferToken);

		/* offerTags array (can be empty) */
		{
			TArray<TSharedPtr<FJsonValue>> TagVals;
			for (const FString& Tag : OfferTags)
			{
				TagVals.Add(MakeShared<FJsonValueString>(Tag));
			}
			Obj->SetArrayField(TEXT("offerTags"), TagVals);
		}

		/* installmentPlanDetails (optional) */
		{
			const FString InstJson = InstallmentPlanDetails.ToJson();
			if (!InstJson.Equals(TEXT("{}")))
			{
				TSharedPtr<FJsonObject> InstObj;
				TSharedRef<TJsonReader<>> R = TJsonReaderFactory<>::Create(InstJson);
				FJsonSerializer::Deserialize(R, InstObj);
				Obj->SetObjectField(TEXT("installmentPlanDetails"), InstObj);
			}
		}

		/* pricingPhases array */
		{
			TArray<TSharedPtr<FJsonValue>> PhaseVals;
			for (const FGPB_PricingPhase& Phase : PricingPhases)
			{
				TSharedPtr<FJsonObject> PhaseObj;
				TSharedRef<TJsonReader<>> R =
					TJsonReaderFactory<>::Create(Phase.ToJson());
				FJsonSerializer::Deserialize(R, PhaseObj);
				PhaseVals.Add(MakeShared<FJsonValueObject>(PhaseObj));
			}
			Obj->SetArrayField(TEXT("pricingPhases"), PhaseVals);
		}

		return JsonObjToString(Obj);
	}

};

/** Product type as defined by Play Billing */
UENUM(BlueprintType)
enum class EProductType : uint8
{
	Unknown     = 0 UMETA(DisplayName = "Unknown"),
	InApp       = 1 UMETA(DisplayName = "In-App"),
	Subscription = 2 UMETA(DisplayName = "Subscription")
};

/**
 * Product details root
 */
USTRUCT(BlueprintType)
struct FGPB_ProductDetails
{
	GENERATED_BODY()

	/** The description of the product */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString Description;

	/** The name of the product */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString Name;

	/** The unique product identifier */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString ProductId;

	/** The type of the product (InApp or Subscription) */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	EProductType ProductType = EProductType::Unknown;

	/** The title of the product */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString Title;

	/** The one-time purchase offer details for the product */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FGPB_OneTimePurchaseOfferDetails OneTimePurchaseOfferDetails;

	/** The list of subscription offer details for the product */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	TArray<FGPB_SubscriptionOfferDetails> SubscriptionOfferDetails;

	/* ---------- JSON helpers ---------- */

	/** Parse from a UE JSON object (already deserialized) */
	static FGPB_ProductDetails ParseFromJson(const TSharedPtr<FJsonObject>& Obj)
	{
		FGPB_ProductDetails Out;
		if (!Obj.IsValid()) return Out;

		Obj->TryGetStringField(TEXT("description"), Out.Description);
		Obj->TryGetStringField(TEXT("name"),        Out.Name);
		Obj->TryGetStringField(TEXT("productId"),   Out.ProductId);
		
		// Parse product type
		FString ProductTypeStr;
		if (Obj->TryGetStringField(TEXT("productType"), ProductTypeStr))
		{
			if (ProductTypeStr.Equals(TEXT("inapp"), ESearchCase::IgnoreCase))
			{
				Out.ProductType = EProductType::InApp;
			}
			else if (ProductTypeStr.Equals(TEXT("subs"), ESearchCase::IgnoreCase))
			{
				Out.ProductType = EProductType::Subscription;
			}
			else
			{
				Out.ProductType = EProductType::Unknown;
			}
		}

		Obj->TryGetStringField(TEXT("title"),       Out.Title);

		/* One-time purchase ------------- */
		if (const TSharedPtr<FJsonObject>* OneTimeObj; Obj->TryGetObjectField(TEXT("oneTimePurchaseOfferDetails"), OneTimeObj))
		{
			(*OneTimeObj)->TryGetStringField(TEXT("formattedPrice"),   Out.OneTimePurchaseOfferDetails.FormattedPrice);
			(*OneTimeObj)->TryGetNumberField(TEXT("priceAmountMicros"),Out.OneTimePurchaseOfferDetails.PriceAmountMicros);
			(*OneTimeObj)->TryGetStringField(TEXT("priceCurrencyCode"),Out.OneTimePurchaseOfferDetails.PriceCurrencyCode);
		}

		/* Subscription offers ----------- */
		if (const TArray<TSharedPtr<FJsonValue>>* OffersArray; Obj->TryGetArrayField(TEXT("subscriptionOfferDetails"), OffersArray))
		{
			for (const TSharedPtr<FJsonValue>& OfferVal : *OffersArray)
			{
				const TSharedPtr<FJsonObject> OfferObj = OfferVal->AsObject();
				if (!OfferObj.IsValid()) continue;

				FGPB_SubscriptionOfferDetails Offer;
				OfferObj->TryGetStringField(TEXT("basePlanId"), Offer.BasePlanId);
				OfferObj->TryGetStringField(TEXT("offerId"),    Offer.OfferId);
				OfferObj->TryGetStringField(TEXT("offerToken"), Offer.OfferToken);

				/* offerTags */
				if (const TArray<TSharedPtr<FJsonValue>>* TagsArray; OfferObj->TryGetArrayField(TEXT("offerTags"), TagsArray))
				{
					for (const TSharedPtr<FJsonValue>& TagVal : *TagsArray)
					{
						FString Tag; TagVal->TryGetString(Tag);
						Offer.OfferTags.Add(Tag);
					}
				}

				/* installmentPlanDetails */
				if (const TSharedPtr<FJsonObject>* InstObj; OfferObj->TryGetObjectField(TEXT("installmentPlanDetails"), InstObj))
				{
					(*InstObj)->TryGetNumberField(TEXT("installmentPlanCommitmentPaymentsCount"),
						Offer.InstallmentPlanDetails.InstallmentPlanCommitmentPaymentsCount);
					(*InstObj)->TryGetNumberField(TEXT("subsequentInstallmentPlanCommitmentPaymentsCount"),
						Offer.InstallmentPlanDetails.SubsequentInstallmentPlanCommitmentPaymentsCount);
				}

				/* pricingPhases */
				if (const TArray<TSharedPtr<FJsonValue>>* PhasesArray; OfferObj->TryGetArrayField(TEXT("pricingPhases"), PhasesArray))
				{
					for (const TSharedPtr<FJsonValue>& PhaseVal : *PhasesArray)
					{
						const TSharedPtr<FJsonObject> PhaseObj = PhaseVal->AsObject();
						if (!PhaseObj.IsValid()) continue;

						FGPB_PricingPhase Phase;
						PhaseObj->TryGetNumberField(TEXT("billingCycleCount"), Phase.BillingCycleCount);
						PhaseObj->TryGetStringField(TEXT("billingPeriod"),     Phase.BillingPeriod);
						PhaseObj->TryGetStringField(TEXT("formattedPrice"),    Phase.FormattedPrice);
						PhaseObj->TryGetNumberField(TEXT("priceAmountMicros"), Phase.PriceAmountMicros);
						PhaseObj->TryGetStringField(TEXT("priceCurrencyCode"), Phase.PriceCurrencyCode);
						
						int32 RecurrenceModeInt = 0;
						if (PhaseObj->TryGetNumberField(TEXT("recurrenceMode"), RecurrenceModeInt))
						{
							Phase.RecurrenceMode = static_cast<ERecurrenceMode>(RecurrenceModeInt);
						}

						Offer.PricingPhases.Add(Phase);
					}
				}

				Out.SubscriptionOfferDetails.Add(Offer);
			}
		}

		return Out;
	}

	/** Parse directly from a JSON string coming from Java's `toJson()` */
	static FGPB_ProductDetails ParseFromJson(const FString& JsonString)
	{
		TSharedPtr<FJsonObject> Obj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(Reader, Obj) && Obj.IsValid())
		{
			return ParseFromJson(Obj);
		}
		return FGPB_ProductDetails();
	}

	FString ToJson() const
	{
		const TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();

		Obj->SetStringField(TEXT("description"), Description);
		Obj->SetStringField(TEXT("name"),        Name);
		Obj->SetStringField(TEXT("productId"),   ProductId);
		
		// Convert product type to string
		FString ProductTypeStr;
		switch (ProductType)
		{
			case EProductType::InApp:
				ProductTypeStr = TEXT("inapp");
				break;
			case EProductType::Subscription:
				ProductTypeStr = TEXT("subs");
				break;
			default:
				ProductTypeStr = TEXT("unknown");
				break;
		}
		Obj->SetStringField(TEXT("productType"), ProductTypeStr);
		
		Obj->SetStringField(TEXT("title"),       Title);

		/* oneTimePurchaseOfferDetails (optional) */
		{
			const FString OneJson = OneTimePurchaseOfferDetails.ToJson();
			if (!OneJson.Equals(TEXT("{}")))
			{
				TSharedPtr<FJsonObject> OneObj;
				TSharedRef<TJsonReader<>> R =
					TJsonReaderFactory<>::Create(OneJson);
				FJsonSerializer::Deserialize(R, OneObj);
				Obj->SetObjectField(TEXT("oneTimePurchaseOfferDetails"), OneObj);
			}
		}

		/* subscriptionOfferDetails array (may be empty) */
		{
			TArray<TSharedPtr<FJsonValue>> OfferVals;
			for (const FGPB_SubscriptionOfferDetails& Offer : SubscriptionOfferDetails)
			{
				TSharedPtr<FJsonObject> OfferObj;
				TSharedRef<TJsonReader<>> R =
					TJsonReaderFactory<>::Create(Offer.ToJson());
				FJsonSerializer::Deserialize(R, OfferObj);
				OfferVals.Add(MakeShared<FJsonValueObject>(OfferObj));
			}
			Obj->SetArrayField(TEXT("subscriptionOfferDetails"), OfferVals);
		}

		return JsonObjToString(Obj);
	}

};

/** Java constants 0 / 1 / 2 expressed as a scoped enum */
UENUM(BlueprintType)
enum class EPurchaseState : uint8
{
	Unspecified		= 0		UMETA(DisplayName = "Unspecified"),
	Purchased		= 1		UMETA(DisplayName = "Purchased"),
	Pending			= 2		UMETA(DisplayName = "Pending")
};

/** Root struct that represents a Google Play Billing Purchase */
USTRUCT(BlueprintType)
struct FGPB_Purchase
{
	GENERATED_BODY()

	/** The order ID of the purchase */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString OrderId;

	/** The list of product IDs included in the purchase */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	TArray<FString> Products;

	/** The time of the purchase in milliseconds since the epoch */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	int64 PurchaseTime = 0;

	/** The purchase token for the purchase */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString PurchaseToken;

	/** The quantity of the purchase */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	int32 Quantity = 0;

	/** The signature of the purchase */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	FString Signature;

	/** Whether the purchase has been acknowledged */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	bool bIsAcknowledged = false;

	/** The state of the purchase (Unspecified, Purchased, or Pending) */
	UPROPERTY(BlueprintReadOnly, Category = "Play Billing")
	EPurchaseState PurchaseState = EPurchaseState::Unspecified;

	/* ----------- JSON helpers ------------ */

	/** Parse from an already-deserialized UE JSON object */
	static FGPB_Purchase ParseFromJson(const TSharedPtr<FJsonObject>& Obj)
	{
		FGPB_Purchase Out;
		if (!Obj.IsValid()) return Out;

		Obj->TryGetStringField(TEXT("orderId"),       Out.OrderId);
		Obj->TryGetStringField(TEXT("purchaseToken"), Out.PurchaseToken);
		Obj->TryGetStringField(TEXT("signature"),     Out.Signature);

		Obj->TryGetNumberField(TEXT("purchaseTime"),  Out.PurchaseTime);
		Obj->TryGetNumberField(TEXT("quantity"),      Out.Quantity);

		bool Ack = false;
		if (Obj->TryGetBoolField(TEXT("isAcknowledged"), Ack)) Out.bIsAcknowledged = Ack;

		/* products array */
		if (const TArray<TSharedPtr<FJsonValue>>* ProdArray; Obj->TryGetArrayField(TEXT("products"), ProdArray))
		{
			for (const TSharedPtr<FJsonValue>& Val : *ProdArray)
			{
				FString Prod; Val->TryGetString(Prod);
				Out.Products.Add(Prod);
			}
		}

		/* purchaseState integer → enum */
		int32 StateInt = 0;
		if (Obj->TryGetNumberField(TEXT("purchaseState"), StateInt))
		{
			// Ensure the value is within valid enum range
			if (StateInt >= 0 && StateInt <= 2)
			{
				Out.PurchaseState = static_cast<EPurchaseState>(StateInt);
			}
			else
			{
				Out.PurchaseState = EPurchaseState::Unspecified;
			}
		}

		return Out;
	}

	/** Parse directly from a raw JSON string generated by Java */
	static FGPB_Purchase ParseFromJson(const FString& JsonString)
	{
		TSharedPtr<FJsonObject> Obj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		if (FJsonSerializer::Deserialize(Reader, Obj) && Obj.IsValid())
		{
			return ParseFromJson(Obj);
		}
		return FGPB_Purchase();
	}

	FString ToJson() const
	{
		const TSharedRef<FJsonObject> Obj = MakeShared<FJsonObject>();

		Obj->SetStringField(TEXT("orderId"),        OrderId);
		Obj->SetStringField(TEXT("purchaseToken"),  PurchaseToken);
		Obj->SetStringField(TEXT("signature"),      Signature);
		Obj->SetNumberField(TEXT("purchaseTime"),   static_cast<double>(PurchaseTime));
		Obj->SetNumberField(TEXT("quantity"),       Quantity);
		Obj->SetBoolField  (TEXT("isAcknowledged"), bIsAcknowledged);
		Obj->SetNumberField(TEXT("purchaseState"),  static_cast<int32>(PurchaseState));

		TArray<TSharedPtr<FJsonValue>> ProductVals;
		for (const FString& P : Products)
		{
			ProductVals.Add(MakeShared<FJsonValueString>(P));
		}
		Obj->SetArrayField(TEXT("products"), ProductVals);

		return JsonObjToString(Obj);
	}

};