// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Lobby_JoinLobby.h"

#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Lobby_JoinLobby* UEIK_Lobby_JoinLobby::EIK_Lobby_JoinLobby(FEIK_Lobby_JoinLobbyOptions Options)
{
	UEIK_Lobby_JoinLobby* Node = NewObject<UEIK_Lobby_JoinLobby>();
	Node->Var_Options = Options;
	return Node;
}

void UEIK_Lobby_JoinLobby::OnJoinLobbyComplete(const EOS_Lobby_JoinLobbyCallbackInfo* Data)
{
	if (UEIK_Lobby_JoinLobby* Node = static_cast<UEIK_Lobby_JoinLobby*>(Data->ClientData))
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

void UEIK_Lobby_JoinLobby::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_JoinLobbyOptions Options = Var_Options.ToEOSOptions();
			EOS_Lobby_JoinLobby(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_JoinLobby::OnJoinLobbyComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to join lobby either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
