// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_QueryAntiCheatServiceStatus.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_QueryAntiCheatServiceStatus : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|AntiCheat")
	static UEIK_API_QueryAntiCheatServiceStatus* QueryAntiCheatServiceStatus(FString Authorization, FString DeploymentId);

private:
	
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_DeploymentId;
};
