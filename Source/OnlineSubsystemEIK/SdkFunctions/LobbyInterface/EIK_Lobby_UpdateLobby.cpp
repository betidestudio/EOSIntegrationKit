// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Lobby_UpdateLobby.h"

UEIK_Lobby_UpdateLobby* UEIK_Lobby_UpdateLobby::EIK_Lobby_UpdateLobby(FEIK_HLobbyModification LobbyModificationHandle)
{
	UEIK_Lobby_UpdateLobby* Node = NewObject<UEIK_Lobby_UpdateLobby>();
	Node->Var_LobbyModificationHandle = LobbyModificationHandle;
	return Node;
}

void UEIK_Lobby_UpdateLobby::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_UpdateLobbyOptions Options = { };
			Options.ApiVersion = EOS_LOBBY_UPDATELOBBY_API_LATEST;
			Options.LobbyModificationHandle = *Var_LobbyModificationHandle.Ref;
			EOS_Lobby_UpdateLobby(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_UpdateLobby::OnUpdateLobbyComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to update lobby either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
void UEIK_Lobby_UpdateLobby::OnUpdateLobbyComplete(const EOS_Lobby_UpdateLobbyCallbackInfo* Data)
{
	if (UEIK_Lobby_UpdateLobby* Node = static_cast<UEIK_Lobby_UpdateLobby*>(Data->ClientData))
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