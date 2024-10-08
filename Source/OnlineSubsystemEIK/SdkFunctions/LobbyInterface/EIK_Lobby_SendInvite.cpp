// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Lobby_SendInvite.h"

UEIK_Lobby_SendInvite* UEIK_Lobby_SendInvite::EIK_Lobby_SendInvite(FEIK_ProductUserId LocalUserId, FEIK_LobbyId LobbyId,
	FEIK_ProductUserId TargetUserId)
{
	UEIK_Lobby_SendInvite* Node = NewObject<UEIK_Lobby_SendInvite>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_LobbyId = LobbyId;
	Node->Var_TargetUserId = TargetUserId;
	return Node;
}

void UEIK_Lobby_SendInvite::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_SendInviteOptions Options = { };
			Options.ApiVersion = EOS_LOBBY_SENDINVITE_API_LATEST;
			Options.LobbyId = Var_LobbyId.GetValueAsEosType();
			Options.TargetUserId = Var_TargetUserId.GetValueAsEosType();
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_Lobby_SendInvite(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_SendInvite::OnSendInviteComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to send invite either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

void UEIK_Lobby_SendInvite::OnSendInviteComplete(const EOS_Lobby_SendInviteCallbackInfo* Data)
{
	if (UEIK_Lobby_SendInvite* Node = static_cast<UEIK_Lobby_SendInvite*>(Data->ClientData))
	{
		AsyncTask(ENamedThreads::GameThread, [Node, Data]()
		{
			Node->OnCallback.Broadcast(static_cast<EEIK_Result>(Data->ResultCode), Data->LobbyId);
			Node->SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION == 5
Node->MarkAsGarbage();
#else
Node->MarkPendingKill();
#endif
		});
	}
}
