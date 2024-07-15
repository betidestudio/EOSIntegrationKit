// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
THIRD_PARTY_INCLUDES_START
#include <eos_auth.h>
THIRD_PARTY_INCLUDES_END
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "EIK_Auth_Logout.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLogoutCallback, TEnumAsByte<EEIK_Result>, ResultCode, FEIK_EpicAccountId, EpicAccountId);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Auth_Logout : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Signs the player out of the online service.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName = "EOS_Auth_Logout")
	static UEIK_Auth_Logout* Logout(FEIK_EpicAccountId EpicAccountId);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FOnLogoutCallback OnCallback;
private:
	FEIK_EpicAccountId Var_EpicAccountId;
	virtual void Activate() override;
	static void EOS_CALL OnLogoutCallback(const EOS_Auth_LogoutCallbackInfo* Data);
};
