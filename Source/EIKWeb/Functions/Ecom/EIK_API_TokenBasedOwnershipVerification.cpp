// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_TokenBasedOwnershipVerification.h"

UEIK_API_TokenBasedOwnershipVerification* UEIK_API_TokenBasedOwnershipVerification::TokenBasedOwnershipVerification(FString AuthorizationToken, FString Platform, FString IdentityId, TMap<FString, FString> CatalogItemId)
{
	UEIK_API_TokenBasedOwnershipVerification* Node = NewObject<UEIK_API_TokenBasedOwnershipVerification>();
	Node->Var_Platform = Platform;
	Node->Var_IdentityId = IdentityId;
	Node->Var_CatalogItemId = CatalogItemId;
	Node->Var_Authorization = AuthorizationToken;
	return Node;
}

void UEIK_API_TokenBasedOwnershipVerification::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/epic/ecom/v3/platforms/%s/identities/%s/ownershipToken"), *APIEndpoint, *Var_Platform, *Var_IdentityId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetURL(URL);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	if(Var_Authorization.Contains("Bearer"))
	{
		HttpRequest->SetHeader(TEXT("Authorization"), Var_Authorization);
	}
	else
	{
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Var_Authorization));
	}
	FString PostParameters;
	for (auto& Elem : Var_CatalogItemId)
	{
		PostParameters.Append(FString::Printf(TEXT("%s=%s&"), *Elem.Key, *Elem.Value));
	}
	HttpRequest->SetContentAsString(PostParameters);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_TokenBasedOwnershipVerification::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
	
