// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_RemoveSanctions.h"

UEIK_API_RemoveSanctions* UEIK_API_RemoveSanctions::RemoveSanctions(FString Authorization, FString DeploymentId,
	TArray<FString> ReferenceIds, FString Justification)
{
	UEIK_API_RemoveSanctions* Proxy = NewObject<UEIK_API_RemoveSanctions>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_ReferenceIds = ReferenceIds;
	Proxy->Var_Justification = Justification;
	return Proxy;
}

void UEIK_API_RemoveSanctions::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/sanctions/v1/%s/sanctions"), *APIEndpoint, *Var_DeploymentId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("DELETE"));
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
	if (!Var_Justification.IsEmpty())
	{
		RequestBodyJson->SetStringField(TEXT("justification"), Var_Justification);
	}
	FString RequestBodyString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBodyString);
	FJsonSerializer::Serialize(RequestBodyJson.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(RequestBodyString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_RemoveSanctions::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
