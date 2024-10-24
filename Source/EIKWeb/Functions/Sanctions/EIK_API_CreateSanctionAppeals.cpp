// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_CreateSanctionAppeals.h"

UEIK_API_CreateSanctionAppeals* UEIK_API_CreateSanctionAppeals::CreateSanctionAppeals(FString Authorization,
	FString DeploymentId, FString ReferenceId, int32 Reason)
{
	UEIK_API_CreateSanctionAppeals* Proxy = NewObject<UEIK_API_CreateSanctionAppeals>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_ReferenceId = ReferenceId;
	Proxy->Var_Reason = Reason;
	return Proxy;
}

void UEIK_API_CreateSanctionAppeals::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/sanctions/v1/%s/appeals"), *APIEndpoint, *Var_DeploymentId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("POST"));
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
	TSharedPtr<FJsonObject> RequestBodyJson = MakeShareable(new FJsonObject);
	if (!Var_ReferenceId.IsEmpty())
	{
		RequestBodyJson->SetStringField(TEXT("referenceId"), Var_ReferenceId);
	}
	if (Var_Reason > 0)
	{
		RequestBodyJson->SetNumberField(TEXT("reason"), Var_Reason);
	}
	FString RequestBodyString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBodyString);
	FJsonSerializer::Serialize(RequestBodyJson.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(RequestBodyString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_CreateSanctionAppeals::OnResponseReceived);
	HttpRequest->ProcessRequest();
}