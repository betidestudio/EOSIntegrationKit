// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_RequestAccessToken.h"

UEIK_API_RequestAccessToken* UEIK_API_RequestAccessToken::RequestAccessToken(FString Authorization, FString ClientId,
	FString ClientSecret, FString GrantType, FString Nonce, FString DeploymentId, FString ExternalAuthToken,
	FString ExternalAuthType)
{
	UEIK_API_RequestAccessToken* Node = NewObject<UEIK_API_RequestAccessToken>();
	Node->Var_Authorization = Authorization;
	Node->Var_ClientId = ClientId;
	Node->Var_ClientSecret = ClientSecret;
	Node->Var_GrantType = GrantType;
	Node->Var_Nonce = Nonce;
	Node->Var_DeploymentId = DeploymentId;
	Node->Var_ExternalAuthToken = ExternalAuthToken;
	Node->Var_ExternalAuthType = ExternalAuthType;
	return Node;
}

void UEIK_API_RequestAccessToken::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/auth/v1/oauth/token"), *APIEndpoint);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetURL(URL);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	FString Credentials = FString::Printf(TEXT("%s:%s"), *Var_ClientId, *Var_ClientSecret);
	FString Base64Credentials = FBase64::Encode(Credentials);

	HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Basic %s"), *Base64Credentials));
	FString RequestBody = FString::Printf(TEXT("grant_type=%s&deployment_id=%s"), *Var_GrantType, *Var_DeploymentId);

	// Add nonce if it's provided and needed
	if (!Var_Nonce.IsEmpty())
	{
		RequestBody += FString::Printf(TEXT("&nonce=%s"), *Var_Nonce);
	}
	if (!Var_ExternalAuthToken.IsEmpty() && !Var_ExternalAuthType.IsEmpty())
	{
		RequestBody += FString::Printf(TEXT("&external_auth_token=%s&external_auth_type=%s"), *Var_ExternalAuthToken, *Var_ExternalAuthType);
	}
	HttpRequest->SetContentAsString(RequestBody);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_RequestAccessToken::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
