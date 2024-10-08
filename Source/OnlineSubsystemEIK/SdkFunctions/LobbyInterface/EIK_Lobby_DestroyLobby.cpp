// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Lobby_DestroyLobby.h"

#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Lobby_DestroyLobby* UEIK_Lobby_DestroyLobby::EIK_Lobby_DestroyLobby(FEIK_ProductUserId LocalUserId,
	FEIK_LobbyId LobbyId)
{
	UEIK_Lobby_DestroyLobby* LobbyDestroyLobby = NewObject<UEIK_Lobby_DestroyLobby>();
	LobbyDestroyLobby->Var_LocalUserId = LocalUserId;
	LobbyDestroyLobby->Var_LobbyId = LobbyId;
	return LobbyDestroyLobby;
}

void UEIK_Lobby_DestroyLobby::OnDestroyLobbyComplete(const EOS_Lobby_DestroyLobbyCallbackInfo* Data)
{
	if (UEIK_Lobby_DestroyLobby* Node = static_cast<UEIK_Lobby_DestroyLobby*>(Data->ClientData))
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

void UEIK_Lobby_DestroyLobby::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_DestroyLobbyOptions Options = {};
			Options.ApiVersion = EOS_LOBBY_DESTROYLOBBY_API_LATEST;
			Options.LocalUserId = Var_LocalUserId.GetValueAsEosType();
			Options.LobbyId = Var_LobbyId.GetValueAsEosType();
			EOS_Lobby_DestroyLobby(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_DestroyLobby::OnDestroyLobbyComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to destroy lobby either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
