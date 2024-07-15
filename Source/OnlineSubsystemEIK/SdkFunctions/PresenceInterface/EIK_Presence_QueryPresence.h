// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "eos_presence.h"
#include "OnlineSubsystemEOS.h"
#include "EIK_Presence_QueryPresence.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_Presence_QueryPresenceComplete, const FEIK_EpicAccountId&, LocalUserId, const FEIK_EpicAccountId&, TargetUserId, const TEnumAsByte<EEIK_Result>&, Result);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Presence_QueryPresence : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Query a user's presence. This must complete successfully before CopyPresence will have valid results. If HasPresence returns true for a remote user, this does not need to be called.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_QueryPresence")
	static UEIK_Presence_QueryPresence* EIK_Presence_QueryPresence(FEIK_EpicAccountId LocalUserId, FEIK_EpicAccountId TargetUserId);

	UPROPERTY(BlueprintAssignable)
	FEIK_Presence_QueryPresenceComplete OnCallback;

private:
	virtual void Activate() override;
	static void EOS_CALL Internal_OnQueryPresenceComplete(const EOS_Presence_QueryPresenceCallbackInfo* Data);
	FEIK_EpicAccountId Var_LocalUserId;
	FEIK_EpicAccountId Var_TargetUserId;
};
