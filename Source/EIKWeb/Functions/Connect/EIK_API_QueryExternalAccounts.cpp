// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_QueryExternalAccounts.h"

UEIK_API_QueryExternalAccounts* UEIK_API_QueryExternalAccounts::QueryExternalAccounts(FString Authorization,
	TArray<FString> AccountId, FString IdentityProviderId, FString Environment)
{
	UEIK_API_QueryExternalAccounts* Proxy = NewObject<UEIK_API_QueryExternalAccounts>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_AccountId = AccountId;
	Proxy->Var_IdentityProviderId = IdentityProviderId;
	Proxy->Var_Environment = Environment;
	return Proxy;
}

void UEIK_API_QueryExternalAccounts::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/user/v1/accounts"), *APIEndpoint);
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
	FString QueryString = TEXT("?");
	if (Var_AccountId.Num() > 0)
	{
		for (auto AccountId : Var_AccountId)
		{
			QueryString += FString::Printf(TEXT("accountId=%s&"), *AccountId);
		}
	}
	if (!Var_IdentityProviderId.IsEmpty())
	{
		QueryString += FString::Printf(TEXT("identityProviderId=%s&"), *Var_IdentityProviderId);
	}
	if (!Var_Environment.IsEmpty())
	{
		QueryString += FString::Printf(TEXT("environment=%s"), *Var_Environment);
	}
	URL += QueryString;
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_QueryExternalAccounts::OnResponseReceived);
	HttpRequest->ProcessRequest();
}

