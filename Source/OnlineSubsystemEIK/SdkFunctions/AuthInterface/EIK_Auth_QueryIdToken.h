// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
THIRD_PARTY_INCLUDES_START
#include <eos_auth.h>
THIRD_PARTY_INCLUDES_END
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Auth_QueryIdToken.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FEIK_Auth_OnQueryIdTokenCallback, TEnumAsByte<EEIK_Result>, ResultCode, FEIK_EpicAccountId, LocalUserId, FEIK_EpicAccountId, TargetAccountId);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Auth_QueryIdToken : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Query the backend for an ID token that describes one of the merged account IDs of a local authenticated user. The ID token can be used to impersonate a merged account ID when communicating with online services. An ID token for the selected account ID of a locally authenticated user will always be readily available and does not need to be queried explicitly.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName = "EOS_Auth_QueryIdToken")
	static UEIK_Auth_QueryIdToken* QueryIdToken(FEIK_EpicAccountId LocalUserId, FEIK_EpicAccountId TargetAccountId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_Auth_OnQueryIdTokenCallback OnCallback;
private:
	FEIK_EpicAccountId Var_LocalUserId;
	FEIK_EpicAccountId Var_TargetAccountId;
	static void EOS_CALL OnQueryIdTokenCallback(const EOS_Auth_QueryIdTokenCallbackInfo* Data);
	virtual void Activate() override;
	
};
