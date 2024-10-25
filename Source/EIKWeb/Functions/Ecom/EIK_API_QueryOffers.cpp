// Copyright (c) 2024 Betide Studio. All Rights Reserved.


#include "EIK_API_QueryOffers.h"

UEIK_API_QueryOffers* UEIK_API_QueryOffers::QueryOffers(FString Authorization, FString IdentityId, FString SandboxId)
{
	UEIK_API_QueryOffers* Node = NewObject<UEIK_API_QueryOffers>();
	Node->Var_Authorization = Authorization;
	Node->Var_IdentityId = IdentityId;
	Node->Var_SandboxId = SandboxId;
	return Node;
}

void UEIK_API_QueryOffers::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/epic/ecom/v3/identities/%s/namespaces/%s/offers"), *APIEndpoint, *Var_IdentityId, *Var_SandboxId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetURL(URL);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	if (Var_Authorization.Contains("Bearer"))
	{
		HttpRequest->SetHeader(TEXT("Authorization"), Var_Authorization);
	}
	else
	{
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Var_Authorization));
	}
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_QueryOffers::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
