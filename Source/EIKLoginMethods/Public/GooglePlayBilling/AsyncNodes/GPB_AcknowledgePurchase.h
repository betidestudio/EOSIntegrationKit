// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GooglePlayBilling/GooglePlayBillingStructures.h"
#include "GooglePlayBilling/GooglePlayBillingMethods.h"
#include "GPB_AcknowledgePurchase.generated.h"

UCLASS()
class EIKLOGINMETHODS_API UGPB_AcknowledgePurchase : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UGPB_AcknowledgePurchase();

    /** Acknowledges a purchase
     * @param PurchaseToken - The purchase token to acknowledge
     * @param WorldContextObject - The world context object
     * @return The async node instance
     */
    UFUNCTION(BlueprintCallable, Category = "Google Play Billing", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
    static UGPB_AcknowledgePurchase* AcknowledgePurchase(const FString& PurchaseToken, UObject* WorldContextObject);

    virtual void Activate() override;
    virtual void BeginDestroy() override;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnComplete, bool, bSuccess, const FString&, PurchaseToken, const FString&, Error);
    UPROPERTY(BlueprintAssignable, Category = "Google Play Billing")
    FOnComplete OnComplete;

    TWeakObjectPtr<UWorld> World;
    static TWeakObjectPtr<UGPB_AcknowledgePurchase> StaticInstance;
    static FTimerHandle TimeoutTimerHandle;

    void OnTimeout();
    FString PurchaseToken;
}; 