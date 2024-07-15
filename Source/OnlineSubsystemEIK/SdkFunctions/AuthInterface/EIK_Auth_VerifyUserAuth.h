// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
THIRD_PARTY_INCLUDES_START
#include <eos_auth.h>
THIRD_PARTY_INCLUDES_END
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Auth_VerifyUserAuth.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEIK_Auth_OnVerifyUserAuthCallback, TEnumAsByte<EEIK_Result>, ResultCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Auth_VerifyUserAuth : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Contact the backend service to verify validity of an existing user auth token. This function is intended for server-side use only.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName = "EOS_Auth_VerifyUserAuth")
	static UEIK_Auth_VerifyUserAuth* VerifyUserAuth(FEIK_Auth_Token AuthToken);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FEIK_Auth_OnVerifyUserAuthCallback OnCallback;

private:
	FEIK_Auth_Token Var_AuthToken;
	static void EOS_CALL OnVerifyUserAuthCallback(const EOS_Auth_VerifyUserAuthCallbackInfo* Data);
	virtual void Activate() override;
};
