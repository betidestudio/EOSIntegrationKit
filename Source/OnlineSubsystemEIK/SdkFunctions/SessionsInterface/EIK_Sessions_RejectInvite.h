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
#include "EIK_Sessions_RejectInvite.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_OnRejectInviteCallback, const TEnumAsByte<EEIK_Result>&, ResultCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sessions_RejectInvite : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Reject an invite from another player.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_RejectInvite")
	static UEIK_Sessions_RejectInvite* EIK_Sessions_RejectInvite(FString InviteId, FEIK_ProductUserId LocalUserId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_OnRejectInviteCallback OnCallback;

private:
	FString Var_InviteId;
	FEIK_ProductUserId Var_LocalUserId;
	virtual void Activate() override;
	static void EOS_CALL OnRejectInviteCallback(const EOS_Sessions_RejectInviteCallbackInfo* Data);
};
