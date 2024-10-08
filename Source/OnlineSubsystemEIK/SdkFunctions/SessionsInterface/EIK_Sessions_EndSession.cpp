// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Sessions_EndSession.h"

#include "Async/Async.h"

UEIK_Sessions_EndSession* UEIK_Sessions_EndSession::EIK_Sessions_EndSession(FString SessionName)
{
	UEIK_Sessions_EndSession* BlueprintNode = NewObject<UEIK_Sessions_EndSession>();
	BlueprintNode->Var_SessionName = SessionName;
	return BlueprintNode;
}

void UEIK_Sessions_EndSession::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_EndSessionOptions EndSessionOptions = { };
			EndSessionOptions.ApiVersion = EOS_SESSIONS_ENDSESSION_API_LATEST;
			EndSessionOptions.SessionName = TCHAR_TO_ANSI(*Var_SessionName);
			EOS_Sessions_EndSession(EOSRef->SessionsHandle, &EndSessionOptions, this, &UEIK_Sessions_EndSession::OnEndSessionCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to end session either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Sessions_EndSession::OnEndSessionCallback(const EOS_Sessions_EndSessionCallbackInfo* Data)
{
	if (UEIK_Sessions_EndSession* Node = static_cast<UEIK_Sessions_EndSession*>(Data->ClientData))
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
