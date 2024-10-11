// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Sessions_RejectInvite.h"

#include "Async/Async.h"

UEIK_Sessions_RejectInvite* UEIK_Sessions_RejectInvite::EIK_Sessions_RejectInvite(FString InviteId,
                                                                                  FEIK_ProductUserId LocalUserId)
{
	UEIK_Sessions_RejectInvite* Node = NewObject<UEIK_Sessions_RejectInvite>();
	Node->Var_InviteId = InviteId;
	Node->Var_LocalUserId = LocalUserId;
	return Node;
}

void UEIK_Sessions_RejectInvite::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Sessions_RejectInviteOptions RejectInviteOptions = { };
			RejectInviteOptions.ApiVersion = EOS_SESSIONS_REJECTINVITE_API_LATEST;
			RejectInviteOptions.InviteId = TCHAR_TO_ANSI(*Var_InviteId);
			RejectInviteOptions.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_Sessions_RejectInvite(EOSRef->SessionsHandle, &RejectInviteOptions, this, &UEIK_Sessions_RejectInvite::OnRejectInviteCallback);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to reject invite either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound);
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Sessions_RejectInvite::OnRejectInviteCallback(const EOS_Sessions_RejectInviteCallbackInfo* Data)
{
	if (UEIK_Sessions_RejectInvite* Node = static_cast<UEIK_Sessions_RejectInvite*>(Data->ClientData))
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
