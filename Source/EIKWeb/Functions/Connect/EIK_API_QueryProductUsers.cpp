// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_QueryProductUsers.h"

UEIK_API_QueryProductUsers* UEIK_API_QueryProductUsers::QueryProductUsers(FString Authorization,
	TArray<FString> ProductUserIds)
{
	UEIK_API_QueryProductUsers* Node = NewObject<UEIK_API_QueryProductUsers>();
	Node->Var_Authorization = Authorization;
	Node->Var_ProductUserIds = ProductUserIds;
	return Node;
}

void UEIK_API_QueryProductUsers::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/user/v1/product-users"), *APIEndpoint);
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
	if (Var_ProductUserIds.Num() > 0)
	{
		for (auto ProductUserId : Var_ProductUserIds)
		{
			QueryString += FString::Printf(TEXT("productUserId=%s&"), *ProductUserId);
		}
	}
	URL += QueryString;
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_QueryProductUsers::OnResponseReceived);
	HttpRequest->ProcessRequest();
}	
