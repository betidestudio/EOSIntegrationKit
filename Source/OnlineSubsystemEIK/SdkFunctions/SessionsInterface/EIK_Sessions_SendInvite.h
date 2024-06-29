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
#include "EIK_Sessions_SendInvite.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_OnSendInviteCallback, const TEnumAsByte<EEIK_Result>&, ResultCode);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sessions_SendInvite : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	//Send an invite to another player. User must have created the session or be registered in the session or else the call will fail
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_SendInvite")
	static UEIK_Sessions_SendInvite* EIK_Sessions_SendInvite(FString SessionName, FEIK_ProductUserId LocalUserId, FEIK_ProductUserId TargetUserId);

	 UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_OnSendInviteCallback OnCallback;

private:
	FString Var_SessionName;
	FEIK_ProductUserId Var_LocalUserId;
	FEIK_ProductUserId Var_TargetUserId;
	virtual void Activate() override;
	static void EOS_CALL OnSendInviteCallback(const EOS_Sessions_SendInviteCallbackInfo* Data);
};
