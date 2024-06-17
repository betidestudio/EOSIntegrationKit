// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Ecom_RedeemEntitlements.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_Ecom_RedeemEntitlementsCallback, const FEIK_EpicAccountId&, LocalUserId, const TEnumAsByte<EEIK_Result>, ResultCode, int32, RedeemedEntitlementIdsCount);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Ecom_RedeemEntitlements : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Requests that the provided entitlement be marked redeemed. This will cause that entitlement to no longer be returned from QueryEntitlements unless the include redeemed request flag is set true.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface", DisplayName="EOS_Ecom_RedeemEntitlements")
	static UEIK_Ecom_RedeemEntitlements* EIK_Ecom_RedeemEntitlements(const FEIK_EpicAccountId& LocalUserId, const TArray<FEIK_Ecom_EntitlementId>& EntitlementIds);
	
	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Ecom Interface")
	FEIK_Ecom_RedeemEntitlementsCallback OnCallback;
private:
	static void EOS_CALL OnRedeemEntitlementsCallback(const EOS_Ecom_RedeemEntitlementsCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_EpicAccountId Var_LocalUserId;
	TArray<FEIK_Ecom_EntitlementId> Var_EntitlementIds;
};
