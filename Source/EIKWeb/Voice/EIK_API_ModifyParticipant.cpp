// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_ModifyParticipant.h"

UEIK_API_ModifyParticipant* UEIK_API_ModifyParticipant::ModifyParticipant(FString Authorization, FString DeploymentId,
	FString RoomId, FString ProductUserId, bool bHardMuted)
{
	UEIK_API_ModifyParticipant* Proxy = NewObject<UEIK_API_ModifyParticipant>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_RoomId = RoomId;
	Proxy->Var_ProductUserId = ProductUserId;
	Proxy->Var_bHardMuted = bHardMuted;
	return Proxy;
}

void UEIK_API_ModifyParticipant::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/rtc/v1/%s/room/%s/participants/%s"), *APIEndpoint, *Var_DeploymentId, *Var_RoomId, *Var_ProductUserId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetURL(URL);
	if (Var_Authorization.Contains("Bearer"))
	{
		HttpRequest->SetHeader(TEXT("Authorization"), Var_Authorization);
	}
	else
	{
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Var_Authorization));
	}
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	TSharedPtr<FJsonObject> RequestBodyJson = MakeShareable(new FJsonObject);
	RequestBodyJson->SetBoolField(TEXT("hardMuted"), Var_bHardMuted);
	FString RequestBody;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestBodyJson.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(RequestBody);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_ModifyParticipant::OnResponseReceived);
	HttpRequest->ProcessRequest();
}