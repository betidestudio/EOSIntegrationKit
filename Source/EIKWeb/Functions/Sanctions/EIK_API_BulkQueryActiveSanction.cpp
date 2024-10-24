// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_BulkQueryActiveSanction.h"

UEIK_API_BulkQueryActiveSanction* UEIK_API_BulkQueryActiveSanction::BulkQueryActiveSanction(FString Authorization,
	FString DeploymentId, TArray<FString> ProductUserId, TArray<FString> Action)
{
	UEIK_API_BulkQueryActiveSanction* Proxy = NewObject<UEIK_API_BulkQueryActiveSanction>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_ProductUserId = ProductUserId;
	Proxy->Var_Action = Action;
	return Proxy;
}

void UEIK_API_BulkQueryActiveSanction::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/sanctions/v1/%s/active-sanctions"), *APIEndpoint, *Var_DeploymentId);
	if (Var_ProductUserId.Num() > 0)
	{
		FString ProductUserIdString = TEXT("productUserId=");
		for (int i = 0; i < Var_ProductUserId.Num(); i++)
		{
			ProductUserIdString += Var_ProductUserId[i];
			if (i < Var_ProductUserId.Num() - 1)
			{
				ProductUserIdString += TEXT("&productUserId=");
			}
		}
		URL += FString::Printf(TEXT("?%s"), *ProductUserIdString);
	}
	if (Var_Action.Num() > 0)
	{
		FString ActionString = TEXT("action=");
		for (int i = 0; i < Var_Action.Num(); i++)
		{
			ActionString += Var_Action[i];
			if (i < Var_Action.Num() - 1)
			{
				ActionString += TEXT("&action=");
			}
		}
		URL += FString::Printf(TEXT("&%s"), *ActionString);
	}
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetURL(URL);
	if (Var_Authorization.Contains("Bearer"))
	{
		HttpRequest->SetHeader(TEXT("Authorization"), Var_Authorization);
	}
	else
	{
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Var_Authorization));
	}
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_BulkQueryActiveSanction::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
