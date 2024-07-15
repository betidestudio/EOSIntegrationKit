// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include <eos_base.h>

#include "CoreMinimal.h"
#include "eos_auth_types.h"
#include "OnlineSubsystemEIK/SdkFunctions/EIK_SharedFunctionFile.h"
#include "eos_auth.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_Auth_DeletePersistentAuth.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeletePersistentAuthCallback, TEnumAsByte<EEIK_Result>, ResultCode);

UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Auth_DeletePersistentAuth : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//Deletes a previously received and locally stored persistent auth access token for the currently logged in user of the local device. On Desktop and Mobile platforms, the access token is deleted from the keychain of the local user and a backend request is made to revoke the token on the authentication server. On Console platforms, even though the caller is responsible for storing and deleting the access token on the local device, this function should still be called with the access token before its deletion to make the best effort in attempting to also revoke it on the authentication server. If the function would fail on Console, the caller should still proceed as normal to delete the access token locally as intended.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName = "EOS_Auth_DeletePersistentAuth")
	static UEIK_Auth_DeletePersistentAuth* DeletePersistentAuth(FString RefreshToken);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FOnDeletePersistentAuthCallback OnCallback;
private:
	FString Var_RefreshToken;
	virtual void Activate() override;
	static void EOS_CALL OnDeletePersistentAuthCallback(const EOS_Auth_DeletePersistentAuthCallbackInfo* Data);
};
