// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
THIRD_PARTY_INCLUDES_START
#include "eos_friends.h"
#include "eos_friends_types.h"
THIRD_PARTY_INCLUDES_END
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Friends_SendInvite.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_Friends_SendInviteCallback, const TEnumAsByte<EEIK_Result>&, Result, const FEIK_EpicAccountId&, LocalUserId, const FEIK_EpicAccountId&, TargetUserId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Friends_SendInvite : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Starts an asynchronous task that sends a friend invitation to another user. The completion delegate is executed after the backend response has been received. It does not indicate that the target user has responded to the friend invitation.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Friends Interface", DisplayName="EOS_Friends_SendInvite")
	static UEIK_Friends_SendInvite* EIK_Friends_SendInvite(FEIK_EpicAccountId LocalUserId, FEIK_EpicAccountId TargetUserId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | Friends Interface")
	FEIK_Friends_SendInviteCallback OnCallback;
private:
	static void EOS_CALL OnSendInviteCallback(const EOS_Friends_SendInviteCallbackInfo* Data);
	virtual void Activate() override;
	FEIK_EpicAccountId Var_LocalUserId;
	FEIK_EpicAccountId Var_TargetUserId;
};
