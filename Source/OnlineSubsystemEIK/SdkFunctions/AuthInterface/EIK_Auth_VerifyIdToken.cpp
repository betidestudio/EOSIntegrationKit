// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Auth_VerifyIdToken.h"

#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Auth_VerifyIdToken* UEIK_Auth_VerifyIdToken::VerifyIdToken(FEIK_Auth_IdToken IdToken)
{
	UEIK_Auth_VerifyIdToken* Node = NewObject<UEIK_Auth_VerifyIdToken>();
	Node->Var_IdToken = IdToken;
	return Node;
}

void UEIK_Auth_VerifyIdToken::OnVerifyIdTokenCallback(const EOS_Auth_VerifyIdTokenCallbackInfo* Data)
{
	UEIK_Auth_VerifyIdToken* Node = static_cast<UEIK_Auth_VerifyIdToken*>(Data->ClientData);
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

void UEIK_Auth_VerifyIdToken::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Auth_VerifyIdTokenOptions Options = {};
			Options.ApiVersion = EOS_AUTH_VERIFYIDTOKEN_API_LATEST;
			EOS_Auth_IdToken TokenRef = Var_IdToken.GetValueAsEosType();
			Options.IdToken = &TokenRef;
			EOS_Auth_VerifyIdToken(EOSRef->AuthHandle, &Options, this, &UEIK_Auth_VerifyIdToken::OnVerifyIdTokenCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to verify id token either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_Auth_VerifyIdTokenCallbackInfo());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
