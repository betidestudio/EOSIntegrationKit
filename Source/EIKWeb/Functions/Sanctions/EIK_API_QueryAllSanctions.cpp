// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_QueryAllSanctions.h"

UEIK_API_QueryAllSanctions* UEIK_API_QueryAllSanctions::QueryAllSanctions(FString Authorization, FString DeploymentId,
	int32 Limit, int32 Offset)
{
	UEIK_API_QueryAllSanctions* Proxy = NewObject<UEIK_API_QueryAllSanctions>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_Limit = Limit;
	Proxy->Var_Offset = Offset;
	return Proxy;
}

void UEIK_API_QueryAllSanctions::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/sanctions/v1/%s/sanctions?limit=%d&offset=%d"), *APIEndpoint, *Var_DeploymentId, Var_Limit, Var_Offset);
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
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_QueryAllSanctions::OnResponseReceived);
	HttpRequest->ProcessRequest();	
}
