// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Auth_Login.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Auth_Login* UEIK_Auth_Login::Login(FEIK_Auth_LoginOptions Options)
{
	UEIK_Auth_Login* Node = NewObject<UEIK_Auth_Login>();
	Node->Var_Options = Options;
	return Node;
}

void UEIK_Auth_Login::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_LoginOptions Options = Var_Options.ToEOSAuthLoginOptions();
			EOS_Auth_Login(EOSRef->AuthHandle, &Options, this, &UEIK_Auth_Login::Internal_OnLoginComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to login either OnlineSubsystem is not valid or EOSRef is not valid."));
	FEIK_Auth_LoginCallbackInfo Info;
	Info.Result = EEIK_Result::EOS_NotFound;
	OnCallback.Broadcast(Info);
}

void UEIK_Auth_Login::Internal_OnLoginComplete(const EOS_Auth_LoginCallbackInfo* Data)
{
	UEIK_Auth_Login* Node = static_cast<UEIK_Auth_Login*>(Data->ClientData);
	if(Node)
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(*Data);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
