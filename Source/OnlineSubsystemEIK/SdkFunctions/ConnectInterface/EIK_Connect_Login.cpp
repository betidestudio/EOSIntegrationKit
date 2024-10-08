// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Connect_Login.h"

#include "EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"

UEIK_Connect_Login* UEIK_Connect_Login::EIK_Connect_Login(FEIK_Connect_Credentials Credentials,
                                                          FEIK_Connect_UserLoginInfo UserLoginInfo)
{
	UEIK_Connect_Login* Node = NewObject<UEIK_Connect_Login>();
	Node->Local_Credentials = Credentials;
	Node->Local_UserLoginInfo = UserLoginInfo;
	return Node;
}

void UEIK_Connect_Login::OnLoginCallback(const EOS_Connect_LoginCallbackInfo* Data)
{
	if(UEIK_Connect_Login* Node = static_cast<UEIK_Connect_Login*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LocalUserId, Data->ContinuanceToken);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}

void UEIK_Connect_Login::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_LoginOptions LoginOptions = { };
			LoginOptions.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
			EOS_Connect_Credentials Cred = Local_Credentials.ToEOSConnectCredentials();
			EOS_Connect_UserLoginInfo UserLoginInfo = Local_UserLoginInfo.ToEOSConnectUserLoginInfo();
			LoginOptions.Credentials = &Cred;
			LoginOptions.UserLoginInfo = &UserLoginInfo;
			EOS_Connect_Login(EOSRef->ConnectHandle, &LoginOptions, this, &UEIK_Connect_Login::OnLoginCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to login user either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_ProductUserId(), FEIK_ContinuanceToken());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
