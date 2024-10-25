// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_TokenBasedOwnershipVerification.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_TokenBasedOwnershipVerification : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|AntiCheat")
	static UEIK_API_TokenBasedOwnershipVerification* TokenBasedOwnershipVerification(FString AuthorizationToken, FString Platform, FString IdentityId, TMap<FString, FString> CatalogItemId);

private:
	
	virtual void Activate() override;
	FString Var_Platform;
	FString Var_IdentityId;
	TMap<FString, FString> Var_CatalogItemId;
	FString Var_Authorization;
};
