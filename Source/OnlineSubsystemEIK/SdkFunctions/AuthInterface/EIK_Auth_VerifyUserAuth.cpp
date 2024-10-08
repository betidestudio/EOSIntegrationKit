// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Auth_VerifyUserAuth.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Auth_VerifyUserAuth* UEIK_Auth_VerifyUserAuth::VerifyUserAuth(FEIK_Auth_Token AuthToken)
{
	UEIK_Auth_VerifyUserAuth* Node = NewObject<UEIK_Auth_VerifyUserAuth>();
	Node->Var_AuthToken = AuthToken;
	return Node;
}

void UEIK_Auth_VerifyUserAuth::OnVerifyUserAuthCallback(const EOS_Auth_VerifyUserAuthCallbackInfo* Data)
{
	UEIK_Auth_VerifyUserAuth* Node = static_cast<UEIK_Auth_VerifyUserAuth*>(Data->ClientData);
	if(Node)
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode));
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}

void UEIK_Auth_VerifyUserAuth::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_VerifyUserAuthOptions Options = {};
			Options.ApiVersion = EOS_AUTH_VERIFYUSERAUTH_API_LATEST;
			EOS_Auth_Token TokenRef = Var_AuthToken.GetValueAsEosType();
			Options.AuthToken = &TokenRef;
			EOS_Auth_VerifyUserAuth(EOSRef->AuthHandle, &Options, this, &UEIK_Auth_VerifyUserAuth::OnVerifyUserAuthCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to verify user auth either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
