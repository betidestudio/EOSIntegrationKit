// Copyright (c) 2023 Betide Studio. All Rights Reserved.


#include "EIK_Lobby_CreateLobby.h"

#include "OnlineSessionEOS.h"
#include "OnlineSubsystemEOS.h"
#include "Async/Async.h"
#include "OnlineSubsystemEIK/SdkFunctions/ConnectInterface/EIK_ConnectSubsystem.h"

UEIK_Lobby_CreateLobby* UEIK_Lobby_CreateLobby::EIK_Lobby_CreateLobby(FEIK_Lobby_CreateLobbyOptions CreateLobbyOptions)
{
	UEIK_Lobby_CreateLobby* LobbyCreateLobby = NewObject<UEIK_Lobby_CreateLobby>();
	LobbyCreateLobby->Var_CreateLobbyOptions = CreateLobbyOptions;
	return LobbyCreateLobby;
}

void UEIK_Lobby_CreateLobby::OnCreateLobbyComplete(const EOS_Lobby_CreateLobbyCallbackInfo* Data)
{
	if (UEIK_Lobby_CreateLobby* Node = static_cast<UEIK_Lobby_CreateLobby*>(Data->ClientData))
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
void UEIK_Lobby_CreateLobby::Activate()
{
	Super::Activate();
	if (IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get("EIK"))
	{
		if (FOnlineSubsystemEOS* EOSRef = static_cast<FOnlineSubsystemEOS*>(OnlineSub))
		{
			EOS_Lobby_CreateLobbyOptions Options = Var_CreateLobbyOptions.GetCreateLobbyOptions();
			EOS_Lobby_CreateLobby(EOSRef->SessionInterfacePtr->LobbyHandle, &Options, this, &UEIK_Lobby_CreateLobby::OnCreateLobbyComplete);
			return;
		}
	}
	UE_LOG(LogEIK, Error, TEXT("Failed to create lobby either OnlineSubsystem is not valid or EOSRef is not valid."));
	OnCallback.Broadcast(EEIK_Result::EOS_NotFound, FEIK_LobbyId());
	SetReadyToDestroy();
	#if ENGINE_MAJOR_VERSION == 5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
