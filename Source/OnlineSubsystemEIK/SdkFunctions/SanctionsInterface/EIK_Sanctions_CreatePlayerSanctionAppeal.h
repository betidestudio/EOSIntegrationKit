// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "eos_sanctions.h"
#include "eos_sanctions_types.h"
#include "OnlineSubsystemEOS.h"
#include "EIK_Sanctions_CreatePlayerSanctionAppeal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Sanctions_CreatePlayerSanctionAppealComplete, const FEIK_ProductUserId&, LocalUserId, const TEnumAsByte<EEIK_Result>&, Result);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sanctions_CreatePlayerSanctionAppeal : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Create a sanction appeal on behalf of a local user. Note that for creating the sanction appeal you'll need the sanction reference id, which is available through CopyPlayerSanctionByIndex.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sanctions Interface", DisplayName="EOS_Sanctions_CreatePlayerSanctionAppeal")
	static UEIK_Sanctions_CreatePlayerSanctionAppeal* EIK_Sanctions_CreatePlayerSanctionAppeal(FEIK_ProductUserId LocalUserId, const TEnumAsByte<EEIK_ESanctionAppealReason>& AppealReason, const FString& ReferenceId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sanctions Interface")
	FEIK_Sanctions_CreatePlayerSanctionAppealComplete OnCallback;
	
private:
	void Activate() override;
	static void EOS_CALL Internal_OnCreatePlayerSanctionAppealComplete(const EOS_Sanctions_CreatePlayerSanctionAppealCallbackInfo* Data);
	FEIK_ProductUserId Var_LocalUserId;
	TEnumAsByte<EEIK_ESanctionAppealReason> Var_AppealReason;
	FString Var_ReferenceId;
	
};
