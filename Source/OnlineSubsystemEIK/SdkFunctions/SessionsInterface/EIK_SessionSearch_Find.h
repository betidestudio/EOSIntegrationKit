// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_sessions.h"
#include "eos_sessions_types.h"
THIRD_PARTY_INCLUDES_END
#include "EIK_SessionSearch_Find.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_OnSessionSearch_FindCallback, const TEnumAsByte<EEIK_Result>&, ResultCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_SessionSearch_Find : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Find sessions matching the search criteria setup via this session search handle. When the operation completes, this handle will have the search results that can be parsed
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_SessionSearch_Find")
	static UEIK_SessionSearch_Find* EIK_SessionSearch_Find(FEIK_HSessionSearch SessionSearchHandle, FEIK_ProductUserId LocalUserId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_OnSessionSearch_FindCallback OnCallback;

private:
	FEIK_HSessionSearch Var_SessionSearchHandle;
	FEIK_ProductUserId Var_LocalUserId;
	virtual void Activate() override;
	static void EOS_CALL OnSessionSearch_FindCallback(const EOS_SessionSearch_FindCallbackInfo* Data);
};
