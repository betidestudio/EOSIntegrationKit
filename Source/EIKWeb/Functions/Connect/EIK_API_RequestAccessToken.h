// Copyright (c) 2023 Betide Studio. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EIK_BaseWebApi.h"
#include "EIK_API_RequestAccessToken.generated.h"

/**
 * 
 */
UCLASS()
class EIKWEB_API UEIK_API_RequestAccessToken : public UEIK_BaseWebApi
{
	GENERATED_BODY()

public:

	//The EOS Connect backend provides an OAuth 2.0 token endpoint to request access tokens for the EOS Game Services Web APIs.
	UFUNCTION(BlueprintCallable, Category = "EOS Integration Kit|Web API|Connect")
	static UEIK_API_RequestAccessToken* RequestAccessToken(FString Authorization, FString ClientId, FString ClientSecret, FString GrantType = "client_credentials", FString Nonce = "", FString DeploymentId = "", FString ExternalAuthToken = "", FString ExternalAuthType = "");

private:
	
	virtual void Activate() override;
	FString Var_Authorization;
	FString Var_ClientId;
	FString Var_ClientSecret;
	FString Var_GrantType;
	FString Var_Nonce;
	FString Var_DeploymentId;
	FString Var_ExternalAuthToken;
	FString Var_ExternalAuthType;
};
