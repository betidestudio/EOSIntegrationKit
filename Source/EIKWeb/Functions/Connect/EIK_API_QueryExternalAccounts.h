// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_QueryExternalAccounts.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_QueryExternalAccounts : public UEIK_BaseWebApi
{
	GENERATED_BODY()

	//The request queryExternalAccountsForAnyUser returns associated Product User IDs from a list of external account IDs.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|Connect")
	static UEIK_API_QueryExternalAccounts* QueryExternalAccounts(FString Authorization, TArray<FString> AccountId, FString IdentityProviderId, FString Environment);

private:
	
	virtual void Activate() override;
	FString Var_Authorization;
	TArray<FString> Var_AccountId;
	FString Var_IdentityProviderId;
	FString Var_Environment;
};
