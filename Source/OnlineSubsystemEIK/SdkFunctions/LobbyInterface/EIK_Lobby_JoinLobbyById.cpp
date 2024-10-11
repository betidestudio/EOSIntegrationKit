// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Lobby_JoinLobbyById.h"

#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Lobby_JoinLobbyById* UEIK_Lobby_JoinLobbyById::EIK_Lobby_JoinLobbyById(FEIK_Lobby_JoinLobbyByIdOptions Options)
{
	UEIK_Lobby_JoinLobbyById* Node = NewObject<UEIK_Lobby_JoinLobbyById>();
	Node->Var_Options = Options;
	return Node;
}

void UEIK_Lobby_JoinLobbyById::OnJoinLobbyByIdComplete(const EOS_Lobby_JoinLobbyByIdCallbackInfo* Data)
{
	if (UEIK_Lobby_JoinLobbyById* Node = static_cast<UEIK_Lobby_JoinLobbyById*>(Data->ClientData))
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

void UEIK_Lobby_JoinLobbyById::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_JoinLobbyByIdOptions Options = Var_Options.ToEOSOptions();
			EOS_Lobby_JoinLobbyById(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_JoinLobbyById::OnJoinLobbyByIdComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to join lobby by id either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
