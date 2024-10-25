// Copyright (c) 2024 Betide Studio. All Rights Reserved.


#include "EIK_API_QueryClawbacks.h"

UEIK_API_QueryClawbacks* UEIK_API_QueryClawbacks::QueryClawbacks(FString Authorization, FString NameSpace,
	FString ClawbackDate, int32 Count)
{
	UEIK_API_QueryClawbacks* Node = NewObject<UEIK_API_QueryClawbacks>();
	Node->Var_Authorization = Authorization;
	Node->Var_NameSpace = NameSpace;
	Node->Var_ClawbackDate = ClawbackDate;
	Node->Var_Count = Count;
	return Node;
}

void UEIK_API_QueryClawbacks::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/epic/ecom/v3/namespace/%s/entitlements/clawbacks?clawbackDate=%s&count=%d"), *APIEndpoint, *Var_NameSpace, *Var_ClawbackDate, Var_Count);
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
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_QueryClawbacks::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
