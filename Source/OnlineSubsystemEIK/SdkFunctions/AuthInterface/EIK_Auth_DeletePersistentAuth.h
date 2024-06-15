// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include <eos_base.h>

#include "CoreMinimal.h"
#include "eos_auth_types.h"
#include "eos_auth.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "EIK_Auth_DeletePersistentAuth.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeletePersistentAuthCallback, TEnumAsByte<EEIK_Result>, ResultCode);
UCLASS()
class ONLINESUBSYSTEMEIK_API UEIK_Auth_DeletePersistentAuth : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit | SDK Functions | Auth Interface", DisplayName = "EOS_Auth_DeletePersistentAuth")
	static UEIK_Auth_DeletePersistentAuth* DeletePersistentAuth(FString RefreshToken);

	UPROPERTY(BlueprintAssignable, Category = "EOS Integration Kit")
	FOnDeletePersistentAuthCallback OnCallback;
private:
	FString Var_RefreshToken;
	virtual void Activate() override;
	static void EOS_CALL OnDeletePersistentAuthCallback(const EOS_Auth_DeletePersistentAuthCallbackInfo* Data);
};
