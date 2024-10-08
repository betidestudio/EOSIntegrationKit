// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Lobby_RejectInvite.h"

UEIK_Lobby_RejectInvite* UEIK_Lobby_RejectInvite::EIK_Lobby_RejectInvite(FEIK_ProductUserId LocalUserId,
	FString InviteId)
{
	UEIK_Lobby_RejectInvite* Node = NewObject<UEIK_Lobby_RejectInvite>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_InviteId = InviteId;
	return Node;
}

void UEIK_Lobby_RejectInvite::OnRejectInviteComplete(const EOS_Lobby_RejectInviteCallbackInfo* Data)
{
	if (UEIK_Lobby_RejectInvite* Node = static_cast<UEIK_Lobby_RejectInvite*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), FString());
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}

void UEIK_Lobby_RejectInvite::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_RejectInviteOptions Options = { };
			Options.ApiVersion = EOS_LOBBY_REJECTINVITE_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.InviteId = TCHAR_TO_ANSI(*Var_InviteId);
			EOS_Lobby_RejectInvite(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_RejectInvite::OnRejectInviteComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to reject invite either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FString());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
