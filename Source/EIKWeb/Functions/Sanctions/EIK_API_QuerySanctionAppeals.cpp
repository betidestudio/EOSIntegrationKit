// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_QuerySanctionAppeals.h"

UEIK_API_QuerySanctionAppeals* UEIK_API_QuerySanctionAppeals::QuerySanctionAppeals(FString Authorization,
	FString DeploymentId, FString ReferenceId, FString Status, FString ProductUserId, int32 Limit, int32 Offset)
{
	UEIK_API_QuerySanctionAppeals* Node = NewObject<UEIK_API_QuerySanctionAppeals>();
	Node->Var_Authorization = Authorization;
	Node->Var_DeploymentId = DeploymentId;
	Node->Var_ReferenceId = ReferenceId;
	Node->Var_Status = Status;
	Node->Var_ProductUserId = ProductUserId;
	Node->Var_Limit = Limit;
	Node->Var_Offset = Offset;
	return Node;
}

void UEIK_API_QuerySanctionAppeals::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/sanctions/v1/%s/appeals"), *APIEndpoint, *Var_DeploymentId);
	if (!Var_Status.IsEmpty())
	{
		URL.Append(FString::Printf(TEXT("?status=%s"), *Var_Status));
	}
	if (!Var_ProductUserId.IsEmpty())
	{
		URL.Append(FString::Printf(TEXT("&productUserId=%s"), *Var_ProductUserId));
	}
	if (!Var_ReferenceId.IsEmpty())
	{
		URL.Append(FString::Printf(TEXT("&referenceId=%s"), *Var_ReferenceId));
	}
	if (Var_Limit > 0)
	{
		URL.Append(FString::Printf(TEXT("&limit=%d"), Var_Limit));
	}
	if (Var_Offset > 0)
	{
		URL.Append(FString::Printf(TEXT("&offset=%d"), Var_Offset));
	}
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
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_QuerySanctionAppeals::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
