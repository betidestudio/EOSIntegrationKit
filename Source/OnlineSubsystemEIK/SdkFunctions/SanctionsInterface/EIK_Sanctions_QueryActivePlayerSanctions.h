// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "eos_sanctions.h"
#include "eos_sanctions_types.h"
#include "OnlineSubsystemEOS.h"
#include "EIK_Sanctions_QueryActivePlayerSanctions.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_Sanctions_QueryActivePlayerSanctionsComplete, const FEIK_ProductUserId&, LocalUserId, const FEIK_ProductUserId&, TargetUserId, const TEnumAsByte<EEIK_Result>&, Result);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sanctions_QueryActivePlayerSanctions : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Start an asynchronous query to retrieve any active sanctions for a specified user. Call EOS_Sanctions_GetPlayerSanctionCount and EOS_Sanctions_CopyPlayerSanctionByIndex to retrieve the data.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sanctions Interface", DisplayName="EOS_Sanctions_QueryActivePlayerSanctions")
	static UEIK_Sanctions_QueryActivePlayerSanctions* EIK_Sanctions_QueryActivePlayerSanctions(FEIK_ProductUserId LocalUserId, FEIK_ProductUserId TargetUserId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sanctions Interface")
	FEIK_Sanctions_QueryActivePlayerSanctionsComplete OnCallback;
private:
	virtual void Activate() override;
	static void EOS_CALL Internal_OnQueryActivePlayerSanctionsComplete(const EOS_Sanctions_QueryActivePlayerSanctionsCallbackInfo* Data);
	FEIK_ProductUserId Var_LocalUserId;
	FEIK_ProductUserId Var_TargetUserId;
	
};
