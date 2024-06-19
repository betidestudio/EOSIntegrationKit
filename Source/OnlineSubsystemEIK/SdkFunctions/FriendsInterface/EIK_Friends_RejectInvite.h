// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
THIRD_PARTY_INCLUDES_START
#include "eos_friends.h"
#include "eos_friends_types.h"
THIRD_PARTY_INCLUDES_END
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Friends_RejectInvite.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_Friends_RejectInviteCallback, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_EpicAccountId&, LocalUserId, const FEIK_EpicAccountId&, TargetUserId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Friends_RejectInvite : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Starts an asynchronous task that rejects a friend invitation from another user. The completion delegate is executed after the backend response has been received.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_RejectInvite")
	static UEIK_Friends_RejectInvite* EIK_Friends_RejectInvite(FEIK_EpicAccountId LocalUserId, FEIK_EpicAccountId TargetUserId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Friends Interface")
	FEIK_Friends_RejectInviteCallback OnCallback;

private:
	static void EOS_CALL OnRejectInviteCallback(const EOS_Friends_RejectInviteCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_EpicAccountId Var_LocalUserId;
	FEIK_EpicAccountId Var_TargetUserId;
};
