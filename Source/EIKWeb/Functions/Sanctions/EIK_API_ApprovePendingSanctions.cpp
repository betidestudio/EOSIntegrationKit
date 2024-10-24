// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_ApprovePendingSanctions.h"

UEIK_API_ApprovePendingSanctions* UEIK_API_ApprovePendingSanctions::ApprovePendingSanctions(FString Authorization,
	FString DeploymentId, FString EOS_OrganizationId, FString EOS_ClientId, FString EOS_DeploymentId,
	TArray<FString> ReferenceIds)
{
	UEIK_API_ApprovePendingSanctions* Node = NewObject<UEIK_API_ApprovePendingSanctions>();
	Node->Var_Authorization = Authorization;
	Node->Var_DeploymentId = DeploymentId;
	Node->Var_EOS_OrganizationId = EOS_OrganizationId;
	Node->Var_EOS_ClientId = EOS_ClientId;
	Node->Var_EOS_DeploymentId = EOS_DeploymentId;
	Node->Var_ReferenceIds = ReferenceIds;
	return Node;
}

void UEIK_API_ApprovePendingSanctions::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/sanctions/v1/%s/sanctions/pending/approve"), *APIEndpoint, *Var_DeploymentId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("PATCH"));
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
	HttpRequest->SetHeader(TEXT("Eos-Organization-Id"), Var_EOS_OrganizationId);
	HttpRequest->SetHeader(TEXT("Eos-Client-Id"), Var_EOS_ClientId);
	HttpRequest->SetHeader(TEXT("Eos-Deployment-Id"), Var_EOS_DeploymentId);
	TArray<TSharedPtr<FJsonValue>> ReferenceIdsJson;
	TSharedPtr<FJsonObject> RequestBodyJson = MakeShareable(new FJsonObject);
	if (Var_ReferenceIds.Num() > 0)
	{
		for (auto ReferenceId : Var_ReferenceIds)
		{
			TSharedPtr<FJsonValue> ReferenceIdJson = MakeShareable(new FJsonValueString(ReferenceId));
			ReferenceIdsJson.Add(ReferenceIdJson);
		}
	}
	RequestBodyJson->SetArrayField(TEXT("referenceIds"), ReferenceIdsJson);
	FString RequestBodyString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBodyString);
	FJsonSerializer::Serialize(RequestBodyJson.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(RequestBodyString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_ApprovePendingSanctions::OnResponseReceived);
	HttpRequest->ProcessRequest();
}