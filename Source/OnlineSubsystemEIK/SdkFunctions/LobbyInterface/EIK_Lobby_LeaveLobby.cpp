// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Lobby_LeaveLobby.h"

UEIK_Lobby_LeaveLobby* UEIK_Lobby_LeaveLobby::EIK_Lobby_LeaveLobby(FEIK_ProductUserId LocalUserId, FEIK_LobbyId LobbyId)
{
	UEIK_Lobby_LeaveLobby* Node = NewObject<UEIK_Lobby_LeaveLobby>();
	Node->Var_LocalUserId = LocalUserId;
	Node->Var_LobbyId = LobbyId;
	return Node;
}

void UEIK_Lobby_LeaveLobby::OnLeaveLobbyComplete(const EOS_Lobby_LeaveLobbyCallbackInfo* Data)
{
	if (UEIK_Lobby_LeaveLobby* Node = static_cast<UEIK_Lobby_LeaveLobby*>(Data->ClientData))
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

void UEIK_Lobby_LeaveLobby::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_LeaveLobbyOptions Options = { };
			Options.ApiVersion = EOS_LOBBY_LEAVELOBBY_API_LATEST;
			Options.LobbyId = Var_LobbyId.GetValueAsEosType();
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			EOS_Lobby_LeaveLobby(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_LeaveLobby::OnLeaveLobbyComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to leave lobby either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
