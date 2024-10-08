// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Sessions_UpdateSession.h"

#include "Async/Async.h"

UEIK_Sessions_UpdateSession* UEIK_Sessions_UpdateSession::EIK_Sessions_UpdateSession(
	FEIK_HSessionModification SessionModificationHandle)
{
	UEIK_Sessions_UpdateSession* Node = NewObject<UEIK_Sessions_UpdateSession>();
	Node->Var_SessionModificationHandle = SessionModificationHandle;
	return Node;
}

void UEIK_Sessions_UpdateSession::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_UpdateSessionOptions UpdateSessionOptions = { };
			UpdateSessionOptions.ApiVersion = EOS_SESSIONS_UPDATESESSION_API_LATEST;
			UpdateSessionOptions.SessionModificationHandle = Var_SessionModificationHandle.Ref;
			EOS_Sessions_UpdateSession(EOSRef->SessionsHandle, &UpdateSessionOptions, this, &UEIK_Sessions_UpdateSession::OnUpdateSessionCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to update session either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, "", "");
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Sessions_UpdateSession::OnUpdateSessionCallback(const EOS_Sessions_UpdateSessionCallbackInfo* Data)
{
	if (UEIK_Sessions_UpdateSession* Node = static_cast<UEIK_Sessions_UpdateSession*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->SessionName, Data->SessionId);
		});
		Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
		Node->MarkAsGarbage();
#else
		Node->MarkPendingKill();
#endif
	}
}
