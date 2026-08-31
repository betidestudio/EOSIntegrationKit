// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GooglePlayBilling/GooglePlayBillingStructures.h"
#include "GooglePlayBilling/GooglePlayBillingMethods.h"
#include "GPB_QueryPurchases.generated.h"

UCLASS()
class EIKLOGINMETHODS_API UGPB_QueryPurchases : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UGPB_QueryPurchases();

    /** Queries purchases from Google Play Billing
     * @param bSubscription - Whether to query subscription purchases
     * @param WorldContextObject - The world context object
     * @return The async node instance
     */
    UFUNCTION(BlueprintCallable, Category = "Google Play Billing", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
    static UGPB_QueryPurchases* QueryPurchases(bool bSubscription, UObject* WorldContextObject);

    virtual void Activate() override;
    virtual void BeginDestroy() override;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnComplete, bool, bSuccess, const TArray<FGPB_Purchase>&, Purchases, const FString&, Error);
    UPROPERTY(BlueprintAssignable, Category = "Google Play Billing")
    FOnComplete OnComplete;

    void OnTimeout();

    bool bSubscription;
    TWeakObjectPtr<UWorld> World;

    static TWeakObjectPtr<UGPB_QueryPurchases> StaticInstance;
    static FTimerHandle TimeoutTimerHandle;
}; 