// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_QuerySanctionAppeals.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_QuerySanctionAppeals : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_QuerySanctionAppeals* QuerySanctionAppeals(FString Authorization, FString DeploymentId, FString ReferenceId, FString Status, FString ProductUserId, int32 Limit = 100, int32 Offset = 0);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
	FString Var_ReferenceId;
	FString Var_Status;
	FString Var_ProductUserId;
	int32 Var_Limit;
	int32 Var_Offset;
};
