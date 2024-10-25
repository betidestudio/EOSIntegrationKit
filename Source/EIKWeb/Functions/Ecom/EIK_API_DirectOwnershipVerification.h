// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_DirectOwnershipVerification.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_DirectOwnershipVerification : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:
	//The endpoint to check if a user owns an item, or a list of items.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|AntiCheat")
	static UEIK_API_DirectOwnershipVerification* DirectOwnershipVerification(FString Authorization, FString CurrentAccountId, TArray<FString> NsCatalogItemIds, FString SandboxId);

private:
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_CurrentAccountId;
	TArray<FString> Var_NsCatalogItemIds;
	FString Var_SandboxId;
};
