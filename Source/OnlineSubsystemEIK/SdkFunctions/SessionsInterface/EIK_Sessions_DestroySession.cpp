// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Sessions_DestroySession.h"

#include "Async/Async.h"

UEIK_Sessions_DestroySession* UEIK_Sessions_DestroySession::EIK_Sessions_DestroySession(FString SessionName)
{
	UEIK_Sessions_DestroySession* Node = NewObject<UEIK_Sessions_DestroySession>();
	Node->Var_SessionName = SessionName;
	return Node;
}

void UEIK_Sessions_DestroySession::Activate()
{
	Super::Activate();
	if(	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_DestroySessionOptions DestroySessionOptions = { };
			DestroySessionOptions.ApiVersion = EOS_SESSIONS_DESTROYSESSION_API_LATEST;
			DestroySessionOptions.SessionName = TCHAR_TO_ANSI(*Var_SessionName);
			EOS_Sessions_DestroySession(EOSRef->SessionsHandle, &DestroySessionOptions, this, &UEIK_Sessions_DestroySession::OnDestroySessionCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to destroy session either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Sessions_DestroySession::OnDestroySessionCallback(const EOS_Sessions_DestroySessionCallbackInfo* Data)
{
	if(UEIK_Sessions_DestroySession* Node = static_cast<UEIK_Sessions_DestroySession*>(Data->ClientData))
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
