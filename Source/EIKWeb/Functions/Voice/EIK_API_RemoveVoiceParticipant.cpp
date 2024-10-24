// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_RemoveVoiceParticipant.h"

UEIK_API_RemoveVoiceParticipant* UEIK_API_RemoveVoiceParticipant::RemoveVoiceParticipant(FString Authorization,
	FString DeploymentId, FString RoomId, FString ProductUserId)
{
	UEIK_API_RemoveVoiceParticipant* Proxy = NewObject<UEIK_API_RemoveVoiceParticipant>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_RoomId = RoomId;
	Proxy->Var_ProductUserId = ProductUserId;
	return Proxy;
}

void UEIK_API_RemoveVoiceParticipant::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/rtc/v1/%s/room/%s/participants/%s"), *APIEndpoint, *Var_DeploymentId, *Var_RoomId, *Var_ProductUserId);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("DELETE"));
	HttpRequest->SetURL(URL);
	if (Var_Authorization.Contains("Bearer"))
	{
		HttpRequest->SetHeader(TEXT("Authorization"), Var_Authorization);
	}
	else
	{
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Var_Authorization));
	}
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_RemoveVoiceParticipant::OnResponseReceived);
	HttpRequest->ProcessRequest();
}