// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_BulkQueryActiveSanction.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_BulkQueryActiveSanction : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web")
	static UEIK_API_BulkQueryActiveSanction* BulkQueryActiveSanction(FString Authorization, FString DeploymentId, TArray<FString> ProductUserId, TArray<FString> Action);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
	TArray<FString> Var_ProductUserId;
	TArray<FString> Var_Action;


};
