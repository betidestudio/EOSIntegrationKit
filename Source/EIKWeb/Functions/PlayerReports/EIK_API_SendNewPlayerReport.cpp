// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_SendNewPlayerReport.h"

UEIK_API_SendNewPlayerReport* UEIK_API_SendNewPlayerReport::SendNewPlayerReport(FString Authorization, FString ReportingPlayerId, FString ReportedPlayerId, FString Time, int32 ReasonId, FString Message, FString Context)
{
	UEIK_API_SendNewPlayerReport* Node = NewObject<UEIK_API_SendNewPlayerReport>();
	Node->Var_ReportingPlayerId = ReportingPlayerId;
	Node->Var_ReportedPlayerId = ReportedPlayerId;
	Node->Var_Time = Time;
	Node->Var_ReasonId = ReasonId;
	Node->Var_Message = Message;
	Node->Var_Context = Context;
	return Node;
}

void UEIK_API_SendNewPlayerReport::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/player-reports/v1/report"), *APIEndpoint);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetURL(URL);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	if(Var_Authorization.Contains("Bearer"))
	{
		HttpRequest->SetHeader(TEXT("Authorization"), Var_Authorization);
	}
	else
	{
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Var_Authorization));
	}
	TSharedPtr<FJsonObject> RequestBodyJson = MakeShareable(new FJsonObject);
	RequestBodyJson->SetStringField(TEXT("reportingPlayerId"), Var_ReportingPlayerId);
	RequestBodyJson->SetStringField(TEXT("reportedPlayerId"), Var_ReportedPlayerId);
	RequestBodyJson->SetStringField(TEXT("time"), Var_Time);
	RequestBodyJson->SetNumberField(TEXT("reasonId"), Var_ReasonId);
	RequestBodyJson->SetStringField(TEXT("message"), Var_Message);
	RequestBodyJson->SetStringField(TEXT("context"), Var_Context);
	FString RequestBodyString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&RequestBodyString);
	FJsonSerializer::Serialize(RequestBodyJson.ToSharedRef(), JsonWriter);
	HttpRequest->SetContentAsString(RequestBodyString);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_SendNewPlayerReport::OnResponseReceived);
	HttpRequest->ProcessRequest();
	
}
