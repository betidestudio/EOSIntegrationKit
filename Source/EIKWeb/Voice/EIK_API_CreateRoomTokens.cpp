// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_CreateRoomTokens.h"


UEIK_API_CreateRoomTokens* UEIK_API_CreateRoomTokens::CreateVoiceRoomToken(FString Authorization, FString DeploymentId,
                                                                           FString RoomId, TArray<FEWebApi_EosRoomParticipant> Participants)
{
	UEIK_API_CreateRoomTokens* Proxy = NewObject<UEIK_API_CreateRoomTokens>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_RoomId = RoomId;
	Proxy->Var_Participants = Participants;
	return Proxy;
}

void UEIK_API_CreateRoomTokens::Activate()
{
	FString URL = FString::Printf(TEXT("%s/rtc/v1/%s/room/%s"),*APIEndpoint, *Var_DeploymentId, *Var_RoomId);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetURL(URL);
	if(Var_Authorization.Contains("Bearer"))
	{
		HttpRequest->SetHeader(TEXT("Authorization"), Var_Authorization);
	}
	else
	{
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Var_Authorization));
	}
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	TSharedPtr<FJsonObject> RequestBodyJson = MakeShareable(new FJsonObject);
	if(Var_Participants.Num() > 0)
	{
		TArray<TSharedPtr<FJsonValue>> ParticipantsJson;
		for (auto ParticipantId : Var_Participants)
		{
			TSharedPtr<FJsonObject> ParticipantJson = MakeShareable(new FJsonObject);
			if(!ParticipantId.ProductUserId.IsEmpty())
			{
				ParticipantJson->SetStringField(TEXT("puid"), ParticipantId.ProductUserId);
			}
			if(!ParticipantId.ClientIp.IsEmpty())
			{
				ParticipantJson->SetStringField(TEXT("clientIP"), ParticipantId.ClientIp);
			}
			ParticipantJson->SetBoolField(TEXT("hardMuted"), ParticipantId.bHardMuted);
			ParticipantsJson.Add(MakeShareable(new FJsonValueObject(ParticipantJson)));
		}
		RequestBodyJson->SetArrayField(TEXT("participants"), ParticipantsJson);
	}
	FString RequestBody;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestBodyJson.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(RequestBody);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_CreateRoomTokens::OnResponseReceived);
	HttpRequest->ProcessRequest();
}