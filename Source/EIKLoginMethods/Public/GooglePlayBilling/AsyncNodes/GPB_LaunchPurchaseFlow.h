// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GooglePlayBilling/GooglePlayBillingStructures.h"
#include "GooglePlayBilling/GooglePlayBillingMethods.h"
#include "GPB_LaunchPurchaseFlow.generated.h"

UCLASS()
class EIKLOGINMETHODS_API UGPB_LaunchPurchaseFlow : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UGPB_LaunchPurchaseFlow();

    /** Launches the purchase flow for a product
     * @param ProductDetails - The product details to purchase
     * @param OfferToken - The offer token for the purchase (empty for one-time purchases)
     * @return The async node instance
     */
    UFUNCTION(BlueprintCallable, Category = "Google Play Billing", meta = (BlueprintInternalUseOnly = "true"))
    static UGPB_LaunchPurchaseFlow* LaunchPurchaseFlow(const FGPB_ProductDetails& ProductDetails, const FString& OfferToken);

    virtual void Activate() override;
    virtual void BeginDestroy() override;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnComplete, bool, bSuccess, const FGPB_Purchase&, Purchase, const FString&, Error);
    UPROPERTY(BlueprintAssignable, Category = "Google Play Billing")
    FOnComplete OnComplete;

    FGPB_ProductDetails ProductDetails;
    FString OfferToken;
    TWeakObjectPtr<UWorld> World;
    static TWeakObjectPtr<UGPB_LaunchPurchaseFlow> StaticInstance;
    static FTimerHandle TimeoutTimerHandle;

    void OnTimeout();
}; 