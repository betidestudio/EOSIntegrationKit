// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Sessions_StartSession.h"

#include "Async/Async.h"

UEIK_Sessions_StartSession* UEIK_Sessions_StartSession::EIK_Sessions_StartSession(FString SessionName)
{
	UEIK_Sessions_StartSession* Node = NewObject<UEIK_Sessions_StartSession>();
	Node->Var_SessionName = SessionName;
	return Node;
}

void UEIK_Sessions_StartSession::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_StartSessionOptions StartSessionOptions = { };
			StartSessionOptions.ApiVersion = EOS_SESSIONS_STARTSESSION_API_LATEST;
			StartSessionOptions.SessionName = TCHAR_TO_ANSI(*Var_SessionName);
			EOS_Sessions_StartSession(EOSRef->SessionsHandle, &StartSessionOptions, this, &UEIK_Sessions_StartSession::OnStartSessionCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to start session either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Sessions_StartSession::OnStartSessionCallback(const EOS_Sessions_StartSessionCallbackInfo* Data)
{
	if (UEIK_Sessions_StartSession* Node = static_cast<UEIK_Sessions_StartSession*>(Data->ClientData))
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
