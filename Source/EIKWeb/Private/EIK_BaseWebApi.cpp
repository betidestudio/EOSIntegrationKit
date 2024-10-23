// Copyright (c) 2024 Betide Studio. All Rights Reserved.


#include "EIK_BaseWebApi.h"


void UEIK_BaseWebApi::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(!bWasSuccessful)
	{
		OnResponse.Broadcast(false, FEIK_BaseWebApiResponse(-1, "Request failed"));
		return;
	}
	if(!Response.IsValid())
	{
		OnResponse.Broadcast(false, FEIK_BaseWebApiResponse(-1, "Response is invalid"));
		return;
	}
	OnResponse.Broadcast(true, FEIK_BaseWebApiResponse(Response->GetResponseCode(), Response->GetContentAsString()));
}
