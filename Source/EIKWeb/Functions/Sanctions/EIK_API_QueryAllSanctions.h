// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_QueryAllSanctions.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_QueryAllSanctions : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_QueryAllSanctions* QueryAllSanctions(FString Authorization, FString DeploymentId, int32 Limit = 100, int32 Offset = 0);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
	int32 Var_Limit;
	int32 Var_Offset;
};
