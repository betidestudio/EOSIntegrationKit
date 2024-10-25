// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_DirectEntitlementEnumeration.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_DirectEntitlementEnumeration : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|Ecom")
	static UEIK_API_DirectEntitlementEnumeration* DirectEntitlementEnumeration(FString Authorization, FString IdentityId, FString SandboxId, FString EntitlementName, bool bIncludeRedeemed);

private:
	
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_IdentityId;
	FString Var_SandboxId;
	FString Var_EntitlementName;
	bool Var_bIncludeRedeemed;
};
