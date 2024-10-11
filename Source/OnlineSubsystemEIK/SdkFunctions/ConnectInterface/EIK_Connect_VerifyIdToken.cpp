// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Connect_VerifyIdToken.h"

#include "EIK_ConnectSubsystem.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"

UEIK_Connect_VerifyIdToken* UEIK_Connect_VerifyIdToken::EIK_Connect_VerifyIdToken(FEIK_Connect_IdToken IdToken)
{
	UEIK_Connect_VerifyIdToken* Node = NewObject<UEIK_Connect_VerifyIdToken>();
	Node->Var_IdToken = IdToken;
	return Node;
}

void UEIK_Connect_VerifyIdToken::OnVerifyIdTokenCallback(const EOS_Connect_VerifyIdTokenCallbackInfo* Data)
{
	if (UEIK_Connect_VerifyIdToken* Proxy = static_cast<UEIK_Connect_VerifyIdToken*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Proxy, Data]()
		{
			Proxy->OnCallback.Broadcast(Data);
		});
		Proxy->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Proxy->MarkAsGarbage();
#else
		Proxy->MarkPendingKill();
#endif
	}
}	

void UEIK_Connect_VerifyIdToken::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Connect_VerifyIdTokenOptions VerifyIdTokenOptions = {};
			VerifyIdTokenOptions.ApiVersion = EOS_CONNECT_VERIFYIDTOKEN_API_LATEST;
			EOS_Connect_IdToken Temp = Var_IdToken.GetValueAsEosType();
			VerifyIdTokenOptions.IdToken = &Temp;
			EOS_Connect_VerifyIdToken(EOSRef->ConnectHandle, &VerifyIdTokenOptions, this, &UEIK_Connect_VerifyIdToken::OnVerifyIdTokenCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to verify id token either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(FEIK_Connect_VerifyIdTokenCallbackInfo());
	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
