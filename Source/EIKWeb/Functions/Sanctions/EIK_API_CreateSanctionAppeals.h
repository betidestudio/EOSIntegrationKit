// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_CreateSanctionAppeals.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_CreateSanctionAppeals : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_CreateSanctionAppeals* CreateSanctionAppeals(FString Authorization, FString DeploymentId, FString ReferenceId, int32 Reason);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
	FString Var_ReferenceId;
	int32 Var_Reason;
};
