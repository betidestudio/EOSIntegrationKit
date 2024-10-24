// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_CreateSanctionAppealForLocalUser.h"

UEIK_API_CreateSanctionAppealForLocalUser* UEIK_API_CreateSanctionAppealForLocalUser::CreateSanctionAppealForLocalUser(
	FString Authorization, FString DeploymentId, FString ProductUserId, FString ReferenceId, FString Reason)
{
	UEIK_API_CreateSanctionAppealForLocalUser* Proxy = NewObject<UEIK_API_CreateSanctionAppealForLocalUser>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_ProductUserId = ProductUserId;
	Proxy->Var_ReferenceId = ReferenceId;
	Proxy->Var_Reason = Reason;
	return Proxy;
}

void UEIK_API_CreateSanctionAppealForLocalUser::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/sanctions/v1/%s/productUser/%s/appeals"), *APIEndpoint, *Var_DeploymentId, *Var_ProductUserId);
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
	if (!Var_Reason.IsEmpty())
	{
		RequestBodyJson->SetStringField(TEXT("reason"), Var_Reason);
	}
	FString RequestBodyString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBodyString);
	FJsonSerializer::Serialize(RequestBodyJson.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(RequestBodyString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_CreateSanctionAppealForLocalUser::OnResponseReceived);
	HttpRequest->ProcessRequest();
}