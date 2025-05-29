// Copyright (C) 2025 Betide Studio. All Rights Reserved.
// Written by AvnishGameDev.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GooglePlayBilling/GooglePlayBillingStructures.h"
#include "GooglePlayBilling/GooglePlayBillingMethods.h"
#include "GPB_QueryProduct.generated.h"

UCLASS()
class EIKLOGINMETHODS_API UGPB_QueryProduct : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UGPB_QueryProduct();

    /** Queries product details from Google Play Billing
     * @param ProductID - The product ID to query
     * @param bSubscription - Whether this is a subscription product
     * @param WorldContextObject - The world context object
     * @return The async node instance
     */
    UFUNCTION(BlueprintCallable, Category = "Google Play Billing", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
    static UGPB_QueryProduct* QueryProduct(const FString& ProductID, bool bSubscription, UObject* WorldContextObject);

    virtual void Activate() override;
    virtual void BeginDestroy() override;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnComplete, bool, bSuccess, const TArray<FGPB_ProductDetails>&, ProductDetails, const FString&, Error);
    UPROPERTY(BlueprintAssignable, Category = "Google Play Billing")
    FOnComplete OnComplete;

    TWeakObjectPtr<UWorld> World;
    static TWeakObjectPtr<UGPB_QueryProduct> StaticInstance;
    static FTimerHandle TimeoutTimerHandle;

    void OnTimeout();
    FString ProductID;
    bool bSubscription;
}; 