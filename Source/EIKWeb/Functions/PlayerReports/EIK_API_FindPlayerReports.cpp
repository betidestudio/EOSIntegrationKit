// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_API_FindPlayerReports.h"

UEIK_API_FindPlayerReports* UEIK_API_FindPlayerReports::FindPlayerReports(FString Authorization, FString DeploymentId,
	FString ReportingPlayerId, FString ReportedPlayerId, int32 ReasonId, FString StartTime, FString EndTime,
	bool bPagination, int32 Offset, int32 Limit, FString Order)
{
	UEIK_API_FindPlayerReports* Proxy = NewObject<UEIK_API_FindPlayerReports>();
	Proxy->Var_Authorization = Authorization;
	Proxy->Var_DeploymentId = DeploymentId;
	Proxy->Var_ReportingPlayerId = ReportingPlayerId;
	Proxy->Var_ReportedPlayerId = ReportedPlayerId;
	Proxy->Var_ReasonId = ReasonId;
	Proxy->Var_StartTime = StartTime;
	Proxy->Var_EndTime = EndTime;
	Proxy->Var_bPagination = bPagination;
	Proxy->Var_Offset = Offset;
	Proxy->Var_Limit = Limit;
	Proxy->Var_Order = Order;
	return Proxy;
}

void UEIK_API_FindPlayerReports::Activate()
{
	Super::Activate();
	FString URL = FString::Printf(TEXT("%s/player-reports/v1/report/%s"), *APIEndpoint, *Var_DeploymentId);
	FString QueryString = TEXT("?");
	if (!Var_ReportingPlayerId.IsEmpty())
	{
		QueryString += FString::Printf(TEXT("reportingPlayerId=%s&"), *Var_ReportingPlayerId);
	}
	if (!Var_ReportedPlayerId.IsEmpty())
	{
		QueryString += FString::Printf(TEXT("reportedPlayerId=%s&"), *Var_ReportedPlayerId);
	}
	if (Var_ReasonId > 0)
	{
		QueryString += FString::Printf(TEXT("reasonId=%d&"), Var_ReasonId);
	}
	if (!Var_StartTime.IsEmpty())
	{
		QueryString += FString::Printf(TEXT("startTime=%s&"), *Var_StartTime);
	}
	if (!Var_EndTime.IsEmpty())
	{
		QueryString += FString::Printf(TEXT("endTime=%s&"), *Var_EndTime);
	}
	if (Var_bPagination)
	{
		QueryString += FString::Printf(TEXT("offset=%d&limit=%d&order=%s"), Var_Offset, Var_Limit, *Var_Order);
	}
	URL += QueryString;
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetURL(URL);
	if (Var_Authorization.Contains("Bearer"))
	{
		HttpRequest->SetHeader(TEXT("Authorization"), Var_Authorization);
	}
	else
	{
		HttpRequest->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Var_Authorization));
	}
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UEIK_API_FindPlayerReports::OnResponseReceived);
	HttpRequest->ProcessRequest();
}
