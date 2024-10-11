// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Lobby_QueryInvites.h"

UEIK_Lobby_QueryInvites* UEIK_Lobby_QueryInvites::EIK_Lobby_QueryInvites(FEIK_ProductUserId LocalUserId)
{
	UEIK_Lobby_QueryInvites* Node = NewObject<UEIK_Lobby_QueryInvites>();
	Node->Var_LocalUserId = LocalUserId;
	return Node;
}

void UEIK_Lobby_QueryInvites::OnQueryInvitesComplete(const EOS_Lobby_QueryInvitesCallbackInfo* Data)
{
	if (UEIK_Lobby_QueryInvites* Node = static_cast<UEIK_Lobby_QueryInvites*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), FEIK_LobbyId());
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}

void UEIK_Lobby_QueryInvites::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_QueryInvitesOptions Options = { };
			Options.ApiVersion = EOS_LOBBY_QUERYINVITES_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_Lobby_QueryInvites(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_QueryInvites::OnQueryInvitesComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to query invites either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
