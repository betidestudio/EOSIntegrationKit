// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Connect_UnlinkAccount.h"

#include "EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"

UEIK_Connect_UnlinkAccount* UEIK_Connect_UnlinkAccount::EIK_Connect_UnlinkAccount(FEIK_ProductUserId LocalUserId)
{
	UEIK_Connect_UnlinkAccount* Node = NewObject<UEIK_Connect_UnlinkAccount>();
	Node->Var_LocalUserId = LocalUserId;
	return Node;
}

void UEIK_Connect_UnlinkAccount::OnUnlinkAccountCallback(const EOS_Connect_UnlinkAccountCallbackInfo* Data)
{
	if(UEIK_Connect_UnlinkAccount* Proxy = static_cast<UEIK_Connect_UnlinkAccount*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Proxy, Data]()
		{
			Proxy->OnCallback.Broadcast(FEIK_ProductUserId(Data->LocalUserId), static_cast<EEIK_Result>(Data->ResultCode));
		});
		Proxy->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Proxy->MarkAsGarbage();
#else
		Proxy->MarkPendingKill();
#endif
	}
}

void UEIK_Connect_UnlinkAccount::Activate()
{
	Super::Activate();
	if(IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if(FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_UnlinkAccountOptions UnlinkAccountOptions = {};
			UnlinkAccountOptions.ApiVersion = EOS_CONNECT_UNLINKACCOUNT_API_LATEST;
			UnlinkAccountOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_Connect_UnlinkAccount(EOSRef->ConnectHandle, &UnlinkAccountOptions, this, &UEIK_Connect_UnlinkAccount::OnUnlinkAccountCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to unlink account either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_ProductUserId(), EEIK_Result::EOS_ServiceFailure);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}


