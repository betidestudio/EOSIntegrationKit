// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_QueryAllSanctionsForPlayer.h"

UEIK_API_QueryAllSanctionsForPlayer* UEIK_API_QueryAllSanctionsForPlayer::QueryAllSanctionsForPlayer(
	FString Authorization, FString DeploymentId, FString ProductUserId, int32 Limit, int32 Offset)
{
	UEIK_API_QueryAllSanctionsForPlayer* Proxy = NewObject<UEIK_API_QueryAllSanctionsForPlayer>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_ProductUserId = ProductUserId;
	Proxy->Var_Limit = Limit;
	Proxy->Var_Offset = Offset;
	return Proxy;
}

void UEIK_API_QueryAllSanctionsForPlayer::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/sanctions/v1/%s/users/%s?limit=%d&offset=%d"), *APIEndpoint, *Var_DeploymentId, *Var_ProductUserId, Var_Limit, Var_Offset);
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
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_QueryAllSanctionsForPlayer::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
