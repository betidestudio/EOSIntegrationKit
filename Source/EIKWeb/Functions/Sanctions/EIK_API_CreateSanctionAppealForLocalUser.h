// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_CreateSanctionAppealForLocalUser.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_CreateSanctionAppealForLocalUser : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_CreateSanctionAppealForLocalUser* CreateSanctionAppealForLocalUser(FString Authorization, FString DeploymentId, FString ProductUserId, FString ReferenceId, FString Reason);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
	FString Var_ProductUserId;
	FString Var_ReferenceId;
	FString Var_Reason;
};
