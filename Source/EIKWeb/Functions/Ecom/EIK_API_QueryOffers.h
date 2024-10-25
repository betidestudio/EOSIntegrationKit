// Copyright (c) 2024 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_QueryOffers.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_QueryOffers : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|Ecom")
	static UEIK_API_QueryOffers* QueryOffers(FString Authorization, FString IdentityId, FString SandboxId);

private:
	
	virtual void Activate() override;
	FString Var_IdentityId;
	FString Var_SandboxId;
	FString Var_Authorization;
};
