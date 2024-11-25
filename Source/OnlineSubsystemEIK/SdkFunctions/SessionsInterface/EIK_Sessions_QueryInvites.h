// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Async/TaskGraphInterfaces.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
THIRD_PARTY_INCLUDES_START
#include "eos_sessions.h"
#include "eos_sessions_types.h"
THIRD_PARTY_INCLUDES_END
#include "EIK_Sessions_QueryInvites.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_OnQueryInvitesCallback, const TEnumAsByte<EEIK_Result>&, ResultCode, const FEIK_ProductUserId&, LocalUserId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Sessions_QueryInvites : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//Retrieve all existing invites for a single user
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface", DisplayName="EOS_Sessions_QueryInvites")
	static UEIK_Sessions_QueryInvites* EIK_Sessions_QueryInvites(FEIK_ProductUserId LocalUserId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit | SDK Functions | Sessions Interface")
	FEIK_OnQueryInvitesCallback OnCallback;

private:
	FEIK_ProductUserId Var_LocalUserId;
	virtual void Activate() override;
	static void EOS_CALL OnQueryInvitesCallback(const EOS_Sessions_QueryInvitesCallbackInfo* Data);
	
};
