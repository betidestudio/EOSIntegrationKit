// Copyright (c) 2024 Betide Studio. All Rights Reserved.


#include "EIK_BaseWebApi.h"
#include "Runtime/Launch/Resources/Version.h"


void UEIK_BaseWebApi::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(!bWasSuccessful)
	{
		OnResponse.Broadcast(false, FEIK_BaseWebApiResponse(-1, "Request failed"));
		DestroyAsyncTask();
		return;
	}
	if(!Response.IsValid())
	{
		OnResponse.Broadcast(false, FEIK_BaseWebApiResponse(-1, "Response is invalid"));
		DestroyAsyncTask();
		return;
	}
	OnResponse.Broadcast(true, FEIK_BaseWebApiResponse(Response->GetResponseCode(), Response->GetContentAsString()));
	DestroyAsyncTask();
}

void UEIK_BaseWebApi::DestroyAsyncTask()
{
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
