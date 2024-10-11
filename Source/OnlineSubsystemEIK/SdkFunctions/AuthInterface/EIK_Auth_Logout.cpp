// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Auth_Logout.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Auth_Logout* UEIK_Auth_Logout::Logout(FEIK_EpicAccountId EpicAccountId)
{
	UEIK_Auth_Logout* Node = NewObject<UEIK_Auth_Logout>();
	Node->Var_EpicAccountId = EpicAccountId;
	return Node;
}

void UEIK_Auth_Logout::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_LogoutOptions Options = {};
			Options.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
			Options.LocalUserId = Var_EpicAccountId.GetValueAsEosType();
			EOS_Auth_Logout(EOSRef->AuthHandle, &Options, this, &UEIK_Auth_Logout::OnLogoutCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to logout either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, Var_EpicAccountId);
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Auth_Logout::OnLogoutCallback(const EOS_Auth_LogoutCallbackInfo* Data)
{
	UEIK_Auth_Logout* Node = static_cast<UEIK_Auth_Logout*>(Data->ClientData);
	if(Node)
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
