// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "eos_presence.h"
#include "OnlineSubsystemEOS.h"
#include "EIK_Presence_SetPresence.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEIK_Presence_SetPresenceComplete, const FEIK_EpicAccountId&, LocalUserId, const TEnumAsByte<EEIK_Result>&, Result);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Presence_SetPresence : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Sets your new presence with the data applied to a PresenceModificationHandle. The PresenceModificationHandle can be released safely after calling this function.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Presence Interface", DisplayName="EOS_Presence_SetPresence")
	static UEIK_Presence_SetPresence* EIK_Presence_SetPresence(FEIK_EpicAccountId LocalUserId, FEIK_HPresenceModification PresenceModificationHandle);

	UPROPERTY(BlueprintAssignable)
	FEIK_Presence_SetPresenceComplete OnCallback;

private:
	virtual void Activate() override;
	static void EOS_CALL Internal_OnSetPresenceComplete(const EOS_Presence_SetPresenceCallbackInfo* Data);
	FEIK_EpicAccountId Var_LocalUserId;
	FEIK_HPresenceModification Var_PresenceModificationHandle;
};
