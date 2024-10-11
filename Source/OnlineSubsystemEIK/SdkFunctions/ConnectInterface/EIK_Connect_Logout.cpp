// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Connect_Logout.h"

#include "EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"

UEIK_Connect_Logout* UEIK_Connect_Logout::EIK_Connect_Logout(FEIK_ProductUserId ProductUserId)
{
	UEIK_Connect_Logout* Proxy = NewObject<UEIK_Connect_Logout>();
	Proxy->Var_ProductUserId = ProductUserId;
	return Proxy;
}

void UEIK_Connect_Logout::OnLogoutCallback(const EOS_Connect_LogoutCallbackInfo* Data)
{
	if(UEIK_Connect_Logout* Proxy = static_cast<UEIK_Connect_Logout*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Proxy, Data]()
		{
			Proxy->OnCallback.Broadcast(Proxy->Var_ProductUserId);
		});
		Proxy->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Proxy->MarkAsGarbage();
#else
		Proxy->MarkPendingKill();
#endif
		}
}

void UEIK_Connect_Logout::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_LogoutOptions LogoutOptions = { };
			LogoutOptions.ApiVersion = EOS_CONNECT_LOGOUT_API_LATEST;
			LogoutOptions.LocalUserId = Var_ProductUserId.GetValueAsEosType();
			EOS_Connect_Logout(EOSRef->ConnectHandle, &LogoutOptions, this, &UEIK_Connect_Logout::OnLogoutCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to logout user either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_ProductUserId());
};
