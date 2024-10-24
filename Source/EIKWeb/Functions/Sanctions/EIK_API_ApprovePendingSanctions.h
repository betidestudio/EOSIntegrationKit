// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_ApprovePendingSanctions.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_ApprovePendingSanctions : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_ApprovePendingSanctions* ApprovePendingSanctions(FString Authorization, FString DeploymentId, FString EOS_OrganizationId, FString EOS_ClientId, FString EOS_DeploymentId, TArray<FString> ReferenceIds);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
	FString Var_EOS_OrganizationId;
	FString Var_EOS_ClientId;
	FString Var_EOS_DeploymentId;
	TArray<FString> Var_ReferenceIds;
};
